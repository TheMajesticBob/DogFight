#pragma once
#include "engine.h"
#include <SFML/Graphics.hpp>

class MainMenu : public Scene {
private:
    //Entity text_mainTitle;// = makeEntity();
    sf::Font font_mainTitle;

public:
    MainMenu() = default;
    ~MainMenu() override = default;

    void Load() override;

    void Update(const double& dt) override;
};
