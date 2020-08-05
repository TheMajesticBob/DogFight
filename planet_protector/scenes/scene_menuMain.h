#pragma once
#include <engine.h>
#include <SFML/Graphics.hpp>

class MainMenu : public Scene {
protected:
    std::shared_ptr<class Button> _head;
    std::shared_ptr<class Button> _play;
    std::shared_ptr<class Button> _settings;

	void MoveToGame();
	void MoveToSettings();

public:
    MainMenu() = default;
    ~MainMenu() override = default;

    void Load() override;

    void UnLoad() override;

	void Quit();

    void Update(const double& dt) override;
};
