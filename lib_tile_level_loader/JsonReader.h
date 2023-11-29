//
// Created by Cian Farrell on 29/11/2023.
//

#include <SFML/Graphics.hpp>
#include <iostream>
#include <nlohmann/json.hpp>
#include <fstream>
#include "LevelSystem.h"


#ifndef DROP_POD_JSONREADER_H
#define DROP_POD_JSONREADER_H

using json = nlohmann::json;




class JsonReader {
public:

    static LevelSystem::Level loadLevel(std::string path);

private:


};


#endif //DROP_POD_JSONREADER_H
