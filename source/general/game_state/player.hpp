#ifndef PLAYER_HPP
#define PLAYER_HPP


#include <string>
#include "hand.hpp"
#include "../serialization/uuid_generator.h"
#include "../../../rapidjson/include/rapidjson/document.h"
#include "../serialization/unique_serializable.h"
#include "../serialization/serializable_value.h"

class player : public unique_serializable {
private:
    serializable_value<std::string>* _player_name;
    serializable_value<int>* _bet_size;
    serializable_value<int>* _money;
    serializable_value<bool>* _has_insurance;
    serializable_value<bool>* _has_doubled_down;
    std::vector<card*> _hand;
    

#ifdef BLACKJACK_SERVER                 // is this macro needed?
    std::string _game_id;
#endif

    /*
     * Deserialization constructor
     */
    player(std::string id,
           serializable_value<std::string>* player_name,
           serializable_value<int>* bet_size,
           serializable_value<int>* money,
           serializable_value<bool>* has_insurance,
           serializable_value<bool>* has_doubled_down,
           std::vector<card*> hand);

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
    int get_nof_cards() const noexcept;     // return number of cards player has on hand
    int get_bet_size() const noexcept;
    int get_money() const noexcept;
    bool is_insured() const noexcept;
    bool is_doubled_down() const noexcept;
    std::vector<card*> get_hand() const noexcept;
    std::string get_player_name() const noexcept;

#ifdef BLACKJACK_SERVER
    // state update functions
    bool add_card(card* card, std::string& err);
    void wrap_up_round(std::string& err); //???
    void setup_round(std::string& err);   //???

    // I don't know which of those functions we will need in the end, implementing all of them
    void hit();
    void stand();
    void take_insurance();
    void double_down();
    int get_points();
    bool is_broke();
    bool check_if_over_21();
    bool check_if_less_than_dealer();
    void won_round();
    void lost_round();

#endif

    // serialization
    static player* from_json(const rapidjson::Value& json);
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};

#endif
