#pragma once
#include "raylib.h"
#include "Inventory.h"

class Player {
public:
    Player(Vector3 startPos);

    void Update(float dt);
    Camera3D camera;

    Inventory& GetInventory() { return inventory; }

private:
    Inventory inventory;
};









