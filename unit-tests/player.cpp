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

class PlayerTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        cards.resize(14);
        for (int i = 1; i < 14; ++i) {
            for (int j = 0; j < 4; ++j) {
                cards[i].push_back(new card(i, j));
            }
        }
    }

    /* Any object and subroutine declared here can be accessed in the tests */

    std::vector<std::vector<card*>> cards;
    player player;
    std::string player_name;
    int bet_size = 0;
    int money = 100;
    bool finished_turn = false;
    hand player_hand;
    std::string err;
};

// If the player has finished their turn, the corresponding flag attribute has to be set
TEST_F(PlayerTest, SetFinishedTurn) {
    player.set_finished_turn();
    EXPECT_TRUE(player.has_finished_turn());
}

// After initialization a player must not be broke
TEST_F(PlayerTest, IsBrokeFalse) {
    EXPECT_EQ(money, player.get_money());
    EXPECT_FALSE(player.is_broke());
}

// A player with no money should be broke
// TODO how to set the money of a player to 0 so that I can check this
TEST_F(PlayerTest, IsBrokeTrue) {
    EXPECT_TRUE(player_hand.add_card(cards[1][0], err));
    EXPECT_TRUE(player_hand.add_card(cards[3][0], err));
    EXPECT_TRUE(player_hand.add_card(cards[13][0], err));
    std::vector<card*> expected_hand = {cards[1][0], cards[3][0], cards[13][0]};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// TODO one of these for a broke player
// When starting a new round only the finished_round flag and the bet_size have to be reset
TEST_F(PlayerTest, SetupRound) {
    player_name = player.get_player_name();
    money = player.get_money();
    player_hand.setup_round(err);
    player.setup_round(err);
    EXPECT_EQ(bet_size, player.get_bet_size());
    EXPECT_EQ(money, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
    EXPECT_FALSE(player.has_finished_turn());
}

// After winning a round, the new amount of money has to be computed
// and nothing else changes
TEST_F(PlayerTest, WonRound) {
    bet_size = player.get_bet_size();
    money = player.get_money();
    player_name = player.get_player_name();
    int money_new = bet_size * 2 + money;
    ASSERT_TRUE(player.has_finished_turn());
    player.won_round();
    EXPECT_EQ(bet_size, player.get_bet_size());
    EXPECT_EQ(money_new, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
}

// After a draw round, the bet is returned to the player and nothing else changes
TEST_F(PlayerTest, DrawRound) {
    bet_size = player.get_bet_size();
    money = player.get_money();
    player_name = player.get_player_name();
    int money_new = bet_size + money;
    ASSERT_TRUE(player.has_finished_turn());
    player.draw_round();
    EXPECT_EQ(bet_size, player.get_bet_size());
    EXPECT_EQ(money_new, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
    EXPECT_FALSE(player.has_finished_turn());
}

// todo
// After a draw round, the bet is returned to the player and nothing else changes
TEST_F(PlayerTest, MakeBetHalf) {
    player_name = player.get_player_name();
    EXPECT_TRUE(player.make_bet(50, err));
    EXPECT_EQ(50, player.get_bet_size());
    EXPECT_EQ(50, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
    EXPECT_FALSE(player.has_finished_turn());
}

// After a draw round, the bet is returned to the player and nothing else changes
TEST_F(PlayerTest, MakeBetAllMoney) {
    player_name = player.get_player_name();
    EXPECT_TRUE(player.make_bet(100, err));
    EXPECT_EQ(100, player.get_bet_size());
    EXPECT_EQ(0, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
    EXPECT_FALSE(player.has_finished_turn());
}

// After a draw round, the bet is returned to the player and nothing else changes
TEST_F(PlayerTest, MakeBetMoneyOver) {
    player_name = player.get_player_name();
    EXPECT_FALSE(player.make_bet(200, err));
    EXPECT_EQ(0, player.get_bet_size());
    EXPECT_EQ(100, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
    EXPECT_FALSE(player.has_finished_turn());
}

// After a draw round, the bet is returned to the player and nothing else changes
TEST_F(PlayerTest, MakeBetNegative) {
    player_name = player.get_player_name();
    EXPECT_FALSE(player.make_bet(-2, err));
    EXPECT_EQ(0, player.get_bet_size());
    EXPECT_EQ(100, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
    EXPECT_FALSE(player.has_finished_turn());
}

/*
// When starting a new round only the finished_round flag and the bet_size have to be reset
TEST_F(PlayerTest, WrapupRound) {
    player_name = player.get_player_name();
    money = player.get_money();
    player_hand.setup_round(err);
    player.setup_round(err);
    EXPECT_EQ(bet_size, player.get_bet_size());
    EXPECT_EQ(money, player.get_money());
    EXPECT_EQ(player_hand, player.get_hand());
    EXPECT_EQ(player_name, player.get_player_name());
    EXPECT_FALSE(player.has_finished_turn());
}*/
