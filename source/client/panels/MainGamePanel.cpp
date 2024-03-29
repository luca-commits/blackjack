#include "MainGamePanel.hpp"
#include "../uiElements/ImagePanel.hpp"
#include "../GameController.hpp"

#include <map>

#include <chrono>
#include <thread>


MainGamePanel::MainGamePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(960, 680)) {
}

void MainGamePanel::buildGameState(game_state* gameState, player* me) {

    // remove any existing UI
    this->DestroyChildren();

    wxColor green = wxColor(50, 168, 54);
    this->SetBackgroundColour(green);

    std::vector<player*> players = gameState->get_players();
    int numberOfPlayers = players.size();

    // find our own player object in the list of players
    int myPosition = -1;
    std::vector<player*>::iterator it = std::find_if(players.begin(), players.end(), [me](const player* x) {
        return x->get_id() == me->get_id();
    });
    if (it < players.end()) {
        me = *it;
        myPosition = it - players.begin();
    } else {
        GameController::showError("Game state error", "Could not find this player among players of server game.");
        return;
    }

    double anglePerPlayer = MainGamePanel::twoPi / (double) numberOfPlayers;

    // show all other players
    for(int i = 1; i < numberOfPlayers; i++) {
        // get player at i-th position after myself
        player* otherPlayer = players.at((myPosition + i) % numberOfPlayers);

        double playerAngle = (double) i * anglePerPlayer;
        int side = (2 * i) - numberOfPlayers; // side < 0 => right, side == 0 => center, side > 0 => left

        this->buildOthers(gameState, otherPlayer, playerAngle, side);
    }

    bool am_broke = me->get_money() + me->get_bet_size() < gameState->_min_bet;
    if (!am_broke) {
      // show our own player
      this->buildThisPlayer(gameState, me);
    } else {
      this->buildThisPlayerBroke();
    }

    // show the number of rounds
    this->buildRoundCounter(gameState);

    // show cards of dealer
    this->buildDealer(gameState);

    // update layout
    this->Layout();
}




// Places labels and images of all opposed players onto the GUI.
void MainGamePanel::buildOthers(game_state* gameState, player* otherPlayer, double playerAngle, int side) {

    std::string err;
    long textAlignment = wxALIGN_CENTER;
    int labelOffsetX = 0;
    int betOffsetX = 0;

    if (side < 0) { // right side
        textAlignment = wxALIGN_LEFT;
        labelOffsetX = 70;
        betOffsetX = 20;
    } else if(side > 0) { // left side
        textAlignment = wxALIGN_RIGHT;
        labelOffsetX = -10;
        betOffsetX = 40;
    }


    // define the ellipse which represents the virtual player circle
    double verticalRadius_hand = MainGamePanel::otherPlayerHandDistanceFromCenter;
    double verticalRadius_label = MainGamePanel::otherPlayerLabelDistanceFromCenter;
    double verticalRadius_bet = MainGamePanel::otherPlayerBetDistanceFromCenter;

    // get player's position on that ellipse
    wxPoint handPosition = MainGamePanel::tableCenter;
    wxPoint labelPosition = MainGamePanel::tableCenter;
    wxPoint betPosition = MainGamePanel::tableCenter;

    handPosition  +=    this->getPointOnEllipse(verticalRadius_hand*1.3,  verticalRadius_hand,  playerAngle);
    labelPosition +=    this->getPointOnEllipse(verticalRadius_label*1.4, verticalRadius_label, playerAngle);
    betPosition   +=    this->getPointOnEllipse(verticalRadius_bet*1.9,   verticalRadius_bet,   playerAngle);
    labelPosition += wxSize(labelOffsetX, 0);


    // LABEL(name)
    this->buildStaticText(
            otherPlayer->get_player_name(),
            labelPosition + wxSize(-100, -36),
            wxSize(200, 18),
            textAlignment,
            true
    );

    // LABEL(status)
    bool player_is_broke = otherPlayer->get_money()+otherPlayer->get_bet_size() < gameState->_min_bet;
    if(!gameState->is_started()) {
        
        this->buildStaticText(
                "waiting for game to start...",
                labelPosition + wxSize(-100, -18),
                wxSize(200, 18),
                textAlignment
        );
    }
    else if(player_is_broke){
        this->buildStaticText(
                "Status: LOST GAME",
                labelPosition + wxSize(-100, -18),
                wxSize(200, 18),
                textAlignment
        );
    }
    else{
        std::string status_message = "Status: ";

        if(otherPlayer->get_hand()->is_over_21(err)) 
            status_message += "LOST ROUND" ;
        else if(gameState->everyone_finished()){
            std::string err;
            int pp = otherPlayer->get_hand()->get_points(err);
            int dp = gameState->get_dealers_hand()->get_points(err);
            if(dp>21 || pp > dp )   status_message += "WON ROUND";
            else if(pp < dp)             status_message += "LOST ROUND";
            else                status_message += "DRAW WITH DEALER";

        }
        else if(gameState->get_current_player() == otherPlayer && !gameState->everyone_finished()) 
            status_message += "PLAYING";
        else
            status_message += "WAITING";
        
        this->buildStaticText(
                    status_message,
                    labelPosition + wxSize(-100, -18),
                    wxSize(200, 18),
                    textAlignment
            );


        // LABEL(money)
        this->buildStaticText(
                "Money:" + std::to_string(otherPlayer->get_money()) + "$",
                labelPosition + wxSize(-100, 0),
                wxSize(200, 18),
                textAlignment
        );
        // LABEL(bet)
        this->buildStaticText(
                "Bet:" + std::to_string(otherPlayer->get_bet_size()) + "$",
                labelPosition + wxSize(-100, 18),
                wxSize(200, 18),
                textAlignment
        );


        //HAND(cards) IMAGE
        int numberOfCards = otherPlayer->get_hand()->get_nof_cards();
        std::string cardImage;
        wxSize weirdSize(76, 90);
        double cAngle = playerAngle + MainGamePanel::twoPi/4;
        int cDist = MainGamePanel::otherPlayerHandSize;
        wxSize card_dist((int)(sin(cAngle)*cDist), (int)(cos(cAngle) * cDist));
        handPosition -= 0.5*card_dist + 1.5*wxSize(betOffsetX, 0);

        for(int i = 0; i<numberOfCards;++i){
            card *handCard = otherPlayer->get_hand()->get_cards().at(i);
            cardImage = getPngFileName(handCard->get_value(), handCard->get_suit());
            new ImagePanel(this, cardImage, wxBITMAP_TYPE_ANY, handPosition, weirdSize, playerAngle);
            handPosition += card_dist*0.65;
        }

        //BET(chips) IMAGE
        int bet=otherPlayer->get_bet_size();
        wxSize chipSize(25, 25);
        wxSize bet_dist = 0.15*card_dist;
        betPosition -= 4* bet_dist + wxSize(betOffsetX, 0);

        while(bet>=100){
            new ImagePanel(this, "assets/png-chips/gold.png", wxBITMAP_TYPE_ANY, betPosition, chipSize, playerAngle);
            betPosition += bet_dist;
            bet -= 100;
        }
        while(bet>=25){
            new ImagePanel(this, "assets/png-chips/green.png", wxBITMAP_TYPE_ANY, betPosition, chipSize, playerAngle);
            betPosition += bet_dist;
            bet -= 25;
        }
        while(bet>=10){
            new ImagePanel(this, "assets/png-chips/black.png", wxBITMAP_TYPE_ANY, betPosition, chipSize, playerAngle);
            betPosition += bet_dist;
            bet -= 10;
        }
        while(bet>=5){
            new ImagePanel(this, "assets/png-chips/red.png", wxBITMAP_TYPE_ANY, betPosition, chipSize, playerAngle);
            betPosition += bet_dist;
            bet -= 5;
        }
        while(bet>=1){
            new ImagePanel(this, "assets/png-chips/blue.png", wxBITMAP_TYPE_ANY, betPosition, chipSize, playerAngle);
            betPosition += bet_dist;
            bet -= 1;
        }
    }
}



void MainGamePanel::buildThisPlayer(game_state* gameState, player* me) {

    // Setup two nested box sizers, in order to align our player's UI to the bottom center
    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(outerLayout);
    wxBoxSizer* innerLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(innerLayout, 1, wxALIGN_BOTTOM);

    // Show the label with our player name
    wxStaticText* playerName = buildStaticText(
            me->get_player_name(),
            wxDefaultPosition,
            wxSize(200, 18),
            wxALIGN_CENTER,
            true
    );
    innerLayout->Add(playerName, 0, wxALIGN_CENTER);

    // if the game has not yet started we say so
    if(!gameState->is_started()) {

        wxStaticText* playerPoints = buildStaticText(
                "waiting for game to start...",
                wxDefaultPosition,
                wxSize(200, 18),
                wxALIGN_CENTER
        );
        innerLayout->Add(playerPoints, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        // show button that allows our player to start the game
        wxButton* startGameButton = new wxButton(this, wxID_ANY, "Start Game!", wxDefaultPosition, wxSize(160, 64));
        startGameButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
            GameController::startGame();
        });
        innerLayout->Add(startGameButton, 0, wxALIGN_CENTER | wxBOTTOM, 8);

    } else {

        wxStaticText *playerMoney = buildStaticText(
                 "You currently have " + std::to_string(me->get_money()) + "$",
                 wxDefaultPosition,
                 wxSize(200, 18),
                 wxALIGN_CENTER
                );

        wxStaticText *playerBet;
        if(gameState->everyone_finished()) {
            playerBet = buildStaticText(
                 "Your bet was " + std::to_string(me->get_bet_size()) + "$",
                 wxDefaultPosition,
                 wxSize(200, 18),
                 wxALIGN_CENTER
                );
        } else {
            playerBet = buildStaticText(
                 "Your current bet is " + std::to_string(me->get_bet_size()) + "$",
                 wxDefaultPosition,
                 wxSize(200, 18),
                 wxALIGN_CENTER
                );
        }

        innerLayout->Add(playerMoney, 0, wxALIGN_CENTER | wxBOTTOM, 8);
        innerLayout->Add(playerBet, 0, wxALIGN_CENTER | wxBOTTOM, 8);


        //Display chip according to the betsize of the player
        int bet=me->get_bet_size();
        wxSize chipSize(25, 25);

        wxPoint chipPosition = MainGamePanel::tableCenter + MainGamePanel::chipOffset;
        const wxPoint chipdispl = wxPoint(20,0);

        while(bet>=100){
            new ImagePanel(this, "assets/png-chips/gold.png", wxBITMAP_TYPE_ANY, chipPosition, chipSize, wxALIGN_CENTER);
            chipPosition += chipdispl;
            bet -= 100;
        }
        while(bet>=25){
            new ImagePanel(this, "assets/png-chips/green.png", wxBITMAP_TYPE_ANY, chipPosition, chipSize, wxALIGN_CENTER);
            chipPosition += chipdispl;
            bet -= 25;
        }
        while(bet>=10){
            new ImagePanel(this, "assets/png-chips/black.png", wxBITMAP_TYPE_ANY, chipPosition, chipSize, wxALIGN_CENTER);
            chipPosition += chipdispl;
            bet -= 10;
        }
        while(bet>=5){
            new ImagePanel(this, "assets/png-chips/red.png", wxBITMAP_TYPE_ANY, chipPosition, chipSize, wxALIGN_CENTER);
            chipPosition += chipdispl;
            bet -= 5;
        }
        while(bet>=1){
            new ImagePanel(this, "assets/png-chips/blue.png", wxBITMAP_TYPE_ANY, chipPosition, chipSize, wxALIGN_CENTER);
            chipPosition += chipdispl;
            bet -= 1;
        }

        // if our player already played, we display that as status
        // TODO from has_folded make has_played

        if(gameState->everyone_finished()){
            std::string err;
            std::string status_message;
            int pp = me->get_hand()->get_points(err);
            int dp = gameState->get_dealers_hand()->get_points(err);

            if(me->get_hand()->is_over_21(err))
                                            status_message += "YOU LOST THIS ROUND";
            else if(dp>21 || pp > dp)       status_message += "YOU WON THIS ROUND";
            else if(pp < dp)                status_message += "YOU LOST THIS ROUND";
            else                            status_message += "YOU TIED WITH THE DEALER";

        
            wxStaticText *playerStatus = buildStaticText(
                    status_message,
                    wxDefaultPosition,
                    wxSize(200, 32),
                    wxALIGN_CENTER
            );
            innerLayout->Add(playerStatus, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        }
        else if (me->has_finished_turn()) {
            wxStaticText *playerStatus = buildStaticText(
                    "You finished your turn",
                    wxDefaultPosition,
                    wxSize(200, 32),
                    wxALIGN_CENTER
            );
            innerLayout->Add(playerStatus, 0, wxALIGN_CENTER | wxBOTTOM, 8);
        }

        // if it's our turn, display Hit and Stand button
        if (gameState->get_current_player() == me && !gameState->everyone_finished()) {
            wxButton *hitButton = new wxButton(this, wxID_ANY, "Hit", wxDefaultPosition, wxSize(80, 32));
            hitButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
                GameController::hit();
            });
            innerLayout->Add(hitButton, 0, wxALIGN_CENTER | wxBOTTOM, 8);

            wxButton *standButton = new wxButton(this, wxID_ANY, "Stand", wxDefaultPosition, wxSize(80, 32));
            standButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
                GameController::stand();
            });
            innerLayout->Add(standButton, 0, wxALIGN_CENTER | wxBOTTOM, 8);
        // if it's not our turn, display "waiting..."
        } else {
            wxStaticText *playerStatus = buildStaticText(
                    "waiting...",
                    wxDefaultPosition,
                    wxSize(200, 32),
                    wxALIGN_CENTER
            );
            innerLayout->Add(playerStatus, 0, wxALIGN_CENTER | wxBOTTOM, 8);
        }

        // display our player's hand, if we have cards
        int numberOfCards = me->get_hand()->get_nof_cards();
        if (numberOfCards > 0) {

            // create horizontal layout for the individual hand cards of our player
            wxBoxSizer *handLayout = new wxBoxSizer(wxHORIZONTAL);
            innerLayout->Add(handLayout, 0, wxALIGN_CENTER);

            wxSize scaledCardSize = MainGamePanel::cardSize;

            // Adjust card size (if the number of cards does not fit on the screen)
            if (numberOfCards * (MainGamePanel::cardSize.GetWidth() + 8) >
                MainGamePanel::panelSize.GetWidth()) { // 8 -> 4 pixel padding on both sides
                int scaledCardWidth = (MainGamePanel::panelSize.GetWidth() / numberOfCards) - 8;
                double cardAspectRatio =
                        (double) MainGamePanel::cardSize.GetHeight() / (double) MainGamePanel::cardSize.GetWidth();
                int scaledCardHeight = (int) ((double) scaledCardWidth * cardAspectRatio);
                scaledCardSize = wxSize(scaledCardWidth, scaledCardHeight);
            }

            // Show all cards of this player
            for (int i = 0; i < me->get_hand()->get_cards().size(); i++) {

                card *handCard = me->get_hand()->get_cards().at(i);
                std::string cardFile = getPngFileName(handCard->get_value(), handCard->get_suit());
                ImagePanel *cardPanel = new ImagePanel(this, cardFile, wxBITMAP_TYPE_ANY, wxDefaultPosition, scaledCardSize);
                handLayout->Add(cardPanel, 0, wxLEFT | wxRIGHT, 4);
            }
        }
    }
}

void MainGamePanel::buildThisPlayerBroke(){
    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(outerLayout);
    //Display text to tell player he has lost
    wxPoint Offset(-100, 200);
    wxPoint textPosition = MainGamePanel::tableCenter + Offset;
    wxStaticText *playerName =
        buildStaticText("You are broke :(", textPosition, wxSize(400, 36),
                        wxALIGN_CENTER, true);
}


void MainGamePanel::buildRoundCounter(game_state* gameState){
  if(gameState->is_started() && gameState->get_current_player() != nullptr) {

      std::string roundindicator = "We are in round: " + std::to_string(gameState->get_round_number());

      //wxPoint turnIndicatorPosition = MainGamePanel::tableCenter + MainGamePanel::turnIndicatorOffset;
      wxPoint roundIndicatorPosition = MainGamePanel::tableCenter + MainGamePanel::roundIndicatorOffset;

      this->buildStaticText(
              roundindicator,
              roundIndicatorPosition,
              wxSize(200, 18),
              wxALIGN_CENTER,
              true
      );
  }
}


void MainGamePanel::buildDealer(game_state* gameState){
    if(gameState->is_started()) {
   
        wxBoxSizer* handLayout = new wxBoxSizer(wxVERTICAL);

        wxPoint offset(80, 0);

        hand* dealers_hand = gameState->get_dealers_hand();
        std::vector<card*> dealers_cards = dealers_hand->get_cards();

        std::vector<wxPoint> offsets(dealers_cards.size());
        for(unsigned i = 0; i < dealers_cards.size(); ++i){
            offsets[i] = i * offset;
        }
        std::string backside = "assets/png-cards/backside.png";
        std::string left_frontside = getPngFileName(dealers_cards.at(0)->get_value(), dealers_cards.at(0)->get_suit());

        std::vector<std::string> dealer_cards_file_names(dealers_cards.size());
        std::transform(dealers_cards.begin(), dealers_cards.end(),
                        dealer_cards_file_names.begin(), [this](card* c) -> std::string {
                        return this->getPngFileName(c->get_value(), c->get_suit());
                        });

        // display DEALER LABEL
        std::string dealerindicator = "Dealer";
        wxPoint dealerIndicatorPosition = MainGamePanel::tableCenter + MainGamePanel::leftDealerCardOffset + MainGamePanel::dealerIndicatorOffset;
        this->buildStaticText(
                dealerindicator,
                dealerIndicatorPosition,
                wxSize(200, 18),
                wxALIGN_CENTER,
                true
        );

        // display DEALER CARDS
        // A round has two parts: in the first part the dealer has two cards, one of which is flipped, and in the second part 
        // he gets more cards according to the hard coded rules
        bool first_part = !gameState->everyone_finished();
        wxPoint leftCardPosition = MainGamePanel::tableCenter + MainGamePanel::leftDealerCardOffset;
        // during round
        if(first_part){
            wxPoint rightCardPosition = MainGamePanel::tableCenter + MainGamePanel::rightDealerCardOffset;
            ImagePanel* rightDealerCard = new ImagePanel(this, backside, wxBITMAP_TYPE_ANY, rightCardPosition, MainGamePanel::cardSize);
            ImagePanel* leftDealerCard = new ImagePanel(this, getPngFileName(dealers_cards[0]->get_value(), dealers_cards[0]->get_suit()), wxBITMAP_TYPE_ANY, leftCardPosition, MainGamePanel::cardSize);
        }
        // end of round
        else {
            for(unsigned i = 0; i < dealers_cards.size(); ++i){
                ImagePanel *image_panel = new ImagePanel(this, dealer_cards_file_names[i], wxBITMAP_TYPE_ANY, leftCardPosition + offsets[i], MainGamePanel::cardSize);
                handLayout->Add(image_panel, 0, wxLEFT | wxRIGHT, 4);
            }
            // halt game till displayed continue button is pressed
            wxButton* continueButton = new wxButton(this, wxID_ANY, "Continue", wxDefaultPosition, wxSize(100, 40));
            continueButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
                GameController::continue_to_bet_panel();
            });
            handLayout->Add(continueButton, 0, wxALIGN_RIGHT | wxALL, 10);
        }
    }
}

wxStaticText* MainGamePanel::buildStaticText(std::string content, wxPoint position, wxSize size, long textAlignment, bool bold) {
    wxStaticText* staticText = new wxStaticText(this, wxID_ANY, content, position, size, textAlignment);
    if(bold) {
        wxFont font = staticText->GetFont();
        font.SetWeight(wxFONTWEIGHT_BOLD);
        staticText->SetFont(font);
    }
    return staticText;
}


wxSize MainGamePanel::getBoundsOfRotatedSquare(double edgeLength, double rotationAngle) {
    double newEdgeLength = this->getEdgeLengthOfRotatedSquare(edgeLength, rotationAngle);
    return wxSize(newEdgeLength, newEdgeLength);
}


double MainGamePanel::getEdgeLengthOfRotatedSquare(double originalEdgeLength, double rotationAngle) {
    return originalEdgeLength * (abs(sin(rotationAngle)) + abs(cos(rotationAngle)));
}


wxPoint MainGamePanel::getPointOnEllipse(double horizontalRadius, double verticalRadius, double angle) {
    return wxPoint((int) (sin(angle) * horizontalRadius), (int) (cos(angle) * verticalRadius));
}

// Returns file path to the correct corresponding card
std::string MainGamePanel::getPngFileName(int value, int suit){

    std::map<int, std::string> value_map {{11, "jack"}, {12, "queen"}, {1, "ace"}, {13, "king"}};
    std::map<int, std::string> suit_map{
        {0, "spades"}, {1, "clubs"}, {2, "diamonds"}, {3, "hearts"}};

    std::string path = "assets/png-cards/";

    if (value > 1 && value <= 10) {
      return path + std::to_string(value) + "_of_" + suit_map[suit] + ".png";
    }
    else{
      return path + value_map[value] + "_of_" + suit_map[suit] + ".png";
    }
}
