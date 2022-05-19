#include "GameController.hpp"
#include "../general/network/requests/join_game_request.hpp"
#include "../general/network/requests/start_game_request.hpp"
#include "../general/network/requests/hit_request.hpp"
#include "../general/network/requests/make_bet_request.hpp"
#include "../general/network/requests/stand_request.hpp"
#include "network/ClientNetworkManager.hpp"

// initialize static members
GameWindow* GameController::_gameWindow = nullptr;
ConnectionPanel* GameController::_connectionPanel = nullptr;
MainGamePanel* GameController::_mainGamePanel = nullptr;
BetPanel* GameController::_betPanel = nullptr;

player* GameController::_me = nullptr;
game_state* GameController::_current_game_state = nullptr;


void GameController::init(GameWindow* gameWindow) {

    GameController::_gameWindow = gameWindow;

    // Set up main panels
    GameController::_connectionPanel = new ConnectionPanel(gameWindow);
    GameController::_mainGamePanel = new MainGamePanel(gameWindow);
    GameController::_betPanel = new BetPanel(gameWindow);

    // Hide all panels
    GameController::_connectionPanel->Show(false);
    GameController::_mainGamePanel->Show(false);
    GameController::_betPanel->Show(false);

    // Only show connection panel at the start of the game
    GameController::_gameWindow->showPanel(GameController::_connectionPanel);

    // Set status bar
    GameController::showStatus("Not connected");
}


void GameController::connectToServer() {

    // get values form UI input fields
    wxString inputServerAddress = GameController::_connectionPanel->getServerAddress().Trim();
    wxString inputServerPort = GameController::_connectionPanel->getServerPort().Trim();
    wxString inputPlayerName = GameController::_connectionPanel->getPlayerName().Trim();

    // check that all values were provided
    if(inputServerAddress.IsEmpty()) {
        GameController::showError("Input error", "Please provide the server's address");
        return;
    }
    if(inputServerPort.IsEmpty()) {
        GameController::showError("Input error", "Please provide the server's port number");
        return;
    }
    if(inputPlayerName.IsEmpty()) {
        GameController::showError("Input error", "Please enter your desired player name");
        return;
    }

    // convert host from wxString to std::string
    std::string host = inputServerAddress.ToStdString();

    // convert port from wxString to uint16_t
    unsigned long portAsLong;
    if(!inputServerPort.ToULong(&portAsLong) || portAsLong > 65535) {
        GameController::showError("Connection error", "Invalid port");
        return;
    }
    uint16_t port = (uint16_t) portAsLong;

    // convert player name from wxString to std::string
    std::string playerName = inputPlayerName.ToStdString();

    // connect to network
    ClientNetworkManager::init(host, port);

    // send request to join game
    GameController::_me = new player(playerName);
    join_game_request request = join_game_request(GameController::_me->get_id(), GameController::_me->get_player_name());
    ClientNetworkManager::sendRequest(request);

}

void GameController::makeBet() {
    wxString inputPlayerBet = GameController::_betPanel->getBetSize().Trim();

    // recovery?
    if(inputPlayerBet.IsEmpty()) {
        GameController::showError("Input error", "Please enter your bet");
        return;
    }

    std::string bet_string = inputPlayerBet.ToStdString();
    int bet_int = std::stoi(bet_string);

    // check to make sure this in a logical integer + ADD RECOVERY (SHOW BET PANEL AGAIN)
    if(bet_int < game_state::_min_bet && bet_int > 4096) {
        GameController::showError("Input error", "Invalid value given as bet");
        return;
    }

    make_bet_request request = make_bet_request(GameController::_current_game_state->get_id(), GameController::_me->get_id(), bet_int);
    ClientNetworkManager::sendRequest(request);
    // show MainPanel
    GameController::_gameWindow->showPanel(GameController::_mainGamePanel);
    GameController::_mainGamePanel->buildGameState(GameController::_current_game_state, GameController::_me);
}

// THIS HAS TO BE EDITTED FOR SURE
void GameController::updateGameState(game_state* newGameState) {

    // the existing game state is now old
    game_state* oldGameState = GameController::_current_game_state;

    // save the new game state as our current game state
    GameController::_current_game_state = newGameState;
    
    // update myself
    std::vector<player*> players = newGameState->get_players();
    std::vector<player*>::iterator it = std::find_if(players.begin(), players.end(), [](const player* x) {
        return x->get_id() == GameController::_me->get_id();
    });
    if (it < players.end()) {
        GameController::_me = *it;
    } else {
        GameController::showError("Game state error", "Could not find this player among players of server game.");
        return;
    }

    if(oldGameState != nullptr) {

        // check if a new round started, and display message accordingly
        if(oldGameState->get_round_number() > 0 && oldGameState->get_round_number() < newGameState->get_round_number()) {
            GameController::showNewRoundMessage(oldGameState, newGameState);
        }

        // delete the old game state, we don't need it anymore
        delete oldGameState;
    }

    // edit this to trigger when player is bankrupt
    if(GameController::_current_game_state->is_finished()) {
        GameController::showGameOverMessage();
    }

    if(_me->get_bet_size() == 0 && GameController::_current_game_state->is_started()) {
        GameController::_betPanel = new BetPanel(_gameWindow, GameController::_current_game_state, GameController::_me);
        GameController::_gameWindow->showPanel(GameController::_betPanel);
    } else {
        GameController::_gameWindow->showPanel(GameController::_mainGamePanel);
    // command the main game panel to rebuild itself, based on the new game state
        GameController::_mainGamePanel->buildGameState(GameController::_current_game_state, GameController::_me);
    }
}


void GameController::startGame() {
    start_game_request request = start_game_request(GameController::_current_game_state->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


void GameController::hit() {
    hit_request request = hit_request(GameController::_current_game_state->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


void GameController::stand() {
    stand_request request = stand_request(GameController::_current_game_state->get_id(), GameController::_me->get_id());
    ClientNetworkManager::sendRequest(request);
}


wxEvtHandler* GameController::getMainThreadEventHandler() {
    return GameController::_gameWindow->GetEventHandler();
}


void GameController::showError(const std::string& title, const std::string& message) {
    wxMessageBox(message, title, wxICON_ERROR);
}


void GameController::showStatus(const std::string& message) {
    GameController::_gameWindow->setStatus(message);
}


void GameController::showNewRoundMessage(game_state* oldGameState, game_state* newGameState) {
    std::string title = "Round Completed";
    std::string message = "The players have won following amounts of money:\n";
    std::string buttonLabel = "Start next round";

    // add the point differences of all players to the messages
    for(int i = 0; i < oldGameState->get_players().size(); i++) {

        player* oldPlayerState = oldGameState->get_players().at(i);
        player* newPlayerState = newGameState->get_players().at(i);

        int money_difference = newPlayerState->get_money() - oldPlayerState->get_money();
        std::string moneyText = std::to_string(money_difference);
        if(money_difference > 0) {
            moneyText = "+" + moneyText;
        }

        std::string playerName = newPlayerState->get_player_name();
        if(newPlayerState->get_id() == GameController::_me->get_id()) {
            playerName = "You";
        }
        message += "\n" + playerName + ":     " + moneyText;
    }

    wxMessageDialog dialogBox = wxMessageDialog(nullptr, message, title, wxICON_NONE);
    dialogBox.SetOKLabel(wxMessageDialog::ButtonLabel(buttonLabel));
    dialogBox.ShowModal();
}


void GameController::showGameOverMessage() {
    std::string title = "Game Over!";
    std::string message = "Final score:\n";
    std::string buttonLabel = "Close Game";

    // sort players by money  
    std::vector<player*> players = GameController::_current_game_state->get_players();
    std::sort(players.begin(), players.end(), [](const player* a, const player* b) -> bool {
        return a->get_money() < b->get_money();
    });

    // list all players
    for(int i = 0; i < players.size(); i++) {

        player* playerState = players.at(i);
        std::string moneyText = std::to_string(playerState->get_money());

        // first entry is the winner
        std::string winnerText = "";
        if(i == 0) {
            winnerText = "     Winner!";
        }

        std::string playerName = playerState->get_player_name();
        if(playerState->get_id() == GameController::_me->get_id()) {
            playerName = "You";

            if(i == 0) {
                winnerText = "     You won!!!";
            }
        }
        message += "\n" + playerName + ":     " + moneyText + winnerText;
    }

    wxMessageDialog dialogBox = wxMessageDialog(nullptr, message, title, wxICON_NONE);
    dialogBox.SetOKLabel(wxMessageDialog::ButtonLabel(buttonLabel));
    int buttonClicked = dialogBox.ShowModal();
    if(buttonClicked == wxID_OK) {
        GameController::_gameWindow->Close();
    }
}
