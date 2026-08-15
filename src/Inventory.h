#pragma once
#include "Item.h"
#include <vector>

class Inventory {
public:
    void AddItem(const Item& item);
    bool HasItem(int id) const;
    const std::vector<Item>& GetItems() const { return items; }

private:
    std::vector<Item> items;
};