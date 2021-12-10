/**********************************************************************************************
*
*   raylib-tiled - Integrate tiled with raylib, allowing to load images, audio and fonts from data archives.
*
*   Copyright 2021 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       raylib https://www.raylib.com/
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
bool IsMapReady(Map map);
void UnloadTiled(Map map);
void DrawMap(Map map, int posX, int posY, Color tint);

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
#define STRPOOL_EMBEDDED_STRNICMP(s1, s2, len) (TextIsEqual(TextToLower(s1), TextToLower(s2)))

#define STRPOOL_EMBEDDED_MALLOC(ctx, size) (MemAlloc(size))
#define STRPOOL_EMBEDDED_FREE(ctx, ptr) (MemFree(ptr))

// Override how Cute attempts to load files.
#define CUTE_TILED_STDIO
#define CUTE_TILED_SNPRINTF(s, n, format, arg1, arg2) TextCopy(error, TextFormat(format, arg1, arg2))
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
Texture* LoadMapStringTexture(cute_tiled_string_t* image, const char* baseDir) {
    if (image->ptr[0] == '|') {
        if (image->hash_id != 0) {
            Texture* texture = (Texture*)image->hash_id;
            return texture;
        } else {
            return NULL;
        }
    }

    const char* fileName = image->ptr;
    const char* imagePath = fileName;
    if (TextLength(baseDir) > 0) {
        imagePath = TextFormat("%s/%s", baseDir, fileName);
    }
    Texture texture = LoadTexture(imagePath);
    if (texture.id == 0) {
        image->hash_id = 0;
        image->ptr = "|";
        TraceLog(LOG_ERROR, "TILED: Failed to load layer texture %s", imagePath);
        return NULL;
    }

    // Save the texture data back into the string.
    Texture* texturePtr = (Texture*)MemAlloc(sizeof(Texture));
    texturePtr->format = texture.format;
    texturePtr->height = texture.height;
    texturePtr->id = texture.id;
    texturePtr->mipmaps = texture.mipmaps;
    texturePtr->width = texture.width;
    //image->ptr = (const char*)texturePtr;
    image->hash_id = (unsigned long long)texturePtr;
    image->ptr = "<";
    TraceLog(LOG_INFO, "%i", texturePtr->width);
}

void LoadMapLayerData(cute_tiled_layer_t* layer, const char* baseDir) {
    if (TextIsEqual(layer->type.ptr, "imagelayer")) {
        LoadMapStringTexture(&layer->image, baseDir);
    }
    else if (TextIsEqual(layer->type.ptr, "group")) {
        cute_tiled_layer_t* layer = layer->layers;
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
        layer->image.hash_id = 0;
    }
    else if (TextIsEqual(layer->type.ptr, "group")) {
        cute_tiled_layer_t* layer = layer->layers;
        while (layer) {
            UnloadMapLayerData(layer);
            layer = layer->next;
        }
    }
}

Map LoadMapFromMemory(const unsigned char *fileData, int dataSize, const char* baseDir) {
    struct Map output;
    cute_tiled_map_t* map = cute_tiled_load_map_from_memory(fileData, dataSize, 0);
    if (map == NULL) {
        TraceLog(LOG_ERROR, "TILED: Failed to load map data");
        output.map = NULL;
        return output;
    }

    // Load all tileset data
    cute_tiled_tileset_t* tileset = map->tilesets;
    while (tileset) {
        LoadMapStringTexture(&tileset->image, baseDir);
        tileset = tileset->next;
    }

    // Load all associated images.
    cute_tiled_layer_t* layer = map->layers;
    while (layer) {
        LoadMapLayerData(layer, baseDir);

        int* data = layer->data;
        for (int i = 0; i < layer->data_count; i++) {
            int gid = layer->data[i];
        }
        layer = layer->next;
    }

    output.map = map;
    return output;
}

bool IsMapReady(Map map) {
    return map.map != NULL;
}

void UnloadTiled(Map map) {
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

 cute_tiled_tile_descriptor_t* GetTileFromGid(cute_tiled_map_t* map, int gid, Texture** texture, cute_tiled_tileset_t** ts) {
    cute_tiled_tileset_t* tileset = map->tilesets;
    while (tileset) {
        cute_tiled_tile_descriptor_t* tile = tileset->tiles;
        while (tile) {
            if (tile->tile_index == gid) {
                *texture = (Texture*)tileset->image.hash_id;
                *ts = tileset;
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
            TraceLog(LOG_ERROR, "FDSAFDS");
            return tilesets;
        }
        tilesets = tilesets->next;
    }
    return NULL;
}

void DrawMapTile(Texture *texture, unsigned int sx, unsigned int sy, unsigned int sw, unsigned int sh,
               int dx, int dy, float opacity, /*unsigned int flags,*/ Color tint) {
    Color newTint = ColorAlpha(tint, opacity);
    DrawTextureRec(*texture, (Rectangle) {sx, sy, sw, sh}, (Vector2) {dx, dy}, newTint);
}

void DrawMapLayerTiles(cute_tiled_map_t* map, cute_tiled_layer_t* layer, int posX, int posY, Color tint) {
	unsigned long i, j;
	unsigned int gid, x, y, w, h, flags;
	float op;
	cute_tiled_tileset_t *ts;
	cute_tiled_string_t *im;
	void* image;
    Texture* texture;
	op = layer->opacity;
	for (i = 0; i < layer->height; i++) {
		for (j = 0; j < layer->width; j++) {
			gid = layer->data[(i * layer->width) + j];

            cute_tiled_tile_descriptor_t* tile = GetTileFromGid(map, gid, &texture, &ts);
            
            if (tile != NULL && texture != NULL && ts != NULL) {
                
                x = ts->margin + (j * ts->tilewidth)  + (j * ts->spacing);
                y = ts->margin + (i * ts->tileheight)  + (i * ts->spacing);
                // x  = map->tiles[gid]->ul_x;
                // y  = map->tiles[gid]->ul_y;
                w  = ts->tilewidth;
                h  = ts->tileheight;
                // flags = (layer->content.gids[(i*map->width)+j]) & ~TMX_FLIP_BITS_REMOVAL;
                //DrawMapTile((Texture*)ts->image.ptr, x, y, w, h, j*ts->tilewidth + posX, i*ts->tileheight + posY, op, /*flags,*/ tint);

                TraceLog(LOG_INFO, "OMGOGM %ix%i %iw%ih", texture->width, texture->height, w, h);
                DrawRectangle(j*ts->tilewidth + posX, i*ts->tileheight + posY, w,h, RED);
                
                DrawMapTile(texture, x, y, w, h, j*ts->tilewidth + posX, i*ts->tileheight + posY, op, /*flags,*/ tint);
            }
		}
	}
}

void DrawMapLayerImage(cute_tiled_string_t image, int posX, int posY, Color tint) {
    Texture* texture = (Texture*)image.ptr;
    DrawTexture(*texture, posX, posY, tint);
}

void DrawMapLayer(cute_tiled_map_t* map, cute_tiled_layer_t* layer, int posX, int posY, Color tint) {
	while (layer) {
		if (layer->visible == 1) {
            if (TextIsEqual(layer->type.ptr, "group")) {
                DrawMapLayer(map, layer->layers, layer->offsetx + posX, layer->offsety + posY, tint);
            } else if (TextIsEqual(layer->type.ptr, "objectgroup")) {
                //DrawMapLayerObjects(layer->objects, layer->offsetx + posX, layer->offsety + posY, tint);
            } else if (TextIsEqual(layer->type.ptr, "imagelayer")) {
                DrawMapLayerImage(layer->image, layer->offsetx + posX, layer->offsety + posY, tint);
            } else if (TextIsEqual(layer->type.ptr, "tilelayer")) {
                DrawMapLayerTiles(map, layer, layer->offsetx + posX, layer->offsety + posY, tint);
            }
		}
		layer = layer->next;
	}
}

void DrawMap(Map map, int posX, int posY, Color tint) {
    Color background = GetColor(map.map->backgroundcolor);
    DrawRectangle(posX, posY, map.map->width, map.map->height, background);
    DrawMapLayer(map.map, map.map->layers, posX, posY, tint);
}

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_TILED_IMPLEMENTATION_ONCE
#endif  // RAYLIB_TILED_IMPLEMENTATION
