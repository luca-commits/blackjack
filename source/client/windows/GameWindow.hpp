#ifndef BLACKJACKUI_GAMEWINDOW_HPP
#define BLACKJACKUI_GAMEWINDOW_HPP

#include "../panels/ConnectionPanel.hpp"


class GameWindow : public wxFrame
{
public:
    GameWindow(const wxString& title, const wxPoint& pos, const wxSize& size);

    void showPanel(wxPanel* panel);
    void setStatus(const std::string& message);

private:
    wxBoxSizer* _mainLayout;
    wxStatusBar* _statusBar;

    wxPanel* _currentPanel;

};

#endif
