#ifndef BLACKJACKUI_BLACKJACK_HPP
#define BLACKJACKUI_BLACKJACK_HPP

#include <wx/wx.h>
#include "../windows/GameWindow.hpp"
#include "../GameController.hpp"


// Main app class
class Blackjack : public wxApp
{
public:
    virtual bool OnInit();
};


#endif //BLACKJACKUI_BLACKJACK_HPP
