#include "Keypad.h"

Keypad::Keypad(Vector3 pos, int d0, int d1, int d2, int d3, Model* mdl, float sc)
    : position(pos), model(mdl), scale(sc)
{
    target[0] = d0; target[1] = d1; target[2] = d2; target[3] = d3;
}

void Keypad::Interact(Player& player) {
    (void)player;
    if (solved) return;
    digits[currentSlot] = (digits[currentSlot] + 1) % 10;
}

void Keypad::ConfirmDigit() {
    if (solved) return;
    currentSlot = (currentSlot + 1) % 4;

    if (currentSlot == 0) {
        bool match = true;
        for (int i = 0; i < 4; i++) if (digits[i] != target[i]) match = false;
        if (match) solved = true;
    }
}

BoundingBox Keypad::GetBoundingBox() {
    return {
        { position.x - 0.3f, position.y - 0.3f, position.z - 0.1f },
        { position.x + 0.3f, position.y + 0.3f, position.z + 0.1f }
    };
}

void Keypad::Draw() const {
    Color tint = solved ? GREEN : WHITE;
    DrawModel(*model, position, scale, tint);
}