//
// Created by Cian Farrell on 29/11/2023.
//

#include "JsonReader.h"
using namespace std;

LevelSystem::Level JsonReader::loadLevel(std::string path) {
    std::ifstream i(path);
    json j;
    i >> j;
    LevelSystem::Level level;
    level.name = j["name"];
    map<string,int> tileMap = j["tileMap"];
    for (auto& x: tileMap) {
        level.tileMap.insert(pair<char, int>((char) x.first[0], x.second));
    }
    vector<string> map = j["map"];
    string mapString = "";
    for (auto& x: map) {
        mapString += x + "\n";
    }
    level.map = mapString;


    return level;
}