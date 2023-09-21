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
	std::array<glm::vec3, 8> random_positions(std::array<glm::vec3, 8> direction_positions, float offset);
	//----- game state -----

	// input tracking:
	struct Button
	{
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} left, right, down, up, shift, space;

	// local copy of the game scene (so code can change it during gameplay):
	Scene scene;
	Scene::Transform *player = nullptr;
	Scene::Transform *enemy = nullptr;

	// life
	int life = 5;
	bool ishurt = false;

	// jump
	bool isjump = false;
	bool isjumping = false;
	int jumptimer = 0;

	// Bullet
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
		std::array<glm::vec3, 8> random_pos;

	} Bullets;
	// Bullets origin_bullets;
	std::array<glm::vec3, 8> direction_positions;
	std::array<Bullets, 5> bullets_list;
	glm::vec3 original_Pos = glm::vec3(0, 0, 0);

	float bullet_speed = 20;

	// music
	int timer = 0;
	int timer1 = 0;
	int timer2 = 0;
	int timer3 = 0;
	int timer4 = 0;
	int timer5 = 0;

	// beat, notes in each bar, not include the first note, because every bar has one, use a bullets to do the first one
	// int beats[34][4] = {{}, {}, {}, {3}, {3}, {3}, {3}, {3, 5}, {3, 5}, {3, 5}, {5}, {3, 5}, {5}, {3, 5, 7}, {}, {3, 4}, {3, 4}, {3, 4, 5}, {3, 4, 5}, {3, 4, 5, 6}, {3, 4, 5, 6}, {5}, {3, 5, 7}, {3, 4}, {3, 4}, {3, 4, 5}, {3, 4, 5}, {3, 4, 5, 6}, {3, 4, 5, 6}, {5}, {5}, {}, {}, {}};
	std::array<bool, 34> beat3 = {0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 1, 0, 1, 0, 1, 1, 1, 1, 1, 1, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
	std::array<bool, 34> beat4 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 0, 0};
	std::array<bool, 34> beat5 = {0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0};
	std::array<bool, 34> beat6 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0};
	std::array<bool, 34> beat7 = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};

	int beat3_index = 0;
	int beat4_index = 0;
	int beat5_index = 0;
	int beat6_index = 0;
	int beat7_index = 0;

	// music coming from the tip of the leg (as a demonstration):
	std::shared_ptr<Sound::PlayingSample> boss_loop;

	// camera:
	Scene::Camera *camera = nullptr;
};
