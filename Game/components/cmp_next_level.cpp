//
// Created by cianf on 12/12/2023.
//

#include "cmp_next_level.h"



void NextLevelComponent::update(double dt) {
    if(_player == nullptr){
        return;
    }
    auto playerPos = _player->getPosition();
    auto nextLevelBounds = _parent->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds();
    if(nextLevelBounds.contains(playerPos)){
        if(_isEndGame){
            //Engine::ChangeScene((Scene*) new EndGameScene());
            std::cout<<"End the game here"<<std::endl;
            //TODO: End the game here
        }else{
            Engine::ChangeScene( _scene.get());
        }
    }
}

void NextLevelComponent::render() {

}

NextLevelComponent::NextLevelComponent(Entity *p, std::shared_ptr<Entity> player, std::shared_ptr<Scene> scene,
                                       bool isEndGame) : Component(p), _player(std::move(player)), _scene(std::move(scene)), _isEndGame(isEndGame) {

}

