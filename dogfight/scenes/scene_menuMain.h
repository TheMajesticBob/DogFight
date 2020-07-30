#pragma once
#include "engine.h"
#include <SFML/Graphics.hpp>

class MainMenu : public Scene {
protected:
    //Entity text_mainTitle;// = makeEntity();
    //sf::Font font_mainTitle;
    //RectangleShape _rectangle;
    /*Button _play;
    Button _settings;*/

public:
    MainMenu() = default;
    ~MainMenu() override = default;

    void Load() override;

    void UnLoad() override;

    void Update(const double& dt) override;
};