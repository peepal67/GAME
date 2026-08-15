#pragma once
#include "raylib.h"
#include "Wall.h"
#include "Door.h"
#include "Pickup.h"
#include <vector>

class Room {
public:
    ~Room();

    // Door sits on the far (-Z) wall by default, near (+Z) wall solid -
    // matches a room you walk further into. Set doorOnNearSide=true to
    // flip it (room entered from a corridor approaching from +Z). Set
    // buildFarWall=false to skip the opposite wall entirely - used when a
    // neighboring room's own wall+door serves as the shared boundary.
    void BuildRoomWithDoor(Vector3 center, float width, float depth, float height,
                            float doorWidth, float doorHeight, bool doorLocked, int requiredKeyId,
                            Texture2D* wallTexture, Texture2D* doorTexture,
                            Texture2D* floorTexture, Texture2D* ceilingTexture,
                            const char* doorOpenSoundPath = nullptr,
                            bool doorOnNearSide = false,
                            bool buildFarWall = true,
                            float wallThickness = 0.25f);

    void BuildHallway(Vector3 center, float width, float depth, float height,
                       Texture2D* wallTexture, Texture2D* floorTexture, Texture2D* ceilingTexture,
                       float wallThickness = 0.25f);

    void DrawRoomWithDoor() const;
    void DrawHallway() const;

    void AddPickup(Pickup* pickup);

    std::vector<BoundingBox> GetSolidObstacles() const;

    std::vector<Wall*>& GetWalls()     { return walls; }
    std::vector<Door*>& GetDoors()     { return doors; }
    std::vector<Pickup*>& GetPickups() { return pickups; }

private:
    std::vector<Wall*> walls;
    std::vector<Door*> doors;
    std::vector<Pickup*> pickups;
};