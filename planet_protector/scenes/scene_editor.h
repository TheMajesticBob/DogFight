#pragma once
#include <engine.h>

class EditorScene : public Scene
{

public:
	// Begin Scene overrides
	virtual void Load() override;
	virtual void UnLoad() override;
	virtual void Update(const double& dt) override;
	// End Scene overrides

	void Exit();

protected:
	void MovePressed() { _isMovePressed = true; }
	void MoveReleased() { _isMovePressed = false; }

	bool _isMovePressed = false;
	sf::Vector2f _moveStartPos;
	sf::Vector2f _cameraStartPos;

	std::shared_ptr<class ShapeEditor> _shapeEditor;
	std::shared_ptr<class Camera> _camera;
};