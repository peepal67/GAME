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
    Vector3 boxCenter = position;
    Vector3 boxSize = size;

    if (isOpen) {
        // The open door has turned around its left hinge, so its width now
        // runs along the Z axis beside the doorway.
        boxCenter = {
            position.x - size.x / 2.0f,
            position.y,
            position.z + size.x / 2.0f
        };
        boxSize = { size.z, size.y, size.x };
    }

    return {
        { boxCenter.x - boxSize.x/2, boxCenter.y - boxSize.y/2, boxCenter.z - boxSize.z/2 },
        { boxCenter.x + boxSize.x/2, boxCenter.y + boxSize.y/2, boxCenter.z + boxSize.z/2 }
    };
}

void Door::Draw() const {
    // Keep the wood grain visible while making the two door states obvious.
    Color tint = isOpen ? GREEN : RED;

    // Pressing E changes directly between full states. When open, the door
    // pivots about its left edge and rests visibly inside the room.
    Vector3 drawPosition = position;
    float rotation = 0.0f;

    if (isOpen) {
        drawPosition = {
            position.x - size.x / 2.0f,
            position.y,
            position.z + size.x / 2.0f
        };
        rotation = -90.0f;
    }

    DrawModelEx(model, drawPosition, { 0.0f, 1.0f, 0.0f }, rotation,
                { 1.0f, 1.0f, 1.0f }, tint);
}
