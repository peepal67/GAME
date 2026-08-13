#include "Door.h"
#include "Player.h"
#include <iostream>

Door::Door(Vector3 pos, Vector3 sz, bool lock, int keyId, Texture2D* texture)
    : position(pos), size(sz), locked(lock), requiredKeyId(keyId)
{
    Mesh m = GenMeshCube(size.x, size.y, size.z);
    model = LoadModelFromMesh(m);

    if (texture != nullptr) {
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = *texture;
        hasTexture = true;
    }
}

Door::~Door() {
    UnloadModel(model);
}

void Door::Interact(Player& player) {
    if (locked) {
        if (requiredKeyId != -1 && player.GetInventory().HasItem(requiredKeyId)) {
            locked = false;
            std::cout << "Unlocked door!\n";
        } else {
            std::cout << "Door is locked.\n";
            return;
        }
    }
    isOpen = !isOpen;
    std::cout << (isOpen ? "Door opened\n" : "Door closed\n");
}

BoundingBox Door::GetBoundingBox() {
    return {
        { position.x - size.x/2, position.y - size.y/2, position.z - size.z/2 },
        { position.x + size.x/2, position.y + size.y/2, position.z + size.z/2 }
    };
}

void Door::Draw() const {
    Color tint = hasTexture ? WHITE : (locked ? RED : (isOpen ? GREEN : BROWN));

    Vector3 drawPos = position;
    if (isOpen) {
        drawPos.z += size.z + 0.5f; // slides forward when open
    }

    DrawModel(model, drawPos, 1.0f, tint);
}