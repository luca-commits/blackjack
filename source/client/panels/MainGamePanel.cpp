#include "MainGamePanel.hpp"
#include "../uiElements/ImagePanel.hpp"
#include "../GameController.hpp"

#include <map>


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

        this->buildOthers(gameState, otherPlayer, playerAngle, side);
      //  this->buildOtherPlayerLabels(gameState, otherPlayer, playerAngle, side);
    }

    // show our own player
    this->buildThisPlayer(gameState, me);

    // show the number of rounds
    this->buildRoundCounter(gameState);

    // update layout
    this->Layout();
}




// with open hands
void MainGamePanel::buildOthers(game_state* gameState, player* otherPlayer, double playerAngle, int side) {

      
    //   IDONTKNOWMAN
    std::string err;//deugging helper...
    long textAlignment = wxALIGN_CENTER;
    int labelOffsetX = 0;
    if (side < 0) { // right side
        textAlignment = wxALIGN_LEFT;
        labelOffsetX = 85;
    } else if(side > 0) { // left side
        textAlignment = wxALIGN_RIGHT;
        labelOffsetX = -65;
    }


    // define the ellipse which represents the virtual player circle
    double verticalRadius_hand = MainGamePanel::otherPlayerHandDistanceFromCenter; 
    double verticalRadius_label = MainGamePanel::otherPlayerLabelDistanceFromCenter;
    double verticalRadius_bet = MainGamePanel::otherPlayerBetDistanceFromCenter;

    // get this player's position on that ellipse
    wxPoint handPosition = MainGamePanel::tableCenter;
    wxPoint labelPosition = MainGamePanel::tableCenter;
    wxPoint betPosition = MainGamePanel::tableCenter;

    handPosition  +=    this->getPointOnEllipse(verticalRadius_hand*1.2,  verticalRadius_hand,  playerAngle);
    labelPosition +=    this->getPointOnEllipse(verticalRadius_label*1.2, verticalRadius_label, playerAngle);
    betPosition   +=    this->getPointOnEllipse(verticalRadius_bet*1.2,   verticalRadius_bet,   playerAngle);
    labelPosition += wxSize(labelOffsetX, 0);
  

    // NAME
    this->buildStaticText(
            otherPlayer->get_player_name(),
            labelPosition + wxSize(-100, -36),
            wxSize(200, 18),
            textAlignment,
            true
    );


    bool player_is_broke = otherPlayer->get_money() < gameState->_min_bet;
    if(!gameState->is_started()) {
        // STATUS
        this->buildStaticText(
                "waiting for game to start...",
                labelPosition + wxSize(-100, -18),
                wxSize(200, 18),
                textAlignment
        );
    }
    else if(player_is_broke){
        // STATUS
        this->buildStaticText(
                "Status: LOST GAME",
                labelPosition + wxSize(-100, -18),
                wxSize(200, 18),
                textAlignment
        );
    }
    else{
        // STATUS
        if(otherPlayer->get_hand()->is_over_21(err)){
            this->buildStaticText(
                    "Status: LOST ROUND",
                    labelPosition + wxSize(-100, -18),
                    wxSize(200, 18),
                    textAlignment
            );
        }
        else if(gameState->get_current_player() == otherPlayer)
        {
            this->buildStaticText(
                    "Status: Playing",
                    labelPosition + wxSize(-100, -18),
                    wxSize(200, 18),
                    textAlignment
            );
        }
        else
        {
            this->buildStaticText(
                    "Status: Waiting for turn",
                    labelPosition + wxSize(-100, -18),
                    wxSize(200, 18),
                    textAlignment
            );
        }
        // MONEY
        this->buildStaticText(
                "Money in depot:" + std::to_string(otherPlayer->get_money()),
                labelPosition + wxSize(-100, 0),
                wxSize(200, 18),
                textAlignment
        );
        // BET
        this->buildStaticText(
                "Money betted:" + std::to_string(otherPlayer->get_bet_size()),
                labelPosition + wxSize(-100, 18),
                wxSize(200, 18),
                textAlignment
        );


        // ========= add player's HAND IMAGE=================================

        int numberOfCards = otherPlayer->get_hand()->get_nof_cards();
        std::string cardImage;
        wxSize boundsOfRotatedHand =  this->getBoundsOfRotatedSquare(MainGamePanel::otherPlayerHandSize, playerAngle);
        wxSize weirdSize(80, 80);
        double cAngle = playerAngle + MainGamePanel::twoPi/4;
        int cDist = MainGamePanel::otherPlayerHandSize;
        wxSize card_dist((int)(sin(cAngle)*cDist), (int)(cos(cAngle) * cDist));
        handPosition -= card_dist;


        for(int i = 0; i<numberOfCards;++i){
            card *handCard = otherPlayer->get_hand()->get_cards().at(i);
            cardImage = getPngFileName(handCard->get_value(), handCard->get_suit());
            new ImagePanel(this, cardImage, wxBITMAP_TYPE_ANY, handPosition, weirdSize, playerAngle);
            handPosition += card_dist*0.4;
        }

        // add player's BET IMAGE================================= refine??
        int bet=otherPlayer->get_bet_size();

        if(bet<=10){
            new ImagePanel(this, "assets/png-chips/blue.png", wxBITMAP_TYPE_ANY, betPosition, weirdSize, playerAngle);
        }
        else if(bet<=20){
            new ImagePanel(this, "assets/png-chips/red.png", wxBITMAP_TYPE_ANY, betPosition, weirdSize, playerAngle);
        }
        else{
            new ImagePanel(this, "assets/png-chips/black.png", wxBITMAP_TYPE_ANY, betPosition, weirdSize, playerAngle);
    
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

        // This is taken care of in the betpanel
        // wxStaticText *playerPoints = buildStaticText(
        //         "You currently have " + std::to_string(me->get_money()) + "$",
        //         wxDefaultPosition,
        //         wxSize(200, 18),
        //         wxALIGN_CENTER
        // );

        // innerLayout->Add(playerPoints, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        // if our player already played, we display that as status
        // TODO from has_folded make has_played
        if (me->has_finished_turn()) {
            wxStaticText *playerStatus = buildStaticText(
                    "You finished your turn",
                    wxDefaultPosition,
                    wxSize(200, 32),
                    wxALIGN_CENTER
            );
            innerLayout->Add(playerStatus, 0, wxALIGN_CENTER | wxBOTTOM, 8);
            }
           
        // TODO It's our turn, display Hit and Stand button
        if (gameState->get_current_player() == me) {
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
                ImagePanel *cardButton = new ImagePanel(this, cardFile, wxBITMAP_TYPE_ANY, wxDefaultPosition, scaledCardSize);
            }
        }
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


//void buildShoe(){
//  shoeFile = "assets/misc/shoe";
//  wxPoint shoePosition =  MainGamePanel::tableCenter + MainGamePanel::shoeOffset;
//  ImagePanel *shoe = new ImagePanel(this, shoeFile, wxBITMAP_TYPE_ANY,
//                                    shoePosition, scaledCardSize, MainGamePanel::cardSize);
//}

void MainGamePanel::buildDealer(game_state* gameState){
    // Setup two nested box sizers, in order to align our player's UI to the bottom center
    wxBoxSizer* outerLayout = new wxBoxSizer(wxHORIZONTAL);
    this->SetSizer(outerLayout);
    wxBoxSizer* innerLayout = new wxBoxSizer(wxVERTICAL);
    outerLayout->Add(innerLayout, 1, wxALIGN_CENTER);

    wxPoint offset(80, 0);

    // TODO the call in the line below was compute_dealers_hand()
    // which should return the hand the dealer will have in the end
    // but get_dealers_hand returns only the current hand of the dealer
    hand* hand_dealers_hand = gameState->get_dealers_hand();
    std::vector<card*> dealers_cards = hand_dealers_hand->get_cards();

    std::vector<wxPoint> offsets(dealers_cards.size());
    for(unsigned i = 0; i < dealers_cards.size(); ++i){
        offsets[i] = i * offset;
    }
    std::string backside = "assets/png-cards/backside.png";
    std::string left_frontside = getPngFileName(dealers_cards[0]->get_value(), dealers_cards[0]->get_suit());
    std::string right_frontside = getPngFileName(dealers_cards[1]->get_value(), dealers_cards[1]->get_suit());

    std::vector<std::string> dealer_cards_file_names(dealers_cards.size());
    std::transform(dealers_cards.begin(), dealers_cards.end(),
                    dealer_cards_file_names.begin(), [this](card* c) -> std::string {
                    return this->getPngFileName(c->get_value(), c->get_suit());
                    });

    bool first_part = gameState-> everyone_finished();

    wxBoxSizer *handLayout = new wxBoxSizer(wxHORIZONTAL);
    innerLayout->Add(handLayout, 0, wxALIGN_CENTER);


    if(first_part){
        wxPoint leftCardPosition = MainGamePanel::tableCenter + MainGamePanel::leftDealerCardOffset;
        wxPoint rightCardPosition = MainGamePanel::tableCenter + MainGamePanel::rightDealerCardOffset;
        ImagePanel* rightDealerCard = new ImagePanel(this, backside, wxBITMAP_TYPE_ANY, rightCardPosition, MainGamePanel::cardSize);
        ImagePanel* leftDealerCard = new ImagePanel(this, getPngFileName(dealers_cards[0]->get_value(), dealers_cards[0]->get_suit()), wxBITMAP_TYPE_ANY, leftCardPosition, MainGamePanel::cardSize);
    }
    else{
        for(unsigned i = 0; i < dealers_cards.size(); ++i){
            ImagePanel *image_panel = new ImagePanel(this, dealer_cards_file_names[i], wxBITMAP_TYPE_ANY, offsets[i], MainGamePanel::cardSize);
            handLayout->Add(image_panel, 0, wxLEFT | wxRIGHT, 4);
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
