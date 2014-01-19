#include "Play_State.h"

#include "Crawler.h"
#include "PowerSelect.h"
#include "Portal.h"
#include "LevelIntroState.h"

#include <memory>

using namespace std;
using namespace Zeni;

enum Action_ID {ACTION_ESCAPE = 1,
                ACTION_JUMP,
                ACTION_LEFT,
                ACTION_RIGHT,
                ACTION_DEPOSIT,
                ACTION_DEATH_RAY,
                ACTION_TELEPORT
               };

Play_State::Play_State(const int &level_number)
	: m_level_number(level_number),
  m_grid(Zeni::Point2i(50, 32), Vector2f((RES_HORIZ - 50 * TILE_SIZE) / 2.0f, (RES_VERT - 32 * TILE_SIZE) / 2.0f), false),
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
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LCTRL), ACTION_DEPOSIT);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_k), ACTION_DEATH_RAY);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LSHIFT), ACTION_DEATH_RAY);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_l), ACTION_TELEPORT);
  set_action(Zeni_Input_ID(SDL_KEYDOWN, SDLK_LALT), ACTION_TELEPORT);

	//m_grid.load("test_level.txt");
	GameSingleton* sing = GameSingleton::getInstance();
	m_grid.load(sing->level_list[0]);

  const auto seal_positions = m_grid.get_deposits();
  for(auto it = seal_positions.begin(); it != seal_positions.end(); ++it)
    m_power_seals.push_back(PowerSeal(Point2f(*it), POWER_EMPTY));

  const auto crawler_positions = m_grid.get_crawlers();
  for(auto it = crawler_positions.begin(); it != crawler_positions.end(); ++it)
    m_enemies.push_back(new Crawler(Point2f(*it), Crawler::MOVING_LEFT));

	m_player = Player(Zeni::Point2f(m_grid.get_spawn_player().x + 4.5f / 16.0f, float(m_grid.get_spawn_player().y)));
  m_player.set_acceleration(Vector2f(0.0f, 32.0f));
	
	//m_crawler = Crawler(Point2f(512, 256), Crawler::MOVING_LEFT);
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

  case ACTION_DEATH_RAY:
	if (m_player.has_power(POWER_DEATHRAY) && confidence == 1.0)
	{
		m_deathrays.push_back(new DeathRay(m_player.get_position(), DeathRay::MOVING_RIGHT));
	}
	break;

  case ACTION_DEPOSIT:
    if(confidence > 0.5f && m_powerseal && !m_player.get_powers().empty()) {
      if(m_powerseal->getPower() != POWER_EMPTY) {
        m_player.add_power(m_powerseal->getPower());
        m_powerseal->setPower(POWER_EMPTY);
      }
      else {
        if(m_player.get_powers().size() == 1) {
          m_powerseal->setPower(m_player.get_powers()[0]);
          m_player.remove_power(m_player.get_powers()[0]);

          // Wincon unlocked
          m_portal = std::make_shared<Portal>(Point2f(m_grid.get_spawn_player()));
        }
        else
          get_Game().push_state(new PowerSelect(this, &m_player, m_powerseal));
      }
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
  for (list<Enemy*>::iterator i = m_enemies.begin(); i != m_enemies.end(); i++)
  {
	  (*i)->step(time_step);
  }
  for (list<DeathRay*>::iterator i = m_deathrays.begin(); i != m_deathrays.end(); i++)
  {
	  (*i)->step(time_step);
  }

  const size_t width = m_grid.get_width();
  const size_t height = m_grid.get_height();

  m_powerseal = 0;

  const auto pcb = m_player.collision_box();
  const Collision::Parallelepiped player_ppd(Point3f(pcb.first), Vector3f(pcb.second.y), Vector3f(pcb.second.x), Vector3f(0,0,1.0f));
  for(int k = 0; k != 1; ++k) {
    for(int j = int(pcb.first.y) - 2; j != int(pcb.second.y) + 2; ++j) {
      if(j < 0 || j >= height)
        continue;

      for(int i = int(pcb.first.x) - 2; i != int(pcb.second.x) + 2; ++i) {
        if(i < 0 || i >= width)
          continue;

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
              else if(push_left > 0.15f && push_right > 0.15f) {
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
                  float garbage;
                  Point2f rel(modf(pcb.first.x, &garbage), modf(pcb.second.y, &garbage));
                  if(pcb.first.x < i)
                    rel.x = 0.0f;
                  if(j + 1.0f <= pcb.second.y)
                    rel.y = 1.0f;

                  const Point2f top(i + rel.x, j + rel.x);
                  const Point2f bottom(i + rel.x, j + 1.0f);
                  const Point2f left(i, j + rel.y);
              
                  const float push_left = fabs(left.x - pcb.second.x);
                  const float push_up = fabs(top.y - pcb.second.y);
                  const float push_down = fabs(bottom.y - pcb.first.y);
              
                  //if(push_left > 0.05f) {
                    if(push_up < push_down) {
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, -push_up));
                      m_player.state = Player::STATE_ON_LOWER_LEFT;
                    }
                    else
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, push_down));
                    m_player.set_velocity(Vector2f(m_player.get_velocity().i, 0.0f));
                  //}
                  //else if(push_up > 0.25f && push_down > 0.25f) {
                  //  m_player.set_position(m_player.get_position() + Vector2f(-push_left, 0.0f));
                  //  m_player.set_velocity(Vector2f(0.0f, m_player.get_velocity().j));
                  //  m_player.state = Player::STATE_ON_WALL;
                  //}
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
                  float garbage;
                  Point2f rel(modf(pcb.second.x, &garbage), modf(pcb.second.y, &garbage));
                  if(i + 1.0f <= pcb.second.x)
                    rel.x = 1.0f;
                  if(j + 1.0f <= pcb.second.y)
                    rel.y = 1.0f;

                  const Point2f top(i + rel.x, j + 1.0f - rel.x);
                  const Point2f bottom(i + rel.x, j + 1.0f);
                  const Point2f right(i + 1.0f, j + rel.y);
              
                  const float push_right = fabs(right.x - pcb.first.x);
                  const float push_up = fabs(top.y - pcb.second.y);
                  const float push_down = fabs(bottom.y - pcb.first.y);
              
                  //if(push_right > 0.05f) {
                    if(push_up < push_down) {
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, -push_up));
                      m_player.state = Player::STATE_ON_LOWER_RIGHT;
                    }
                    else
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, push_down));
                    m_player.set_velocity(Vector2f(m_player.get_velocity().i, 0.0f));
                  //}
                  //else if(push_up > 0.25f && push_down > 0.25f) {
                  //  m_player.set_position(m_player.get_position() + Vector2f(push_right, 0.0f));
                  //  m_player.set_velocity(Vector2f(0.0f, m_player.get_velocity().j));
                  //  m_player.state = Player::STATE_ON_WALL;
                  //}
                }
              }
            }
            break;

          case TILE_UPPER_LEFT:
            {
              const Collision::Line_Segment ls(Point3f(i, j + 1.0f, 0.0f), Point3f(i + 1.0f, j, 0.0f));
              const auto np = ls.nearest_point(Point3f(pcb.first.x, pcb.first.y, 0.0f));
              if(np.second >= 0.0f && np.second < 1.0f) {
                const bool above = Vector3f(-1.0f, -1.0f, 0.0f).normalized() *
                  (Point3f(pcb.first.x, pcb.first.y, 0.0f) - Point3f(i + 0.5f, j + 0.5f, 0.0f)) > 0.0f;

                if(above && pcb.second.x > i && pcb.first.x < i + 1.0f) {
                  float garbage;
                  Point2f rel(modf(pcb.first.x, &garbage), modf(pcb.first.y, &garbage));
                  if(pcb.first.x < i)
                    rel.x = 0.0f;
                  if(pcb.second.y < j)
                    rel.y = 0.0f;

                  const Point2f top(i + rel.x, j);
                  const Point2f bottom(i + rel.x, j + 1.0f - rel.x);
                  const Point2f left(i, j + rel.y);
              
                  const float push_left = fabs(left.x - pcb.second.x);
                  const float push_up = fabs(top.y - pcb.second.y);
                  const float push_down = fabs(bottom.y - pcb.first.y);
              
                  //if(push_left > 0.05f) {
                    if(push_up < push_down) {
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, -push_up));
                      m_player.state = Player::STATE_ON_LOWER_LEFT;
                    }
                    else
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, push_down));
                    m_player.set_velocity(Vector2f(m_player.get_velocity().i, 0.0f));
                  //}
                  //else if(push_up > 0.25f && push_down > 0.25f) {
                  //  m_player.set_position(m_player.get_position() + Vector2f(-push_left, 0.0f));
                  //  m_player.set_velocity(Vector2f(0.0f, m_player.get_velocity().j));
                  //  m_player.state = Player::STATE_ON_WALL;
                  //}
                }
              }
            }
            break;

          case TILE_UPPER_RIGHT:
            {
              const Collision::Line_Segment ls(Point3f(i, j, 0.0f), Point3f(i + 1.0f, j + 1.0f, 0.0f));
              const auto np = ls.nearest_point(Point3f(pcb.second.x, pcb.first.y, 0.0f));
              if(np.second >= 0.0f && np.second < 1.0f) {
                const bool above = Vector3f(1.0f, -1.0f, 0.0f).normalized() *
                  (Point3f(pcb.second.x, pcb.first.y, 0.0f) - Point3f(i + 0.5f, j + 0.5f, 0.0f)) > 0.0f;

                if(above && pcb.second.x > i && pcb.first.x < i + 1.0f) {
                  float garbage;
                  Point2f rel(modf(pcb.second.x, &garbage), modf(pcb.first.y, &garbage));
                  if(i + 1.0f <= pcb.second.x)
                    rel.x = 1.0f;
                  if(pcb.first.y < j)
                    rel.y = 0.0f;

                  const Point2f top(i + rel.x, j);
                  const Point2f bottom(i + rel.x, j + rel.x);
                  const Point2f right(i + 1.0f, j + rel.y);
              
                  const float push_right = fabs(right.x - pcb.first.x);
                  const float push_up = fabs(top.y - pcb.second.y);
                  const float push_down = fabs(bottom.y - pcb.first.y);
              
                  //if(push_right > 0.05f) {
                    if(push_up < push_down) {
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, -push_up));
                      m_player.state = Player::STATE_ON_LOWER_RIGHT;
                    }
                    else
                      m_player.set_position(m_player.get_position() + Vector2f(0.0f, push_down));
                    m_player.set_velocity(Vector2f(m_player.get_velocity().i, 0.0f));
                  //}
                  //else if(push_up > 0.25f && push_down > 0.25f) {
                  //  m_player.set_position(m_player.get_position() + Vector2f(push_right, 0.0f));
                  //  m_player.set_velocity(Vector2f(0.0f, m_player.get_velocity().j));
                  //  m_player.state = Player::STATE_ON_WALL;
                  //}
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

          case TILE_SPAWN_PLAYER:
            if(m_portal) {
              get_Game().pop_state();
              get_Game().push_state(new LevelIntroState(m_level_number + 1));
            }
            break;

          default:
            break;
          }
        }
      }
    }
  }

  //Enemy collisions with Player
  if (!m_player.has_power(POWER_SHADOW))
  {
	  for (list<Enemy*>::iterator i = m_enemies.begin(); i != m_enemies.end(); i++)
	  {
		  if (m_player.collides_with((*i)->getCollisionBox()))
		  {
			  (*i)->applyCollisionEffect(m_player);

			  //Kill whatever collided with the player
			  (*i)->setDeleteThis(true);
		  }
	  }
  }

  //Deathray collisions
  for (list<DeathRay*>::iterator j = m_deathrays.begin(); j != m_deathrays.end(); j++)
  {
	  for (list<Enemy*>::iterator i = m_enemies.begin(); i != m_enemies.end(); i++)
	  {
		  if ((*j)->collides_with((*i)->getCollisionBox()))
		  {
			  //Apply the Deathray effect
			  (*j)->applyCollisionEffect(*(*i));
		  }
	  }
  }

  //Clean up any Enemies that should be deleted
  list<Enemy*>::iterator i = m_enemies.begin();
  while(i != m_enemies.end())
  {
	  if ((*i)->getDeleteThis())
	  {
		  Enemy* temp = *i;
		  i = m_enemies.erase(i);
		  delete temp;
	  }
	  else
	  {
		  i++;
	  }
  }
  
  //Clean up any Deathrays that should be deleted
  list<DeathRay*>::iterator j = m_deathrays.begin();
  while(j != m_deathrays.end())
  {
	  if ((*j)->getDeleteThis())
	  {
		  DeathRay* temp = *j;
		  j = m_deathrays.erase(j);
		  delete temp;
	  }
	  else
	  {
		  j++;
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
  
  if(m_portal)
    m_portal->render(m_grid.get_render_offset());
	m_player.render(m_grid.get_render_offset());

  for (list<Enemy*>::iterator i = m_enemies.begin(); i != m_enemies.end(); i++)
  {
	  (*i)->render(m_grid.get_render_offset());
  }
  for (list<DeathRay*>::iterator i = m_deathrays.begin(); i != m_deathrays.end(); i++)
  {
	  (*i)->render(m_grid.get_render_offset());
  }
}