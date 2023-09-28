#include "PlayMode.hpp"

#include "LitColorTextureProgram.hpp"

#include "DrawLines.hpp"
#include "Mesh.hpp"
#include "Load.hpp"
#include "gl_errors.hpp"
#include "data_path.hpp"

#include <glm/gtc/type_ptr.hpp>

#include <random>

GLuint hexapod_meshes_for_lit_color_texture_program = 0;
Load<MeshBuffer> hexapod_meshes(LoadTagDefault, []() -> MeshBuffer const *
								{
	MeshBuffer const *ret = new MeshBuffer(data_path("void.pnct"));
	hexapod_meshes_for_lit_color_texture_program = ret->make_vao_for_program(lit_color_texture_program->program);
	return ret; });

Load<Scene> hexapod_scene(LoadTagDefault, []() -> Scene const *
						  { return new Scene(data_path("void.scene"), [&](Scene &scene, Scene::Transform *transform, std::string const &mesh_name)
											 {
												 Mesh const &mesh = hexapod_meshes->lookup(mesh_name);

												 scene.drawables.emplace_back(transform);
												 Scene::Drawable &drawable = scene.drawables.back();

												 drawable.pipeline = lit_color_texture_program_pipeline;

												 drawable.pipeline.vao = hexapod_meshes_for_lit_color_texture_program;
												 drawable.pipeline.type = mesh.type;
												 drawable.pipeline.start = mesh.start;
												 drawable.pipeline.count = mesh.count; }); });

PlayMode::PlayMode() : scene(*hexapod_scene)
{
	// Choice t;
	// t.TestChoice();
	choices.ReadCSVFile(data_path("ChoiceData.csv"));
	// Initialize draw text
	text.HB_FT_Init(data_path("PressStart2P-Regular.ttf").c_str(), 50);
	text_program = text.CreateTextShader();

	// get pointer to camera for convenience:
	if (scene.cameras.size() != 1)
		throw std::runtime_error("Expecting scene to have exactly one camera, but it has " + std::to_string(scene.cameras.size()));
	camera = &scene.cameras.front();
}

PlayMode::~PlayMode()
{
}

bool PlayMode::handle_event(SDL_Event const &evt, glm::uvec2 const &window_size)
{

	if (evt.type == SDL_KEYDOWN)
	{
		if (evt.key.keysym.sym == SDLK_SPACE)
		{

			space.downs += 1;
			space.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_1)
		{
			choice1.downs += 1;
			choice1.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_2)
		{

			choice2.downs += 1;
			choice2.pressed = true;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_3)
		{

			choice3.downs += 1;
			choice3.pressed = true;
			return true;
		}
	}
	else if (evt.type == SDL_KEYUP)
	{
		if (evt.key.keysym.sym == SDLK_SPACE)
		{
			space.pressed = false;
			space.downs = 0;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_1)
		{
			choice1.pressed = false;
			choice1.downs = 0;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_2)
		{
			choice2.pressed = false;
			choice2.downs = 0;
			return true;
		}
		else if (evt.key.keysym.sym == SDLK_3)
		{
			choice3.pressed = false;
			choice3.downs = 0;
			return true;
		}
	}
	return false;
}

void PlayMode::update(float elapsed)
{
	// show dialogue
	show_dialogue();
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

	{
		text.Draw(text_program, base_line, 100.0f, 480.0f, drawable_size, glm::vec3(1.0f, 1.0f, 1.0f), 0.4f);
		text.Draw(text_program, choice_lines[0], 100.0f, 250.0f, drawable_size, glm::vec3(1.0f, 1.0f, 0.3f), 0.4f);
		text.Draw(text_program, choice_lines[1], 100.0f, 210.0f, drawable_size, glm::vec3(1.0f, 1.0f, 0.3f), 0.4f);
		text.Draw(text_program, choice_lines[2], 100.0f, 170.0f, drawable_size, glm::vec3(1.0f, 1.0f, 0.3f), 0.4f);
	}

	GL_ERRORS();
}
void PlayMode::show_dialogue()
{
	if (line_index <= choices.choiceLibrary.size() && line_index >= 0)
	{
		// wait until player input, then show next text
		if (showtext)
		{
			//  type:1-single line

			base_line = choices.GetChoice(line_index).baseChoice.context;

			if (choices.GetChoice(line_index).type == 1)
			{
				for (auto &line : choice_lines)
				{
					line = "";
				}
			}
			// 2-choose
			if (choices.GetChoice(line_index).type == 2)
			{
				// show all choice
				for (int i = 0; i < 3; i++)
				{
					if (i < choices.GetChoice(line_index).choiceCount)
					{
						if (choices.GetChoice(line_index).choiceCount > i)
							choice_lines[i] = std::to_string(i + 1) + ": " + choices.GetChoice(line_index).potentialChoice[i].context;
					}
					else
					{
						choice_lines[i] = "";
					}
				}
			}

			// reset player input
			showtext = false;
			space_downcount = 0;
			choice1_downcount = 0;
			choice2_downcount = 0;
			choice3_downcount = 0;
		}

		// detect player input
		if (space.pressed && space_downcount == 0)
		{
			if (choices.GetChoice(line_index).type == 1)
			{
				if (choices.GetChoice(line_index).baseChoice.choiceNext > 0)
				{
					space_downcount++;
					// std::cout << "next is:" << choices.GetChoice(line_index).baseChoice.choiceNext << std::endl;
					line_index = choices.GetChoice(line_index).baseChoice.choiceNext;

					showtext = true;
				}
				else
				{
					std::cout << "no next line!!" << std::endl;
				}
			}
		}

		if (choices.GetChoice(line_index).type == 2)
		{
			int choiceNum = -1;
			if (choice1.pressed && choice1_downcount == 0)
			{
				choice1_downcount++;
				choiceNum = 0;
			}
			if (choice2.pressed && choice2_downcount == 0 && choices.GetChoice(line_index).choiceCount > 1)
			{
				choice2_downcount++;
				choiceNum = 1;
			}

			if (choice3.pressed && choice3_downcount == 0 && choices.GetChoice(line_index).choiceCount > 2)
			{
				choice3_downcount++;
				choiceNum = 2;
			}

			// show next line
			if (choiceNum >= 0)
			{
				if (choices.GetChoice(line_index).potentialChoice[choiceNum].choiceNext > 0)
				{
					line_index = choices.GetChoice(line_index).potentialChoice[choiceNum].choiceNext;
					showtext = true;
				}
				else
				{
					std::cout << "no choose content!!" << std::endl;
				}
			}
		}
	}
}

void PlayMode::change_san(int add_value)
{
	san += add_value;
}

int PlayMode::dice(int maxnum)
{
	int random = abs((rand() % (maxnum + 1)));
	return random;
}
