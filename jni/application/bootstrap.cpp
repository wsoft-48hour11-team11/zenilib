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
#include "LevelIntroState.h"

#if defined(_DEBUG) && defined(_WINDOWS)
#define DEBUG_NEW new(_NORMAL_BLOCK, __FILE__, __LINE__)
#define new DEBUG_NEW
#endif

using namespace std;
using namespace Zeni;

class Instructions_State : public Widget_Gamestate {
  Instructions_State(const Instructions_State &);
  Instructions_State operator=(const Instructions_State &);

public:
  Instructions_State()
    : Widget_Gamestate(make_pair(Point2f(0.0f, 0.0f), Point2f(800.0f, 600.0f)))
  {
  }

private:
  void on_push() {
    Widget_Gamestate::on_push();
    get_Game().controller_mouse.enabled = false;
  }

  void on_pop() {
    get_Game().controller_mouse.enabled = true;
    Widget_Gamestate::on_pop();
  }

  void on_key(const SDL_KeyboardEvent &event) {
    get_Game().pop_state();
    if(event.keysym.sym == SDLK_ESCAPE && event.state == SDL_PRESSED)
      return;
    get_Game().push_state(new LevelIntroState());
  }

  void on_controller_button(const SDL_ControllerButtonEvent &event) {
    if(event.state == SDL_PRESSED) {
      if(event.button == SDL_CONTROLLER_BUTTON_A || event.button == SDL_CONTROLLER_BUTTON_START) {   
        get_Game().pop_state();
        get_Game().push_state(new LevelIntroState());
      }
      else if(event.button == SDL_CONTROLLER_BUTTON_B || event.button == SDL_CONTROLLER_BUTTON_BACK) {   
        get_Game().pop_state();
      }
    }
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

  class Our_Title_State : public Title_State<LevelIntroState, Instructions_State> {
  public:
    class Play_Button_2 : public Text_Button {
      Play_Button_2(const Play_Button_2 &);
      Play_Button_2 & operator=(const Play_Button_2 &);

    public:
      Play_Button_2()
        : Text_Button(Point2f(200.0f, 250.0f), Point2f(390.0f, 310.0f),
                      "system_36_800x600", "Play")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new Instructions_State);
      }
    } play_button_2;

    class Edit_Button : public Text_Button {
      Edit_Button(const Edit_Button &);
      Edit_Button & operator=(const Edit_Button &);

    public:
      Edit_Button()
        : Text_Button(Point2f(410.0f, 250.0f), Point2f(600.0f, 310.0f),
                      "system_36_800x600", "Edit")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new LevelEditor());
      }
    } edit_button;

    class Level_0 : public Text_Button {
      Level_0(const Level_0 &);
      Level_0 & operator=(const Level_0 &);

    public:
      Level_0()
        : Text_Button(Point2f(200.0f, 330.0f), Point2f(260.0f, 390.0f),
                      "system_36_800x600", "0")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new LevelIntroState(0));
      }
    } level_0_button;

    class Level_1 : public Text_Button {
      Level_1(const Level_1 &);
      Level_1 & operator=(const Level_1 &);

    public:
      Level_1()
        : Text_Button(Point2f(268.0f, 330.0f), Point2f(328.0f, 390.0f),
                      "system_36_800x600", "1")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new LevelIntroState(1));
      }
    } level_1_button;

    class Level_2 : public Text_Button {
      Level_2(const Level_2 &);
      Level_2 & operator=(const Level_2 &);

    public:
      Level_2()
        : Text_Button(Point2f(336.0f, 330.0f), Point2f(396.0f, 390.0f),
                      "system_36_800x600", "2")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new LevelIntroState(2));
      }
    } level_2_button;

    class Level_3 : public Text_Button {
      Level_3(const Level_3 &);
      Level_3 & operator=(const Level_3 &);

    public:
      Level_3()
        : Text_Button(Point2f(404.0f, 330.0f), Point2f(464.0f, 390.0f),
                      "system_36_800x600", "3")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new LevelIntroState(3));
      }
    } level_3_button;

    class Level_4 : public Text_Button {
      Level_4(const Level_4 &);
      Level_4 & operator=(const Level_4 &);

    public:
      Level_4()
        : Text_Button(Point2f(472.0f, 330.0f), Point2f(532.0f, 390.0f),
                      "system_36_800x600", "4")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new LevelIntroState(4));
      }
    } level_4_button;

    class Level_5 : public Text_Button {
      Level_5(const Level_5 &);
      Level_5 & operator=(const Level_5 &);

    public:
      Level_5()
        : Text_Button(Point2f(540.0f, 330.0f), Point2f(600.0f, 390.0f),
                      "system_36_800x600", "5")
      {
      }

      void on_accept() {
        Zeni::play_sound("menuButton", 1.0f, 0.7f);
        get_Sound().update();
        get_Game().push_state(new LevelIntroState(5));
      }
    } level_5_button;

    void on_push() {
      Title_State<LevelIntroState, Instructions_State>::on_push();
      play_music();
    }

    void on_uncover() {
      Title_State<LevelIntroState, Instructions_State>::on_uncover();
      play_music();
    }

    Our_Title_State()
      : Title_State<LevelIntroState, Instructions_State>("Dismantled Demon\nDisconnection")
    {
      m_widgets.unlend_Widget(play_button);
      m_widgets.unlend_Widget(instructions_button);

      m_widgets.lend_Widget(play_button_2);
      m_widgets.lend_Widget(edit_button);
      m_widgets.lend_Widget(level_0_button);
      m_widgets.lend_Widget(level_1_button);
      m_widgets.lend_Widget(level_2_button);
      m_widgets.lend_Widget(level_3_button);
      m_widgets.lend_Widget(level_4_button);
      m_widgets.lend_Widget(level_5_button);
    }

  private:
    void play_music() {
      get_Sound().set_BGM("music/48hr_music2_2");
      get_Sound().set_BGM_looping(true);
      get_Sound().set_BGM_gain(0.5f);
      get_Sound().play_BGM();
    }
  };

  class Gamestate_One_Initializer : public Gamestate_Zero_Initializer {
    virtual Gamestate_Base * operator()() {
      Window::set_title("Dismantled Demon Disconnection");

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
