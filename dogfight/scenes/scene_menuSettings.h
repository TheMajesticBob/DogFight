#pragma once
#include <engine.h>
#include <SFML/Graphics.hpp>

class Settings : public Scene {
protected:

public:
    Settings() = default;
    ~Settings() override = default;

    void Load() override;

    void Update(const double& dt) override;
};
