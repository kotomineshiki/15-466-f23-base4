#include "Mode.hpp"

#include "Scene.hpp"
#include "Sound.hpp"
#include "Choice.hpp"
#include "DrawText.hpp"

#include <glm/glm.hpp>

#include <vector>
#include <deque>

struct PlayMode : Mode
{
	PlayMode();
	virtual ~PlayMode();

	// functions called by main loop:
	virtual bool handle_event(SDL_Event const &, glm::uvec2 const &window_size) override;
	virtual void update(float elapsed) override;
	virtual void draw(glm::uvec2 const &drawable_size) override;
	void show_dialogue();
	void change_san(int add_value);
	int dice(int maxnum);
	//----- game state -----

	// input tracking:
	struct Button
	{
		uint8_t downs = 0;
		uint8_t pressed = 0;
	} space, choice1, choice2, choice3;

	// local copy of the game scene (so code can change it during gameplay):
	Scene scene;

	bool showtext = true;
	int line_index = 1;
	bool choosed = false;

	int space_downcount = 0;
	int choice1_downcount = 0;
	int choice2_downcount = 0;
	int choice3_downcount = 0;

	int san = 10;

	ChoiceTable choices;

	int next_index = 1;
	// camera:
	Scene::Camera *camera = nullptr;

	// text
	DrawText text;
	GLuint text_program;
};
