/* This file is part of the Zenipex Library (zenilib).
 * Copyleft (C) 2011 Mitchell Keith Bloch (bazald).
 *
 * This source file is simply under the public domain.
 */

#include <zenilib.h>

#include "common.h"

#include "player.h"
#include "GameSingleton.h"
#include "LevelEditor.h"


#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Play_State : public Gamestate_II {
  Play_State(const Play_State &);
  Play_State operator=(const Play_State &);

public:
  Play_State()
    : m_grid(Point2i(50, 32), Point2i(0, 0))
  {
    set_pausable(true);

    //m_grid.load("test_level.txt");
	GameSingleton* sing = GameSingleton::getInstance();
	m_grid.load(sing->level_list[0]);

    m_player = Player(Point2f(m_grid.get_spawn_player().x + 4.5f / 16.0f, float(m_grid.get_spawn_player().y)), Vector2f(7.0f / 16.0f, 1.0f));
  }

  ~Play_State() {
    //m_grid.save("test_level.txt");
  }

private:
  void on_push() {
    get_Window().set_mouse_state(Window::MOUSE_HIDDEN);
  }

  void on_pop() {
    get_Controllers().reset_vibration_all();
  }

  void on_cover() {
    get_Controllers().reset_vibration_all();
  }

  void on_controller_button(const SDL_ControllerButtonEvent &event) {
    if(event.button == SDL_CONTROLLER_BUTTON_BACK && event.state == SDL_PRESSED)
      get_Game().push_Popup_Menu_State();
  }

  void prerender() {
    get_Video().set_clear_Color(Color());
  }

  void render() {
    Video &vr = get_Video();

    vr.set_2d(make_pair(Point2f(0.0f, 0.0f), Point2f(float(m_grid.get_width()), float(m_grid.get_height()))), true);

    m_grid.render();
    m_player.render();
  }

  Grid m_grid;
  Player m_player;
};

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_key(const SDL_KeyboardEvent &event) {
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      get_Game().pop_state();
  }

  void render() {
    Widget_Gamestate::render();

    Zeni::Font &fr = get_Fonts()["title"];

    fr.render_text(
#if defined(_WINDOWS)
                   "ALT+F4"
#elif defined(_MACOSX)
                   "Apple+Q"
#else
                   "Ctrl+Q"
#endif
                           " to Quit",
                   Point2f(400.0f, 300.0f - 0.5f * fr.get_text_height()),
                   get_Colors()["title_text"],
                   ZENI_CENTER);
  }
};

class Bootstrap {
  class Popup_Menu_Instructions : public Popup_Menu_State {
  public:
    class Factory : public Popup_Menu_State_Factory {
    public:
      Popup_Menu_Instructions * operator()() {
        return new Popup_Menu_Instructions;
      }
    };

    class Instructions_Button : public Text_Button {
    public:
      Instructions_Button()
        : Zeni::Text_Button(Point2f(200.0f, 230.0f), Point2f(600.0f, 290.0f),
                            "system_36_800x600", "Instructions")
      {
      }

      void on_accept() {
        get_Game().push_state(new Instructions_State);
      }
    } instructions_button;

    Popup_Menu_Instructions() {
      m_widgets.lend_Widget(instructions_button);
      menu_button.set_upper_left(Point2f(200.0f, 310.0f));
      menu_button.set_lower_right(Point2f(400.0f, 370.0f));
      configure_video_button.set_upper_left(Point2f(410.0f, 310.0f));
      configure_video_button.text = "Configure";
    }
  };

  class Our_Title_State : public Title_State<Play_State, Instructions_State> {
  public:
    class Edit_Button : public Text_Button {
      Edit_Button(const Play_Button &);
      Edit_Button & operator=(const Play_Button &);

    public:
      Edit_Button()
        : Text_Button(Point2f(410.0f, 250.0f), Point2f(600.0f, 310.0f),
                      "system_36_800x600", "Edit")
      {
      }

      void on_accept() {
        get_Game().push_state(new LevelEditor());
      }
    } edit_button;

    Our_Title_State()
      : Title_State<Play_State, Instructions_State>("Zenipex Library\nApplication")
    {
      play_button.set_lower_right(Point2f(390.0f, 310.0f));
      m_widgets.lend_Widget(edit_button);
    }
  };

  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("zenilib Application");

      get_Controllers();
      get_Video();
      get_Textures();
      get_Fonts();
      get_Sounds();

      get_Game().replace_Popup_Menu_State_Factory(new Popup_Menu_Instructions::Factory);

      return new Our_Title_State;
    }
  } m_goi;

public:
  Bootstrap() {
    g_gzi = &m_goi;
  }
} g_bootstrap;

int main(int argc, char **argv) {
  return zenilib_main(argc, argv);
}
