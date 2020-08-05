#pragma once
#include <SFML/Graphics/RenderWindow.hpp>
#include <ecm.h>
#include <future>
#include <maths.h>
#include <mutex>
#include <string>
#include <queue>

class Scene 
{
	public:
		Scene() = default;
		virtual ~Scene();
		virtual void Load() = 0;
		
		virtual void LoadAsync();
		virtual void UnLoad();
		virtual void Update(const double& dt);
		virtual void Render();
		bool IsLoaded() const;

		void AddPersistentEntities(std::queue<std::shared_ptr<Entity>> entities)
		{
			while (entities.size() > 0)
			{
				ents.list.push_back(entities.front());
				entities.pop();
			}
		}

		std::shared_ptr<Entity> MakeEntity();

		template <typename T, typename... Targs>
		std::shared_ptr<T> MakeEntity(Targs... params)
		{
			static_assert(std::is_base_of<Entity, T>::value, "T != entity");

			auto e = std::make_shared<T>(this, params...);
			Entity* en = e.get();
			en->setSharedPtr(e);
			ents.list.push_back(e);
			return std::move(e);
		}

		template<typename T>
		std::shared_ptr<T> MakeEntity()
		{
			static_assert(std::is_base_of<Entity, T>::value, "T != entity");

			auto e = std::make_shared<T>(this);
			Entity* en = e.get();
			en->setSharedPtr(e);
			ents.list.push_back(e);
			return std::move(e);
		}

		EntityManager ents;

	protected:
		void SetLoaded(bool);
	
	private:
		mutable bool _loaded;
		mutable std::future<void> _loaded_future;
		mutable std::mutex _loaded_mtx;
};

class Engine 
{
	public:
		Engine() = delete;
		static void Start(unsigned int width, unsigned int height,
                    const std::string& gameName, Scene* scn);
		static void ChangeScene(Scene*, bool forceSync = false);
		static Scene* const GetScene() { return _activeScene; }
		static sf::RenderWindow& GetWindow();
		static sf::Vector2u GetWindowSize();
		static void SetFramerateCap(double newCap) { _framerateCap = newCap; }
		static void SetVsync(bool b);

		static double GetTimeDilation() { return _timeDilation; }
		static void SetTimeDilation(double timeDilation) { _timeDilation = timeDilation; }

		static bool GetPaused() { return _isPaused; }
		static void SetPause(bool isPaused) { _isPaused = isPaused; }

	private:
		static Scene* _activeScene;
		static std::string _gameName;
		static double _framerateCap;
		static bool _isPaused;
		static double _timeDilation;
		static void Update(double dt);
		static void Render(sf::RenderWindow& window); 
};

namespace timing {
// Return time since Epoc
long long now();
// Return time since last() was last called.
long long last();
} // namespace timing