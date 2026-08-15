#include "Inventory.h"

void Inventory::AddItem(const Item& item) { items.push_back(item); }

bool Inventory::HasItem(int id) const {
    for (const auto& i : items) {
        if (i.id == id) return true;
    }
    return false;
}