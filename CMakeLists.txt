cmake_minimum_required(VERSION 3.15)
project(Blackjack) # your project name

set(CMAKE_CXX_STANDARD 20)

include_directories(sockpp/include)

find_package(wxWidgets COMPONENTS core base net REQUIRED)
include(${wxWidgets_USE_FILE})

# define a variable CLIENT_SOURCE_FILES that contains the paths to all source files required to compile the client executable
set(CLIENT_SOURCE_FILES
        source/client/main.cpp
        source/client/app/Blackjack.cpp source/client/app/Blackjack.hpp
        source/client/GameController.cpp source/client/GameController.hpp
        # UI
        source/client/windows/GameWindow.cpp source/client/windows/GameWindow.hpp
        source/client/uiElements/ImagePanel.cpp source/client/uiElements/ImagePanel.hpp
        source/client/panels/ConnectionPanel.cpp source/client/panels/ConnectionPanel.hpp
        source/client/panels/MainGamePanel.cpp source/client/panels/MainGamePanel.hpp
        source/client/panels/BetPanel.cpp source/client/panels/BetPanel.hpp
        source/client/uiElements/InputField.cpp source/client/uiElements/InputField.hpp
        source/client/uiElements/ImagePanel.cpp source/client/uiElements/ImagePanel.hpp
        # network
        source/client/network/ClientNetworkManager.cpp source/client/network/ClientNetworkManager.hpp
        source/client/network/ResponseListenerThread.cpp source/client/network/ResponseListenerThread.hpp
        # game state
        source/general/game_state/card.cpp source/general/game_state/card.hpp
        source/general/game_state/hand.cpp source/general/game_state/hand.hpp
        source/general/game_state/game_state.cpp source/general/game_state/game_state.hpp
        source/general/game_state/player.cpp source/general/game_state/player.hpp
        source/general/game_state/shoe.cpp source/general/game_state/shoe.hpp
        # client requests
        source/general/network/requests/client_request.cpp source/general/network/requests/client_request.hpp
        source/general/network/requests/join_game_request.cpp source/general/network/requests/join_game_request.hpp
        source/general/network/requests/start_game_request.cpp source/general/network/requests/start_game_request.hpp
        source/general/network/requests/hit_request.cpp source/general/network/requests/hit_request.hpp
        source/general/network/requests/stand_request.cpp source/general/network/requests/stand_request.hpp
        source/general/network/requests/make_bet_request.cpp source/general/network/requests/make_bet_request.hpp
        # server responses
        source/general/network/responses/server_response.cpp source/general/network/responses/server_response.hpp
        source/general/network/responses/answer_rqst_response.cpp source/general/network/responses/answer_rqst_response.hpp
        source/general/network/responses/change_gamestate_msg.cpp source/general/network/responses/change_gamestate_msg.hpp
        # serialization
        source/general/serialization/serializable.h
        source/general/serialization/value_type_helpers.h
        source/general/serialization/vector_utils.h
        source/general/serialization/serializable_value.h
        source/general/serialization/json_utils.h
        source/general/serialization/uuid_generator.h
        source/general/serialization/unique_serializable.cpp source/general/serialization/unique_serializable.h)

# define a variable SERVER_SOURCE_FILES that contains the paths to all source files required to compile the server executable
set(SERVER_SOURCE_FILES
        source/server/main.cpp
        source/server/game_instance.cpp source/server/game_instance.hpp
        source/server/game_instance_manager.cpp source/server/game_instance_manager.hpp
        source/server/player_manager.cpp source/server/player_manager.hpp
        source/server/server_network_manager.cpp source/server/server_network_manager.hpp
        # game state
        source/general/game_state/card.cpp source/general/game_state/card.hpp
        source/general/game_state/hand.cpp source/general/game_state/hand.hpp
        source/general/game_state/game_state.cpp source/general/game_state/game_state.hpp
        source/general/game_state/player.cpp source/general/game_state/player.hpp
        source/general/game_state/shoe.cpp source/general/game_state/shoe.hpp
        # client requests
        source/general/network/requests/client_request.cpp source/general/network/requests/client_request.hpp
        source/general/network/requests/join_game_request.cpp source/general/network/requests/join_game_request.hpp
        source/general/network/requests/start_game_request.cpp source/general/network/requests/start_game_request.hpp
        source/general/network/requests/hit_request.cpp source/general/network/requests/hit_request.hpp
        source/general/network/requests/stand_request.cpp source/general/network/requests/stand_request.hpp
        source/general/network/requests/make_bet_request.cpp source/general/network/requests/make_bet_request.hpp
        # server responses
        source/general/network/responses/server_response.cpp source/general/network/responses/server_response.hpp
        source/general/network/responses/answer_rqst_response.cpp source/general/network/responses/answer_rqst_response.hpp
        source/general/network/responses/change_gamestate_msg.cpp source/general/network/responses/change_gamestate_msg.hpp
        # serialization
        source/general/serialization/serializable.h
        source/general/serialization/value_type_helpers.h
        source/general/serialization/vector_utils.h
        source/general/serialization/serializable_value.h
        source/general/serialization/json_utils.h
        source/general/serialization/uuid_generator.h
        source/general/serialization/unique_serializable.cpp source/general/serialization/unique_serializable.h 
        source/server/request_handler.hpp source/server/request_handler.cpp)


# set source files for client-executable
add_executable(Blackjack-client ${CLIENT_SOURCE_FILES})
# set compile directives for client-executable
target_compile_definitions(Blackjack-client PRIVATE BLACKJACK_CLIENT=1 RAPIDJSON_HAS_STDSTRING=1)
# link with wxWidgets
target_link_libraries(Blackjack-client ${wxWidgets_LIBRARIES})
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Blackjack-client PRIVATE PRINT_NETWORK_MESSAGES=1)

# set source files for server-executable
add_executable(Blackjack-server ${SERVER_SOURCE_FILES})
# set compile directives for server-executable
target_compile_definitions(Blackjack-server PRIVATE BLACKJACK_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)
# Comment out if you don't want to print network-related messages into the console
target_compile_definitions(Blackjack-server PRIVATE PRINT_NETWORK_MESSAGES=1)


# linking to sockpp
if(WIN32)
    message("Detected Win32")

    target_link_libraries(Blackjack-client ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib)
    target_link_libraries(Blackjack-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/sockpp-static.lib)

    # Necessary to get sockets working under Windows (with MingW)
    target_link_libraries(Blackjack-client wsock32 ws2_32)
else()
    message("Not Win32, so probably a Linux") # We assume it's Linux in this case

    set(THREADS_PREFER_PTHREAD_FLAG ON)
    find_package(Threads REQUIRED)

    target_link_libraries(Blackjack-client ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.dylib Threads::Threads)
    target_link_libraries(Blackjack-server ${CMAKE_SOURCE_DIR}/sockpp/cmake-build-debug/libsockpp.dylib Threads::Threads)
endif()

# copy assets (images) to binary directory
file(INSTALL assets DESTINATION ${CMAKE_BINARY_DIR})


set(CMAKE_CXX_FLAGS "--coverage")
# set source files for server-library
add_library(Blackjack-lib ${SERVER_SOURCE_FILES})
# set compile directives for server-library
target_compile_definitions(Blackjack-lib PRIVATE BLACKJACK_SERVER=1 RAPIDJSON_HAS_STDSTRING=1)

add_subdirectory(googletest)
add_subdirectory(unit-tests)