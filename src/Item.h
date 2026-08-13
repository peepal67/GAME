#pragma once
#include <string>

enum itemId{
    ITEM_KEY_RUSTY=1, ITEM_BATTERY=2
};

struct Item {
    int id;
    std::string name;
};