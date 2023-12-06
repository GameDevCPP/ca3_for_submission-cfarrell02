//
// Created by Cian Farrell on 06/12/2023.
//

#ifndef DROP_POD_CMP_HUD_H
#define DROP_POD_CMP_HUD_H

#include "ecm.h"
#include "./cmp_player_physics.h"
#include "./cmp_text.h"


class HudComponent : public Component {
protected:
    std::shared_ptr<PlayerPhysicsComponent> _player;
    std::shared_ptr<sf::Text> _healthText;
    std::shared_ptr<sf::Text> _scoreText;
    std::shared_ptr<sf::Font> _font;
public:
    void update(double dt) override;
    void render() override ;
    explicit HudComponent(Entity* p, std::shared_ptr<PlayerPhysicsComponent> player);
    //~HudComponent() override;

};


#endif //DROP_POD_CMP_HUD_H
