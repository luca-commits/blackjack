//
// Created by Flavia Taras on 20.05.22.
//

//TODO: check the get_points function in many different ways with different amounts of aces and stuff
//TODO try to get extra cards when you already have over 21 points
//TODO: adding the same card more than the number of decks in a shoe should not work

#include "gtest/gtest.h"
#include "../source/general/game_state/hand.hpp"


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
class HandTest : public ::testing::Test {

protected:
    virtual void SetUp() {
        cards.resize(8); //TODO: do we need this... rather just declare a shoe...
        for (int i = 1; i < 14; ++i) {
            for (int j = 0; j < 5; ++j) {
                cards[i].push_back(new card(i, j));
            }
        }
    }

    /* Any object and subroutine declared here can be accessed in the tests */

    // cards[i][j] holds a pointer to the j-th copy of a card of value i
    std::vector<std::vector<card*>> cards;
    hand player_hand;
    std::string err;
};

// Adding one card to an empty hand must succeed
TEST_F(HandTest, AddOneCard) {
    EXPECT_TRUE(player_hand.add_card(cards[1][0], err));
    std::vector<card*> expected_hand = {cards[1][0]};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// The initial state must be an empty hand
TEST_F(HandTest, AddNoCards) {
    std::vector<card*> expected_hand = {};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// Adding several cards with different values to an empty hand must succeed
TEST_F(HandTest, AddManyCards) {
    EXPECT_TRUE(player_hand.add_card(cards[1][0], err));
    EXPECT_TRUE(player_hand.add_card(cards[3][0], err));
    EXPECT_TRUE(player_hand.add_card(cards[13][0], err));
    std::vector<card*> expected_hand = {cards[1][0], cards[3][0], cards[13][0]};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// Adding several cards with duplicate values to an empty hand must succeed
TEST_F(HandTest, AddManyCardsWithDuplicates) {
    EXPECT_TRUE(player_hand.add_card(cards[1][0], err));
    EXPECT_TRUE(player_hand.add_card(cards[1][1], err));
    EXPECT_TRUE(player_hand.add_card(cards[1][2], err));
    EXPECT_TRUE(player_hand.add_card(cards[3][0], err));
    EXPECT_TRUE(player_hand.add_card(cards[3][1], err));
    EXPECT_TRUE(player_hand.add_card(cards[13][0], err));
    std::vector<card*> expected_hand = {cards[1][0], cards[1][1], cards[1][2],
                                        cards[3][0], cards[3][1], cards[13][0]};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// The score of a hand with a single ace must be 11
TEST_F(HandTest, ScoreOneAce) {
    player_hand.add_card(cards[1][0], err);
    EXPECT_EQ(11, player_hand.get_points(err));
}

// The score of a hand with a single card must be equal to the card's value
TEST_F(HandTest, ScoreOneCard) {
    player_hand.add_card(cards[8][0], err);
    EXPECT_EQ(8, player_hand.get_points(err));
}

//The score of a hand with a single face card must be 10
TEST_F(HandTest, ScoreOneFaceCard) {
    player_hand.add_card(cards[12][0], err);
    EXPECT_EQ(10, player_hand.get_points(err));
}

//TODO: do we even test for this?
// The score of an empty hand must be zero
TEST_F(HandTest, ScoreNoCards) {
    EXPECT_EQ(0, player_hand.get_points(err));
}

//The score of a hand with 2 aces must be 12
TEST_F(HandTest, ScoreTwoAces) {
    player_hand.add_card(cards[1][0], err);
    EXPECT_EQ(11, player_hand.get_points(err));
    player_hand.add_card(cards[1][1], err);
    EXPECT_EQ(12, player_hand.get_points(err));
}

// Each addition of a card must increase the score by that card's value
TEST_F(HandTest, ScoreManyCards) {
    player_hand.add_card(cards[2][0], err);
    ASSERT_EQ(2, player_hand.get_points(err));
    player_hand.add_card(cards[3][0], err);
    ASSERT_EQ(5, player_hand.get_points(err));
    player_hand.add_card(cards[9][0], err);
    EXPECT_EQ(14, player_hand.get_points(err));
}

// Each addition of a card must increase the score by that card's value
//also if one of the cards is a face card
TEST_F(HandTest, ScoreManyCardsWithFaceCard) {
    player_hand.add_card(cards[1][0], err);
    ASSERT_EQ(11, player_hand.get_points(err));
    player_hand.add_card(cards[3][0], err);
    ASSERT_EQ(14, player_hand.get_points(err));
    player_hand.add_card(cards[13][0], err);
    EXPECT_EQ(14, player_hand.get_points(err));
}

// Each addition of a card must increase the score by that card's value
//also in the special case of having an ace (drawn first)
TEST_F(HandTest, ScoreManyCardsWithAceFirst) {
    player_hand.add_card(cards[1][0], err);
    ASSERT_EQ(11, player_hand.get_points(err));
    player_hand.add_card(cards[3][0], err);
    ASSERT_EQ(14, player_hand.get_points(err));
    player_hand.add_card(cards[7][0], err);
    EXPECT_EQ(21, player_hand.get_points(err));
}

// Each addition of a card must increase the score by that card's value
//also in the special case of having an ace (drawn last)
TEST_F(HandTest, ScoreManyCardsWithAceLast) {
    player_hand.add_card(cards[7][0], err);
    ASSERT_EQ(7, player_hand.get_points(err));
    player_hand.add_card(cards[3][0], err);
    ASSERT_EQ(10, player_hand.get_points(err));
    player_hand.add_card(cards[1][0], err);
    EXPECT_EQ(21, player_hand.get_points(err));
}

// Each addition of a card must increase the score by that card's value even
// if several cards have the same value
TEST_F(HandTest, ScoreManyCardsWithDuplicates) {
    player_hand.add_card(cards[2][0], err);
    ASSERT_EQ(2, player_hand.get_points(err));
    player_hand.add_card(cards[2][1], err);
    ASSERT_EQ(4, player_hand.get_points(err));
    player_hand.add_card(cards[2][2], err);
    ASSERT_EQ(6, player_hand.get_points(err));
    player_hand.add_card(cards[7][0], err);
    ASSERT_EQ(13, player_hand.get_points(err));
    player_hand.add_card(cards[7][1], err);
    ASSERT_EQ(20, player_hand.get_points(err));
    player_hand.add_card(cards[9][0], err);
    EXPECT_EQ(29, player_hand.get_points(err));
}

// Each addition of a card must increase the score by that card's value even
// if several cards have the same value (with aces)
TEST_F(HandTest, ScoreManyCardsWithDuplicatesAces) {
    player_hand.add_card(cards[2][0], err);
    ASSERT_EQ(2, player_hand.get_points(err));
    player_hand.add_card(cards[2][1], err);
    ASSERT_EQ(4, player_hand.get_points(err));
    player_hand.add_card(cards[2][2], err);
    ASSERT_EQ(6, player_hand.get_points(err));
    player_hand.add_card(cards[1][0], err);
    ASSERT_EQ(17, player_hand.get_points(err));
    player_hand.add_card(cards[1][1], err);
    ASSERT_EQ(18, player_hand.get_points(err));
    player_hand.add_card(cards[9][0], err);
    EXPECT_EQ(17, player_hand.get_points(err));
}

// A hand of one card must have count 1
TEST_F(HandTest, CountOneCard) {
    player_hand.add_card(cards[1][0], err);
    EXPECT_EQ(1, player_hand.get_nof_cards());
}

// An empty hand must have count 0
TEST_F(HandTest, CountNoCards) {
    EXPECT_EQ(0, player_hand.get_nof_cards());
}

// Each addition of a card must increase the count by 1
TEST_F(HandTest, CountManyCards) {
    player_hand.add_card(cards[1][0], err);
    ASSERT_EQ(1, player_hand.get_nof_cards());
    player_hand.add_card(cards[3][0], err);
    ASSERT_EQ(2, player_hand.get_nof_cards());
    player_hand.add_card(cards[7][0], err);
    EXPECT_EQ(3, player_hand.get_nof_cards());
}

// Each addition of a card must increase the count by 1 even if several cards
// have the same value
TEST_F(HandTest, CountManyCardsWithDuplicates) {
    player_hand.add_card(cards[1][0], err);
    ASSERT_EQ(1, player_hand.get_nof_cards());
    player_hand.add_card(cards[1][1], err);
    ASSERT_EQ(2, player_hand.get_nof_cards());
    player_hand.add_card(cards[1][2], err);
    ASSERT_EQ(3, player_hand.get_nof_cards());
    player_hand.add_card(cards[3][0], err);
    ASSERT_EQ(4, player_hand.get_nof_cards());
    player_hand.add_card(cards[3][1], err);
    ASSERT_EQ(5, player_hand.get_nof_cards());
    player_hand.add_card(cards[7][0], err);
    EXPECT_EQ(6, player_hand.get_nof_cards());
}

// The setup function has to remove the card in a hand if here is only one
TEST_F(HandTest, SetupRoundOneCard) {
    player_hand.add_card(cards[1][0], err);
    player_hand.setup_round(err);
    std::vector<card*> expected_hand = {};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// Adding no cards and calling the setup function should also result in an empty hand
TEST_F(HandTest, SetupRoundNoCards) {
    player_hand.setup_round(err);
    std::vector<card*> expected_hand = {};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// The setup function has to remove all cards in a hand
TEST_F(HandTest, SetupRoundManyCards) {
    player_hand.add_card(cards[1][0], err);
    player_hand.add_card(cards[7][0], err);
    player_hand.add_card(cards[9][0], err);
    player_hand.setup_round(err);
    std::vector<card*> expected_hand = {};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}

// Adding no cards and calling the setup function should also result in an empty hand
TEST_F(HandTest, SetupRoundManyCardsWithDuplicates) {
    player_hand.add_card(cards[1][0], err);
    player_hand.add_card(cards[1][1], err);
    player_hand.add_card(cards[1][2], err);
    player_hand.add_card(cards[7][0], err);
    player_hand.add_card(cards[7][0], err);
    player_hand.add_card(cards[9][0], err);
    player_hand.setup_round(err);
    std::vector<card*> expected_hand = {};
    EXPECT_EQ(expected_hand, player_hand.get_cards());
}