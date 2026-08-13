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

    // Pressing E changes directly between these two full states. The open
    // door stays visible; it simply turns sideways in the doorway.
    float rotation = isOpen ? 90.0f : 0.0f;
    DrawModelEx(model, position, { 0.0f, 1.0f, 0.0f }, rotation,
                { 1.0f, 1.0f, 1.0f }, tint);
}
