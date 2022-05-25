project(Blackjack-unit-tests)

set(TEST_SOURCE_FILES
        hand.cpp
        card.cpp
        player.cpp
        shoe.cpp)

add_executable(Blackjack-tests ${TEST_SOURCE_FILES})

target_compile_definitions(Blackjack-tests PRIVATE BLACKJACK_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)

target_link_libraries(Blackjack-tests gtest gtest_main Blackjack-lib)