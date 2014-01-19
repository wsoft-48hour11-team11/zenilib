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

  class Our_Title_State : public Title_State<LevelIntroState, Instructions_State> {
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
      play_button.set_lower_right(Point2f(390.0f, 310.0f));
      m_widgets.lend_Widget(edit_button);
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
