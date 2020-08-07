#pragma once
#include <SFML/Graphics/ConvexShape.hpp>
#include <SFML/Graphics/Text.hpp>
#include <system_resources.h>
#include <ecm.h>

class ShapeEditor : public Entity
{
	public:
		ShapeEditor() = delete;
		ShapeEditor(class Scene* const s);
		~ShapeEditor();


		virtual void update(double dt) override;

		void PerformNewVertexCheck();

		virtual void render() override;

protected:
		void ResetShape();
		void RegenerateVertexButtons();
		void UpdateVertexButtonPositions();

		void AddVertexAt(int vertexId, sf::Vector2f pos);
		void RemoveVertex(int vertexId);

		void OnButtonHovered(std::shared_ptr<class Button> button);
		void OnButtonUnhovered(std::shared_ptr<class Button> button);
		void OnButtonPressed(std::shared_ptr<class Button> button);
		void OnButtonReleased(std::shared_ptr<class Button> button);

		int FindButtonId(std::shared_ptr<class Button> button);

		// Shape stuff
		std::shared_ptr<class ShapeComponent> _shapeComponent;
		sf::ConvexShape* _shape;

		// Vertices stuff
		std::vector<std::shared_ptr<class Button>> _vertexButtons;

		struct EdgeText
		{
			EdgeText()
			{
				t.setFont(*Resources::get<sf::Font>("RobotoMono-Regular.ttf"));
			}

			void render();

			sf::ConvexShape* shape;
			sf::Text t;
			std::string text = "N/A";

			int v1 = -1, v2 = -1;
		};
		std::vector<EdgeText> _edgeTexts;
		sf::Vector2f _vertexStartPos;
		int _vertexId;


		// Mouse, button stuff (UI)
		sf::Vector2f _mouseStartPos;
		std::shared_ptr<class Button> _currentButton = nullptr;
		std::shared_ptr<class Button> _currentHovered = nullptr;

		// Debug stuff
		std::shared_ptr<class TextComponent> _textComponent;
};