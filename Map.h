#pragma once
#include <string>
#include <vector>

class Map{

public:
    int h;
    int w;
    std::vector<std::vector<int>> _map;

public:
    Map();                             
    Map(std::string filename);      
    Map(const Map& rhs);                
    ~Map();                          
    Map& operator=(const Map& rhs);     
   
    void print() const;                 
    void print(std::vector<std::pair<int,int>>) const; 

    friend bool operator==(const Map& lhs, const Map& rhs); 
    friend class Search;                
};

bool operator==(const Map& lhs, const Map& rhs);   