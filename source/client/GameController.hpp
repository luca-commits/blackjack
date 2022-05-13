#ifndef BLACKJACKUI_GAMECONTROLLER_HPP
#define BLACKJACKUI_GAMECONTROLLER_HPP

// these .h extentions will need to be changed to .hpp probably
#include "windows/GameWindow.h"
#include "panels/ConnectionPanel.h"
#include "panels/MainGamePanel.h"
#include "panels/BetPanel.hpp"
#include "network/ResponseListenerThread.hpp"
#include "../general/game_state/game_state.hpp"


class GameController {

public:
    static void init(GameWindow* gameWindow);

    static void connectToServer();
    static void updateGameState(game_state* newGameState);
    static void startGame();
    static void hit();
    static void stand();

    static wxEvtHandler* getMainThreadEventHandler();
    static void showError(const std::string& title, const std::string& message);
    static void showStatus(const std::string& message);
    static void showNewRoundMessage(game_state* oldGameState, game_state* newGameState);
    static void showGameOverMessage();

private:
    static GameWindow* _gameWindow;
    static ConnectionPanel* _connectionPanel;
    static MainGamePanel* _mainGamePanel;
    static BetPanel* _betPanel;

    static player* _me;
    static game_state* _current_game_state;

};


#endif
