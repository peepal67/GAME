// Keypad.h
#pragma once
#include "Interactable.h"

class Keypad : public Interactable {
public:
    Keypad(Vector3 pos, int d0, int d1, int d2, int d3,
           const char* modelPath = "assets/models/keypad.obj", float scale = 0.3f);
    ~Keypad();

    void Interact(Player& player) override;   // E: increments current digit
    void ConfirmDigit();                       // moves to next slot, checks code on 4th
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

    Model model;
    float scale;
};