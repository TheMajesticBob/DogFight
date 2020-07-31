#pragma once
#include <engine.h>
#include <SFML/Graphics.hpp>

class Settings : public Scene {
protected:
    std::shared_ptr<class Button> _menu;
    std::shared_ptr<class Button> _head;
    void MoveToMenu();

public:
    Settings() = default;
    ~Settings() override = default;

    void Load() override;

    void UnLoad() override;

    void Update(const double& dt) override;
};
