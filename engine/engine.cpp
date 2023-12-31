#include "engine.h"
#include "maths.h"
#include "system_physics.h"
#include "system_renderer.h"
#include "system_resources.h"
#include <SFML/Graphics.hpp>
#include <future>
#include <iostream>

using namespace sf;
using namespace std;
Scene* Engine::_activeScene = nullptr;
std::string Engine::_gameName;

static bool loading = false;
static float loadingspinner = 0.f;
static float loadingTime;
static RenderWindow* _window;

void Loading_update(float dt, const Scene* const scn) {
	//  cout << "Eng: Loading Screen\n";
	if (scn->isLoaded()) {
		cout << "Eng: Exiting Loading Screen\n";
		loading = false;
	}
	else {
		loadingspinner += 800.0f * dt;
		loadingTime += dt;
	}
}
void Loading_render() {
//	cout << "Eng: Loading Screen Render\n";

    auto currentView = _window->getView();
    auto viewPos = currentView.getCenter() - currentView.getSize() * 0.5f;
    const int spriteSize = 128;
    const int width = Engine::GetWindow().getSize().x;
    const int height = Engine::GetWindow().getSize().y;
    static vector<sf::Sprite> backgroundSprites;

    for(int x = 0; x < width; x+=spriteSize) {
        for(int y = 0; y < height;  y+=spriteSize) {
            //Manually make a sprite
            sf::Sprite sprite = sf::Sprite();
            sprite.setTexture(*Resources::get<Texture>("Free/Background/Purple.png"));
            sprite.setScale(2.0f, 2.0f);
            sprite.setPosition(viewPos + Vector2f(x, y));
            backgroundSprites.push_back(sprite);
        }
    }


	static CircleShape octagon(100);
	octagon.setOrigin(Vector2f(100, 100));
	octagon.setRotation(deg2rad(loadingspinner));
	octagon.setPosition(Vcast<float>(currentView.getCenter()));
	auto tex = new Texture();
	tex->loadFromFile("res/img/Loading.png");
	octagon.setTexture(tex);

	static Text t("Loading", *Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
	t.setFillColor(Color(255, 255, 255, min(255.f, 40.f * loadingTime)));
	t.setOutlineThickness(2);
	t.setPosition(Vcast<float>(currentView.getCenter()) + Vector2f(0, 300));
	t.setOrigin(t.getLocalBounds().left + t.getLocalBounds().width / 2.0f,
		t.getLocalBounds().top + t.getLocalBounds().height / 2.0f);
	for (auto& s : backgroundSprites) {
        Renderer::queue(&s);
    }
	Renderer::queue(&t);
	Renderer::queue(&octagon);
}

float frametimes[256] = {};
uint8_t ftc = 0;

void Engine::Update() {
	static sf::Clock clock;
	float dt = clock.restart().asSeconds();
	{
		frametimes[++ftc] = dt;
		static string avg = _gameName + " FPS:";
		if (ftc % 60 == 0) {
			double davg = 0;
			for (const auto t : frametimes) {
				davg += t;
			}
			davg = 1.0 / (davg / 255.0);
			_window->setTitle(avg + toStrDecPt(2, davg));
		}
	}

	if (loading) {
		Loading_update(dt, _activeScene);
	}
	else if (_activeScene != nullptr) {
		Physics::update(dt);
		_activeScene->Update(dt);
	}
}

void Engine::Render(RenderWindow& window) {
	if (loading) {
		Loading_render();
	}
	else if (_activeScene != nullptr) {
        _activeScene->Render();
	}

    Renderer::render();
}

void Engine::Start(unsigned int width, unsigned int height,
	const std::string& gameName, Scene* scn) {
	RenderWindow window(VideoMode(width, height), gameName);
	//window.setVerticalSyncEnabled(true);
	_gameName = gameName;
	_window = &window;
//    _window->setFramerateLimit(60);
	Renderer::initialise(window);
	Physics::initialise();
	ChangeScene(scn);
	while (window.isOpen()) {
		Event event;
		while (window.pollEvent(event)) {
			if (event.type == Event::Closed) {
				window.close();
			}
		}
		if (Keyboard::isKeyPressed(Keyboard::Escape)) {
			window.close();
		}

		window.clear();
		Update();
		Render(window);
		window.display();
	}
	if (_activeScene != nullptr) {
		_activeScene->UnLoad();
		_activeScene = nullptr;
	}
	window.close();
	Physics::shutdown();
	// Render::shutdown();
}

std::shared_ptr<Entity> Scene::makeEntity() {
	auto e = make_shared<Entity>(this);
	ents.list.push_back(e);
	return std::move(e);
}

//void Scene::addEntity(shared_ptr<Entity> entity) {
//	ents.list.push_back(entity);
//}

void Engine::setVsync(bool b) { _window->setVerticalSyncEnabled(b); }

void Engine::setView(View v) { _window->setView(v); }

void Engine::moveView(Vector2f movement) {
	View tempview = _window->getView();
	tempview.move(movement);
	Engine::setView(tempview);
}

// CHANGE RES PART
void Engine::changeResolution(int x, int y)
{
	Vector2f _newResolution(x, y);
	_window->create(VideoMode(_newResolution.x, _newResolution.y), "Drop Pod");
}

void Engine::ChangeScene(Scene* s) {
	cout << "Eng: changing scene: " << s << endl;
	auto old = _activeScene;
	_activeScene = s;

	if (old != nullptr) {
		old->UnLoad(); // todo: Unload Async
	}

	if (!s->isLoaded()) {
		cout << "Eng: Entering Loading Screen\n";
		loadingTime = 0;
		_activeScene->LoadAsync();
		//_activeScene->Load();
		loading = true;
	}
}

sf::Vector2f Engine::flocking(Entity* thisEnemy, Vector2f toPlayer)
{
	//shared_ptr<Entity> choosenEnemy = make_shared<Entity>(thisEnemy);
	Vector2f movement = Vector2f(0, 0);
	Vector2f alignment = Vector2f(0, 0);
	Vector2f cohesion = Vector2f(0, 0);
	Vector2f separation = Vector2f(0, 0);
	int neighborCount = 0;

	auto ecm = _activeScene->getEcm();
	auto enemies = ecm.find("enemy");

	for (shared_ptr<Entity>& enemy : enemies)
	{
		Entity* rawEnemy = enemy.get();
		if (rawEnemy != thisEnemy)
		{
			float xDistance = enemy->getPosition().x - thisEnemy->getPosition().x;
			float yDistance = enemy->getPosition().y - thisEnemy->getPosition().y;

			auto distance = (xDistance * xDistance) + (yDistance * yDistance);

			if (distance < 3000)
			{
				alignment.x += toPlayer.x;
				alignment.y += toPlayer.y;
				neighborCount++;
			}
		}
	}

	if (neighborCount == 0)
	{
		return alignment;
	}

	alignment.x /= neighborCount;
	alignment.y /= neighborCount;
	alignment = normalize(alignment);

	// Cohesion ----------------------------------------------------------------------------------------------
	for (shared_ptr<Entity>& enemy : enemies)
	{
		Entity* rawEnemy = enemy.get();
		if (rawEnemy != thisEnemy)
		{
			float xDistance = enemy->getPosition().x - thisEnemy->getPosition().x;
			float yDistance = enemy->getPosition().y - thisEnemy->getPosition().y;

			auto distance = (xDistance * xDistance) + (yDistance * yDistance);

			if (distance < 3000)
			{
				cohesion.x += enemy->getPosition().x;
				cohesion.y += enemy->getPosition().y;
			}
		}
	}

	cohesion.x /= neighborCount;
	cohesion.y /= neighborCount;

	cohesion = Vector2f(cohesion.x - thisEnemy->getPosition().x, cohesion.y - thisEnemy->getPosition().y);
	cohesion = normalize(cohesion);

	// Separation -------------------------------------------------------------------------------------------
	for (shared_ptr<Entity>& enemy : enemies)
	{
		Entity* rawEnemy = enemy.get();
		if (rawEnemy != thisEnemy)
		{
			float xDistance = enemy->getPosition().x - thisEnemy->getPosition().x;
			float yDistance = enemy->getPosition().y - thisEnemy->getPosition().y;

			auto distance = (xDistance * xDistance) + (yDistance * yDistance);

			if (distance < 3000)
			{
				separation.x += enemy->getPosition().x - thisEnemy->getPosition().x;
				separation.y += enemy->getPosition().y - thisEnemy->getPosition().y;
			}
		}
	}

	separation.x *= -1;
	separation.y *= -1;

	movement.x = toPlayer.x + alignment.x + cohesion.x + separation.x;
	movement.y = toPlayer.y + alignment.y + cohesion.y + separation.y;

	movement = normalize(movement);
	return movement;
}

void Scene::Update(const double& dt) {
    ents.update(dt); }

void Scene::Render() { ents.render(); }

EntityManager Scene::getEcm() { return ents; }

bool Scene::isLoaded() const {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		// Are we already loading asynchronously?
		if (_loaded_future.valid() // yes
			&&                     // Has it finished?
			_loaded_future.wait_for(chrono::seconds(0)) ==
			future_status::ready) {
			// Yes
			_loaded_future.get();
			_loaded = true;
		}
		return _loaded;
	}
}
void Scene::setLoaded(bool b) {
	{
		std::lock_guard<std::mutex> lck(_loaded_mtx);
		_loaded = b;
	}
}

void Scene::UnLoad() {
	ents.list.clear();
	setLoaded(false);
}

void Scene::LoadAsync() { _loaded_future = std::async(&Scene::Load, this); }

sf::Vector2u Engine::getWindowSize() { return _window->getSize(); }

sf::RenderWindow& Engine::GetWindow() { return *_window; }



namespace timing {
	// Return time since Epoc
	long long now() {
		return std::chrono::high_resolution_clock::now()
			.time_since_epoch()
			.count();
	}
	// Return time since last() was last called.
	long long last() {
		auto n = now();
		static auto then = now();
		auto dt = n - then;
		then = n;
		return dt;
	}
} // namespace timing

Scene::~Scene() { UnLoad(); }