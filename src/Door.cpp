#include "Door.h"
#include "Player.h"
#include <iostream>

Door::Door(Vector3 pos, Vector3 sz, bool lock, int keyId, Texture2D* texture,const char* openSoundPath)
    : position(pos), size(sz), locked(lock), requiredKeyId(keyId)
{
    Mesh m = GenMeshCube(size.x, size.y, size.z);
    model = LoadModelFromMesh(m);



    if (texture != nullptr) {
        SetMaterialTexture(&model.materials[0], MATERIAL_MAP_ALBEDO, *texture);
        model.materials[0].maps[MATERIAL_MAP_ALBEDO].color = WHITE;
    }

     if (openSoundPath != nullptr) {
        openSound = LoadSound(openSoundPath);
        hasOpenSound = true;
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

        if (isOpen && hasOpenSound) {
        PlaySound(openSound);
    }
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
    // A light tint keeps the actual wood-plank image visible in both states.
    Color tint = isOpen ? Color{ 140, 255, 160, 255 }
                        : Color{66, 37, 37,255};

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
