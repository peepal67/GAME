#pragma once
#include "raylib.h"

class Player;

class Interactable 
{
public:
    virtual ~Interactable()=default;
    virtual void Interact(Player& player) = 0;
virtual BoundingBox GetBoundingBox() = 0;};

