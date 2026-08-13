#include "raylib.h"
#include "raymath.h"
#include "Item.h"
#include "Inventory.h"
#include "Interactable.h"
#include "Pickup.h"
#include "Player.h"
#include "Door.h"
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

    Player player({0, 3, 3});


    // Test pickups
    Item keyItem = { ITEM_KEY_RUSTY, "Rusty Key" };
    Door testDoor({0, 1, -4}, {1, 2, 0.2f}, true, ITEM_KEY_RUSTY);

    std::vector<Pickup*> pickups;
 
    pickups.push_back(new Pickup({2, 1, 0}, keyItem, "assets/models/Worn_Key2.obj"));

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
        player.Update(dt);

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

                DrawPlane({0, 0, 0}, {20, 20}, DARKGRAY);


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

