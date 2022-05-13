#ifndef BETPANEL_HPP
#define BETPANEL_HPP

#include <wx/wx.h>
#include "../GameController.hpp"
#include "../../general/game_state/game_state.hpp"

class BetPanel {
public:
    BetPanel(wxWindow* parent, game_state* gameState, player* me);
    wxString getBetSize();

private:
    InputField* _betSizeField;
};

#endif