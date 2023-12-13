//
// Created by cianf on 12/12/2023.
//

#include "cmp_next_level.h"



void NextLevelComponent::update(double dt) {
    _timer += dt;
    if(_player == nullptr){
        return;
    }
    auto playerPos = _player->getPosition();
    auto nextLevelBounds = _parent->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds();
    if(nextLevelBounds.contains(playerPos) && !_playerIsAtFlag){
        if(_isEndGame){
            //Engine::ChangeScene((Scene*) new EndGameScene());
            _sound.setBuffer(*_endGameSound);
            _playerIsAtFlag = true;
            _timer = 0.f;
        }else{
            _sound.setBuffer(*_nextLevelSound);
            _playerIsAtFlag = true;
            _timer = 0.f;
        }
        _sound.play();
    }
}

void NextLevelComponent::render() {

}

NextLevelComponent::NextLevelComponent(Entity *p, std::shared_ptr<Entity> player, std::shared_ptr<Scene> scene,
                                       bool isEndGame) : Component(p), _player(std::move(player)), _scene(std::move(scene)), _isEndGame(isEndGame) {
    _nextLevelSound = Resources::get<sf::SoundBuffer>("Success_Bell.wav");
    _endGameSound = Resources::get<sf::SoundBuffer>("Success_Fanfare.wav");

}

bool NextLevelComponent::getIsAtFlag() {
if(_playerIsAtFlag && _timer > 2.f){
        _timer = 0.f;
        return true;
    }
    return false;
}

