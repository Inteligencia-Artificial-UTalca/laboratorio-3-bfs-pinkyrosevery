#pragma once
#include "Map.h"
#include <array>




class ColorMap:public Map{
    std::array<int,5> colors;
public:
    ColorMap(const Map& rhs);
    void print() const;                 
    void print(std::vector<std::pair<int,int>>) const; 


};