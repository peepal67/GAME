#include "Inventory.h"
#include<iostream>

void Inventory::AddItem( Item& item) { items.push_back(item); }

bool Inventory::HasItem(int id) {
    for (auto& i : items) {
        if (i.id == id) {
            return true;
        }
    }
    return false;
}

void Inventory::GetItems()
{
    std::cout<<Inventory::HasItem(1)<<std::endl;
}