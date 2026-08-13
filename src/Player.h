#pragma once
#include "raylib.h"
#include "Inventory.h"
#include <vector>

class Player {
public:
    Player(Vector3 startPos);

    // Moves the first-person camera, but prevents its player-sized footprint
    // from entering any solid world object.
    void Update(float dt, const std::vector<BoundingBox>& solidObstacles);
    Camera3D camera;

    Inventory& GetInventory() { return inventory; }

private:
    bool WouldCollide(Vector3 candidatePosition,
                      const std::vector<BoundingBox>& solidObstacles) const;

    Inventory inventory;
    float collisionRadius = 0.30f;
};









