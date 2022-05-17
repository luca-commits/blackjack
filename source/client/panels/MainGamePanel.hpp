#ifndef LAMA_CLIENT_MAINGAMEPANEL_H
#define LAMA_CLIENT_MAINGAMEPANEL_H

#include <wx/wx.h>
#include "../../general/game_state/game_state.hpp"


class MainGamePanel : public wxPanel {

public:
    MainGamePanel(wxWindow* parent);

    void buildGameState(game_state* gameState, player* me);


private:

    void buildOthers(game_state* gameState, player* otherPlayer, double playerAngle, int side);
    //void buildGameController();  //change this from sds
    void buildRoundCounter(game_state* gameState);
    void buildThisPlayer(game_state* gameState, player* me); //in sds this is build myself
    void buildShoe(game_state* gameState);
    void buildDealer(game_state*);
    void buildCardPiles(game_state* gameState, player *me);

    wxStaticText* buildStaticText(std::string content, wxPoint position, wxSize size, long textAlignment, bool bold = false);

    wxSize getBoundsOfRotatedSquare(double edgeLength, double rotationAngle);
    double getEdgeLengthOfRotatedSquare(double originalEdgeLength, double rotationAngle);

    wxPoint getPointOnEllipse(double horizontalRadius, double verticalRadius, double angle);

    // define key constant layout values
    wxSize const panelSize = wxSize(960, 680); // also set in the constructor implementation
    wxPoint const tableCenter = wxPoint(480, 300);
    wxSize const cardSize = wxSize(80, 124);


    double const otherPlayerHandSize = 160.0; // smaller?

    double const otherPlayerBetDistanceFromCenter = 120.0;
    double const otherPlayerHandDistanceFromCenter = 180.0;
    double const otherPlayerLabelDistanceFromCenter = 260.0;

    wxPoint const shoeOffset = wxPoint(-300, -42);
    wxPoint const dealerOffset = wxPoint(-600, -42);
    wxPoint const drawPileOffset = wxPoint(4, -42);
    wxPoint const turnIndicatorOffset = wxPoint(-100, 98);
    // PERADD 
    wxPoint const roundIndicatorOffset = wxPoint(-300,98);

    wxPoint const leftDealerCardOffset = wxPoint(-80, 0);
    wxPoint const rightDealerCardOffset = wxPoint(80, 0);


    double const twoPi = 6.28318530718;

    std::string getPngFileName(int value, int suit);
};


#endif //LAMA_CLIENT_MAINGAMEPANEL_H
