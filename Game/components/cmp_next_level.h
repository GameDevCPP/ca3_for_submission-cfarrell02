//
// Created by cianf on 12/12/2023.
//

#ifndef DROP_POD_CMP_NEXT_LEVEL_H
#define DROP_POD_CMP_NEXT_LEVEL_H
#include "ecm.h"
#include "engine.h"
#include "cmp_sprite.h"

template<typename T>
class NextLevelComponent : public Component {
public:
    void update(double dt) override;
    void render() override ;
    explicit NextLevelComponent(Entity* p, T* scene, std::shared_ptr<Entity> player, bool isEndGame = false);
    NextLevelComponent() = delete;
    bool getIsEndGame() {return _isEndGame;}
    void setIsEndGame(bool isEndGame) {_isEndGame = isEndGame;}

protected:
    bool _isEndGame;
    std::shared_ptr<Entity> _player;
    T* _scene;

};




#endif //DROP_POD_CMP_NEXT_LEVEL_H
