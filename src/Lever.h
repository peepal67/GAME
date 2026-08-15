#pragma once
#include "Interactable.h"

enum class GameColor { GC_RED, GC_BLUE, GC_ORANGE };

class Lever : public Interactable {
public:
    Lever(Vector3 pos, GameColor optionOne, GameColor optionTwo);
    ~Lever();

    void Interact(Player& player) override;
    BoundingBox GetBoundingBox() override;
    void Draw() const;

    GameColor GetColor() const { return current; }

private:
    Vector3 position;
    GameColor optionOne, optionTwo, current;
};