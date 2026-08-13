#include "raylib.h"
#include "raymath.h"
#include "Item.h"
#include "Inventory.h"
#include "Interactable.h"
#include "Pickup.h"
#include "Player.h"
#include "Door.h"
#include "Wall.h"
#include <iostream>
#include <vector>

int main(void)
{
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "Void Protocol");
    DisableCursor();

    const float roomWidth = 16.0f;
    const float roomDepth = 18.0f;
    const float roomHeight = 4.0f;
    const float wallThickness = 0.25f;
    const float wallCenterY = roomHeight / 2.0f;

    // The camera starts at a normal eye height inside the room.
    Player player({0, 1.75f, 6});


    // Test pickups
    Item keyItem = { ITEM_KEY_RUSTY, "Rusty Key" };
    // A larger enclosed room. The doorway is built into the back wall, with
    // wall sections to its left, right, and above it.
    const float doorWidth = 2.0f;
    const float doorHeight = 2.4f;
    const float doorCenterY = doorHeight / 2.0f;
    const float halfRoomWidth = roomWidth / 2.0f;
    const float halfRoomDepth = roomDepth / 2.0f;
    const float sideWallWidth = (roomWidth - doorWidth) / 2.0f;
    const float topWallHeight = roomHeight - doorHeight;

    Wall leftWall({-halfRoomWidth, wallCenterY, 0}, {wallThickness, roomHeight, roomDepth}, DARKGRAY);
    Wall rightWall({halfRoomWidth, wallCenterY, 0}, {wallThickness, roomHeight, roomDepth}, DARKGRAY);
    Wall frontWall({0, wallCenterY, halfRoomDepth}, {roomWidth, roomHeight, wallThickness}, DARKGRAY);
    Wall backWallLeft({-(doorWidth + sideWallWidth) / 2.0f, wallCenterY, -halfRoomDepth},
                      {sideWallWidth, roomHeight, wallThickness}, DARKGRAY);
    Wall backWallRight({(doorWidth + sideWallWidth) / 2.0f, wallCenterY, -halfRoomDepth},
                       {sideWallWidth, roomHeight, wallThickness}, DARKGRAY);
    Wall wallAboveDoor({0, doorHeight + topWallHeight / 2.0f, -halfRoomDepth},
                       {doorWidth, topWallHeight, wallThickness}, DARKGRAY);
    Wall roof({0, roomHeight, 0}, {roomWidth, wallThickness, roomDepth}, GRAY);
    std::vector<Wall*> walls = {
        &leftWall, &rightWall, &frontWall, &backWallLeft, &backWallRight,
        &wallAboveDoor, &roof
    };

    Door testDoor({0, doorCenterY, -halfRoomDepth}, {doorWidth, doorHeight, 0.18f}, true, ITEM_KEY_RUSTY);

    std::vector<Pickup*> pickups;
 
    pickups.push_back(new Pickup({2, 0.25f, 0}, keyItem, "assets/models/Worn_Key2.obj"));

    float maxInteractDistance = 3.0f;

    SetTargetFPS(60);

    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())
    {
        // Update
        //----------------------------------------------------------------------------------
        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        float dt = GetFrameTime();
        std::vector<BoundingBox> solidObstacles;
        solidObstacles.reserve(walls.size() + 1);

        for (const Wall* wall : walls) {
            solidObstacles.push_back(wall->GetBoundingBox());
        }

        // A closed door is solid. Pressing E changes the state immediately,
        // so it stops blocking movement on the next frame when open.
        if (testDoor.IsBlocking()) {
            solidObstacles.push_back(testDoor.GetBoundingBox());
        }

        player.Update(dt, solidObstacles);

        Ray ray = {
            player.camera.position,
            Vector3Normalize(Vector3Subtract(player.camera.target, player.camera.position))
        };

        Pickup* focusedPickup = nullptr;
        Door* focusedDoor = nullptr;
        float closestDist = maxInteractDistance;

        for (auto* p : pickups) {
            if (p->collected) continue;
            RayCollision hit = GetRayCollisionBox(ray, p->GetBoundingBox());
            if (hit.hit && hit.distance < closestDist) {
                closestDist = hit.distance;
                focusedPickup = p;
                focusedDoor = nullptr;
            }
        }

        RayCollision doorHit = GetRayCollisionBox(ray, testDoor.GetBoundingBox());
        if (doorHit.hit && doorHit.distance < closestDist) {
            closestDist = doorHit.distance;
            focusedDoor = &testDoor;
            focusedPickup = nullptr;
        }

        if (IsKeyPressed(KEY_E)) {
            if (focusedPickup) {
                focusedPickup->Interact(player);
                std::cout << "Picked up: " << focusedPickup->item.name << "\n";
            } else if (focusedDoor) {
                focusedDoor->Interact(player);
            }
        }
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(BLUE);

            BeginMode3D(player.camera);

                DrawPlane({0, 0, 0}, {roomWidth, roomDepth}, LIGHTGRAY);

                for (const Wall* wall : walls) {
                    wall->Draw();
                }

                for (auto* p : pickups) {
                    p->Draw();
                }



                testDoor.Draw();



            EndMode3D();

                if (focusedPickup || focusedDoor) {
                    DrawText("Press E", 20, 20, 20, WHITE);
                }

            DrawText(TextFormat("FPS: %d", GetFPS()), screenWidth - 100, 10, 20, WHITE);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}

