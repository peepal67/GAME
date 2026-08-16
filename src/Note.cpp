#include "Note.h"

Note::Note(Vector3 pos, std::string t) : position(pos), text(std::move(t)) {}
Note::~Note() {}

void Note::Interact(Player& player) {
    (void)player;
    // main.cpp checks GetText() via the focused note pointer to display it
}

BoundingBox Note::GetBoundingBox() {
    return {
        { position.x - 0.3f, position.y - 0.2f, position.z - 0.05f },
        { position.x + 0.3f, position.y + 0.2f, position.z + 0.05f }
    };
}

void Note::Draw() const {
    DrawCube(position, 0.75f,0.01f , 0.55f, BEIGE); // paper-like slab, swap for texture later
}