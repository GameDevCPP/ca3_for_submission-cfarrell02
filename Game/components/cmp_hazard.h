//
// Created by cianf on 05/12/2023.
//

#ifndef DROP_POD_CMP_HAZARD_H
#define DROP_POD_CMP_HAZARD_H


#include "ecm.h"

class HazardComponent : public Component {
protected:
    int _damage;
    std::shared_ptr<Entity> _player;
    float _timer = 0.0f;
    float _rate;

public:
    void update(double dt) override;
    void render() override ;
    explicit HazardComponent(Entity* p, int damage, std::shared_ptr<Entity> player, float rate = 0.1f);
    HazardComponent() = delete;
    int getDamage() {return _damage;}
    void setDamage(int damage) {_damage = damage;}

};


#endif //DROP_POD_CMP_HAZARD_H
