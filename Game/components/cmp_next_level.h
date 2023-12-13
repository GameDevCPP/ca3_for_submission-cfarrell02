//
// Created by cianf on 12/12/2023.
//

#ifndef DROP_POD_CMP_NEXT_LEVEL_H
#define DROP_POD_CMP_NEXT_LEVEL_H
#include "ecm.h"
#include "engine.h"
#include "../game.h"
#include "cmp_sprite.h"

class NextLevelComponent : public Component {
public:
    void update(double dt) override;
    void render() override ;
    explicit NextLevelComponent(Entity* p, std::shared_ptr<Entity> player, std::shared_ptr<Scene> scene, bool isEndGame = false);
    NextLevelComponent() = delete;
    bool getIsAtFlag();

protected:
    std::shared_ptr<Entity> _player;
    std::shared_ptr<Scene> _scene;
    bool _isEndGame;
    bool _playerIsAtFlag = false;
    float _timer = 0.f;
    sf::Sound _sound;
    std::shared_ptr<sf::SoundBuffer> _nextLevelSound;
    std::shared_ptr<sf::SoundBuffer> _endGameSound;


};




#endif //DROP_POD_CMP_NEXT_LEVEL_H
