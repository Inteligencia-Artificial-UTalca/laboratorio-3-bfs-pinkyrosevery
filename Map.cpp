#include "Map.h"
#include <iostream>
#include <fstream>

Map::Map():h(0),w(0){

}

Map::Map(std::string filename){
    
    std::ifstream file(filename);

    if (!file.is_open()) {
        std::cout << "Error al abrir archivo\n";
        h = w = 0;
        return;
    }

    file >> h >> w;

    _map.resize(h, std::vector<int>(w));

    for (int i = 0; i < h; i++) {
        for (int j = 0; j < w; j++) {
            file >> _map[i][j];
        }
    }

    file.close();
}

Map::Map(const Map& rhs):h(rhs.h),w(rhs.w),_map(rhs._map){

}

Map::~Map(){
    
}

Map& Map::operator=(const Map& rhs){
    if(this==&rhs)return *this;
    h=rhs.h;
    w=rhs.w;
    _map=rhs._map;
    return *this;
}