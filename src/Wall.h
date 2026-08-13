#pragma once
#include "raylib.h"

class Wall {
public:
    Wall(Vector3 pos, Vector3 size, Color color, Texture2D* texture = nullptr);
    ~Wall();

    void Draw() const;
    BoundingBox GetBoundingBox() const;

    Vector3 position;
    Vector3 size;

private:
    Model model;
};