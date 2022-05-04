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

    // update layout
    this->Layout();
}


void MainGamePanel::buildOthers(game_state* gameState, player* otherPlayer, double playerAngle) {

    // define the ellipse which represents the virtual player circle
    double horizontalRadius = MainGamePanel::otherPlayerHandDistanceFromCenter * 1.4; // 1.4 to horizontally elongate players' circle
    double verticalRadius = MainGamePanel::otherPlayerHandDistanceFromCenter;

    // get this player's position on that ellipse
    wxPoint handPosition = MainGamePanel::tableCenter;
    handPosition += this->getPointOnEllipse(horizontalRadius, verticalRadius, playerAngle);

    // add image of player's hand
    int numberOfCards = otherPlayer->get_nof_cards();
    if(numberOfCards > 0) {

        // get new bounds of image, as they increase when image is rotated
        wxSize boundsOfRotatedHand = this->getBoundsOfRotatedSquare(MainGamePanel::otherPlayerHandSize, playerAngle);
        handPosition -= boundsOfRotatedHand / 2;

        std::string handImage = "assets/lama_hand_" + std::to_string(numberOfCards) + ".png";
        if(numberOfCards > 10) {
            handImage = "assets/lama_hand_10.png";
        }
        new ImagePanel(this, handImage, wxBITMAP_TYPE_ANY, handPosition, boundsOfRotatedHand, playerAngle);

    } else if(numberOfCards == 0) {

        wxSize nonRotatedSize = wxSize((int) MainGamePanel::otherPlayerHandSize, (int) MainGamePanel::otherPlayerHandSize);
        handPosition -= nonRotatedSize / 2;

        new ImagePanel(this, "assets/lama_hand_0.png", wxBITMAP_TYPE_ANY, handPosition, nonRotatedSize);
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

        // show our player's minus points
        wxStaticText *playerPoints = buildStaticText(
                std::to_string(me->get_score()) + " minus points",
                wxDefaultPosition,
                wxSize(200, 18),
                wxALIGN_CENTER
        );
        innerLayout->Add(playerPoints, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        // if our player folded, we display that as status
        if (me->has_folded()) {
            wxStaticText *playerStatus = buildStaticText(
                    "Folded!",
                    wxDefaultPosition,
                    wxSize(200, 32),
                    wxALIGN_CENTER
            );
            innerLayout->Add(playerStatus, 0, wxALIGN_CENTER | wxBOTTOM, 8);

        // if we haven't folded yet, and it's our turn, display Fold button
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


void buildShoe(){
  shoeFile = "assets/misc/shoe";
  wxPoint shoePosition =  MainGamePanel::tableCenter + MainGamePanel::shoeOffset;
  ImagePanel *shoe = new ImagePanel(this, shoeFile, wxBITMAP_TYPE_ANY,
                                    shoePosition, scaledCardSize, MainGamePanel::cardSize);
}

void buildDealer(){
    
    
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
