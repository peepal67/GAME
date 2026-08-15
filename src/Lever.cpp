#include "Lever.h"

static Color ToRaylibColor(GameColor c) {
    switch (c) {
        case GameColor::GC_RED:    return RED;
        case GameColor::GC_BLUE:   return BLUE;
        case GameColor::GC_ORANGE: return ORANGE;
    }
    return WHITE;
}

Lever::Lever(Vector3 pos, GameColor a, GameColor b)
    : position(pos), optionOne(a), optionTwo(b), current(a) {}

Lever::~Lever() {}

void Lever::Interact(Player& player) {
    (void)player;
    current = (current == optionOne) ? optionTwo : optionOne;
}

BoundingBox Lever::GetBoundingBox() {
    return {
        { position.x - 0.2f, position.y - 0.4f, position.z - 0.2f },
        { position.x + 0.2f, position.y + 0.4f, position.z + 0.2f }
    };
}

void Lever::Draw() const {
    DrawCube(position, 0.15f, 0.8f, 0.15f, DARKGRAY);
    DrawCube({position.x, position.y + 0.4f, position.z}, 0.25f, 0.25f, 0.25f, ToRaylibColor(current));
}