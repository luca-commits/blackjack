• 


class MainGamePanel(){




    public:
    
    // are variables correct?
    void buildOthers(   game_state* gameState, player* otherPlayer, double playerAngle);

}




buildGameController: void, removes existing GUI elements and builds latest game state GUI
• buildOthers: void, builds the GUI elements of other players’ hands, money and bets
• buildRoundCounter: void, builds the GUI of the current round number
• buildMyself: void, builds the GUI of the hand, money and bet of the player
• buildShoe: void, builds the GUI of the shoe
• buildDealer: void, builds the GUI of the dealer’s hand