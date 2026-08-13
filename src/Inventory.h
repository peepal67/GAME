#pragma once
#include "Item.h"
#include <vector>

class Inventory {
    private:
    std::vector<Item> items;
public:
    void AddItem( Item& item);
    bool HasItem(int id) ;
    void GetItems();


};