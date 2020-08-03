#include "explosion.h"
#include "../planet_protector/game.h"
#include <system_renderer.h>
#include <system_ui.h>

using namespace sf;
using namespace std;

unsigned char ExplosionManager::explosionPointer;
Explosion ExplosionManager::explosions[256];

void ExplosionManager::update(double dt)
{
    for (int i = 0; i < 256; ++i)
    {
        explosions[i].update(dt);
    }
}

void ExplosionManager::render()
{
	_shockwaveRT.clear(sf::Color(120, 0, 0, 120));
	_shockwaveRT.setView(Engine::GetWindow().getView());
    
    for (int i = 0; i < 256; ++i)
    {
        if(explosions[i].GetNormalizedLifetime() < 1.0f)
        {
            if(sf::Shader::isAvailable())
            {
				explosions[i].explosionSprite.setColor(sf::Color(255, 255, 255, (int)(128 * (1.0f - explosions[i].GetNormalizedLifetime()))));
				explosions[i].explosionSprite.setPosition(explosions[i].position);
                _shockwaveRT.draw(explosions[i].explosionSprite);
            }
        }
    }
    
    _shockwaveRT.display();

	sf::Texture texture = _shockwaveRT.getTexture();

	UI::queue(&_textureSprite);
}

void ExplosionManager::Fire(const sf::Vector2f &pos, const sf::Vector2f &size, const float &lifetime)
{
    Explosion &explosion = explosions[explosionPointer++];
	explosion.explosionSprite = sf::Sprite(*_shockwaveTexture.get());
	explosion.explosionSprite.setOrigin({ 290, 290 });
    explosion.explosionSprite.setTextureRect(IntRect(0, 0, 580, 580));
    explosion.explosionSprite.setColor(sf::Color(255, 255, 255, 255));
    explosion.size = size;
	explosion.position = pos;
    explosion.currentSize = sf::Vector2f(size.x, size.x);
    explosion.lifetime = lifetime;
    explosion.currentLifetime = 0.0f;
}

ExplosionManager::ExplosionManager(class Scene* const s)
	: Entity(s)
{
	_shockwaveTexture = Resources::get<sf::Texture>("shockwave.png");

	if (!_shockwaveRT.create(Engine::getWindowSize().x, Engine::getWindowSize().y))
	{
		cerr << "Failed to initialize shockwave rendertarget!" << endl;
	}

	if (!_shockwaveShader.loadFromFile("./res/shaders/shockwave.frag", sf::Shader::Fragment))
	{
		std::cerr << "Failed to load shockwave shader!" << std::endl;
	}
	
	_shockwaveShader.setUniform("displacementMap", _shockwaveRT.getTexture());
	_textureSprite = sf::Sprite(_shockwaveRT.getTexture());
	//_textureSprite.setOrigin(sf::Vector2f(Engine::getWindowSize())/2.0f);
	//Renderer::add_postprocess_effect(&_shockwaveShader);
}

ExplosionManager::~ExplosionManager()
{
	//Renderer::remove_postprocess_effect(&_shockwaveShader);
}
