//
// Created by cianf on 12/12/2023.
//

#ifndef DROP_POD_CMP_PICKUP_H
#define DROP_POD_CMP_PICKUP_H

#include "ecm.h"
#include "SFML/Audio/Sound.hpp"


class PickupComponent : public Component {
public:
    enum PickupType {
        HEALTH = true,
        SCORE = false
    };
    void update(double dt) override;
    void render() override ;
    explicit PickupComponent(Entity* p, PickupType type, int value, std::shared_ptr<Entity> player);
    PickupComponent() = delete;
    int getValue() {return _value;}
    void setValue(int value) {_value = value;}
    PickupType getType() {return _type;}
    void setType(PickupType type) {_type = type;}



protected:
    PickupType _type;
    int _value;
    std::shared_ptr<Entity> _player;
};


#endif //DROP_POD_CMP_PICKUP_H
