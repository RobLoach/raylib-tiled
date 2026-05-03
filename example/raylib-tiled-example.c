#include "raylib.h"

#define RAYLIB_TILED_IMPLEMENTATION
#include "raylib-tiled.h"

int main() {

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 600;
    Vector2 position = {0, 0};

    InitWindow(screenWidth, screenHeight, "raylib-tiled example");
    SetTargetFPS(60);

    Map map = LoadMap("resources/desert.json");
    if (!IsMapValid(map)) {
        TraceLog(LOG_ERROR, "Failed to load Map");
        return 1;
    }

    // Write layer names in map - DEBUG
    cute_tiled_layer_t* layer = map.map->layers;
    while (layer) {
        TraceLog(LOG_INFO, "Layer name: %s", layer->name);
        layer = layer->next;
    }

    Vector2 ballPosition = { (float)screenWidth/2, (float)screenHeight/2 };

    //---------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // if (IsKeyDown(KEY_LEFT)) {
        //     position.x += 2;
        // }
        // if (IsKeyDown(KEY_UP)) {
        //     position.y += 2;
        // }
        // if (IsKeyDown(KEY_RIGHT)) {
        //     position.x -= 2;
        // }
        // if (IsKeyDown(KEY_DOWN)) {
        //     position.y -= 2;
        // }
        //
        //--------------------------------------------------
        if (IsKeyDown(KEY_RIGHT)) ballPosition.x += 2.0f;
        if (IsKeyDown(KEY_LEFT)) ballPosition.x -= 2.0f;
        if (IsKeyDown(KEY_UP)) ballPosition.y -= 2.0f;
        if (IsKeyDown(KEY_DOWN)) ballPosition.y += 2.0f;
        //--------------------------------------------------

        if (IsMouseButtonDown(0)) {
            Vector2 delta = GetMouseDelta();
            position.x += delta.x;
            position.y += delta.y;
        }

        // check for collision
        if (CheckCollisionPointLayerPoly(map, ballPosition, position, "Polygon")) {
            TraceLog(LOG_INFO, "Collision! Visible");
        }
        if (CheckCollisionPointLayerPoly(map, ballPosition, position, "InvisiblePolygon")) {
            TraceLog(LOG_INFO, "Collision! Invisible");
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawMap(map, (int)position.x, (int)position.y, RAYWHITE);

            DrawCircleV(ballPosition, 15, MAROON);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadMap(map);             // Unload the Tiled map

    CloseWindow();                // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}
