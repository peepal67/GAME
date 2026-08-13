#pragma once
#include "Interactable.h"
#include "Item.h"

class Pickup : public Interactable {
public:
    Pickup(Vector3 pos, Item item, const char* modelPath);
    ~Pickup();

    void Interact(Player& player) override;
    BoundingBox GetBoundingBox() override;
    void Draw() const;

    Vector3 position;
    Item item;
    bool collected = false;

    Model model;
    float boundsSize = 0.4f;
};