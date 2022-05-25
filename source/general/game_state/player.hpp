#ifndef PLAYER_HPP
#define PLAYER_HPP


#include <string>
#include "card.hpp"
#include "hand.hpp"
#include "../serialization/uuid_generator.h"
#include "../../../rapidjson/include/rapidjson/document.h"
#include "../serialization/unique_serializable.h"
#include "../serialization/serializable_value.h"

//TODO: flag access function for finished turn
//TODO: hit implemented in game_state, also set flag if over 21

class player : public unique_serializable {
private:
    serializable_value<std::string>* _player_name;
    serializable_value<int>* _bet_size;
    serializable_value<int>* _money;
    serializable_value<bool>* _finished_turn;
    hand* _player_hand;
    

#ifdef BLACKJACK_SERVER                  // is this macro needed?
    std::string _game_id;
#endif

    /*
     * Deserialization constructor
     */
    player(std::string id,
           serializable_value<std::string>* player_name,
           serializable_value<int>* bet_size,
           serializable_value<int>* money,
           serializable_value<bool>* finished_turn,
           hand* player_hand);

public:
// constructors
    explicit player(std::string name);   // for client
    ~player();

#ifdef BLACKJACK_SERVER
    player(std::string id, std::string name);  // for server

    std::string get_game_id();
    void set_game_id(std::string game_id);
#endif

    // accessors
    int get_bet_size() const noexcept;
    int get_money() const noexcept;
    bool has_finished_turn() const noexcept;
    hand* get_hand() const noexcept;
    std::string get_player_name() const noexcept;
    void set_finished_turn();
    bool is_broke();

#ifdef BLACKJACK_SERVER
    // state update functions
    void wrap_up_round(int dealer_points, std::string& err);
    void setup_round(std::string& err);

    // player actions (probably not needed)
    bool make_bet(int bet_size, std::string &err);

    // helper functions for game_state
    // helper functions to calculate winnings
    void won_round();
    void lost_round();
    void draw_round();

#endif

    // serialization
    static player* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};

#endif
