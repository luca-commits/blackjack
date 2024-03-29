#ifndef BLACKJACKUI_GAMECONTROLLER_HPP
#define BLACKJACKUI_GAMECONTROLLER_HPP

#include <string>
#include "windows/GameWindow.hpp"
#include "panels/ConnectionPanel.hpp"
#include "panels/MainGamePanel.hpp"
#include "panels/BetPanel.hpp"
#include "network/ResponseListenerThread.hpp"
#include "../general/game_state/game_state.hpp"

class BetPanel; // forward declaration   

class GameController {

public:
    static void init(GameWindow* gameWindow);

    static void connectToServer();
    static void makeBet();
    static void continue_to_bet_panel();
    static void updateGameState(game_state* newGameState);
    static void startGame();
    static void hit();
    static void stand();

    static wxEvtHandler* getMainThreadEventHandler();
    static void showError(const std::string& title, const std::string& message);
    static void showStatus(const std::string& message);
    static void showGameOverMessage();

private:
  static bool _is_number(const wxString s);

  static GameWindow *_gameWindow;
  static ConnectionPanel *_connectionPanel;
  static MainGamePanel *_mainGamePanel;
  static BetPanel *_betPanel;

  static player *_me;
  static game_state *_current_game_state;
  static game_state *_previous_game_state;

};


#endif
