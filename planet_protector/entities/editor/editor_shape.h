#pragma once
#include <SFML/Graphics/ConvexShape.hpp>
#include <ecm.h>

class ShapeEditor : public Entity
{
	public:
		ShapeEditor() = delete;
		ShapeEditor(class Scene* const s);
		~ShapeEditor();


		virtual void update(double dt) override;

protected:
		void ResetShape();
		void RegenerateVertexButtons();
		void UpdateVertexButtonPositions();

		void OnButtonPressed(std::shared_ptr<class Button> button);

		void OnButtonReleased(std::shared_ptr<class Button> button);

		// Shape stuff
		std::shared_ptr<class ShapeComponent> _shapeComponent;
		sf::ConvexShape* _shape;

		// Vertices stuff
		std::vector<std::shared_ptr<class Button>> _vertexButtons;
		sf::Vector2f _vertexStartPos;
		int _vertexId;


		// Mouse, button stuff (UI)
		sf::Vector2f _mouseStartPos;
		std::shared_ptr<class Button> _currentButton = nullptr;
};