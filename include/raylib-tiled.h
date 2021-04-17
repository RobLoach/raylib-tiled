/**********************************************************************************************
*
*   raylib-physfs - Integrate PhysFS with raylib, allowing to load images, audio and fonts from data archives.
*
*   Copyright 2021 Rob Loach (@RobLoach)
*
*   DEPENDENCIES:
*       raylib https://www.raylib.com/
*       physfs https://www.icculus.org/physfs/
*
*   LICENSE: zlib/libpng
*
*   raylib-physfs is licensed under an unmodified zlib/libpng license, which is an OSI-certified,
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
Map LoadMapFromMemory(const unsigned char *fileData, int dataSize);
bool IsMapReady(Map map);
void UnloadTiled(Map map);

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
    Map map = LoadMapFromMemory(fileData, bytesRead);
    UnloadFileData(fileData);
    return map;
}

Map LoadMapFromMemory(const unsigned char *fileData, int dataSize) {
    struct Map map;
    map.map = cute_tiled_load_map_from_memory(fileData, dataSize, 0);
    if (map.map == NULL) {
        TraceLog(LOG_ERROR, "TILED: Failed to load map data");
        return map;
    }
    return map;
}

bool IsMapReady(Map map) {
    return map.map != NULL;
}

void UnloadTiled(Map map) {
    if (map.map == NULL) {
        return;
    }
    cute_tiled_free_map(map.map);
}

#ifdef __cplusplus
}
#endif

#endif  // RAYLIB_TILED_IMPLEMENTATION_ONCE
#endif  // RAYLIB_TILED_IMPLEMENTATION


