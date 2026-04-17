#include "Search.h"
#include <queue>
#include <chrono>
#include <unordered_map>
#include <iostream>
#include <limits>
#include <deque>
#include <cmath>

namespace std
{
    template<> struct hash<std::pair<int,int>>
    {
        std::size_t operator()( const std::pair<int,int> & p) const noexcept
        {
            hash<int> hasher;
            return hasher(p.first) ^ (hasher(p.second)<<1);
        }
    };
}

// Greedy
struct Compare
{
    bool operator()(const std::pair<std::pair<int,int>, float>& a,
                    const std::pair<std::pair<int,int>, float>& b)
    {
        return a.second > b.second;
    }
};

// A*
struct CompareAStar
{
    bool operator()(const std::pair<std::pair<int,int>, float>& a,
                    const std::pair<std::pair<int,int>, float>& b)
    {
        return a.second > b.second;
    }
};

// Heurística Manhattan
float Search::Heuristic(std::pair<int,int> a, std::pair<int,int> b)
{
    return abs(a.first - b.first) + abs(a.second - b.second);
}

std::vector<std::pair<int,int>> Search::reconstruct(
    const std::unordered_map<std::pair<int,int>,std::pair<int,int>> &pathCache,
    const std::pair<int,int> &start)
{
    std::deque<std::pair<int,int>> nodes;
    auto node = start;

    while(true){
        nodes.push_front(node);

        if(pathCache.find(node) == pathCache.end())
            break;

        node = pathCache.at(node);
    }

    std::vector<std::pair<int,int>> vec;
    for(auto p:nodes){
        vec.push_back(p);
    }
    return vec;
}

// BFS 
std::vector<std::pair<int,int>> Search::BFS(
    const Map& map,
    std::pair<int,int> start,
    std::pair<int,int> goal)
{
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    bool visited[map.h][map.w]{false};
    std::queue<std::pair<int,int>> OPEN;
    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;

    OPEN.push(start);
    visited[start.first][start.second] = true;

    while(!OPEN.empty()){
        auto pos = OPEN.front();
        OPEN.pop();

        if(pos == goal){
            return reconstruct(pathCache,pos);
        }

        for(auto dir:dirs){
            std::pair<int,int> newPos = pos;

            newPos.first += dir.first;
            newPos.second += dir.second;

            if(newPos.first < 0 || newPos.first >= map.h ||
               newPos.second < 0 || newPos.second >= map.w)
                continue;

            if(map._map[newPos.first][newPos.second] == 1)
                continue;

            if(visited[newPos.first][newPos.second])
                continue;

            visited[newPos.first][newPos.second] = true;
            OPEN.push(newPos);

            pathCache[newPos] = pos;
        }
    }

    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}

// Greedy BFS
std::vector<std::pair<int,int>> Search::greedyBFS(
    const Map& map,
    std::pair<int,int> start,
    std::pair<int,int> goal)
{
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w,false));

    std::priority_queue<
        std::pair<std::pair<int,int>, float>,
        std::vector<std::pair<std::pair<int,int>, float>>,
        Compare
    > OPEN;

    std::unordered_map<std::pair<int,int>,std::pair<int,int>> pathCache;

    OPEN.push({start, Heuristic(start, goal)});

    while(!OPEN.empty()){
        auto current = OPEN.top().first;
        OPEN.pop();

        if(visited[current.first][current.second])
            continue;

        visited[current.first][current.second] = true;

        if(current == goal){
            return reconstruct(pathCache,current);
        }

        for(auto dir:dirs){
            std::pair<int,int> newPos = current;

            newPos.first += dir.first;
            newPos.second += dir.second;

            if(newPos.first < 0 || newPos.first >= map.h ||
               newPos.second < 0 || newPos.second >= map.w)
                continue;

            if(map._map[newPos.first][newPos.second] == 1)
                continue;

            if(visited[newPos.first][newPos.second])
                continue;

            pathCache[newPos] = current;

            float h = Heuristic(newPos, goal);
            OPEN.push({newPos, h});
        }
    }

    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}

// A*
std::vector<std::pair<int,int>> Search::AStar(
    const Map& map,
    std::pair<int,int> start,
    std::pair<int,int> goal)
{
    std::pair<int,int> dirs[]{{-1,0},{0,1},{1,0},{0,-1}};

    std::vector<std::vector<bool>> visited(map.h, std::vector<bool>(map.w,false));

    std::priority_queue<
        std::pair<std::pair<int,int>, float>,
        std::vector<std::pair<std::pair<int,int>, float>>,
        CompareAStar
    > OPEN;

    std::unordered_map<std::pair<int,int>, std::pair<int,int>> pathCache;
    std::unordered_map<std::pair<int,int>, float> gCost;

    gCost[start] = 0;
    OPEN.push({start, Heuristic(start, goal)});

    while(!OPEN.empty()){
        auto current = OPEN.top().first;
        OPEN.pop();

        if(visited[current.first][current.second])
            continue;

        visited[current.first][current.second] = true;

        if(current == goal){
            return reconstruct(pathCache,current);
        }

        for(auto dir:dirs){
            std::pair<int,int> newPos = current;

            newPos.first += dir.first;
            newPos.second += dir.second;

            if(newPos.first < 0 || newPos.first >= map.h ||
               newPos.second < 0 || newPos.second >= map.w)
                continue;

            if(map._map[newPos.first][newPos.second] == 1)
                continue;

            float newG = gCost[current] + 1;

            if(!gCost.count(newPos) || newG < gCost[newPos])
            {
                gCost[newPos] = newG;
                pathCache[newPos] = current;

                float f = newG + Heuristic(newPos, goal);
                OPEN.push({newPos, f});
            }
        }
    }

    std::vector<std::pair<int,int>> path;
    path.push_back(start);
    path.push_back(goal);
    return path;
}