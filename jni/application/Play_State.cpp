#include "Play_State.h"

#include "PowerSelect.h"

using namespace std;
using namespace Zeni;

enum Action_ID {ACTION_ESCAPE = 1,
                ACTION_JUMP,
                ACTION_LEFT,
                ACTION_RIGHT,
                ACTION_DEPOSIT
               };

Play_State::Play_State()
	: m_grid(Zeni::Point2i(50, 32), Vector2f(0.0f, 0.0f), false),
	m_time_passed(0.0f),
	m_max_time_step(1.0f / 30.0f), // make the largest physics step 1/30 of a second
	m_max_time_steps(10.0f), // allow no more than 10 physics steps per frame,
  m_powerseal(0)
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
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LCTRL), ACTION_DEPOSIT);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_j), ACTION_DEPOSIT);

	//m_grid.load("test_level.txt");
	GameSingleton* sing = GameSingleton::getInstance();
	m_grid.load(sing->level_list[0]);

  const auto seal_positions = m_grid.get_deposits();
  for(auto it = seal_positions.begin(); it != seal_positions.end(); ++it)
    m_power_seals.push_back(PowerSeal(Point2f(*it), POWER_EMPTY));

	m_player = Player(Zeni::Point2f(m_grid.get_spawn_player().x + 4.5f / 16.0f, float(m_grid.get_spawn_player().y)));
  m_player.set_acceleration(Vector2f(0.0f, 32.0f));
	
	m_crawler = Crawler(Point2f(512, 256), Crawler::MOVING_LEFT);
}

Play_State::~Play_State() {
	//m_grid.save("test_level.txt");
}

void Play_State::on_push() {
	get_Window().set_mouse_state(Window::MOUSE_HIDDEN);
}

void Play_State::on_pop() {
	get_Controllers().reset_vibration_all();

  get_Video().set_clear_Color(Color(1.0f, 0.0f, 0.0f, 0.0f));
}

void Play_State::on_cover() {
  get_Controllers().reset_vibration_all();
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

  case ACTION_DEPOSIT:
    if(confidence > 0.5f && m_powerseal) {
      if(m_powerseal->getPower() != POWER_EMPTY) {
        m_player.add_power(m_powerseal->getPower());
        m_powerseal->setPower(POWER_EMPTY);
      }
      else if(!m_player.get_powers().empty())
        get_Game().push_state(new PowerSelect(this, &m_player, m_powerseal));
    }
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

  m_powerseal = 0;

  const auto pcb = m_player.collision_box();
  const Collision::Parallelepiped player_ppd(Point3f(pcb.first), Vector3f(pcb.second.y), Vector3f(pcb.second.x), Vector3f(0,0,1.0f));
  for(size_t j = 0; j != height; ++j) {
    for(size_t i = 0; i != width; ++i) {
      if(m_player.collides_with(std::make_pair(Point2f(i, j), Point2f(i + 1, j + 1)))) {
        switch(m_grid[j][i]) {
        case TILE_FULL:
          {
            const float push_left = fabs(i - pcb.second.x);
            const float push_right = fabs((i + 1) - pcb.first.x);
            const float push_up = fabs(j - pcb.second.y);
            const float push_down = fabs((j + 1) - pcb.first.y);

            if(push_up > 0.15f && push_down > 0.15f) {
              if(push_left < push_right) {
                m_player.set_position(m_player.get_position() + Vector2f(-push_left, 0.0f));
                m_player.set_velocity(Vector2f(0.0f, m_player.get_velocity().j));
              }
              else {
                m_player.set_position(m_player.get_position() + Vector2f(push_right, 0.0f));
                m_player.set_velocity(Vector2f(0.0f, m_player.get_velocity().j));
              }
              m_player.state = Player::STATE_ON_WALL;
            }

            if(push_left > 0.15f && push_right > 0.15f) {
              if(push_up < push_down) {
                m_player.set_position(m_player.get_position() + Vector2f(0.0f, -push_up));
                m_player.state = Player::STATE_ON_GROUND;
              }
              else
                m_player.set_position(m_player.get_position() + Vector2f(0.0f, push_down));
              m_player.set_velocity(Vector2f(m_player.get_velocity().i, 0.0f));
            }
          }
          break;

        case TILE_LOWER_LEFT:
          {
            const Collision::Line_Segment ls(Point3f(i, j, 0.0f), Point3f(i + 1.0f, j + 1.0f, 0.0f));
            const auto np = ls.nearest_point(Point3f(pcb.first.x, pcb.second.y, 0.0f));
            if(np.second >= 0.0f && np.second < 1.0f) {
              const bool below = Vector3f(-1.0f, 1.0f, 0.0f).normalized() *
                (Point3f(pcb.first.x, pcb.second.y, 0.0f) - Point3f(i + 0.5f, j + 0.5f, 0.0f)) > 0.0f;

              if(below) {
                m_player.set_position(m_player.get_position() + Vector2f(1.0f, -1.0f, 0.0f).normalized() * np.first);
                m_player.set_velocity(Vector2f(std::max(0.0f, m_player.get_velocity().i), std::min(0.0f, m_player.get_velocity().j)));
                m_player.state = Player::STATE_ON_LOWER_LEFT;
              }
            }
          }
          break;

        case TILE_LOWER_RIGHT:
          {
            const Collision::Line_Segment ls(Point3f(i, j + 1.0f, 0.0f), Point3f(i + 1.0f, j, 0.0f));
            const auto np = ls.nearest_point(Point3f(pcb.second.x, pcb.second.y, 0.0f));
            if(np.second >= 0.0f && np.second < 1.0f) {
              const bool below = Vector3f(1.0f, 1.0f, 0.0f).normalized() *
                (Point3f(pcb.second.x, pcb.second.y, 0.0f) - Point3f(i + 0.5f, j + 0.5f, 0.0f)) > 0.0f;

              if(below) {
                m_player.set_position(m_player.get_position() + Vector2f(-1.0f, -1.0f, 0.0f).normalized() * np.first);
                m_player.set_velocity(Vector2f(std::min(0.0f, m_player.get_velocity().i), std::min(0.0f, m_player.get_velocity().j)));
                m_player.state = Player::STATE_ON_LOWER_RIGHT;
              }
            }
          }
          break;

        case TILE_UPPER_LEFT:
          if(m_player.state != Player::STATE_ON_WALL) {
            const Collision::Line_Segment ls(Point3f(i, j + 1.0f, 0.0f), Point3f(i + 1.0f, j, 0.0f));
            const auto np = ls.nearest_point(Point3f(pcb.first.x, pcb.first.y, 0.0f));
            if(np.second >= 0.0f && np.second < 1.0f) {
              const bool above = Vector3f(-1.0f, -1.0f, 0.0f).normalized() *
                (Point3f(pcb.first.x, pcb.first.y, 0.0f) - Point3f(i + 0.5f, j + 0.5f, 0.0f)) > 0.0f;

              if(above) {
                m_player.set_position(m_player.get_position() + Vector2f(1.0f, 1.0f, 0.0f).normalized() * np.first);
                m_player.set_velocity(Vector2f(std::max(0.0f, m_player.get_velocity().i), std::max(0.0f, m_player.get_velocity().j)));
              }
            }
          }
          break;

        case TILE_UPPER_RIGHT:
          if(m_player.state != Player::STATE_ON_WALL) {
            const Collision::Line_Segment ls(Point3f(i, j, 0.0f), Point3f(i + 1.0f, j + 1.0f, 0.0f));
            const auto np = ls.nearest_point(Point3f(pcb.second.x, pcb.first.y, 0.0f));
            if(np.second >= 0.0f && np.second < 1.0f) {
              const bool above = Vector3f(1.0f, -1.0f, 0.0f).normalized() *
                (Point3f(pcb.second.x, pcb.first.y, 0.0f) - Point3f(i + 0.5f, j + 0.5f, 0.0f)) > 0.0f;

              if(above) {
                m_player.set_position(m_player.get_position() + Vector2f(-1.0f, 1.0f, 0.0f).normalized() * np.first);
                m_player.set_velocity(Vector2f(std::min(0.0f, m_player.get_velocity().i), std::max(0.0f, m_player.get_velocity().j)));
              }
            }
          }
          break;

        case TILE_DEPOSIT:
          for(auto it = m_power_seals.begin(); it != m_power_seals.end(); ++it) {
            if(m_player.collides_with(std::make_pair(it->get_position(), it->get_position() + Vector2f(1.0f, 1.0f)))) {
              m_powerseal = &*it;
              break;
            }
          }
          break;

        default:
          break;
        }
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

  for(auto it = m_power_seals.begin(); it != m_power_seals.end(); ++it)
    it->render(m_grid.get_render_offset());

	m_player.render(m_grid.get_render_offset());
	m_crawler.render(m_grid.get_render_offset());
}