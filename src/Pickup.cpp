#include "Pickup.h"
#include "Player.h"

Pickup::Pickup(Vector3 pos, Item it, const char* modelPath, float sc)
    : position(pos), item(it), scale(sc)
{
    model = LoadModel(modelPath);
}

Pickup::~Pickup() {
    UnloadModel(model);
}

void Pickup::Interact(Player& player) {
    player.GetInventory().AddItem(item);
    collected = true;
}

BoundingBox Pickup::GetBoundingBox() {
    return {
        { position.x - boundsSize/2, position.y - boundsSize/2, position.z - boundsSize/2 },
        { position.x + boundsSize/2, position.y + boundsSize/2, position.z + boundsSize/2 }
    };
}

void Pickup::Draw() const {
    if (!collected) {
        DrawModel(model, position, scale, WHITE);
    }
}