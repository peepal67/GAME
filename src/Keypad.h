#pragma once
#include "Interactable.h"

class Keypad : public Interactable {
public:
    Keypad(Vector3 pos, int d0, int d1, int d2, int d3, Model* model, float scale = 0.3f);

    void Interact(Player& player) override;
    void ConfirmDigit();
    BoundingBox GetBoundingBox() override;
    void Draw() const;

    bool IsSolved() const { return solved; }
    int GetDigit(int slot) const { return digits[slot]; }
    int GetCurrentSlot() const { return currentSlot; }

private:
    Vector3 position;
    int digits[4] = {0, 0, 0, 0};
    int target[4];
    int currentSlot = 0;
    bool solved = false;

    Model* model;   // pointer to a model owned/loaded in main.cpp
    float scale;
};