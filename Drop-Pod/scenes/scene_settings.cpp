// Scene for settings menu
#include "scene_settings.h"
#include "../drop_pod_game.h"
#include <LevelSystem.h>
#include <iostream>
#include"../components/cmp_button.h"
#include "../components/cmp_text.h"
#include <SFML/Window.hpp>
#include <engine.h>
#include "../components/cmp_player.h"
#include <SFML/Graphics.hpp>
using namespace std;
using namespace sf;

View settingsView;
bool vsync = false;

shared_ptr<Entity> btnArrows;
shared_ptr<Entity> btnWsad;
shared_ptr<Entity> btn1280;
shared_ptr<Entity> btn1920;
shared_ptr<Entity> btnFrame;
//  Loads the scene
void SettingsScene::Load() {
	cout << "Setting Load \n";

	settingsView = Engine::GetWindow().getView();
	RenderWindow& window = Engine::GetWindow();
	Vector2u windowSize = window.getSize();

	auto txt2 = makeEntity();
	auto pos = Vector2f(settingsView.getSize().x / 2.0f, 50);
	auto t = txt2->addComponent<TextComponent>(pos.x, pos.y, "Settings");

	btnArrows = makeEntity();
	auto btn4Pos = Vector2f(settingsView.getSize().x / 1.6f, settingsView.getSize().y / 5.0f);
	auto button = btnArrows->addComponent<Button>(btn4Pos, "Arrows Movment", sf::Color::White, sf::Color::Green, sf::Color::Red);

	auto txt3 = makeEntity();
	auto t3 = txt3->addComponent<TextComponent>(settingsView.getSize().x / 2.0f, settingsView.getSize().y / 5.0f, switchState == WSAD ? "WSAD" : "Arrows");
	t3->setTextSize(20);

	btnWsad = makeEntity();
	auto btn5Pos = Vector2f(settingsView.getSize().x / 2.7f, settingsView.getSize().y / 5.0f);
	auto button2 = btnWsad->addComponent<Button>(btn5Pos, "WSAD Movment", sf::Color::White, sf::Color::Green, sf::Color::Red);

	btn1280 = makeEntity();
	auto btn6Pos = Vector2f(settingsView.getSize().x / 1.6f, settingsView.getSize().y / 3.0f);
	auto button3 = btn1280->addComponent<Button>(btn6Pos, "1280 x 720", sf::Color::White, sf::Color::Green, sf::Color::Red);

	auto txt4 = makeEntity();
	auto t4 = txt4->addComponent<TextComponent>(settingsView.getSize().x / 2.0f, settingsView.getSize().y / 3.0f, to_string(static_cast<int>(settingsView.getSize().x)) + "x" + to_string(static_cast<int>(settingsView.getSize().y)));
	t4->setTextSize(20);

	btn1920 = makeEntity();
	auto btn7Pos = Vector2f(settingsView.getSize().x / 2.7f, settingsView.getSize().y / 3.0f);
	auto button4 = btn1920->addComponent<Button>(btn7Pos, "1920 x 1080", sf::Color::White, sf::Color::Green, sf::Color::Red);

	btnFrame = makeEntity();
	auto btn8Pos = Vector2f(settingsView.getSize().x / 2.0f, settingsView.getSize().y / 1.5f);
	auto button5 = btnFrame->addComponent<Button>(btn8Pos, "Vsync: OFF", sf::Color::White, sf::Color::Green, sf::Color::Red);

	btnSoundUp = makeEntity();
	auto btn9Pos = Vector2f(settingsView.getSize().x / 2.7f, settingsView.getSize().y / 2.0f);
	auto button6 = btnSoundUp->addComponent<Button>(btn9Pos, "Sound + 10", sf::Color::White, sf::Color::Green, sf::Color::Red);

	auto txt5 = makeEntity();
	auto t5 = txt5->addComponent<TextComponent>(settingsView.getSize().x / 2.0f, settingsView.getSize().y / 2.0f, std::to_string(volume));
	t5->setTextSize(20);

	btnSoundDown = makeEntity();
	auto btn10Pos = Vector2f(settingsView.getSize().x / 1.6f, settingsView.getSize().y / 2.0f);
	auto button7 = btnSoundDown->addComponent<Button>(btn10Pos, "Sound - 10", sf::Color::White, sf::Color::Green, sf::Color::Red);

	btnBack = makeEntity();
	auto btn11Pos = Vector2f(settingsView.getSize().x / 2.0f, settingsView.getSize().y / 1.2f);
	auto button8 = btnBack->addComponent<Button>(btn11Pos, "Back", sf::Color::White, sf::Color::Green, sf::Color::Red);

	setLoaded(true);
}

void SettingsScene::Update(const double& dt) {
	if (btnArrows->GetCompatibleComponent<Button>()[0]->isPressed()) {
		switchState = Arrows;
		Engine::ChangeScene(&settings);
	}
	else if (btnWsad->GetCompatibleComponent<Button>()[0]->isPressed()) {
		switchState = WSAD;
		Engine::ChangeScene(&settings);
	}
	else if (btn1280->GetCompatibleComponent<Button>()[0]->isPressed()) {
		resolution = Vector2i(1280, 720);
		Engine::changeResolution(resolution.x, resolution.y);
		Engine::ChangeScene(&settings);
	}
	else if (btn1920->GetCompatibleComponent<Button>()[0]->isPressed()) {
		resolution = Vector2i(1920, 1080);
		Engine::changeResolution(resolution.x, resolution.y);
		Engine::ChangeScene(&settings);
	}

	else if (btnFrame->GetCompatibleComponent<Button>()[0]->isPressed()) {
		if (vsync)
		{
			Engine::GetWindow().setVerticalSyncEnabled(false);
			btnFrame->GetCompatibleComponent<Button>()[0]->_text.setString("Vsync: OFF");
			vsync = false;
		}
		else
		{
			Engine::GetWindow().setVerticalSyncEnabled(true);
			btnFrame->GetCompatibleComponent<Button>()[0]->_text.setString("Vsync: ON");
			vsync = true;
		}
	}

	else if (btnSoundUp->GetCompatibleComponent<Button>()[0]->isPressed()) {
		volume >= 100 ? volume = 100 : volume += 10;
		Engine::ChangeScene(&settings);
	}

	else if (btnSoundDown->GetCompatibleComponent<Button>()[0]->isPressed()) {
		volume <= 0 ? volume = 0 : volume -= 10;
		Engine::ChangeScene(&settings);
	}

	else if (btnBack->GetCompatibleComponent<Button>()[0]->isPressed())
	{
		Engine::ChangeScene(&menu);
	}

	if (!sf::Mouse::isButtonPressed(Mouse::Button::Left))
		Button::_mouseState = BUTTON_IDLE;

	Scene::Update(dt);
}