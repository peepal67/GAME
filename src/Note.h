#pragma once
#include "Interactable.h"
#include <string>

class Note : public Interactable {
public:
    Note(Vector3 pos, std::string text);
    ~Note();

    void Interact(Player& player) override;
    BoundingBox GetBoundingBox() override;
    void Draw() const;

    const std::string& GetText() const { return text; }

private:
    Vector3 position;
    std::string text;
};