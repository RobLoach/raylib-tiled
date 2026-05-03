/**********************************************************************************************
*
*   raylib-tiled - Integrate tiled with raylib, allowing to load images, audio and fonts from data archives.
*
*   Copyright 2021 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       raylib https://www.raylib.com/
*       cute_tiled.h https://github.com/RandyGaul/cute_headers/blob/master/cute_tiled.h
*
*   LICENSE: zlib/libpng
*
*   raylib-tiled is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software:
*
*   This software is provided "as-is", without any express or implied warranty. In no event
*   will the authors be held liable for any damages arising from the use of this software.
*
*   Permission is granted to anyone to use this software for any purpose, including commercial
*   applications, and to alter it and redistribute it freely, subject to the following restrictions:
*
*     1. The origin of this software must not be misrepresented; you must not claim that you
*     wrote the original software. If you use this software in a product, an acknowledgment
*     in the product documentation would be appreciated but is not required.
*
*     2. Altered source versions must be plainly marked as such, and must not be misrepresented
*     as being the original software.
*
*     3. This notice may not be removed or altered from any source distribution.
*
**********************************************************************************************/

#ifndef INCLUDE_RAYLIB_TILED_H_
#define INCLUDE_RAYLIB_TILED_H_

#include "raylib.h" // NOLINT

#ifdef __cplusplus
extern "C" {
#endif

typedef struct Map Map;

Map LoadMap(const char* fileName);
Map LoadMapFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir);
bool IsMapValid(Map map);
void UnloadMap(Map map);
void DrawMap(Map map, int posX, int posY, Color tint);
bool CheckCollisionPointLayerPoly(Map map, Vector2 point, Vector2 position, const char* layerName);

#ifdef __cplusplus
}
#endif

#endif

#ifdef RAYLIB_TILED_IMPLEMENTATION
#ifndef RAYLIB_TILED_IMPLEMENTATION_ONCE
#define RAYLIB_TILED_IMPLEMENTATION_ONCE

#ifndef CUTE_TILED_IMPLEMENTATION
#define CUTE_TILED_IMPLEMENTATION
#endif

#define CUTE_TILED_ALLOC(size, ctx) MemAlloc(size)
#define CUTE_TILED_FREE(mem, ctx) MemFree(mem)

#define CUTE_TILED_WARNING(msg) TraceLog(LOG_WARNING, "TILED: %s (cute_tiled.h:%i)", msg, __LINE__)

#define STRPOOL_EMBEDDED_ASSERT(condition) if (!(condition)) { TraceLog(LOG_ERROR, "TILED: Failed assert \"%s\" in %s:%i", #condition, __FILE__, __LINE__); }
// #define STRPOOL_EMBEDDED_MEMSET( ptr, val, cnt ) ( memset( ptr, val, cnt ) )
// #define STRPOOL_EMBEDDED_MEMCPY( dst, src, cnt ) ( memcpy( dst, src, cnt ) )
//#define STRPOOL_EMBEDDED_MEMCMP( pr1, pr2, cnt ) ( memcmp( pr1, pr2,
#define STRPOOL_EMBEDDED_STRNICMP(s1, s2, len) (TextIsEqual(TextToLower(s1), TextToLower(s2)))

#define STRPOOL_EMBEDDED_MALLOC(ctx, size) (MemAlloc(size))
#define STRPOOL_EMBEDDED_FREE(ctx, ptr) (MemFree(ptr))

//#define CUTE_TILED_MEMCPY memcpy
//#define CUTE_TILED_MEMSET memset

#define CUTE_TILED_SNPRINTF(s, n, format, arg1, arg2) (void)0
#define CUTE_TILED_SEEK_SET 0
#define CUTE_TILED_SEEK_END 0
#define CUTE_TILED_FILE void
CUTE_TILED_FILE* raylib_tiled_fopen(const char* file, const char* property) {
    (void)file;
    (void)property;
    return 0;
}
void raylib_tiled_fseek(CUTE_TILED_FILE* fp, int sz, int pos) {
    (void)fp;
    (void)sz;
    (void)pos;
}
void raylib_tiled_fread(char* data, int sz, int num, CUTE_TILED_FILE* fp) {
    (void)data;
    (void)sz;
    (void)num;
    (void)fp;
}
int raylib_tiled_ftell(CUTE_TILED_FILE* fp) {
    (void)fp;
    return 0;
}
void raylib_tiled_fclose(CUTE_TILED_FILE* fp) {
    (void)fp;
}
#define CUTE_TILED_FOPEN raylib_tiled_fopen
#define CUTE_TILED_FSEEK raylib_tiled_fseek
#define CUTE_TILED_FREAD raylib_tiled_fread
#define CUTE_TILED_FTELL raylib_tiled_ftell
#define CUTE_TILED_FCLOSE raylib_tiled_fclose

#ifdef __cplusplus
extern "C" {
#endif

#include "./cute_tiled.h" // NOLINT

struct Map {
    cute_tiled_map_t* map;
    Color backgroundcolor;
};

Map LoadMap(const char* fileName) {
    unsigned int bytesRead;
    unsigned char* fileData = LoadFileData(fileName, &bytesRead);
    Map map = LoadMapFromMemory(fileData, bytesRead, GetDirectoryPath(fileName));
    UnloadFileData(fileData);
    return map;
}

/**
 * Load the given cute_tiled string as a texture.
 */
void LoadMapStringTexture(cute_tiled_string_t* image, const char* baseDir) {
    const char* fileName = image->ptr;
    const char* imagePath = fileName;
    if (TextLength(baseDir) > 0) {
        imagePath = TextFormat("%s/%s", baseDir, fileName);
    }
    Texture texture = LoadTexture(imagePath);
    if (texture.id == 0) {
        TraceLog(LOG_ERROR, "TILED: Failed to load layer texture %s", imagePath);
        return;
    }

    // Save the texture data back into the string.
    Texture* texturePtr = (Texture*)MemAlloc(sizeof(Texture));
    texturePtr->format = texture.format;
    texturePtr->height = texture.height;
    texturePtr->id = texture.id;
    texturePtr->mipmaps = texture.mipmaps;
    texturePtr->width = texture.width;
    image->ptr = (const char*)texturePtr;
}

void LoadMapLayerData(cute_tiled_layer_t* layer, const char* baseDir) {
    if (TextIsEqual(layer->type.ptr, "imagelayer")) {
        LoadMapStringTexture(&layer->image, baseDir);
    }
    else if (TextIsEqual(layer->type.ptr, "group")) {
        cute_tiled_layer_t* layer;
        layer = layer->layers;
        while (layer) {
            LoadMapLayerData(layer, baseDir);
            layer = layer->next;
        }
    }
}

void UnloadMapLayerData(cute_tiled_layer_t* layer) {
    if (TextIsEqual(layer->type.ptr, "imagelayer")) {
        Texture* texture = (Texture*)layer->image.ptr;
        UnloadTexture(*texture);
        layer->image.ptr = "";
    }
    else if (TextIsEqual(layer->type.ptr, "group")) {
        cute_tiled_layer_t* layer;
        layer = layer->layers;
        while (layer) {
            UnloadMapLayerData(layer);
            layer = layer->next;
        }
    }
}

Map LoadMapFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir) {
    struct Map output = {0};
    cute_tiled_map_t* map = cute_tiled_load_map_from_memory(fileData, dataSize, 0);
    if (map == NULL) {
        TraceLog(LOG_ERROR, "TILED: Failed to load map data");
        return output;
    }

    // Load all associated images.
    cute_tiled_layer_t* layer = map->layers;
    while (layer) {
        LoadMapLayerData(layer, baseDir);
        layer = layer->next;
    }

    cute_tiled_tileset_t* tileset = map->tilesets;
    while (tileset) {
        LoadMapStringTexture(&tileset->image, baseDir);
        tileset = tileset->next;
    }

    output.map = map;

    // Other Data
    output.backgroundcolor = GetColor(map->backgroundcolor);
    return output;
}

bool IsMapValid(Map map) {
    return map.map != NULL;
}

void UnloadMap(Map map) {
    if (map.map == NULL) {
        return;
    }

    // Unload all the internal raylib data.
    cute_tiled_layer_t* layer = map.map->layers;
    while (layer) {
        UnloadMapLayerData(layer);
        layer = layer->next;
    }

    cute_tiled_tileset_t* tileset = map.map->tilesets;
    while (tileset) {
        if (tileset->image.ptr != NULL) {
            Texture* texture = (Texture*)tileset->image.ptr;
            UnloadTexture(*texture);
        }
        tileset = tileset->next;
    }

    cute_tiled_free_map(map.map);
}

 cute_tiled_tile_descriptor_t* GetTileFromGid(cute_tiled_map_t* map, int gid) {
    cute_tiled_tileset_t* tileset = map->tilesets;
    while (tileset) {
        cute_tiled_tile_descriptor_t* tile = tileset->tiles;
        while (tile) {
            if (tile->tile_index == gid) {
                return tile;
            }
            tile = tile->next;
        }
        tileset = tileset->next;
    }
    return NULL;
}

cute_tiled_tileset_t* GetTilesetFromHash(cute_tiled_tileset_t* tilesets, CUTE_TILED_U64 hash_id) {
    while (tilesets) {
        if (tilesets->image.hash_id == hash_id) {
            return tilesets;
        }
        tilesets = tilesets->next;
    }
    return NULL;
}


/**
 * Retrieves the polygon shape from a given Tiled layer object.
 *
 * This function extracts the points of a polygon defined in a Tiled map layer and returns them as an array of `Vector2`.
 * WARNING: One point is added to close the polygon loop.
 *
 * If the layer is null, has no objects, or the object's type is not a polygon, the function logs an error and returns null.
 *
 * @param object A pointer to the `cute_tiled_object_t` structure representing the Tiled layer object.
 * @return A dynamically allocated array of `Vector2` representing the polygon points, or `NULL` if an error occurs.
 *         The caller is responsible for freeing the allocated memory.
 */
Vector2* GetPolygonShapeFromObject(cute_tiled_object_t* object) {
    if (object == NULL) {
        TraceLog(LOG_ERROR, "TILED: Fail to read layer object");
        return NULL;
    }

    if (!object->vert_type && !object->vert_count) {
        TraceLog(LOG_ERROR, "TILED: Layer type is not polygon");
        return NULL;
    }

    int num_points = object->vert_count + 1; // Add one for looping back
    Vector2* points = (Vector2*)MemAlloc(sizeof(Vector2) * num_points);

    for (int i = 0; i < object->vert_count; i++) {
        points[i].x = object->x + object->vertices[i * 2];
        points[i].y = object->y + object->vertices[i * 2 + 1];
    }
    // First and last point are the same
    points[object->vert_count] = points[0];

    return points;
}


/**
 * Applies an offset to a given array of 2D points, modifying them in place.
 * Each point in the array is adjusted by adding the specified offset.
 * Keep in mind that vert_count is +1 when it comes from GetPolygonShape function
 *
 * @param points Pointer to an array of Vector2 structures representing the points to offset.
 * @param pointCount The number of points in the array.
 * @param offset The offset to apply to all points in the array, represented as a Vector2.
 * @return Pointer to the updated array of points if the input is not NULL,
 *         otherwise returns NULL if the input points array is NULL.
 */
Vector2* SetPolygonOffset(Vector2* points, int pointCount, Vector2 offset) {
    if (points == NULL) {
        return NULL;
    }

    for (int i = 0; i < pointCount; i++) {
        points[i].x += offset.x;
        points[i].y += offset.y;
    }
    return points;
}

cute_tiled_layer_t* GetLayerByName(cute_tiled_map_t* map, const char* layerName) {
    if (map == NULL || layerName == NULL) {
        TraceLog(LOG_ERROR, "TILED: Fail to read map");
        return NULL;
    }
    cute_tiled_layer_t* layer = map->layers;
    while (layer != NULL) {
        if (TextIsEqual(layer->name.ptr, layerName)) {
            return layer;
        }
        layer = layer->next;
    }
    TraceLog(LOG_ERROR, "TILED: No layer %s", layerName);
    return NULL;
}

/**
 * Checks whether a point collides with a polygon shape in a specific layer of a map.
 *
 * @param map The map containing the layers and objects to check.
 * @param point The point to check for collision.
 * @param position The offset position applied to the polygon.
 * @param layerName The name of the layer containing the polygon shape to check.
 * @return True if the point collides with the polygon on the specified layer, false otherwise.
 */
bool CheckCollisionPointLayerPoly(Map map, Vector2 point, Vector2 position, const char* layerName) {
    cute_tiled_layer_t* layer = GetLayerByName(map.map, layerName);
    if (layer == NULL) {
        TraceLog(LOG_ERROR, "TILED: Fail to load layer %s", layerName);
        return NULL;
    }
    cute_tiled_object_t* object = layer->objects;

    while (object != NULL) {
        Vector2* points = GetPolygonShapeFromObject(object);
        if (points == NULL) {
            object = object->next;
            continue;
        }

        int pointCount = object->vert_count + 1;
        points = SetPolygonOffset(points, pointCount, position);

        bool colide = CheckCollisionPointPoly(point, points, pointCount);

        //DrawLineStrip(points, pointCount, RED); // DEBUG
        MemFree(points);

        if (colide) {
            return true;
        }
        object = object->next;
    }
    MemFree(object);
    return false;
}

void DrawMapTile(Texture *texture, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               int dx, int dy, float opacity, /*unsigned int flags,*/ Color tint) {
    DrawTextureRec(*texture, (Rectangle) {sx, sy, sw, sh}, (Vector2) {dx, dy}, ColorAlpha(tint, opacity));
}

void DrawMapLayerTiles(cute_tiled_map_t* map, cute_tiled_layer_t* layer, int posX, int posY, Color tint) {
    Vector2 origin = {0};
    float rotation = 0.0f;
    for (int i = 0; i < layer->height; i++) {
        for (int j = 0; j < layer->width; j++) {
            int gid = layer->data[(i * layer->width) + j];

            // Flags
            int hflip, vflip, dflip;
            cute_tiled_get_flags(gid, &hflip, &vflip, &dflip);
            gid = cute_tiled_unset_flags(gid);

            if (gid == 0) continue;

            // Find tileset
            cute_tiled_tileset_t* ts = map->tilesets;
            cute_tiled_tileset_t* active_tileset = NULL;

            while (ts) {
                if (gid >= ts->firstgid && gid < ts->firstgid + ts->tilecount) {
                    active_tileset = ts;
                    break;
                }
                ts = ts->next;
            }

            if (active_tileset == NULL) {
                continue;
            }

            int localId = gid - active_tileset->firstgid;
            int tileWidth = active_tileset->tilewidth;
            int tileHeight = active_tileset->tileheight;
            int spacing = active_tileset->spacing;
            int margin = active_tileset->margin;
            int columns = active_tileset->columns;

            if (columns <= 0) {
                continue;
            }

            int tileX = localId % columns;
            int tileY = localId / columns;

            Rectangle src = {
                (float)(margin + (tileX * (tileWidth + spacing))),
                (float)(margin + (tileY * (tileHeight + spacing))),
                (float)tileWidth,
                (float)tileHeight
            };

            Rectangle dest = {
                (float)(j * tileWidth + posX),
                (float)(i * tileHeight + posY),
                (float)tileWidth,
                (float)tileHeight
            };

            if (dflip) {
                if (hflip && vflip) {
                    src.height *= -1;
                    rotation = 270.0f;
                }
                else if (hflip) {
                    rotation = 90.0f;
                }
                else if (vflip) {
                    rotation = 270.0f;
                }
                else {
                    rotation = -270.0f;
                    src.height *= -1;
                }
                origin.x = dest.width * 0.5f;
                origin.y = dest.height * 0.5f;
                dest.x += origin.x;
                dest.y += origin.y;
            }
            else {
                if (hflip) src.width *= -1;
                if (vflip) src.height *= -1;
                rotation = 0.0f;
                origin.x = 0.0f;
                origin.y = 0.0f;
            }

            Texture* tex = (Texture*)active_tileset->image.ptr;
            if (tex && tex->id > 0) {
                DrawTexturePro(*tex, src, dest, origin, rotation, ColorAlpha(tint, layer->opacity));
            }
        }
    }
}

void DrawMapLayerImage(cute_tiled_layer_t* layer, int posX, int posY, Color tint) {
    Texture* texture = (Texture*)layer->image.ptr;
    if (!texture || texture->id == 0 || layer->opacity == 0.0f) return;

    int startX = layer->offsetx + posX;
    int startY = layer->offsety + posY;

    int endX = layer->repeatx ? GetScreenWidth() : startX + texture->width;
    int endY = layer->repeaty ? GetScreenHeight() : startY + texture->height;

    int tileStartX = layer->repeatx ? (startX % texture->width) - texture->width : startX;
    int tileStartY = layer->repeaty ? (startY % texture->height) - texture->height : startY;

    for (int y = tileStartY; y < endY; y += texture->height) {
        for (int x = tileStartX; x < endX; x += texture->width) {
            // TODO: Add the layer's `tintcolor`
            DrawTexture(*texture, x, y, ColorAlpha(tint, layer->opacity));
            if (!layer->repeatx) break;
        }
        if (!layer->repeaty) break;
    }
}

void DrawMapLayerObjects(cute_tiled_layer_t* layer, int posX, int posY, Color tint) {
    cute_tiled_object_t* object = layer->objects;
    Color color = ColorTint(GetColor(layer->tintcolor), tint);
    while (object != NULL) {
        if (object->visible == 1) {
            if (object->ellipse) {
                DrawEllipseLines(
                    object->x + posX + object->width / 2,
                    object->y + posY + object->height / 2,
                    object->width / 2,
                    object->height / 2,
                    color);
            }
            else if (object->point) {
                DrawCircle(object->x + posX, object->y + posY, 5.0f, color);
            }
            else if (object->vert_count) {
                // check if its polygon or polyline 1 is polygon 0 is polyline
                if (object->vert_type) {
                    Vector2* points = GetPolygonShapeFromObject(object);
                    // last point is added to be the same as first
                    int num_points = object->vert_count + 1;

                    points = SetPolygonOffset(points, num_points, (Vector2){posX, posY});
                    // DrawRed line, at the moment there is no way to assign individual color per layer
                    DrawLineStrip(points, num_points, RED);

                    MemFree(points);
                }
            }
            // TODO: Add gid drawing
            // TODO: Add Polyline drawing with vertices
        }
        object = object->next;
    }
}

void DrawMapLayer(cute_tiled_map_t* map, cute_tiled_layer_t* layer, int posX, int posY, Color tint) {
	while (layer) {
		if (layer->visible == 1) {
            if (TextIsEqual(layer->type.ptr, "group")) {
                DrawMapLayer(map, layer->layers, layer->offsetx + posX, layer->offsety + posY, tint);
            } else if (TextIsEqual(layer->type.ptr, "objectgroup")) {
                DrawMapLayerObjects(layer, layer->offsetx + posX, layer->offsety + posY, tint);
            } else if (TextIsEqual(layer->type.ptr, "imagelayer")) {
                DrawMapLayerImage(layer, posX, posY, tint);
            } else if (TextIsEqual(layer->type.ptr, "tilelayer")) {
                DrawMapLayerTiles(map, layer, layer->offsetx + posX, layer->offsety + posY, tint);
            }
		}
		layer = layer->next;
	}
}

void DrawMap(Map map, int posX, int posY, Color tint) {
    DrawRectangle(posX, posY, map.map->width, map.map->height, map.backgroundcolor);
    DrawMapLayer(map.map, map.map->layers, posX, posY, tint);
}

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_TILED_IMPLEMENTATION_ONCE
#endif  // RAYLIB_TILED_IMPLEMENTATION
