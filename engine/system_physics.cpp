#include "system_physics.h"
#include "SFML/OpenGL.hpp"

using namespace std;
using namespace sf;

namespace Physics 
{
	std::shared_ptr<b2World> world;
	std::shared_ptr<class PhysicsDebugDraw> debugDraw;
	uint32 debugDrawFlag = 0;

	ContactListener contactListener;

	class PhysicsDebugDraw : public b2Draw
	{

	public:
		PhysicsDebugDraw() : b2Draw() {}

		virtual void DrawPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
		{
			// 			GLfloat verts[16];
			// 			
			// 			glVertexPointer(2, GL_FLOAT, 0, verts);
			// 			glEnableClientState(GL_VERTEX_ARRAY);
			// 
			// 			for (int i = 0; i < vertexCount; ++i)
			// 			{
			// 				verts[i * 2] = vertices[i].x * 300;
			// 				verts[i * 2 + 1] = vertices[i].y * 300;
			// 			}
			// 
			// 			// Draw solid
			// 			glColor4f(0, 0, 0, 0);
			// 			glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);
			// 
			// 			// Draw lines
			// 			glLineWidth(3);
			// 			glColor4f(color.r, color.g, color.b, color.a);
			// 			glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
		}

		virtual void DrawSolidPolygon(const b2Vec2* vertices, int32 vertexCount, const b2Color& color) override
		{
			GLfloat verts[16];

			glVertexPointer(2, GL_FLOAT, 0, verts);
			glEnableClientState(GL_VERTEX_ARRAY);

			for (int i = 0; i < vertexCount; ++i)
			{
				verts[i * 2] = bv2_to_sv2(vertices[i]).x;
				verts[i * 2 + 1] = invert_height(bv2_to_sv2(vertices[i])).y;
			}

			// Draw solid
	// 			glColor4f(0, 0, 0, 0);
	// 			glDrawArrays(GL_TRIANGLE_FAN, 0, vertexCount);

				// Draw lines
			glLineWidth(0.1f);
			glColor4f(color.r, color.g, color.b, color.a);
			glDrawArrays(GL_LINE_LOOP, 0, vertexCount);
		}


		virtual void DrawCircle(const b2Vec2& center, float32 radius, const b2Color& color) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		virtual void DrawSolidCircle(const b2Vec2& center, float32 radius, const b2Vec2& axis, const b2Color& color) override
		{
			const int vertexCount = 50;

			glLineWidth(.1f);
			glColor4f(color.r, color.g, color.b, color.a);
			glBegin(GL_LINE_LOOP);
			for (float a = 0; a < 360; a += 5)
			{
				sf::Vector2f sfPos = invert_height(bv2_to_sv2(center)) + sf::Vector2f(sinf(sf::deg2rad(a)), cosf(sf::deg2rad(a))) * radius * physics_scale;
				glVertex2f(sfPos.x, sfPos.y);
			}
			glEnd();
		}


		virtual void DrawSegment(const b2Vec2& p1, const b2Vec2& p2, const b2Color& color) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		virtual void DrawTransform(const b2Transform& xf) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}


		virtual void DrawPoint(const b2Vec2& p, float32 size, const b2Color& color) override
		{
			throw std::logic_error("The method or operation is not implemented.");
		}

	};

	void initialise()
	{
		b2Vec2 gravity(0.0f, 0.0f); // -10.0f);
		debugDraw.reset(new PhysicsDebugDraw());

		// Construct a world object, which will hold and simulate the rigid bodies.
		world.reset(new b2World(gravity));

		// Set our contact listener
		world->SetContactListener(&contactListener);

		// Setup debug draw
		world->SetDebugDraw(debugDraw.get());
	}

	void shutdown()
	{
		world.reset();
		debugDraw.reset();
	}

	void update(const double& dt)
	{
		world->Step((float)dt, velocityIterations, positionIterations);
	}

	std::shared_ptr<b2World> GetWorld() { return world; }

	const bool getDebugDraw()
	{
		return (debugDrawFlag & b2Draw::e_shapeBit) == b2Draw::e_shapeBit;
	}

	void setDebugDraw(const bool isActive)
	{
		debugDrawFlag = isActive ? b2Draw::e_shapeBit : 0;
		debugDraw->SetFlags(debugDrawFlag);
	}

	const sf::Vector2f bv2_to_sv2(const b2Vec2& in, bool scale)
	{
		if (scale)
		{
			return Vector2f((in.x * physics_scale), (in.y * physics_scale));
		}
		else
		{
			return Vector2f(in.x, in.y);
		}
	}

	const b2Vec2 sv2_to_bv2(const sf::Vector2f& in, bool scale)
	{
		if (scale)
		{
			return b2Vec2((in.x * physics_scale_inv), (in.y * physics_scale_inv));
		}
		else
		{
			return b2Vec2(in.x, in.y);
		}
	}

	const sf::Vector2f invert_height(const sf::Vector2f& in)
	{
		return sf::Vector2f(in.x, 720 - in.y);
	}

	const float vector_to_angle(const sf::Vector2f& in)
	{
		return atan2(in.y, in.x);
	}
} // namespace Physics