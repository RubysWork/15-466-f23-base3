#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>
#include <array>
#include <string>

struct PlayMode : Mode
{
	PlayMode();
	virtual ~PlayMode();

	// functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	glm::vec3 current_Pos(glm::vec3 origin_Pos, glm::vec3 final_Pos, float time);
	//----- game state -----

	// input tracking:
	struct Button
	{
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up;

	// local copy of the game scene (so code can change it during gameplay):
	Scene scene;
	Scene::Transform *player = nullptr;
	Scene::Transform *enemy = nullptr;

	typedef struct Bullet
	{
		int index = 0;
		std::string name = "";
		bool isSafe = false;
		Scene::Transform *transform = nullptr;
	} Bullet;
	typedef struct Bullets
	{
		std::array<Bullet, 8> bullets;
		float current_time = 0;

	} Bullets;
	Bullets origin_bullets;
	std::array<glm::vec3, 8> final_positions;
	std::list<Bullets> bullets_list;
	glm::vec3 original_Pos = glm::vec3(0, 0, 0);

	float bullet_speed = 10;
	float current_time = 0;

	int timer = 0;

	// glm::vec3 get_leg_tip_position();

	// music coming from the tip of the leg (as a demonstration):
	std::shared_ptr<Sound::PlayingSample> boss_loop;

	// camera:
	Scene::Camera *camera = nullptr;
};
