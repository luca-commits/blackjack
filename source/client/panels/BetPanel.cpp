#include "BetPanel.hpp"

BetPanel::BetPanel(wxWindow* parent) : wxPanel(parent, wxID_ANY) {}

BetPanel::BetPanel(wxWindow* parent, game_state* gameState, player* me) : wxPanel(parent, wxID_ANY) {
    // update myself
    std::vector<player*> players = gameState->get_players();
    std::vector<player*>::iterator it = std::find_if(players.begin(), players.end(), [me](const player* x) {
        return x->get_id() == me->get_id();
    });
    if (it < players.end()) {
        me = *it;
    } else {
        GameController::showError("Game state error", "Could not find this player among players of server game.");
        return;
    }

    // color of background
    wxColor lightGreen = wxColor(210, 210, 210);
    this->SetBackgroundColour(lightGreen);

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
        wxString::Format("%i", game_state::_min_bet),
        240
    );
    verticalLayout->Add(this->_betSizeField, 0, wxTOP | wxLEFT | wxRIGHT, 10);

    // button to confirm
    wxButton* makeBetButton = new wxButton(this, wxID_ANY, "Make bet", wxDefaultPosition, wxSize(100, 40));
    makeBetButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
        GameController::makeBet();
    });
    verticalLayout->Add(makeBetButton, 0, wxALIGN_RIGHT | wxALL, 10);

    this->SetSizerAndFit(verticalLayout);
}

// get value from input field
wxString BetPanel::getBetSize() {
    return this->_betSizeField->getValue();
}

// build text on screen
wxStaticText* BetPanel::buildStaticText(std::string content, wxPoint position, wxSize size, long textAlignment, bool bold) {
    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, content, position, size, textAlignment);
    if(bold) {
        wxFont font = staticText->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        staticText->SetFont(font);
    }
    return staticText;
}