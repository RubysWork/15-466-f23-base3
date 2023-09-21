#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>
#include <string>

GLuint boss_meshes_for_lit_color_texture_program = 0;
Load<MeshBuffer> boss_meshes(LoadTagDefault, []() -> MeshBuffer const *
							 {
	MeshBuffer const *ret = new MeshBuffer(data_path("boss.pnct"));
	boss_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret; });

Load<Scene> boss_scene(LoadTagDefault, []() -> Scene const *
					   { return new Scene(data_path("boss.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name)
										  {
											  Mesh const &mesh = boss_meshes->lookup(mesh_name);

											  scene.drawables.emplace_back(transform);
											  Scene::Drawable &drawable = scene.drawables.back();

											  drawable.pipeline = lit_color_texture_program_pipeline;

											  drawable.pipeline.vao = boss_meshes_for_lit_color_texture_program;
											  drawable.pipeline.type = mesh.type;
											  drawable.pipeline.start = mesh.start;
											  drawable.pipeline.count = mesh.count; }); });

Load<Sound::Sample> sound_boss(LoadTagDefault, []() -> Sound::Sample const *
							   { return new Sound::Sample(data_path("boss.wav")); });

Load<Sound::Sample> sound_shoot(LoadTagDefault, []() -> Sound::Sample const *
								{ return new Sound::Sample(data_path("shoot.wav")); });

Load<Sound::Sample> sound_gethurt(LoadTagDefault, []() -> Sound::Sample const *
								  { return new Sound::Sample(data_path("gethurt.wav")); });

Load<Sound::Sample> sound_heal(LoadTagDefault, []() -> Sound::Sample const *
							   { return new Sound::Sample(data_path("heal.wav")); });

PlayMode::PlayMode() : scene(*boss_scene)
{
	int bullet_index = 0;
	Bullets bullets;
	// get pointers to leg for convenience:
	for (auto &transform : scene.transforms)
	{
		if (transform.name == "Player")
			player = &transform;
		else if (transform.name == "BulletF" || transform.name == "BulletF1" || transform.name == "BulletF2" || transform.name == "BulletF3" || transform.name == "BulletF4" || transform.name == "BulletF5" || transform.name == "BulletF6" || transform.name == "Safe")
		{
			Bullet bullet;
			bullet.name = "BulletF";
			bullet.index = bullet_index;
			bullet.transform = &transform;
			if (transform.name == "Safe")
				bullet.isSafe = true;
			else
				bullet.isSafe = false;
			direction_positions[bullet_index] = transform.position;
			bullets.bullets[bullet_index] = bullet;
			bullets.random_pos = direction_positions;
			bullet_index++;
			if (bullet_index > 7)
			{
				bullets_list[0] = bullets;
				bullet_index = 0;
			}
		}
		else if (transform.name == "BulletF7" || transform.name == "BulletF8" || transform.name == "BulletF9" || transform.name == "BulletF10" || transform.name == "BulletF11" || transform.name == "BulletF12" || transform.name == "BulletF13" || transform.name == "Safe1")
		{
			Bullet bullet;
			bullet.name = "BulletF7";
			bullet.index = bullet_index;
			bullet.transform = &transform;
			if (transform.name == "Safe1")
				bullet.isSafe = true;
			else
				bullet.isSafe = false;

			bullets.bullets[bullet_index] = bullet;
			bullets.random_pos = direction_positions;
			bullet_index++;
			if (bullet_index > 7)
			{
				bullets_list[1] = bullets;
				bullet_index = 0;
			}
		}
		else if (transform.name == "BulletF14" || transform.name == "BulletF15" || transform.name == "BulletF16" || transform.name == "BulletF17" || transform.name == "BulletF18" || transform.name == "BulletF19" || transform.name == "BulletF20" || transform.name == "Safe2")
		{
			Bullet bullet;
			bullet.index = bullet_index;
			bullet.transform = &transform;
			if (transform.name == "Safe2")
				bullet.isSafe = true;
			else
				bullet.isSafe = false;

			bullets.bullets[bullet_index] = bullet;
			bullets.random_pos = direction_positions;
			bullet_index++;
			if (bullet_index > 7)
			{
				// bullets_list.emplace_back(bullets);
				bullets_list[2] = bullets;
				bullet_index = 0;
			}
		}
		else if (transform.name == "BulletF21" || transform.name == "BulletF22" || transform.name == "BulletF23" || transform.name == "BulletF24" || transform.name == "BulletF25" || transform.name == "BulletF26" || transform.name == "BulletF27" || transform.name == "Safe3")
		{
			Bullet bullet;
			bullet.index = bullet_index;
			bullet.transform = &transform;
			if (transform.name == "Safe3")
				bullet.isSafe = true;
			else
				bullet.isSafe = false;

			bullets.bullets[bullet_index] = bullet;
			bullets.random_pos = direction_positions;
			bullet_index++;
			if (bullet_index > 7)
			{
				// bullets_list.emplace_back(bullets);
				bullets_list[3] = bullets;
				bullet_index = 0;
			}
		}
		else if (transform.name == "BulletF28" || transform.name == "BulletF29" || transform.name == "BulletF30" || transform.name == "BulletF31" || transform.name == "BulletF32" || transform.name == "BulletF33" || transform.name == "BulletF34" || transform.name == "Safe4")
		{
			Bullet bullet;
			bullet.index = bullet_index;
			bullet.transform = &transform;
			if (transform.name == "Safe4")
				bullet.isSafe = true;
			else
				bullet.isSafe = false;

			bullets.bullets[bullet_index] = bullet;
			bullets.random_pos = direction_positions;

			bullet_index++;
			if (bullet_index > 7)
			{

				bullets_list[4] = bullets;
				bullet_index = 0;
			}
		}
		else if (transform.name == "Enemy")
		{
			enemy = &transform;
		}
	}

	// std::cout << "bullet_size" << bullets_list.size() << "newbullet1: " << bullets_list.begin()->bullets[0].name << " ,Bullet2: " << (bullets_list.begin())++->bullets[0].name << std::endl;

	// get pointer to camera for convenience:
	if (scene.cameras.size() != 1)
		throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	// start music loop playing:
	//  (note: position will be over-ridden in update())
	// boss_loop = Sound::loop_3D(*sound_boss, 1.0f, enemy->position, 10.0f);
	boss_loop = Sound::play_3D(*sound_boss, 1.0f, enemy->position, 10.0f);
	shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
}

PlayMode::~PlayMode()
{
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size)
{
	if (evt.type == SDL_KEYDOWN)
	{
		if (evt.key.keysym.sym == SDLK_ESCAPE)
		{
			SDL_SetRelativeMouseMode(SDL_FALSE);
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_LEFT)
		{
			left.downs += 1;
			left.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_RIGHT)
		{
			right.downs += 1;
			right.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_UP)
		{
			up.downs += 1;
			up.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN)
		{
			down.downs += 1;
			down.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_LSHIFT)
		{
			shift.downs += 1;
			shift.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_SPACE)
		{
			space.downs += 1;
			space.pressed = true;
			return true;
		}
	}
	else if (evt.type == SDL_KEYUP)
	{
		if (evt.key.keysym.sym == SDLK_LEFT)
		{
			left.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_RIGHT)
		{
			right.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_UP)
		{
			up.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_DOWN)
		{
			down.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_LSHIFT)
		{
			shift.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_SPACE)
		{
			space.pressed = false;
			return true;
		}
	}

	return false;
}

void PlayMode::update(float elapsed)
{

	// set bullets position
	int bulletsindex = 0;
	for (auto &bullets : bullets_list)
	{
		bullets.current_time += bullet_speed * elapsed;

		for (auto &bullet : bullets.bullets)
		{ // if out of screen, stop move
			if (bullet.transform->position.x > -60 && bullet.transform->position.y < 35 && bullet.transform->position.y > -35)
			{
				if (bullet.transform != nullptr)
				{
					bullet.transform->position = current_Pos(original_Pos, bullets.random_pos[bullet.index], bullets.current_time);
				}
			}
		}
		bulletsindex++;
	}

	// respawn
	all_timer++;

	if (all_timer % 100 == 0)
	{
		if (beat3_index < 34)
		{
			shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
			bullets_list[0].current_time = 0;
			bullets_list[0].random_pos = random_positions(direction_positions, 0);
			// reset first beat position
			for (auto &bullet : bullets_list[0].bullets)
			{
				bullet.transform->position = current_Pos(original_Pos, direction_positions[bullet.index], bullets_list.begin()->current_time);
			}
		}
	}

	if (all_timer % 138 == 0)
	{
		if (beat3[beat3_index] && beat3_index < 34)
		{
			shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
			bullets_list[1].current_time = 0;
			bullets_list[1].random_pos = random_positions(direction_positions, 9);
			// reset first beat position
			for (auto &bullet : bullets_list[1].bullets)
			{
				bullet.transform->position = current_Pos(original_Pos, bullets_list[1].random_pos[bullet.index], bullets_list[1].current_time);
			}
		}
		beat3_index++;
	}

	if (all_timer % 150 == 0)
	{
		if (beat4[beat4_index] && beat4_index < 34)
		{
			shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
			bullets_list[2].current_time = 0;
			bullets_list[2].random_pos = random_positions(direction_positions, 18);
			// reset first beat position
			for (auto &bullet : bullets_list[2].bullets)
			{
				bullet.transform->position = current_Pos(original_Pos, bullets_list[2].random_pos[bullet.index], bullets_list[2].current_time);
			}
		}
		beat4_index++;
	}

	if (all_timer % 163 == 0)
	{
		if (beat5[beat5_index] && beat5_index < 34)
		{
			shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
			bullets_list[3].current_time = 0;
			bullets_list[3].random_pos = random_positions(direction_positions, -7);
			// reset first beat position
			for (auto &bullet : bullets_list[3].bullets)
			{
				bullet.transform->position = current_Pos(original_Pos, bullets_list[3].random_pos[bullet.index], bullets_list[3].current_time);
			}
		}
		beat5_index++;
	}

	if (all_timer % 175 == 0)
	{
		if (beat6[beat6_index] && beat6_index < 34)
		{
			shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
			bullets_list[4].current_time = 0;
			bullets_list[4].random_pos = random_positions(direction_positions, -14);
			// reset first beat position
			for (auto &bullet : bullets_list[4].bullets)
			{
				bullet.transform->position = current_Pos(original_Pos, bullets_list[4].random_pos[bullet.index], bullets_list[4].current_time);
			}
		}
		beat6_index++;
	}

	if (all_timer % 188 == 0)
	{
		if (beat7[beat7_index] && beat7_index < 34)
		{
			shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
			bullets_list[4].current_time = 0;
			bullets_list[4].random_pos = random_positions(direction_positions, -23);
			// reset first beat position
			for (auto &bullet : bullets_list[4].bullets)
			{
				bullet.transform->position = current_Pos(original_Pos, bullets_list[4].random_pos[bullet.index], bullets_list[4].current_time);
			}
		}
		beat7_index++;
		if (beat7_index > 34)
			over = true;

		all_timer = 88;
	}

	// detect touch
	for (auto &bullets : bullets_list)
	{
		for (auto &bullet : bullets.bullets)
		{
			ishurt = false;
			if (player->position.x > bullet.transform->position.x - 1.5 && player->position.x < bullet.transform->position.x + 1.5 && player->position.y > bullet.transform->position.y - 1.5 && player->position.y < bullet.transform->position.y + 1.5 && player->position.z < 2)
			{
				bullet.transform->position.x = -100;
				bullet.transform->position.y = -50;
				if (!ishurt)
				{
					if (bullet.isSafe)
					{
						heal_sound = Sound::play_3D(*sound_heal, 1.0f, enemy->position, 10.0f);
						life++;
					}

					else
					{
						gethurt_sound = Sound::play_3D(*sound_gethurt, 1.0f, enemy->position, 10.0f);
						life--;
					}

					ishurt = true;
				}
			}
		}
	}

	// player control
	float PlayerSpeed = 30.0f;
	if (!over)
	{
		if (shift.pressed)
			PlayerSpeed = 10.0f;
		else
			PlayerSpeed = 30.0f;
		glm::vec2 move = glm::vec2(0.0f);
		if (left.pressed && !right.pressed)
			move.x = -1.0f;
		if (!left.pressed && right.pressed)
			move.x = 1.0f;
		if (down.pressed && !up.pressed)
			move.y = -1.0f;
		if (!down.pressed && up.pressed)
			move.y = 1.0f;

		// make it so that moving diagonally doesn't go faster:
		if (move != glm::vec2(0.0f))
			move = glm::normalize(move) * PlayerSpeed * elapsed;

		glm::vec3 frame_right = glm::vec3(0, -1, 0);
		glm::vec3 frame_forward = glm::vec3(1, 0, 0);

		// jump
		float jump = 0.0f;
		if (!isjump && !isjumping && space.pressed)
		{
			isjump = true;
			isjumping = true;
		}

		if (isjump)
		{
			jump = +1.0f;
		}
		if (player->position.z > 8)
			isjump = false;

		if (!isjump)
		{
			if (player->position.z > 1.9)
				jump -= 0.3f;
			else if (player->position.z < 1.8)
				player->position.z = 1.9f;
			else
				isjumping = false;
		}

		if ((player->position + (move.x * frame_right + move.y * frame_forward)).x < 5 && (player->position + (move.x * frame_right + move.y * frame_forward)).x > -35 && (player->position + (move.x * frame_right + move.y * frame_forward)).y < 20 && (player->position + (move.x * frame_right + move.y * frame_forward)).y > -20)
			player->position += move.x * frame_right + move.y * frame_forward + jump * glm::vec3(0, 0, 1);
	}
	else
	{
		// restart
		if (down.pressed && down.downs == 1)
		{
			restart_game();
		}
	}

	// reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
	shift.downs = 0;
	space.downs = 0;
}

void PlayMode::draw(glm::uvec2 const &drawable_size)
{
	// update camera aspect ratio for drawable:
	camera->aspect = float(drawable_size.x) / float(drawable_size.y);

	// set up light type and position for lit_color_texture_program:
	//  TODO: consider using the Light(s) in the scene to do this
	glUseProgram(lit_color_texture_program->program);
	glUniform1i(lit_color_texture_program->LIGHT_TYPE_int, 1);
	glUniform3fv(lit_color_texture_program->LIGHT_DIRECTION_vec3, 1, glm::value_ptr(glm::vec3(0.0f, 0.0f, -1.0f)));
	glUniform3fv(lit_color_texture_program->LIGHT_ENERGY_vec3, 1, glm::value_ptr(glm::vec3(1.0f, 1.0f, 0.95f)));
	glUseProgram(0);

	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
	glClearDepth(1.0f); // 1.0 is actually the default value to clear the depth buffer to, but FYI you can change it.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS); // this is the default depth comparison function, but FYI you can change it.

	scene.draw(*camera);

	{ // use DrawLines to overlay some text:
		glDisable(GL_DEPTH_TEST);
		float aspect = float(drawable_size.x) / float(drawable_size.y);
		DrawLines lines(glm::mat4(
			1.0f / aspect, 0.0f, 0.0f, 0.0f,
			0.0f, 1.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f));

		constexpr float H = 0.09f;
		lines.draw_text("Move - Up/Down/Left/Right Arrow; Jump - Space; Move Slow - Shift",
						glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
						glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
						glm::u8vec4(0x00, 0x00, 0x00, 0x00));

		lines.draw_text("Score: " + std::to_string(life),
						glm::vec3(-aspect + 0.1f * H + 2300.0f / drawable_size.y, -1.0 + +0.1f * H, 0.0),
						glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
						glm::u8vec4(0xff, 0x00, 0x00, 0x00));

		// save the ocean
		if (over)
		{
			constexpr float height = 0.3f;
			float offset = 1100.0f / drawable_size.y;
			lines.draw_text("Clear!",
							glm::vec3(-aspect + 0.1f * height + 1096.0f / drawable_size.y, -1.0 + +0.1f * height + 746.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			lines.draw_text("Clear!",
							glm::vec3(-aspect + 0.1f * height + 1098.0f / drawable_size.y, -1.0 + 0.1f * height + 748.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			lines.draw_text("Clear!",
							glm::vec3(-aspect + 0.1f * height + offset, -1.0 + +0.1f * height + 750.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0xff, 0x00, 0xff, 0x00));
			lines.draw_text("Your Final Score Is:",
							glm::vec3(-aspect + 0.1f * height + 496.0f / drawable_size.y, -1.0 + +0.1f * height + 496.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			lines.draw_text("Your Final Score Is:",
							glm::vec3(-aspect + 0.1f * height + 498.0f / drawable_size.y, -1.0 + 0.1f * height + 498.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			lines.draw_text("Your Final Score Is:",
							glm::vec3(-aspect + 0.1f * height + 500.0f / drawable_size.y, -1.0 + +0.1f * height + 500.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0x00, 0x00, 0xff, 0x00));
			lines.draw_text(std::to_string(life),
							glm::vec3(-aspect + 0.1f * height + 1196.0f / drawable_size.y, -1.0 + +0.1f * height + 246.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0xff, 0xff, 0xff, 0x00));
			lines.draw_text(std::to_string(life),
							glm::vec3(-aspect + 0.1f * height + 1198.0f / drawable_size.y, -1.0 + 0.1f * height + 248.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0x00, 0x00, 0x00, 0x00));
			lines.draw_text(std::to_string(life),
							glm::vec3(-aspect + 0.1f * height + 1200.0f / drawable_size.y, -1.0 + +0.1f * height + 250.0f / drawable_size.y, 0.0),
							glm::vec3(height, 0.0f, 0.0f), glm::vec3(0.0f, height, 0.0f),
							glm::u8vec4(0x00, 0x00, 0xff, 0x00));
			lines.draw_text("Press 'down' to replay",
							glm::vec3(-aspect + 0.1f * height + 1050.0f / drawable_size.y, -1.0 + +0.1f * height + 146.0f / drawable_size.y, 0.0),
							glm::vec3(0.1f, 0.0f, 0.0f), glm::vec3(0.0f, 0.1f, 0.0f),
							glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		}
	}
	GL_ERRORS();
}
glm::vec3 PlayMode::current_Pos(glm::vec3 origin_Pos, glm::vec3 final_Pos, float time)
{
	glm::vec3 dir = glm::normalize(final_Pos - origin_Pos);
	glm::vec3 current_Pos = glm::vec3(0, 0, 1) + dir * (1 + time);
	return current_Pos;
}

std::array<glm::vec3, 8> PlayMode::random_positions(std::array<glm::vec3, 8> dir_pos, float offset)
{
	std::array<glm::vec3, 8> ran_pos;
	for (int i = 0; i < dir_pos.size(); i++)
	{
		ran_pos[i] = dir_pos[i];
		ran_pos[i].y -= offset;
	}

	return ran_pos;
}

float PlayMode::ranNum(int min, int max)
{
	float random = (float)abs((rand() % (max - min + 1)) + min);
	// std::cout << "min= " << min << " max: " << max << " random: " << randomX << std::endl;
	return random;
}

void PlayMode::restart_game()
{
	over = false;
	life = 5;
	all_timer = 1;
	beat3_index = 1;
	beat4_index = 1;
	beat5_index = 1;
	beat6_index = 1;
	beat7_index = 1;
	boss_loop = Sound::play_3D(*sound_boss, 1.0f, enemy->position, 10.0f);
	shoot_sound = Sound::play_3D(*sound_shoot, 1.0f, enemy->position, 10.0f);
}