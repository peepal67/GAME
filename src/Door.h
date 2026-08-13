#pragma once
#include "Interactable.h"

class Door : public Interactable {
public:
    Door(Vector3 pos, Vector3 size, bool locked, int requiredKeyId = -1, Texture2D* texture = nullptr);
    ~Door();

    void Interact(Player& player) override;
    BoundingBox GetBoundingBox() override;
    void Draw() const;
    bool IsBlocking() const { return !isOpen; }

    Vector3 position;
    Vector3 size;
    bool locked;
    bool isOpen = false;

private:
    Model model;
    int requiredKeyId;
    bool hasTexture = false;
};
