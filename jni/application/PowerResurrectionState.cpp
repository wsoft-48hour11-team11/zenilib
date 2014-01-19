#include "PowerResurrectionState.h"

#include "DefeatState.h"

using namespace std;
using namespace Zeni;

PowerResurrectionState::PowerResurrectionState(Zeni::Gamestate gamestate, const int &level_number, Player* player, std::map<Power, Zeni::Point2f> power_locations, Zeni::Vector2f grid_offset)
	: m_game_state(gamestate),
	m_time_passed(0.0f),
  m_time_to_process(0.0f),
  m_time_processed(0.0f),
	m_max_time_step(1.0f / 60.0f), // make the largest physics step 1/30 of a second
	m_max_time_steps(10.0f), // allow no more than 10 physics steps per frame,
	m_level_number(level_number),
	m_power_locations(power_locations),
	m_grid_offset(grid_offset),
	m_game_over(false),
    m_death_countdown(5.0f)
{
	m_player = player;
}

PowerResurrectionState::~PowerResurrectionState()
{
}

void PowerResurrectionState::on_cover()
{
	m_chrono.stop();
}

void PowerResurrectionState::on_uncover()
{
	m_chrono.start();
}

void PowerResurrectionState::on_push()
{
	m_chrono.start();
}

void PowerResurrectionState::on_pop()
{
	m_chrono.stop();
}

void PowerResurrectionState::on_event(const Zeni::Zeni_Input_ID &id, const float &confidence, const int &action)
{
}

void PowerResurrectionState::perform_logic()
{
	{// Update time_passed
		const float time_passed = m_chrono.seconds();
    m_time_to_process += time_passed - m_time_passed;
		m_time_passed = time_passed;
	}

	/* Shrink time passed to an upper bound
	*
	* If your program is ever paused by your computer for 10 
	* minutes and then allowed to continue running, you don't want 
	* it to pause at the physics loop for another 10 minutes.
	*/
	if(m_time_to_process / m_max_time_step > m_max_time_steps)
	{
		m_time_to_process = m_max_time_steps * m_max_time_step;
	}

	/* Constant time-step physics update
	*
	* This type of update increases the stability of your program 
	* in scenarios where the time step could allow things to move 
	* further than you expect, potentially jumping though walls or 
	* falling through floors... that sort of thing.  Keep the 
	* max_time_step small and you are safe.
	*/
	while(m_time_to_process > m_max_time_step)
	{
		step(m_max_time_step);
		m_time_to_process -= m_max_time_step;
    m_time_processed += m_max_time_step;
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
	//{
	//	step(m_time_to_process);
	//	m_time_passed = 0.0f;
  //  m_time_processed += m_time_to_process;
	//}
}

void PowerResurrectionState::step(const float &time_step)
{
	if (m_game_over)
	{
		m_death_countdown -= time_step;
		if (m_death_countdown <= 0)
		{
			get_Game().pop_state();
			get_Game().push_state(new DefeatState(m_level_number));
		}
	}
	bool powers_combined = true;
	for (map<Power, Zeni::Point2f>::iterator i = m_power_locations.begin(); i != m_power_locations.end(); i++)
	{
		Point2f player_pos = m_player->get_position();
		float close_enough = 0.1;
		if (i->second.x != player_pos.x)
		{
			powers_combined = false;
			float delta_x = 2 * (player_pos.x - i->second.x) * time_step;
			if ((player_pos.x - i->second.x < close_enough && player_pos.x - i->second.x > -close_enough) ||
				(i->second.x < player_pos.x && i->second.x + delta_x - player_pos.x > 0) ||
				(i->second.x > player_pos.x && i->second.x + delta_x - player_pos.x < 0))
			{
				//Overshot
				i->second.x = player_pos.x;
			}
			else
			{
				i->second.x += delta_x;
			}
		}
		if (i->second.y != player_pos.y)
		{
			powers_combined = false;
			float delta_y = 2 * (player_pos.y - i->second.y) * time_step;
			if ((player_pos.y - i->second.y < close_enough && player_pos.y - i->second.y > -close_enough) ||
				(i->second.y < player_pos.y && i->second.y + delta_y - player_pos.y > 0) ||
				(i->second.y > player_pos.y && i->second.y + delta_y - player_pos.y < 0))
			{
				//Overshot
				i->second.y = player_pos.y;
			}
			else
			{
				i->second.y += delta_y;
			}
		}
	}
	
	if (!m_game_over && powers_combined)
	{
		m_game_over = true;
		m_death_countdown = 5.0f;
	}
}

void PowerResurrectionState::render()
{
	Video &vr = get_Video();
	Colors &cr = get_Colors();
	
	m_game_state.render();

	vr.set_2d(make_pair(Point2f(), Point2f(RES_HORIZ, RES_VERT)), true);

	//Render faded bg
	Vertex2f_Color p0(Point2f(0, 0), cr["transblack"]);
	Vertex2f_Color p1(Point2f(0, RES_VERT), cr["transblack"]);
	Vertex2f_Color p2(Point2f(RES_HORIZ, RES_VERT), cr["transblack"]);
	Vertex2f_Color p3(Point2f(RES_HORIZ, 0), cr["transblack"]);
	Quadrilateral<Vertex2f_Color> top(p0, p1, p2, p3);

	vr.render(top);

	//Render powers
	for (map<Power, Zeni::Point2f>::iterator i = m_power_locations.begin(); i != m_power_locations.end(); i++)
	{
		Point2f image_ul = Point2f(i->second.x * 20.0f, i->second.y * 20.0f) + m_grid_offset;
		Point2f image_lr = Point2f(image_ul.x + 20.0f, image_ul.y + 20.0f);

		if (i->second.x != m_player->get_position().x ||
			i->second.y != m_player->get_position().y)
		{
			render_image(power_asset(i->first), image_ul, image_lr);
		}
	}

	if (m_game_over)
	{
		Point2f image_ul = Point2f(m_player->get_position().x * 20.0f - 10.0f - 20.0f, m_player->get_position().y * 20.0f - 12.0f - 32.0f) + m_grid_offset;
		Point2f image_lr = Point2f(image_ul.x + 128.0f, image_ul.y + 128.0f);
		render_image("demon", image_ul, image_lr);
	}
}
