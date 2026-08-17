#include "raylib.h"
#include "raymath.h"
#include "Item.h"
#include "Player.h"
#include "Room.h"
#include "Lever.h"
#include "Note.h"
#include "Keypad.h"
#include <iostream>
#include <vector>
#include <string>
#include <cmath>

namespace {
const char* kWallTexturePath     = "assets/textures/dark_rock_diff_1k.png";
const char* kDoorTexturePath     = "assets/textures/rusty_metal_grid_diff_1k.png";
const char* kFloorTexturePath    = "assets/textures/black_painted_planks_diff_1k.png";
const char* kCeilingTexturePath  = "assets/textures/dark_rock_diff_1k.png";
const char* kPortalTexturePath   = "assets/textures/portal.png";
const char* kKeyModelPath        = "assets/models/Worn_Key2.obj";
const char* kZombieModelPath     = "assets/models/zombie.obj";
const char* kFootstepsPath       = "assets/sounds/footsteps_trimmed.wav";
const char* kBackgroundMusicPath = "assets/music/background.mp3";
const char* kWinSoundPath        = "assets/music/congratulations.wav";
const char* ktableModelPath  = "assets/models/Table.obj";
const char* kchandelierModelPath = "assets/models/Chandelier.obj";
const char* kcandleModelPath = "assets/models/candle1.obj";
const char* kmirrorModelPath = "assets/models/Mirror.obj";
const char* kkeypadModelPath = "assets/models/keypad.obj";


}

struct ZombieHazard {
    Vector3 position;
    float scale;
};

struct QuizGate {
    Wall* correctSide;
    Wall* wrongSide;
    const char* question;
    float triggerZ;
};

struct StaticProp {
    Model* model;
    Vector3 position;
    float scale;
    float rotationY; // degrees, for slight variation
};

int main(void)
{
    const int screenWidth = 800;
    const int screenHeight = 450;
    InitWindow(screenWidth, screenHeight, "Void Protocol");
    InitAudioDevice();
    DisableCursor();

    Sound footstepsSound = LoadSound(kFootstepsPath);
    Sound winSound = LoadSound(kWinSoundPath);
    Music backgroundMusic = LoadMusicStream(kBackgroundMusicPath);
    SetMusicVolume(backgroundMusic, 0.3f);
    PlayMusicStream(backgroundMusic);
    Texture2D wallTexture    = LoadTexture(kWallTexturePath);
    Texture2D doorTexture    = LoadTexture(kDoorTexturePath);
    Texture2D floorTexture   = LoadTexture(kFloorTexturePath);
    Texture2D ceilingTexture = LoadTexture(kCeilingTexturePath);
    Texture2D portalTexture  = LoadTexture(kPortalTexturePath);
    Model zombieModel = LoadModel(kZombieModelPath);
    Model tableModel = LoadModel(ktableModelPath);
    Model chandelierModel = LoadModel(kchandelierModelPath);
    Model candleModel = LoadModel(kcandleModelPath);
    Model mirrorModel = LoadModel(kmirrorModelPath);
    Model keypadModel = LoadModel(kkeypadModelPath);
    

    if (wallTexture.id == 0 || doorTexture.id == 0 || floorTexture.id == 0 || ceilingTexture.id == 0 || portalTexture.id == 0) {
        CloseWindow();
        return 1;
    }

    SetTextureWrap(wallTexture, TEXTURE_WRAP_REPEAT);
    SetTextureWrap(doorTexture, TEXTURE_WRAP_REPEAT);
    SetTextureWrap(floorTexture, TEXTURE_WRAP_REPEAT);
    SetTextureWrap(ceilingTexture, TEXTURE_WRAP_REPEAT);
    SetTextureFilter(wallTexture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(doorTexture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(floorTexture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(ceilingTexture, TEXTURE_FILTER_BILINEAR);
    SetTextureFilter(portalTexture, TEXTURE_FILTER_BILINEAR);
    Item keyItem = { ITEM_KEY_RUSTY, "Rusty Key" };

    std::vector<StaticProp> staticProps = {
    // ---- Room A: table under/near the key, chandelier as ceiling focal point ----
    { &tableModel,      {2.0f, 0.0f, 1.0f},   0.02, 12.0f },   // near the key pickup
    { &chandelierModel, {0.0f, 3.0f, 0.0f},  0.07f, 0.0f },    // main room ceiling centerpiece

    // ---- Room A candles, corners ----
    { &candleModel, {-11.0f, 0.0f, 11.0f}, 0.0035f, 0.0f },
    { &candleModel, { 11.0f, 0.0f, -11.0f}, 0.0035f, 180.0f },
    {&mirrorModel, {7.0f, -4.7f, 11.0f}, 22.0f, 150.0f},

    // ---- Hallway: candle near Room C doorway threshold ----
    { &candleModel, {-7.0f, 0.0f, -29.5f}, 0.0035f, 270.0f },
    { &candleModel, {-7.0f, 0.0f, -34.5f}, 0.0035f, 270.0f },


    { &candleModel, {5.5f, 0.0f, -21.0f}, 0.0035f, 90.0f },
    { &candleModel, {-7.0f, 0.0f, -19.0f}, 0.0035f, 270.0f },

    { &candleModel, {4.0f, 0.0f, -38.0f}, 0.0035f, 180.0f },
    { &candleModel, {-4.0f, 0.0f, -38.0f}, 0.0035f, 180.0f },



    // ---- Room D: candles in all four corners ----
    { &candleModel, {-11.0f, 0.0f, -71.0f}, 0.0035f, 180.0f },
    { &candleModel, { 11.0f, 0.0f, -71.0f}, 0.0035f, 180.0f },
    { &candleModel, {-4.0f, 0.0f, -90.0f}, 0.0035f, 180.0f },
    { &candleModel, { 4.0f, 0.0f, -90.0f}, 0.0035f, 180.0f },
    { &candleModel, {-11.0f, 0.0f, -85.0f}, 0.0035f, 180.0f },
    { &candleModel, { 11.0f, 0.0f, -85.0f}, 0.0035f, 180.0f },

    // ---- Room D: table under keypad area, framing it ----
    { &tableModel, {0.0f, 0.0f, -78.0f}, 0.02f, 0.0f },
 };

    // ===================== ROOM A =====================
    Room roomA;
    roomA.BuildRoomWithDoor({0, 0, 0}, 26.0f, 26.0f, 6.5f, 4.0f, 6.0f,
                             true, ITEM_KEY_RUSTY, &wallTexture, &doorTexture, &floorTexture, &ceilingTexture,
                             "assets/sounds/jumpscare.wav");
    roomA.AddPickup(new Pickup({7, 0.25f, 12.5f}, keyItem, kKeyModelPath, 3.0f));

    std::vector<Wall*> decorPatches;
    //on front wall
    decorPatches.push_back(new Wall({-10.0f, 3.0f, -12.0f}, {0.6f, 0.6f, 0.02f}, RED));
    decorPatches.push_back(new Wall({-8.0f, 3.0f, -12.0f}, {0.6f, 0.6f, 0.02f}, BLUE));
    decorPatches.push_back(new Wall({-6.0f, 3.0f, -12.0f},   {0.6f, 0.6f, 0.02f}, ORANGE));

    for (auto* w : decorPatches) w->isPassable = true;

    // ===================== HALLWAY =====================
    const float hallNearZ = -13.0f, hallFarZ = -39.0f;
    const float hallHalfWidth = 8.0f;
    const float hallHeight = 6.5f;
    const float hallWallY = hallHeight / 2.0f;
    const float roomCGapZ = -32.0f;
    const float roomCGapHalf = 1.5f;

    std::vector<Wall*> hallwayWalls;
    hallwayWalls.push_back(new Wall({hallHalfWidth, hallWallY, (hallNearZ + hallFarZ) / 2.0f},
                                     {0.25f, hallHeight, hallNearZ - hallFarZ}, WHITE, &wallTexture));

    float leftSeg1Len = hallNearZ - (roomCGapZ + roomCGapHalf);
    hallwayWalls.push_back(new Wall({-hallHalfWidth, hallWallY, hallNearZ - leftSeg1Len / 2.0f},
                                     {0.25f, hallHeight, leftSeg1Len}, WHITE, &wallTexture));
    float leftSeg2Len = (roomCGapZ - roomCGapHalf) - hallFarZ;
    hallwayWalls.push_back(new Wall({-hallHalfWidth, hallWallY, hallFarZ + leftSeg2Len / 2.0f},
                                     {0.25f, hallHeight, leftSeg2Len}, WHITE, &wallTexture));

    Wall* hallFloor = new Wall({0, 0, (hallNearZ + hallFarZ) / 2.0f},
                                {hallHalfWidth * 2, 0.25f, hallNearZ - hallFarZ}, WHITE, &floorTexture);
    hallFloor->isFloorOrCeiling = true;
    Wall* hallRoof = new Wall({0, hallHeight, (hallNearZ + hallFarZ) / 2.0f},
                               {hallHalfWidth * 2, 0.25f, hallNearZ - hallFarZ}, WHITE, &ceilingTexture);
    hallRoof->isFloorOrCeiling = true;
    hallwayWalls.push_back(hallFloor);
    hallwayWalls.push_back(hallRoof);

    const Vector3 lever1StartPos = {5.0f, 1.2f, -17.0f};
    const Vector3 lever2StartPos = {5.0f, 1.2f, -21.0f};
    const Vector3 lever3StartPos = {5.0f, 1.2f, -25.0f};
    Lever lever1(lever1StartPos, GameColor::GC_RED, GameColor::GC_BLUE);
    Lever lever2(lever2StartPos, GameColor::GC_ORANGE, GameColor::GC_BLUE);
    Lever lever3(lever3StartPos, GameColor::GC_RED, GameColor::GC_ORANGE);
    std::vector<Lever*> levers = { &lever1, &lever2, &lever3 };

    Door doorToC({-hallHalfWidth, 3.0f, roomCGapZ}, {0.18f, 6.0f, roomCGapHalf * 2}, true, -1, &doorTexture);
    bool exitCodeKnown = false;

    std::vector<Wall*> roomCWalls;
    const float rcCenterX = -hallHalfWidth - 2.5f, rcCenterZ = roomCGapZ;
    roomCWalls.push_back(new Wall({rcCenterX - 2.5f, hallWallY, rcCenterZ}, {0.25f, hallHeight, 5.0f}, WHITE, &wallTexture));
    roomCWalls.push_back(new Wall({rcCenterX, hallWallY, rcCenterZ - 2.5f}, {5.0f, hallHeight, 0.25f}, WHITE, &wallTexture));
    roomCWalls.push_back(new Wall({rcCenterX, hallWallY, rcCenterZ + 2.5f}, {5.0f, hallHeight, 0.25f}, WHITE, &wallTexture));
    Wall* rcFloor = new Wall({rcCenterX, 0, rcCenterZ}, {5.0f, 0.25f, 5.0f}, WHITE, &floorTexture);
    rcFloor->isFloorOrCeiling = true;
    Wall* rcRoof = new Wall({rcCenterX, hallHeight, rcCenterZ}, {5.0f, 0.25f, 5.0f}, WHITE, &ceilingTexture);
    rcRoof->isFloorOrCeiling = true;
    roomCWalls.push_back(rcFloor);
    roomCWalls.push_back(rcRoof);

    // ===================== ROOM B (quiz gates) =====================
    Room roomB;
    roomB.BuildRoomWithDoor({0, 0, -52}, 26.0f, 26.0f, 6.5f, 4.0f, 6.0f,
                             true, -1, &wallTexture, &doorTexture, &floorTexture, &ceilingTexture,
                             nullptr, true, false);

    // Both sides of every gate are now walkable (isPassable = true) - the
    // wrong side doesn't block movement, it kills the player on touch
    // instead. See the death check below in the main loop.
    Wall* q1Correct = new Wall({-6.5f, 3.25f, -45}, {13.0f, 6.5f, 0.2f}, WHITE, &portalTexture);   q1Correct->isPassable = true;
    Wall* q1Wrong   = new Wall({ 6.5f, 3.25f, -45}, {13.0f, 6.5f, 0.2f}, WHITE, &portalTexture);   q1Wrong->isPassable = true;

    Wall* q2Correct = new Wall({ 6.5f, 3.25f, -52}, {13.0f, 6.5f, 0.2f}, WHITE, &portalTexture);   q2Correct->isPassable = true;
    Wall* q2Wrong   = new Wall({-6.5f, 3.25f, -52}, {13.0f, 6.5f, 0.2f}, WHITE, &portalTexture);   q2Wrong->isPassable = true;

    Wall* q3Correct = new Wall({ 6.5f, 3.25f, -59}, {13.0f, 6.5f, 0.2f}, WHITE, &portalTexture);   q3Correct->isPassable = true;
    Wall* q3Wrong   = new Wall({-6.5f, 3.25f, -59}, {13.0f, 6.5f, 0.2f}, WHITE, &portalTexture);   q3Wrong->isPassable = true;

    std::vector<Wall*> quizGates = { q1Correct, q1Wrong, q2Correct, q2Wrong, q3Correct, q3Wrong };

    std::vector<QuizGate> quizQuestions = {
        { q1Correct, q1Wrong, "Q1: Which keyword declares a constant in C++?\nLEFT: const   |   RIGHT: static", -45 },
        { q2Correct, q2Wrong, "Q2: Which operator accesses a member via pointer?\nLEFT: .   |   RIGHT: ->", -52 },
        { q3Correct, q3Wrong, "Q3: Which loop always runs at least once?\nLEFT: for   |   RIGHT: do-while", -59 },
    };

    // ===================== ROOM D =====================
    Room roomD;
    roomD.BuildRoomWithDoor({0, 0, -78}, 26.0f, 26.0f, 6.5f, 4.0f, 6.0f,
                             false, -1, &wallTexture, &doorTexture, &floorTexture, &ceilingTexture,
                             nullptr, true, true);

    std::vector<Note*> notes;
    notes.push_back(new Note({-4, 0.2f, 0}, "The old records show the year was 19__."));
    notes.push_back(new Note({4, 0.2f, -70},  "Third digit: count the skulls"));
    notes.push_back(new Note({-4, 0.2f, -86}, "Final digit :- suiiiiii"));

    const Vector3 keypadStartPos = {0, 2.0f, -78};
    Keypad keypad(keypadStartPos, 1, 9, 6, 7, &keypadModel);

    // ===================== Zombie hazards =====================
    std::vector<ZombieHazard> zombies = {
        {{-4.0f, 2, -15.0f}, 2.0f},
        {{ 3.5f, 2, -17.5f}, 2.0f},
        {{-2.0f, 2, -20.0f}, 2.5f},
        {{ 6.0f, 2, -18.0f}, 2.0f},
        {{-6.0f, 2, -24.0f}, 2.0f},
        {{ 2.0f, 2, -26.5f}, 3.5f},
        {{-3.5f, 2, -28.0f}, 2.5f},
        {{ 5.0f, 2, -30.0f}, 2.0f},
        {{ 1.5f, 2, -37.0f}, 3.5f},
    };
    const float zombieDeathRadius = 1.2f;

    // ===================== Player + state =====================
    const Vector3 spawnPoint = {-1, 1.75f, -11.5};
    Player player(spawnPoint);
    float maxInteractDistance = 3.0f;
    std::string displayedNoteText;

    bool isDead = false;
    float deathTimer = 0.0f;
    bool hasWon = false;

    std::vector<Door*> allDoors;
    for (auto* d : roomA.GetDoors()) allDoors.push_back(d);
    for (auto* d : roomB.GetDoors()) allDoors.push_back(d);
    for (auto* d : roomD.GetDoors()) allDoors.push_back(d);
    allDoors.push_back(&doorToC);

    auto ResetLevel = [&]() {
        player.camera.position = spawnPoint;
        player.camera.target = { spawnPoint.x + 1, spawnPoint.y, spawnPoint.z + 12 };
        player.GetInventory().Clear(); 

        roomA.GetDoors()[0]->locked = true;
        roomA.GetDoors()[0]->isOpen = false;
        if (!roomB.GetDoors().empty()) {
            roomB.GetDoors()[0]->locked = true;
            roomB.GetDoors()[0]->isOpen = false;
        }
        if (!roomD.GetDoors().empty()) {
        roomD.GetDoors()[0]->isOpen = false;  
        }

        doorToC.locked = true;
        doorToC.isOpen = false;

        lever1 = Lever(lever1StartPos, GameColor::GC_RED, GameColor::GC_BLUE);
        lever2 = Lever(lever2StartPos, GameColor::GC_ORANGE, GameColor::GC_BLUE);
        lever3 = Lever(lever3StartPos, GameColor::GC_RED, GameColor::GC_ORANGE);

        keypad = Keypad(keypadStartPos, 1, 9, 6, 7, &keypadModel);

        exitCodeKnown = false;
        displayedNoteText.clear();

        if (!roomA.GetPickups().empty() && roomA.GetPickups()[0]->collected) {
            roomA.GetPickups()[0]->collected = false;
        }
    };

    SetTargetFPS(60);

    while (!WindowShouldClose())
    {
        UpdateMusicStream(backgroundMusic);

        if (!isDead && !hasWon && (IsKeyDown(KEY_W) || IsKeyDown(KEY_A) || IsKeyDown(KEY_S) || IsKeyDown(KEY_D))) {
            if (!IsSoundPlaying(footstepsSound)) { PlaySound(footstepsSound); }
        }
        if (IsKeyPressed(KEY_F)) ToggleFullscreen();

        float dt = GetFrameTime();

        bool leversSolved = lever1.GetColor() == GameColor::GC_RED &&
                            lever2.GetColor() == GameColor::GC_BLUE &&
                            lever3.GetColor() == GameColor::GC_ORANGE;
        if (leversSolved && !roomB.GetDoors().empty()) roomB.GetDoors()[0]->locked = false;

        if (keypad.IsSolved()) exitCodeKnown = true;
        if (exitCodeKnown) doorToC.locked = false;

        std::vector<BoundingBox> allObstacles;
        auto addRoom = [&](Room& r) {
            auto obs = r.GetSolidObstacles();
            allObstacles.insert(allObstacles.end(), obs.begin(), obs.end());
        };
        addRoom(roomA); addRoom(roomB); addRoom(roomD);

        for (auto* w : hallwayWalls) if (!w->isFloorOrCeiling) allObstacles.push_back(w->GetBoundingBox());
        for (auto* w : roomCWalls)   if (!w->isFloorOrCeiling) allObstacles.push_back(w->GetBoundingBox());
        if (doorToC.IsBlocking()) allObstacles.push_back(doorToC.GetBoundingBox());
        for (auto* g : quizGates) if (!g->isPassable) allObstacles.push_back(g->GetBoundingBox());

        if (!isDead && !hasWon) {
            player.Update(dt, allObstacles);
        }

        if (!isDead && !hasWon) {
            for (const auto& z : zombies) {
                float dx = player.camera.position.x - z.position.x;
                float dz = player.camera.position.z - z.position.z;
                float distSq = dx*dx + dz*dz;
                float radius = zombieDeathRadius + (z.scale * 0.15f);
                if (distSq < radius * radius) {
                    isDead = true;
                    deathTimer = 2.0f;
                    break;
                }
            }
        }

        // Wrong-side quiz portal touch = death. Both gate sides are now
        // walkable, so this checks specifically against each QuizGate's
        // wrongSide wall rather than filtering on isPassable.
        if (!isDead && !hasWon) {
            for (const auto& q : quizQuestions) {
                BoundingBox box = q.wrongSide->GetBoundingBox();
                Vector3 pos = player.camera.position;
                bool inside = pos.x > box.min.x - 0.3f && pos.x < box.max.x + 0.3f &&
                              pos.z > box.min.z - 0.3f && pos.z < box.max.z + 0.3f;
                if (inside) {
                    isDead = true;
                    deathTimer = 2.0f;
                    break;
                }
            }
        }

        if (isDead) {
            deathTimer -= dt;
            if (deathTimer <= 0.0f) {
                ResetLevel();
                isDead = false;
            }
        }

        Ray ray = {
            player.camera.position,
            Vector3Normalize(Vector3Subtract(player.camera.target, player.camera.position))
        };

        Pickup* focusedPickup = nullptr;
        Door* focusedDoor = nullptr;
        Lever* focusedLever = nullptr;
        Keypad* focusedKeypad = nullptr;
        Note* focusedNote = nullptr;
        float closestDist = maxInteractDistance;

        if (!isDead && !hasWon) {
            for (auto* p : roomA.GetPickups()) {
                if (p->collected) continue;
                RayCollision hit = GetRayCollisionBox(ray, p->GetBoundingBox());
                if (hit.hit && hit.distance < closestDist) {
                    closestDist = hit.distance; focusedPickup = p;
                    focusedDoor = nullptr; focusedLever = nullptr; focusedKeypad = nullptr; focusedNote = nullptr;
                }
            }

            for (auto* d : allDoors) {
                RayCollision hit = GetRayCollisionBox(ray, d->GetBoundingBox());
                if (hit.hit && hit.distance < closestDist) {
                    closestDist = hit.distance; focusedDoor = d;
                    focusedPickup = nullptr; focusedLever = nullptr; focusedKeypad = nullptr; focusedNote = nullptr;
                }
            }

            for (auto* l : levers) {
                RayCollision hit = GetRayCollisionBox(ray, l->GetBoundingBox());
                if (hit.hit && hit.distance < closestDist) {
                    closestDist = hit.distance; focusedLever = l;
                    focusedPickup = nullptr; focusedDoor = nullptr; focusedKeypad = nullptr; focusedNote = nullptr;
                }
            }

            for (auto* n : notes) {
                RayCollision hit = GetRayCollisionBox(ray, n->GetBoundingBox());
                if (hit.hit && hit.distance < closestDist) {
                    closestDist = hit.distance; focusedNote = n;
                    focusedPickup = nullptr; focusedDoor = nullptr; focusedLever = nullptr; focusedKeypad = nullptr;
                }
            }

            RayCollision hitKeypad = GetRayCollisionBox(ray, keypad.GetBoundingBox());
            if (hitKeypad.hit && hitKeypad.distance < closestDist) {
                closestDist = hitKeypad.distance; focusedKeypad = &keypad;
                focusedPickup = nullptr; focusedDoor = nullptr; focusedLever = nullptr; focusedNote = nullptr;
            }

            if (IsKeyPressed(KEY_E)) {
                if (focusedPickup) {
                    focusedPickup->Interact(player);
                    std::cout << "Picked up: " << focusedPickup->item.name << "\n";
                } else if (focusedDoor) {
                    bool wasOpen = focusedDoor->isOpen;
                    focusedDoor->Interact(player);

                    // Win condition: Door C transitioning from closed to open.
                    if (focusedDoor == &doorToC && !wasOpen && focusedDoor->isOpen) {
                        hasWon = true;
                        PlaySound(winSound);
                    }
                } else if (focusedLever) {
                    focusedLever->Interact(player);
                } else if (focusedKeypad) {
                    focusedKeypad->Interact(player);
                } else if (focusedNote) {
                    displayedNoteText = focusedNote->GetText();
                }
            }
            if (focusedKeypad && IsKeyPressed(KEY_ENTER)) {
                focusedKeypad->ConfirmDigit();
            }
        }

        // Read screen size fresh every frame so fullscreen toggling (KEY_F)
        // keeps all UI text/overlays correctly positioned and sized.
        int currentScreenWidth = GetScreenWidth();
        int currentScreenHeight = GetScreenHeight();

        BeginDrawing();
            ClearBackground(Color{22, 28, 38, 255});

            BeginMode3D(player.camera);
                roomA.DrawRoomWithDoor();
                for (auto* w : decorPatches) w->Draw();

                for (auto* w : hallwayWalls) w->Draw();
                doorToC.Draw();
                for (auto* l : levers) l->Draw();
                for (auto* w : roomCWalls) w->Draw();

                for (const auto& z : zombies) {
                    DrawModel(zombieModel, z.position, z.scale, WHITE);
                }

                roomB.DrawRoomWithDoor();
                for (auto* g : quizGates) g->Draw();

                roomD.DrawRoomWithDoor();
                for (auto* n : notes) n->Draw();
                keypad.Draw();
                for (const auto& prop : staticProps) {
                DrawModelEx(*prop.model, prop.position, {0, 1, 0}, prop.rotationY,
                {prop.scale, prop.scale, prop.scale}, WHITE);
                }
            EndMode3D();

            if (hasWon) {
                DrawRectangle(0, 0, currentScreenWidth, currentScreenHeight, Color{10, 10, 10, 230});
                const char* winMsg = "YOU WIN!";
                int winFontSize = currentScreenHeight / 6;
                int winTextWidth = MeasureText(winMsg, winFontSize);
                DrawText(winMsg,
                         currentScreenWidth / 2 - winTextWidth / 2,
                         currentScreenHeight / 2 - winFontSize / 2,
                         winFontSize, GOLD);
            } else if (!isDead) {
                if (focusedPickup || focusedDoor || focusedLever || focusedKeypad || focusedNote) {
                    if (focusedDoor && focusedDoor->locked) {
                        DrawText("Door is locked", 20, 40, 30, WHITE);
                    } else {
                        DrawText("Press E", 20, 40, 30, WHITE);
                    }
                }
                if (focusedKeypad) {
                    DrawText(TextFormat("Code: %d %d %d %d  (ENTER to confirm digit)",
                              keypad.GetDigit(0), keypad.GetDigit(1), keypad.GetDigit(2), keypad.GetDigit(3)),
                              20, 70, 30, YELLOW);
                }
                if (!displayedNoteText.empty()) {
                    DrawText(displayedNoteText.c_str(), 20, currentScreenHeight - 110, 30, WHITE);
                }

                // Question text - large, centered on screen while inside
                // the quiz stretch and near a gate's Z position.
                for (const auto& q : quizQuestions) {
                    if (fabsf(player.camera.position.z - q.triggerZ) < 4.0f &&
                        player.camera.position.z < -39 && player.camera.position.z > -66) {

                        int questionFontSize = 28;
                        int textWidth = MeasureText(q.question, questionFontSize);

                        int boxWidth = textWidth + 60;
                        int boxHeight = 100;
                        int boxX = currentScreenWidth / 2 - boxWidth / 2;
                        int boxY = currentScreenHeight / 2 - boxHeight / 2 - 60;

                        DrawRectangle(boxX, boxY, boxWidth, boxHeight, Color{0, 0, 0, 200});
                        DrawText(q.question,
                                 currentScreenWidth / 2 - textWidth / 2,
                                 boxY + boxHeight / 2 - questionFontSize / 2 - 12,
                                 questionFontSize, WHITE);
                        break;
                    }
                }

                DrawText(TextFormat("X: %.2f  Y: %.2f  Z: %.2f",
                          player.camera.position.x, player.camera.position.y, player.camera.position.z),
                          20, 10, 18, YELLOW);
            } else {
                DrawRectangle(0, 0, currentScreenWidth, currentScreenHeight, Color{0, 0, 0, 200});
                const char* msg = "YOU HAVE DIED";
                int fontSize = currentScreenHeight / 10;
                int textWidth = MeasureText(msg, fontSize);
                DrawText(msg, currentScreenWidth/2 - textWidth/2, currentScreenHeight/2 - fontSize/2, fontSize, RED);
            }

            DrawText(TextFormat("FPS: %d", GetFPS()), currentScreenWidth - 100, 10, 20, WHITE);
        EndDrawing();
    }

    for (auto* w : decorPatches) delete w;
    for (auto* w : hallwayWalls) delete w;
    for (auto* w : roomCWalls) delete w;
    for (auto* g : quizGates) delete g;
    for (auto* n : notes) delete n;

    UnloadTexture(wallTexture);
    UnloadTexture(doorTexture);
    UnloadTexture(floorTexture);
    UnloadTexture(ceilingTexture);
    UnloadTexture(portalTexture);
    UnloadMusicStream(backgroundMusic);
    UnloadSound(footstepsSound);
    UnloadSound(winSound);
    UnloadModel(zombieModel);
    UnloadModel(tableModel);
    UnloadModel(chandelierModel);
    UnloadModel(candleModel);
    UnloadModel(mirrorModel);
    UnloadModel(keypadModel);
    CloseAudioDevice();
    CloseWindow();
    return 0;
}