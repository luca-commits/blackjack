//
// Created by Flavia Taras on 24.05.22.
//

#include "gtest/gtest.h"
#include "../source/general/game_state/player.hpp"
#include "../source/general/exceptions/BlackjackException.hpp"
#include "../source/general/serialization/json_utils.h"


/* A test fixture allows to reuse the same configuration of objects for all
 * tests in a test suite. The name of the fixture must match the test suite.
 *
 * For each test defined with TEST_F(), googletest will create a fresh test
 * fixture at runtime, immediately initialize it via SetUp(), run the test,
 * clean up by calling TearDown(), and then delete the test fixture.
 * Note that different tests in the same test suite have different test fixture
 * objects, and googletest always deletes a test fixture before it creates the
 * next one. googletest does not reuse the same test fixture for multiple
 * tests. Any changes one test makes to the fixture do not affect other tests.
 */

// friend functions of player class
void set_money(player& player_test, const int& money) {
    player_test._money->set_value(money);
}

void set_bet_size(player& player_test, const int& bet_size) {
    int current_money = player_test.get_money();
    player_test._bet_size->set_value(bet_size);
    player_test._money->set_value(current_money - bet_size);
}

class PlayerTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        cards.resize(14);
        for (int i = 1; i < 14; ++i) {
            for (int j = 0; j < 4; ++j) {
                cards[i].push_back(new card(i, j));
            }
        }

        player_ = new player("Test");
    }

    /* Any object and subroutine declared here can be accessed in the tests */

    std::vector<std::vector<card*>> cards;
    player* player_ = nullptr;
    player* player_broke = nullptr;
    std::string player_name;
    int bet_size = 0;
    int money = 100;
    bool finished_turn = false;
    hand player_hand;
    std::string err;
};

// If the player has finished their turn, the corresponding flag attribute has to be set
TEST_F(PlayerTest, SetFinishedTurn) {
    player_->set_finished_turn();
    EXPECT_TRUE(player_->has_finished_turn());
}

// After initialization a player must not be broke
TEST_F(PlayerTest, IsBrokeFalse) {
    EXPECT_EQ(money, player_->get_money());
    EXPECT_FALSE(player_->is_broke());
}

// A player with no money should be broke
TEST_F(PlayerTest, IsBrokeTrue) {
    set_money(*player_, 0);
    EXPECT_TRUE(player_->is_broke());
}

// A player with negative money should also be broke
TEST_F(PlayerTest, IsBrokeTrueNegativeMoney) {
    set_money(*player_, -1);
    EXPECT_TRUE(player_->is_broke());
}

// When starting a new round only the finished_round flag and the bet_size have to be reset
TEST_F(PlayerTest, SetUpRound) {
    player_name = player_->get_player_name();
    money = player_->get_money();
    player_hand.setup_round(err);
    player_->setup_round(err);
    EXPECT_EQ(bet_size, player_->get_bet_size());
    EXPECT_EQ(money, player_->get_money());
    EXPECT_EQ(player_hand.get_cards(), player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
    EXPECT_FALSE(player_->has_finished_turn());
}

//todo
// Setting up a new round for a broke player should not be possible
TEST_F(PlayerTest, SetUpRoundBroke) {
    set_money(*player_, 0);
    EXPECT_THROW(player_->setup_round(err), BlackjackException);
}

// After winning a round, the new amount of money has to be computed
// and nothing else changes
TEST_F(PlayerTest, WonRound) {
    bet_size = player_->get_bet_size();
    money = player_->get_money();
    player_name = player_->get_player_name();
    int money_new = bet_size * 2 + money;
    player_->won_round();
    EXPECT_EQ(bet_size, player_->get_bet_size());
    EXPECT_EQ(money_new, player_->get_money());
    EXPECT_EQ(player_hand.get_cards(), player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
}

// After a draw round, the bet is returned to the player and nothing else changes
TEST_F(PlayerTest, DrawRound) {
    bet_size = player_->get_bet_size();
    money = player_->get_money();
    player_name = player_->get_player_name();
    int money_new = bet_size + money;
    player_->draw_round();
    EXPECT_EQ(bet_size, player_->get_bet_size());
    EXPECT_EQ(money_new, player_->get_money());
    EXPECT_EQ(player_hand.get_cards(), player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
    EXPECT_FALSE(player_->has_finished_turn());
}

// When making a bet with half the money, only the bet_size and the money of the player are adjusted
TEST_F(PlayerTest, MakeBetHalf) {
    player_name = player_->get_player_name();
    EXPECT_TRUE(player_->make_bet(50, err));
    EXPECT_EQ(50, player_->get_bet_size());
    EXPECT_EQ(50, player_->get_money());
    EXPECT_EQ(player_hand.get_cards(), player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
    EXPECT_FALSE(player_->has_finished_turn());
}

// Making an all in bet should be possible
TEST_F(PlayerTest, MakeBetAllIn) {
    player_name = player_->get_player_name();
    EXPECT_TRUE(player_->make_bet(100, err));
    EXPECT_EQ(100, player_->get_bet_size());
    EXPECT_EQ(0, player_->get_money());
    EXPECT_EQ(player_hand.get_cards(), player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
    EXPECT_FALSE(player_->has_finished_turn());
}

// todo look into if it's throwing an exception
// A bet with more money than the player has is illegal
TEST_F(PlayerTest, MakeBetMoneyOver) {
    player_name = player_->get_player_name();
    EXPECT_FALSE(player_->make_bet(200, err));
    EXPECT_EQ(0, player_->get_bet_size());
    EXPECT_EQ(100, player_->get_money());
    EXPECT_EQ(player_hand.get_cards(), player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
    EXPECT_FALSE(player_->has_finished_turn());
}

// todo look into if it's throwing an exception
// A negative bet is illegal
TEST_F(PlayerTest, MakeBetNegative) {
    player_name = player_->get_player_name();
    EXPECT_FALSE(player_->make_bet(-2, err));
    EXPECT_EQ(0, player_->get_bet_size());
    EXPECT_EQ(100, player_->get_money());
    EXPECT_EQ(player_hand.get_cards(), player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
    EXPECT_FALSE(player_->has_finished_turn());
}

// Wrapping up a round for a player who won should call won_round()
// Since we cannot check for the function call directly, we will look for
// the side effects it creates (increasing the player's money by the bet_size)
TEST_F(PlayerTest, WrapUpRoundWon) {
    player_->get_hand()->add_card(cards[2][0], err);
    player_->get_hand()->add_card(cards[9][0], err);
    player_->get_hand()->add_card(cards[8][0], err);
    player_name = player_->get_player_name();
    std::vector<card*> expected_hand = {cards[2][0], cards[9][0], cards[8][0]};
    set_bet_size(*player_, 50);
    player_->wrap_up_round(18, err);
    EXPECT_EQ(50, player_->get_bet_size());
    EXPECT_EQ(150, player_->get_money());
    EXPECT_EQ(expected_hand, player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
}

// Wrapping up a round for a player who won should call won_round()
// Since we cannot check for the function call directly, we will look for
// the side effects it creates (increasing the player's money by the bet_size)
TEST_F(PlayerTest, WrapUpRoundWonDealerLost) {
    player_->get_hand()->add_card(cards[2][0], err);
    player_->get_hand()->add_card(cards[9][0], err);
    player_->get_hand()->add_card(cards[8][0], err);
    player_name = player_->get_player_name();
    std::vector<card*> expected_hand = {cards[2][0], cards[9][0], cards[8][0]};
    set_bet_size(*player_, 50);
    player_->wrap_up_round(23, err);
    EXPECT_EQ(50, player_->get_bet_size());
    EXPECT_EQ(150, player_->get_money());
    EXPECT_EQ(expected_hand, player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
}

// Wrapping up a round for a player who make a draw should call draw_round()
// Since we cannot check for the function call directly, we will look for
// the side effects it creates (returning the bet_size to the player's money)
TEST_F(PlayerTest, WrapUpRoundDraw) {
    player_->get_hand()->add_card(cards[2][0], err);
    player_->get_hand()->add_card(cards[9][0], err);
    player_->get_hand()->add_card(cards[8][0], err);
    player_name = player_->get_player_name();
    std::vector<card*> expected_hand = {cards[2][0], cards[9][0], cards[8][0]};
    set_bet_size(*player_, 50);
    player_->wrap_up_round(19, err);
    EXPECT_EQ(50, player_->get_bet_size());
    EXPECT_EQ(100, player_->get_money());
    EXPECT_EQ(expected_hand, player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
}

// Wrapping up a round for a player who won should call won_round()
// Since we cannot check for the function call directly, we will look for
// the side effects it creates (increasing the player's money by the bet_size)
TEST_F(PlayerTest, WrapUpRoundBlackjackWon) {
    player_->get_hand()->add_card(cards[4][0], err);
    player_->get_hand()->add_card(cards[9][0], err);
    player_->get_hand()->add_card(cards[8][0], err);
    player_name = player_->get_player_name();
    std::vector<card*> expected_hand = {cards[4][0], cards[9][0], cards[8][0]};
    set_bet_size(*player_, 50);
    player_->wrap_up_round(20, err);
    EXPECT_EQ(50, player_->get_bet_size());
    EXPECT_EQ(150, player_->get_money());
    EXPECT_EQ(expected_hand, player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
}

// Wrapping up a round for a player who make a draw should call draw_round()
// Since we cannot check for the function call directly, we will look for
// the side effects it creates (returning the bet_size to the player's money)
TEST_F(PlayerTest, WrapUpRoundBlackjackDraw) {
    player_->get_hand()->add_card(cards[4][0], err);
    player_->get_hand()->add_card(cards[9][0], err);
    player_->get_hand()->add_card(cards[8][0], err);
    player_name = player_->get_player_name();
    std::vector<card*> expected_hand = {cards[4][0], cards[9][0], cards[8][0]};
    set_bet_size(*player_, 50);
    player_->wrap_up_round(21, err);
    EXPECT_EQ(50, player_->get_bet_size());
    EXPECT_EQ(100, player_->get_money());
    EXPECT_EQ(expected_hand, player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
}

// Wrapping up a round for a player who make a draw should call draw_round()
// Since we cannot check for the function call directly, we will look for
// the side effects it creates (returning the bet_size to the player's money)
TEST_F(PlayerTest, WrapUpRoundBlackjackLost) {
    player_->get_hand()->add_card(cards[6][0], err);
    player_->get_hand()->add_card(cards[9][0], err);
    player_->get_hand()->add_card(cards[8][0], err);
    player_name = player_->get_player_name();
    std::vector<card*> expected_hand = {cards[6][0], cards[9][0], cards[8][0]};
    set_bet_size(*player_, 50);
    player_->wrap_up_round(21, err);
    EXPECT_EQ(50, player_->get_bet_size());
    EXPECT_EQ(50, player_->get_money());
    EXPECT_EQ(expected_hand, player_->get_hand()->get_cards());
    EXPECT_EQ(player_name, player_->get_player_name());
}
