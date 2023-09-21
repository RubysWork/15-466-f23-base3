#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

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

Load<Sound::Sample> sound_sample(LoadTagDefault, []() -> Sound::Sample const *
								 { return new Sound::Sample(data_path("boss.wav")); });

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
			bullet.index = bullet_index;
			bullet.transform = &transform;
			if (transform.name == "Safe")
				bullet.isSafe = true;
			else
				bullet.isSafe = false;
			direction_positions[bullet_index] = transform.position;
			bullets.bullets[bullet_index] = bullet;
			bullet_index++;
			if (bullet_index > 7)
			{
				bullets_list.emplace_back(bullets);
				bullet_index = 0;
			}
		}
		else if (transform.name == "BulletF7" || transform.name == "BulletF8" || transform.name == "BulletF9" || transform.name == "BulletF10" || transform.name == "BulletF11" || transform.name == "BulletF12" || transform.name == "BulletF13" || transform.name == "Safe1")
		{
			Bullet bullet;
			bullet.index = bullet_index;
			bullet.transform = &transform;
			if (transform.name == "Safe1")
				bullet.isSafe = true;
			else
				bullet.isSafe = false;
			bullets.bullets[bullet_index] = bullet;
			bullet_index++;
			if (bullet_index > 7)
			{
				bullets_list.emplace_back(bullets);
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
			bullet_index++;
			if (bullet_index > 7)
			{
				bullets_list.emplace_back(bullets);
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
			bullet_index++;
			if (bullet_index > 7)
			{
				bullets_list.emplace_back(bullets);
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
			// transform.position = glm::vec3(0, 0, 0);
			bullet_index++;
			if (bullet_index > 7)
			{
				bullets_list.emplace_back(bullets);
				bullet_index = 0;
			}
		}
		else if (transform.name == "Enemy")
		{
			enemy = &transform;
		}
	}
	// get pointer to camera for convenience:
	if (scene.cameras.size() != 1)
		throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();

	// start music loop playing:
	//  (note: position will be over-ridden in update())
	boss_loop = Sound::loop_3D(*sound_sample, 1.0f, enemy->position, 10.0f);
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
		else if (evt.key.keysym.sym == SDLK_a)
		{
			left.downs += 1;
			left.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_d)
		{
			right.downs += 1;
			right.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_w)
		{
			up.downs += 1;
			up.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_s)
		{
			down.downs += 1;
			down.pressed = true;
			return true;
		}
	}
	else if (evt.type == SDL_KEYUP)
	{
		if (evt.key.keysym.sym == SDLK_a)
		{
			left.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_d)
		{
			right.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_w)
		{
			up.pressed = false;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_s)
		{
			down.pressed = false;
			return true;
		}
	}
	/*
	else if (evt.type == SDL_MOUSEBUTTONDOWN) {
		if (SDL_GetRelativeMouseMode() == SDL_FALSE) {
			SDL_SetRelativeMouseMode(SDL_TRUE);
			return true;
		}
	} else if (evt.type == SDL_MOUSEMOTION) {
		if (SDL_GetRelativeMouseMode() == SDL_TRUE) {
			glm::vec2 motion = glm::vec2(
				evt.motion.xrel / float(window_size.y),
				-evt.motion.yrel / float(window_size.y)
			);
			camera->transform->rotation = glm::normalize(
				camera->transform->rotation
				* glm::angleAxis(-motion.x * camera->fovy, glm::vec3(0.0f, 1.0f, 0.0f))
				* glm::angleAxis(motion.y * camera->fovy, glm::vec3(1.0f, 0.0f, 0.0f))
			);
			return true;
		}
	}
*/
	return false;
}

void PlayMode::update(float elapsed)
{

	int bulletsindex = 0;
	std::list<Bullets> old_bullet_list;
	for (auto &bullets : bullets_list)
	{
		// if out of screen, stop move
		if (bullets.bullets[4].transform->position.x > -40 && bullets.bullets[4].transform->position.y < 25 && bullets.bullets[4].transform->position.y > -25)
		{
			for (auto &bullet_list : old_bullet_list)
			{
				for (auto &bullet : bullet_list.bullets)
				{
					bullet.transform->position = glm::vec3(-46, -30, 1);
				}
			}
			bullets.current_time += bullet_speed * elapsed;
			for (auto &bullet : bullets.bullets)
			{
				if (bullet.transform != nullptr)
				{
					bullet.transform->position = current_Pos(original_Pos, direction_positions[bullet.index], bullets.current_time);
				}
			}
		}
		else
		{
			old_bullet_list.emplace_back(bullets);

			//  for (auto bullet : bullets.bullets)
			//  {
			//      if (bullet.transform != nullptr)
			//      {
			//          scene.drawables.remove(bullet.transform);
			//          Scene::Drawable &drawable = scene.drawables.back();
			//      }
			//  }
		}
		std::cout << "bullets" << bulletsindex << ": curent time: " << bullets.current_time << " x:  " << bullets.bullets[0].transform->position.x << ",y: " << bullets.bullets[0].transform->position.y << std::endl;
		bulletsindex++;
	}
	//	std::cout << "bullets1:" << (bullets_list.begin())->bullets[0].transform->position.x << ",y: " << (bullets_list.begin())->bullets[0].transform->position.y << std::endl;
	// std::cout << "bullets2:" << (bullets_list.begin())->bullets[0].transform->position.x << ",y: " << bullets.bullets[index].transform->position.y << std::endl;
	/*
		timer++;
		if (timer > 200)
		{
			int index = 0;
			Bullets new_bullets;
			for (auto bullet : origin_bullets.bullets)
			{
				new_bullets.bullets[index] = bullet;
				scene.drawables.emplace_back(new_bullets.bullets[index].transform);
				index++;
			}
			bullets_list.emplace_back(new_bullets);
			timer = 0;
			std::cout << "generate!!" << scene.drawables.size() << std::endl;
		}
	*/
	timer++;
	// each 2 seconds, generate first beat
	if (timer > 140)
	{
		bullets_list.begin()->current_time = 0;
		// reset first beat position
		for (auto bullet : bullets_list.begin()->bullets)
		{
			bullet.transform->position = current_Pos(original_Pos, direction_positions[bullet.index], bullets_list.begin()->current_time);
		}
		timer = 0;
	}

	// scene.drawables.emplace_back(bullets[0].transform);

	// player control
	constexpr float PlayerSpeed = 30.0f;
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

	if ((player->position + (move.x * frame_right + move.y * frame_forward)).x < 5 && (player->position + (move.x * frame_right + move.y * frame_forward)).x > -35 && (player->position + (move.x * frame_right + move.y * frame_forward)).y < 20 && (player->position + (move.x * frame_right + move.y * frame_forward)).y > -20)
		player->position += move.x * frame_right + move.y * frame_forward;
	// std::cout << "player x:" << player->position.x << " player.y: " << player->position.y << std::endl;

	// reset button press counters:
	left.downs = 0;
	right.downs = 0;
	up.downs = 0;
	down.downs = 0;
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
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
						glm::vec3(-aspect + 0.1f * H, -1.0 + 0.1f * H, 0.0),
						glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
						glm::u8vec4(0x00, 0x00, 0x00, 0x00));
		float ofs = 2.0f / drawable_size.y;
		lines.draw_text("Mouse motion rotates camera; WASD moves; escape ungrabs mouse",
						glm::vec3(-aspect + 0.1f * H + ofs, -1.0 + +0.1f * H + ofs, 0.0),
						glm::vec3(H, 0.0f, 0.0f), glm::vec3(0.0f, H, 0.0f),
						glm::u8vec4(0xff, 0xff, 0xff, 0x00));
	}
	GL_ERRORS();
}
glm::vec3 PlayMode::current_Pos(glm::vec3 origin_Pos, glm::vec3 final_Pos, float time)
{
	glm::vec3 dir = glm::normalize(final_Pos - origin_Pos);
	glm::vec3 current_Pos = glm::vec3(0, 0, 1) + dir * (1 + time);
	return current_Pos;
}
