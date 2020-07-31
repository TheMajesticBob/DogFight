#include "system_resources.h"
#include <fstream>

namespace Resources {


  template<>
  std::shared_ptr<sf::Font> load(const std::string &name) {
    auto f = std::make_shared<sf::Font>();
    if (!f->loadFromFile("res/fonts/" + name)) {
      throw("not found: " + name);
    };
    return f;
  };

  template <> // explicit specialization for T = texture
  std::shared_ptr<sf::Texture> load(const std::string& name) {
    auto tex = std::make_shared<sf::Texture>();
    if (!tex->loadFromFile("res/img/" + name)) {
      throw("not found: " + name);
    };
    return tex;
  };

  template <> // explicit specialization for T = texture
  std::shared_ptr<sf::Shader> load(const std::string& name) 
  {
	  auto shader = std::make_shared<sf::Shader>();
	  std::ifstream frag(("res/shaders/" + name + ".frag").c_str());
	  std::ifstream vert(("res/shaders/" + name + ".vert").c_str());

	  if (frag.good() && vert.good())
	  {
		  if (!shader->loadFromFile("res/shaders/" + name + ".vert", "res/shaders/" + name + ".frag")) 
		  {
			  throw("not found: " + name);
		  }
	  }
	  else 
	  {
		  if (frag.good())
		  {
			  if (!shader->loadFromFile("res/shaders/" + name + ".frag", sf::Shader::Fragment))
			  {
				  throw("not found: " + name);
			  }
		  }
		  if (vert.good())
		  {
			  if (!shader->loadFromFile("res/shaders/" + name + ".vert", sf::Shader::Vertex))
			  {
				  throw("not found: " + name);
			  }
		  }
	  }

	  return shader;
  };

#ifdef SOUND
  template <> // explicit specialization for T = SoundBuffer
  std::shared_ptr<sf::SoundBuffer> load(const std::string& name) {
    auto buf = std::make_shared<sf::SoundBuffer>();
    if (!buf->loadFromFile("res/sound/" + name)) {
      throw("not found: " + name);
    };
    return buf;
  };

  template <> // explicit specialization for T = Music
  std::shared_ptr<sf::Music> load(const std::string& name) {
    auto music = std::make_shared<sf::Music>();
    if (!music->openFromFile("res/sound/" + name)) {
      throw("not found: " + name);
    };
    return music;
  };
#endif // SOUND

}