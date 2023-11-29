//
// Created by Cian Farrell on 29/11/2023.
//

#include "JsonReader.h"
#include <string>
#include <iostream>

using namespace std;

LevelSystem::Level JsonReader::loadLevel(std::string path) {
    std::ifstream i(path);
    json j;
    i >> j;
    LevelSystem::Level level;
    level.name = j["name"];
    map<string,int> tileMap = j["tileMap"];
    for (auto& x: tileMap) {
        level.tileMap.insert(pair<string , int>(x.first, x.second));
    }
    vector<vector<string>> mapString;
    string rawMap = "";
    for (auto& x: j["map"]) {
        vector<string> row;
        for (auto& y: x) {
            row.push_back(y);
            rawMap +=( (string) y).substr(0,1);
        }
        rawMap += "\n";
        mapString.push_back(row);
    }
    level.map = mapString;
    level.rawMap = rawMap;

    return level;
}