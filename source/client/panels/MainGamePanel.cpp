#include "MainGamePanel.h"
#include "../uiElements/ImagePanel.h"
#include "../GameController.h"


MainGamePanel::MainGamePanel(wxWindow* parent) : wxPanel(parent, wxID_ANY, wxDefaultPosition, wxSize(960, 680)) {
}

void MainGamePanel::buildGameState(game_state* gameState, player* me) {

    // remove any existing UI
    this->DestroyChildren();

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

        this->buildOtherPlayerHand(gameState, otherPlayer, playerAngle);
        this->buildOtherPlayerLabels(gameState, otherPlayer, playerAngle, side);
    }

    // show both card piles at the center
    this->buildCardPiles(gameState, me);

    // show turn indicator below card piles
    this->buildTurnIndicator(gameState, me);

    // show our own player
    this->buildThisPlayer(gameState, me);

    // show the number of rounds
    this->buildRoundCounter(gameState);

    // update layout
    this->Layout();
}




// with closed hands
void MainGamePanel::buildOthers(game_state* gameState, player* otherPlayer, double playerAngle, int side) {

    long textAlignment = wxALIGN_CENTER;
    
    // predefine offsets 
    int labelOffsetX = 0;
    if(side < 0) { // right side
        textAlignment = wxALIGN_LEFT;
        labelOffsetX = 85;

    } else if(side > 0) { // left side
        textAlignment = wxALIGN_RIGHT;
        labelOffsetX = -85;
    }

    // define the ellipse which represents the virtual player circle
    double verticalRadius_hand = MainGamePanel::otherPlayerHandDistanceFromCenter; 
    double verticalRadius_label = MainGamePanel::otherPlayerLabelDistanceFromCenter;
    double verticalRadius_bet = MainGamePanel::otherPlayerBetDistanceFromCenter;

    // get this player's position on that ellipse
    wxPoint handPosition = MainGamePanel::tableCenter;
    wxPoint labelPosition = MainGamePanel::tableCenter;
    wxPoint betPosition = MainGamePanel::tableCenter;

    // 1.6 to horizontally elongate players' circle?...
    handPosition +=     this->getPointOnEllipse(verticalRadius_hand*1.6,  verticalRadius_hand,  playerAngle);
    labelPosition +=    this->getPointOnEllipse(verticalRadius_label*1.6, verticalRadius_label, playerAngle);
    betPosition +=      this->getPointOnEllipse(verticalRadius_bet*1.6,   verticalRadius_bet,   playerAngle);
    labelPosition += wxSize(labelOffsetX, 0);

    // NAME
    this->buildStaticText(
            otherPlayer->get_player_name(),
            labelPosition + wxSize(-100, -36),
            wxSize(200, 18),
            textAlignment,
            true
    );

//    bool is_broke();
//    bool check_if_over_21();


    if(!gameState->is_started()){
        // STATUS
        this->buildStaticText(
                "Status: Waiting for Game to start":std::to_string(otherPlayer->get_bet_size()),
                labelPosition + wxSize(-100, 18),
                wxSize(200, 18),
                textAlignment
        );
    else if(otherPLayer->get_hand()->is_broke()){
        // STATUS
        this->buildStaticText(
                "Status: LOST GAME",
                labelPosition + wxSize(-100, 18),
                wxSize(200, 18),
                textAlignment
        );
    else{
        // STATUS
        if(otherPLayer->get_hand()->check_if_over_21()){
            this->buildStaticText(
                    "Status: LOST ROUND",
                    labelPosition + wxSize(-100, 18),
                    wxSize(200, 18),
                    textAlignment
            );
        }
        else{
            this->buildStaticText(
                    "Status: Playing/Waiting for turn",
                    labelPosition + wxSize(-100, 18),
                    wxSize(200, 18),
                    textAlignment
            );
        }
        // MONEY
        this->buildStaticText(
                "Money in depot:":std::to_string(otherPlayer->get_money()),
                labelPosition + wxSize(-100, ),
                wxSize(200, 18),
                textAlignment
        );
        // BET
        this->buildStaticText(
                "Money betted:":std::to_string(otherPlayer->get_bet_size()),
                labelPosition + wxSize(-100, 18),
                wxSize(200, 18),
                textAlignment
        );

        // add player's HAND IMAGE=================================
        int numberOfCards = otherPlayer->get_nof_cards();

        if(numberOfCards > 0) {

            // get new bounds of image, as they increase when image is rotated
            wxSize boundsOfRotatedHand = this->getBoundsOfRotatedSquare(MainGamePanel::otherPlayerHandSize, playerAngle);
            handPosition -= boundsOfRotatedHand / 2;

            std::string handImage = "assets/png-bcards/back" + std::to_string(numberOfCards) + ".png";
            if(numberOfCards > 8) {
                handImage = "assets/png-cards/black8.png";
            }
            new ImagePanel(this, handImage, wxBITMAP_TYPE_ANY, handPosition, boundsOfRotatedHand, playerAngle);

        }

        // add player's BET IMAGE=================================
        int bet=otherPlayer->get_bet_size();

        if(bet<=10){
            new ImagePanel(this, "assets/png-chips/blue.png", wxBITMAP_TYPE_ANY, betPosition, boundsOfRotatedHand, playerAngle);
        }
        else if(bet<=20){
            new ImagePanel(this, "assets/png-chips/red.png", wxBITMAP_TYPE_ANY, betPosition, boundsOfRotatedHand, playerAngle);
        }
        else{
            new ImagePanel(this, "assets/png-chips/black.png", wxBITMAP_TYPE_ANY, betPosition, boundsOfRotatedHand, playerAngle);
    
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

        // // show our player's minus points
        // wxStaticText *playerPoints = buildStaticText(
        //         std::to_string(me->get_score()) + " minus points",
        //         wxDefaultPosition,
        //         wxSize(200, 18),
        //         wxALIGN_CENTER
        // );
        // innerLayout->Add(playerPoints, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        // Show our pla

        // if our player already played, we display that as status
        if (me->has_folded()) {
            wxStaticText *playerStatus = buildStaticText(
                    "You already played",
                    wxDefaultPosition,
                    wxSize(200, 32),
                    wxALIGN_CENTER
            );
            innerLayout->Add(playerStatus, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        // TODO It's our turn, display Hit and Stand button
        } else if (gameState->get_current_player() == me) {
            wxButton *foldButton = new wxButton(this, wxID_ANY, "Fold", wxDefaultPosition, wxSize(80, 32));
            foldButton->Bind(wxEVT_BUTTON, [](wxCommandEvent& event) {
                GameController::fold();
            });
            innerLayout->Add(foldButton, 0, wxALIGN_CENTER | wxBOTTOM, 8);

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
        int numberOfCards = me->get_nof_cards();
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

            // Show all cards
            for (int i = 0; i < me->get_hand()->get_cards().size(); i++) {

                card *handCard = me->get_hand()->get_cards().at(i);
                std::string cardFile = "assets/lama_" + std::to_string(handCard->get_value()) + ".png";

                ImagePanel *cardButton = new ImagePanel(this, cardFile, wxBITMAP_TYPE_ANY, wxDefaultPosition, scaledCardSize);

                if (gameState->get_current_player() == me && !me->has_folded()) {
                    cardButton->SetToolTip("Play card");
                    cardButton->SetCursor(wxCursor(wxCURSOR_HAND));
                    cardButton->Bind(wxEVT_LEFT_UP, [handCard](wxMouseEvent& event) {
                        GameController::playCard(handCard);
                    });
                }
                handLayout->Add(cardButton, 0, wxLEFT | wxRIGHT, 4);
            }
        }
    }
}

//This function has to be changed surely TODO
void MainGamePanel::buildCardPiles(game_state* gameState, player *me) {

    if(gameState->is_started()) {

        // Show discard pile
        const card* topCard = gameState->get_discard_pile()->get_top_card();
        if(topCard != nullptr) {
            std::string cardImage = "assets/lama_" + std::to_string(topCard->get_value()) + ".png";

            wxPoint discardPilePosition = MainGamePanel::tableCenter + MainGamePanel::discardPileOffset;

            ImagePanel* discardPile = new ImagePanel(this, cardImage, wxBITMAP_TYPE_ANY, discardPilePosition, MainGamePanel::cardSize);
            discardPile->SetToolTip("Discard pile");
        }

        // Show draw pile
        wxPoint drawPilePosition = MainGamePanel::tableCenter + MainGamePanel::drawPileOffset;

        ImagePanel* drawPile = new ImagePanel(this, "assets/png-cards/backside.png", wxBITMAP_TYPE_ANY, drawPilePosition, MainGamePanel::cardSize);

        if(gameState->get_current_player() == me && !me->has_folded()) {
            drawPile->SetToolTip("Draw card");
            drawPile->SetCursor(wxCursor(wxCURSOR_HAND));
            drawPile->Bind(wxEVT_LEFT_UP, [](wxMouseEvent& event) {
                GameController::drawCard();
            });
        } else {
            drawPile->SetToolTip("Draw pile");
        }

    } else {
        // if the game did not start yet, show a back side of a card in the center (only for the mood)
        wxPoint cardPosition = MainGamePanel::tableCenter - (MainGamePanel::cardSize / 2);
        new ImagePanel(this, "assets/png-cards/backside.png", wxBITMAP_TYPE_ANY, cardPosition, MainGamePanel::cardSize);
    }

}

void MainGamePanel::buildRoundCounter(game_state* gameState){
  if(gameState->is_started() && gameState->get_current_player() != nullptr) {

      // gameState->get_current_player()->get_player_name() is a placeholder, we need a roundnr variable
      std::string roundindicator = "We are in round: " + gameState->get_current_player()->get_player_name();

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


void buildShoe(){
  shoeFile = "assets/misc/shoe";
  wxPoint shoePosition =  MainGamePanel::tableCenter + MainGamePanel::shoeOffset;
  ImagePanel *shoe = new ImagePanel(this, shoeFile, wxBITMAP_TYPE_ANY,
                                    shoePosition, scaledCardSize, MainGamePanel::cardSize);
}

void buildDealer(game_state* gameState){

  std::vector<Card> dealers_cards = gameState->compute_dealers_hand();
  std::string backside = "assets/png-cards/backside.png";
  std::string left_frontside = getPngFileName(dealers_cards[0]);
  std::string right_frontside = getPngFileName(dealers_cards[1]);
  wxPoints leftDealerCardPosition = MainGamePanel::tableCenter +
                                    MainGamePanel::dealerOffset +
                                    MainGamePanel::leftDealerCardOffset;
  wxPoints rightDealerCardPosition = MainGamePanel::tableCenter +
                                     MainGamePanel::dealerOffset +
                                     MainGamePanel::rightDealerCardOffset;
  if(gameState -> first_part){
      ImagePanel* leftDealerCard = new ImagePanel(this, backside, wxBITMAP_TYPE_ANY, leftDealerCardPosition, MainGamePanel::cardSize);
  }
  else{
    ImagePanel *leftDealerCard = "assets/lama_" + std::to_string(handCard->get_value()) + ".png"
        new ImagePanel(this, leftCard, wxBITMAP_TYPE_ANY,
                       leftDealerCardPosition, MainGamePanel::cardSize);
  }
  ImagePanel* rightDealerCard = new ImagePanel(this, rightCard,  wxBITMAP_TYPE_ANY, rightDealerCardPosition, MainGamePanel::cardSize)
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


string getPngFileName(int value, int suit){

    std::map<int, std::string> value_map {{11, "jack"}, {12, "queen"}, {1, "ace"}};
    std::map<int, std::string> suit_map{
        {0, "spades"}, {1, "clubs"}, {2, "diamonds"}, {3, "hearts"}};

    std::string path = "assets/png-card/";

    if (value <= 10) {
      return path + std::to_string(value) + "of" + suit_map[suit] + ".png";
    }
    else{
      return path + value_map[value] + "of" + suit_map[suit] + ".png";
    }
}
