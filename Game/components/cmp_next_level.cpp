//
// Created by cianf on 12/12/2023.
//

#include "cmp_next_level.h"
#include "engine.h"


template<typename T>
void NextLevelComponent<T>::update(double dt) {
    if(_player == nullptr){
        return;
    }
    auto playerBounds = _player->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds();
    auto nextLevelBounds = _parent->get_components<SpriteComponent>()[0]->getSprite().getGlobalBounds();
    if(playerBounds.intersects(nextLevelBounds)){
        if(_isEndGame){
            //Engine::ChangeScene((Scene*) new EndGameScene());
            std::cout<<"End the game here"<<std::endl;
            //TODO: End the game here
        }else{
            Engine::ChangeScene((Scene*) _scene);
        }
    }


}

template<typename T>
void NextLevelComponent<T>::render() {

}

template<typename T>
NextLevelComponent<T>::NextLevelComponent(Entity *p, T *scene, std::shared_ptr<Entity> player, bool isEndGame)
: Component(p), _scene(scene), _player(player), _isEndGame(isEndGame) {

}