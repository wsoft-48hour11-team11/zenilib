#include "Play_State.h"

using namespace std;
using namespace Zeni;

enum Action_ID {ACTION_ESCAPE = 1,
                ACTION_JUMP,
                ACTION_LEFT,
                ACTION_RIGHT
               };

Play_State::Play_State()
	: m_grid(Zeni::Point2i(50, 32), Vector2f(0.0f, 0.0f)),
	m_time_passed(0.0f),
	m_max_time_step(1.0f / 30.0f), // make the largest physics step 1/30 of a second
	m_max_time_steps(10.0f) // allow no more than 10 physics steps per frame
{
	set_pausable(true);
  
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_ESCAPE), ACTION_ESCAPE);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_w), ACTION_JUMP);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_a), ACTION_LEFT);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_d), ACTION_RIGHT);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_UP), ACTION_JUMP);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LEFT), ACTION_LEFT);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_RIGHT), ACTION_RIGHT);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_SPACE), ACTION_JUMP);

	//m_grid.load("test_level.txt");
	GameSingleton* sing = GameSingleton::getInstance();
	m_grid.load(sing->level_list[0]);

	m_player = Player(Zeni::Point2f(m_grid.get_spawn_player().x + 4.5f / 16.0f, float(m_grid.get_spawn_player().y)));
  m_player.set_acceleration(Vector2f(0.0f, 16.0f));
	
	m_crawler = Crawler(Point2f(512, 256), Crawler::MOVING_LEFT);
}

Play_State::~Play_State() {
	//m_grid.save("test_level.txt");
}

void Play_State::on_push() {
	get_Window().set_mouse_state(Window::MOUSE_HIDDEN);

  get_Video().set_clear_Color(Color(1.0f, 0.0f, 0.0f, 0.0f));
}

void Play_State::on_pop() {
	get_Controllers().reset_vibration_all();
}

void Play_State::on_cover() {
  get_Controllers().reset_vibration_all();

  get_Video().set_clear_Color(Color(1.0f, 0.0f, 0.0f, 0.0f));
}

void Play_State::on_event(const Zeni_Input_ID &/*id*/, const float &confidence, const int &action) {
  switch(action) {
  case ACTION_ESCAPE:
    if(confidence > 0.5f)
      get_Game().push_Popup_Menu_State();
    break;

  case ACTION_JUMP:
    m_player.jump = confidence > 0.5f;
    break;

  case ACTION_LEFT:
    m_player.left = confidence > 0.5f;
    break;

  case ACTION_RIGHT:
    m_player.right = confidence > 0.5f;
    break;

  default:
    break;
  }
}

void Play_State::perform_logic()
{
	{// Update time_passed
		const Time temp_time = get_Timer().get_time();
		m_time_passed += temp_time.get_seconds_since(m_current_time);
		m_current_time = temp_time;
	}

	/* Shrink time passed to an upper bound
	*
	* If your program is ever paused by your computer for 10 
	* minutes and then allowed to continue running, you don't want 
	* it to pause at the physics loop for another 10 minutes.
	*/
	if(m_time_passed / m_max_time_step > m_max_time_steps)
	{
		m_time_passed = m_max_time_steps * m_max_time_step;
	}

	/* Constant time-step physics update
	*
	* This type of update increases the stability of your program 
	* in scenarios where the time step could allow things to move 
	* further than you expect, potentially jumping though walls or 
	* falling through floors... that sort of thing.  Keep the 
	* max_time_step small and you are safe.
	*/
	while(m_time_passed > m_max_time_step)
	{
		step(m_max_time_step);
		m_time_passed -= m_max_time_step;
	}

	/* Simple physics update
	*
	* This type of update could be the only update you do if you 
	* don't have concerns with respect to jumping though walls, 
	* etc...
	*
	* Otherwise, if you have the other update, this serves to 
	* improve the smoothness of your animations.  With the other 
	* version alone, things can jerk a bit from frame to frame.
	*
	* To really see the improvement, turn up multisampling and try 
	* it with this section (A) commented out, and then (B) not
	* commented out.
	*/
	{
		step(m_time_passed);
		m_time_passed = 0.0f;
	}
}

void Play_State::step(const float &time_step)
{
  m_player.step(time_step);
	m_crawler.step(time_step);

  const size_t width = m_grid.get_width();
  const size_t height = m_grid.get_height();

  const auto pcb = m_player.collision_box();
  for(size_t j = 0; j != height; ++j) {
    for(size_t i = 0; i != width; ++i) {
      switch(m_grid[j][i]) {
      case TILE_FULL:
      case TILE_UPPER_RIGHT:
      case TILE_UPPER_LEFT:
      case TILE_LOWER_RIGHT:
      case TILE_LOWER_LEFT:
        if(m_player.collides_with(std::make_pair(Point2f(i, j), Point2f(i + 1, j + 1)))) {
          const float push_left = fabs(i - pcb.second.x);
          const float push_right = fabs((i + 1) - pcb.first.x);
          const float push_up = fabs(j - pcb.second.y);
          const float push_down = fabs((j + 1) - pcb.first.y);

          if(push_up > 0.15f && push_down > 0.15f) {
            if(push_left < push_right)
              m_player.set_position(m_player.get_position() + Vector2f(-push_left, 0.0f));
            else
              m_player.set_position(m_player.get_position() + Vector2f(push_right, 0.0f));
            m_player.set_velocity(Vector2f(0.0f, m_player.get_velocity().j));
          }

          if(push_left > 0.15f && push_right > 0.15f) {
            if(push_up < push_down) {
              m_player.set_position(m_player.get_position() + Vector2f(0.0f, -push_up));
              m_player.set_can_jump(true);
            }
            else
              m_player.set_position(m_player.get_position() + Vector2f(0.0f, push_down));
            m_player.set_velocity(Vector2f(m_player.get_velocity().i, 0.0f));
          }
        }
        break;

      default:
        break;
      }
    }
  }
}

void Play_State::prerender() {
	get_Video().set_clear_Color(Color());
}

void Play_State::render() {
	Video &vr = get_Video();

  vr.set_2d(make_pair(Point2f(), Point2f(RES_HORIZ, RES_VERT)), true);
  
	//vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(TILE_SIZE * m_grid.get_width(), TILE_SIZE * m_grid.get_height())), true);

	m_grid.render();

	m_player.render(m_grid.get_render_offset());
	m_crawler.render(m_grid.get_render_offset());
}