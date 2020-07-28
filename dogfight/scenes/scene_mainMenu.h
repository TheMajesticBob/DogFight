#pragma once
#include "engine.h"
#include <SFML/Graphics.hpp>

class MainMenu : public Scene {
protected:
    sf::Text text_mainTitle;
    sf::Font font_mainTitle;

public:
    MainMenu() = default;
    ~MainMenu() override = default;

    void Load() override;

    void Update(const double& dt) override;
};
