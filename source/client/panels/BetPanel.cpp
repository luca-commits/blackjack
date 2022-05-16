#include "BetPanel.hpp"

BetPanel::BetPanel(wxWindow* parent, game_state* gameState, player* me) {
    // update myself ? (maybe not needed)
    std::vector<player*> players = gameState->get_players();
    std::vector<Player*>::iterator it = std::find_if(players.begin(), players.end(), [me](const Player* x) {
        return x->get_id() == me->get_id();
    });
    if (it < players.end()) {
        me = *it;
    } else {
        GameController::showError("Game state error", "Could not find this player among players of server game.");
        return;
    }

    // color of background
    wxColor green = wxColor(50, 168, 54);
    this->SetBackgroundColour(green);

    wxBoxSizer* verticalLayout = new wxBoxSizer(wxVERTICAL);

    // show players money
    int money = me->get_money();
    std::string money_string = std::to_string(money);
    wxStaticText* buildPlayerMoney = buildStaticText(
        "Your money: $" + money_string,
        wxDefaultPosition,
        wxSize(200, 18),
        wxALIGN_CENTER
    );
    verticalLayout->Add(buildPlayerMoney, 0, wxTOP | wxLEFT | wxRIGHT, 10);

    // bet input
    this->_betSizeField = new InputField(
        this,
        "Your bet:",
        100,
        wxString::Format("%i", game_state::_min_bet),//TODO does this not take our input as minimal bet?
        240
    );
    verticalLayout->Add(this->_betSizeField, 0, wxTOP | wxLEFT | wxRIGHT, 10);

    // button to confirm
    wxButton* makeBetButton = new wxButton(this, wxID_ANY, "Make bet", wxDefaultPosition, wxSize(100, 40));
    connectButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
        GameController::makeBet(); // THIS FUNCTION HAS TO BE IMPLEMENTED !!!
    });
    verticalLayout->Add(makeBetButton, 0, wxALIGN_RIGHT | wxALL, 10);

    this->SetSizerAndFit(verticalLayout);
}

wxString BetPanel::getBetSize() {
    return this->_betSizeField->getValue();
}