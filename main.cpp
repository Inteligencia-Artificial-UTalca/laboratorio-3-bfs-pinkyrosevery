#include "Map.h"
#include "Search.h"
#include "ColorMap.h"
#include <iostream>
#include <cstdlib>

bool esNumero(char* str){
    for(int i = 0; str[i] != '\0'; i++){
        if(str[i] < '0' || str[i] > '9'){
            return false;
        }
    }
    return true;
}

int main(int argc, char *argv[]){

    if (argc != 6) {
        std::cout << "Uso: ./programa mapa.txt x1 y1 x2 y2\n";
        return 1;
    }

    if(!esNumero(argv[2]) || !esNumero(argv[3]) ||
       !esNumero(argv[4]) || !esNumero(argv[5])){
        std::cout << "Error: argumentos no numericos\n";
        return 1;
    }

    Map map(argv[1]);
    ColorMap colorMap(map);

    int x1 = atoi(argv[2]);
    int y1 = atoi(argv[3]);
    int x2 = atoi(argv[4]);
    int y2 = atoi(argv[5]);

    if(x1 == x2 && y1 == y2){
        std::cout << "Distancia: 0\n";
        return 0;
    }


    if(x1 < 0 || y1 < 0 || x2 < 0 || y2 < 0){
        std::cout << "Error: coordenadas negativas\n";
        return 1;
    }

    if(x1 >= map.h || y1 >= map.w || x2 >= map.h || y2 >= map.w){
        std::cout << "Error: coordenadas fuera del mapa\n";
        return 1;
    }

    if(map._map[x1][y1] == 1 || map._map[x2][y2] == 1){
        std::cout << "Error: inicio o fin es un muro\n";
        return 1;
    }

    colorMap.print();

    auto path = Search::greedyBFS(map, {x1,y1}, {x2,y2});
    colorMap.print(path);

    if(path.size() <= 2 && path[0] != path[1]){
        std::cout << "No hay camino valido\n";
    } else {
        int distance = path.size() - 1;
        std::cout << "Distancia: " << distance << std::endl;
    }

    return 0;
}