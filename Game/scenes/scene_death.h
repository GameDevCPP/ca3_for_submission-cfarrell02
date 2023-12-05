//
// Created by cianf on 05/12/2023.
//

#ifndef DROP_POD_SCENE_DEATH_H
#define DROP_POD_SCENE_DEATH_H


#include "engine.h"

class DeathScene : public Scene {
public:
    void Load() override;

    void UnLoad() override;

    void Update(const double &dt) override;

    void Render() override;

    void setScore(int score) { _score = score; }

    int getScore() { return _score; }

    void setLevel(int level) { _level = level; }

protected:
    int _score;
    int _level;
    std::shared_ptr<Entity> _startButton;

};


#endif //DROP_POD_SCENE_DEATH_H
