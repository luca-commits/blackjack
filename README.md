# Blackjack

This is a C++ implementation of a simplified version of Blackjack. The implementation features a client/server architecture for multiplayer scenarios.
It uses [wxWidgets](https://www.wxwidgets.org/) for the GUI, [sockpp](https://github.com/fpagliughi/sockpp) for the network interface, [rapidjson](https://rapidjson.org/md_doc_tutorial.html) for object serialization, and [googletest](https://github.com/google/googletest) for the unit tests.
In our version each player has to decide for a bet, the amount has to be at least a certain unit, in our case 1$. After the player places his bet he receives 2 cards. He then can decide to hit (drawing another card) or stand (finishing his turn). At the end of the turn the hand of the dealer is displayed. If the player has more than 21 points he loses his bet. If his points are closer to 21 than the dealer or the dealer busts and the player not, the player wins his bet. The bets are payed out 2:1. The value of the cards are the corresponding numerical values for numerals, 10 for figures and 1 or 11 for aces. The dealer is automatic and he draws cards until he has at least 16 points or more then he stands.

<img src="./assets/final_logo.png" alt="Blackjack-logo" width="200"/>

This project is based upon a template project for the students of the course Software Engineering. The template can be found [here](https://gitlab.ethz.ch/hraymond/cse-lama-example-project)

## 1. Compile instructions
This project only works on UNIX systems (Linux / MacOS). We recommend using [Ubuntu](https://ubuntu.com/#download), as it offers the easiest way to setup wxWidgets. Therefore, we explain installation only for Ubuntu systems. The following was tested on a Ubuntu 20.4 system, but should also work for earlier versions of Ubuntu.

**Note:** If you create a virtual machine, we recommend to give the virtual machine **at least 12GB** of (dynamic) harddrive space (CLion and wxWidgets need quite a lot of space).

### 1.1 Prepare OS Environment

#### Ubuntu 20.4
The OS should already have git installed. If not, you can use: 
`sudo apt-get install git`

Then use  `git clone` to fetch this repository.

Execute the following commands in a console:
1. `sudo apt-get update`
2. `sudo apt-get install build-essential` followed by `sudo reboot`
3. if on virtual machine : install guest-additions (https://askubuntu.com/questions/22743/how-do-i-install-guest-additions-in-a-virtualbox-vm) and then `sudo reboot`
4. `sudo snap install clion --classic` this installs the latest stable CLion version
5. `sudo apt-get install libwxgtk3.0-gtk3-dev` this installs wxWidgets (GUI library used in this project)


### 1.2 Compile Code
1. Open Clion
2. Click `File > Open...` and there select the **/sockpp** folder of this project
3. Click `Build > Build all in 'Debug'`
4. Wait until sockpp is compiled (from now on you never have to touch sockpp again ;))
5. Click `File > Open...` select the **/blackjack** folder
6. Click `Build > Build all in 'Debug'`
7. Wait until Blackjack-server, Blackjack-client and Blackjack-tests are compiled

## 2. Run the Game
1. Open a console in the project folder, navigate into "cmake-build-debug" `cd cmake-build-debug`
2. Run server `./Blackjack-server`
3. In new consoles run as many clients as you want players `./Blackjack-client`

## 3. Run the Unit Tests
1. CLion should automatically create a Google Test configuration Blackjack-tests which will run all tests. See [Google Test run/debug configuration﻿](https://www.jetbrains.com/help/clion/creating-google-test-run-debug-configuration-for-test.html#gtest-config) for more information.
2. From the list on the main toolbar, select the configuration Lama-tests.
3. Click ![run](https://resources.jetbrains.com/help/img/idea/2021.1/artwork.studio.icons.shell.toolbar.run.svg) or press `Shift+F10`.
   
You can run individual tests or test suites by opening the corresponding file in the **/unit-tests** folder and clicking ![run](https://resources.jetbrains.com/help/img/idea/2021.1/artwork.studio.icons.shell.toolbar.run.svg) next to the test method or class. For more information on testing in CLion read the [documentation](https://www.jetbrains.com/help/clion/performing-tests.html).


## 4. Code Documentation
The code for the game can be found in **/source**, where it is separated into following folders:
- **/client** contains only code that is used on the client side (e.g. UI, sending messages)
- **/general** contains code that is shared between server and client.
    - **/exceptions** contains the exception class used on server and client side.
    - **/game_state** contains the `game_state` that is synchronized between client and server. We use the [conditional pre-compile directive](https://www.cplusplus.com/doc/tutorial/preprocessor/) BLACKJACK_SERVER to enable certain parts of the code only on the server side. Namely, these are the state update functions, as they should only happen on the server. The client simply reflects the current game state as sent by the server without modifying it directly. 
    - **/network** contains all the messages that are being passed between client and server. We use the BLACKJACK_CLIENT pre-compile directive to make `server_repsonses` only executable on the client side (through the function `Process()`) .
    - **/serialization** contains base classes for serializing `game_state`, `client_request` and `server_response` objects. **Serialization** is the process of transforming an object instance into a string that can be sent over a network, where the receiver deserializes it, i.e. recreates the object from the string. If you are interested, [read me on Wikipedia](https://en.wikipedia.org/wiki/Serialization).
- **/server** contains only code that is relevant for the server (e.g. player management, game instance management, receiving messages)

The **/asset** folder stores all the images that are being used to render the GUI.

The **/unit-tests** folder contains all unit tests, which validate the correct behaviour of the functions written in the source code of the game. 

### 5.1 Overview

First off, this project consists of a **server** and a **client**, each with their own main.cpp file. 

The client renders the GUI that is presented to the player, whereas the server is a console application without a user interface. Every action a player performs in the client application (for example trying to draw a card) is sent as a formatted message to the server application, which processes the request.

### 5.2 Network Interface
Everything that is passed between client and server are objects of type `client_request` and `server_response`. Since the underlying network protocol works with TCP, these `client_request` and `server_response` objects are transformed into a **[JSON](https://wiki.selfhtml.org/wiki/JSON) string**, which can then be sent over the network. The receiving end reads the JSON string and constructs an object of type `client_request` resp. `server_response` that reflects the exact parameters that are specified in the JSON string. This process is known as **serialization** (object to string) and **deserialization** (string to object). If you want to read more about serialization, [read me on Wikipedia](https://en.wikipedia.org/wiki/Serialization).


#### 5.2.1 Serialization & Deserialization of messages
Both, the `client_request` and `server_response` base classes, implement the abstract class `serializable` with its `write_into_json(...)` function. It allows to serialize the object instance into a JSON string. Additionally, they have a static function `from_json(...)`, which allows creating an object instance from a JSON string.

```cpp
// All request types of your imlementation
enum RequestType {
    join_game,
    start_game,
    make_bet,
    hit,
    stand,
};

class client_request : public serializable {
protected:
    RequestType _type;   // stores the type of request, such that the receiving end knows how to deserialize it
    std::string _req_id; // unique id of this request
    std::string _player_id; // id of the player sending the request
    std::string _game_id;   // id of the game this request is for

    ...
private:
    // for deserializing RequestType (contains mappings from string to RequestType)
    // IMPORTANT: Add mapping for your own RequestTypes to this unordered_map
    static const std::unordered_map<std::string, RequestType> _string_to_request_type;
    
    // for serializing RequestType (contains mappings from RequestType to string)
    // IMPORTANT: Add mapping for your own RequestTypes to this unordered_map
    static const std::unordered_map<RequestType, std::string> _request_type_to_string;

public:
    // DESERIALIZATION: Attempts to create the specific client_request from the provided json.
    static client_request* from_json(const rapidjson::Value& json);

    // SERIALIZATION: Serializes the client_request into a json object that can be sent over the network
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
};
```

##### Serialization

Here is the **base-class** implementation of the Serialization:
```cpp
// Implementation in the base-class client_request
void client_request::write_into_json(rapidjson::Value &json,
                                     rapidjson::MemoryPoolAllocator<rapidjson::CrtAllocator> &allocator) const {
    // Look up string value of this client_request's RequestType and store it in the json document
    rapidjson::Value type_val(_request_type_to_string.at(this->_type).c_str(), allocator);
    json.AddMember("type", type_val, allocator);

    // Save player_id in the JSON document
    rapidjson::Value player_id_val(_player_id.c_str(), allocator);
    json.AddMember("player_id", player_id_val, allocator);

    // Save game_id in the JSON document
    rapidjson::Value game_id_val(_game_id.c_str(), allocator);
    json.AddMember("game_id", game_id_val, allocator);
    ...
}

##### Deserialization

The deserialization of `client_request` JSONs always goes through the `from_json(...)` function of the `client_request` class. In this function, the "type" field, stored in the JSON, is inspected to determine, which subclass should be called to perform the deserialization: 

```cpp
if (json.HasMember("type") && json["type"].IsString()) {
        // Get the RequestType stored as a string in the JSON
        const std::string type = json["type"].GetString();
        // Lookup the actual RequestType per string from a pre-defined unordered_map
        const RequestType request_type = client_request::_string_to_request_type.at(type);

        // Call the correct from_json() specialization
        if (request_type == RequestType::play_card) {
            return play_card_request::from_json(json);
        }
        else if (request_type == RequestType::draw_card) {
            return draw_card_request::from_json(json);
        }
        else if (...) {
            ...
        } else {
            throw BlackjackException("Encountered unknown ClientRequest type " + type);
        }
    }
    throw BlackjackException("Could not determine type of ClientRequest. JSON was:\n" + json_utils::to_string(&json));
```

There are plenty of examples of subclasses in the network/requests folder, where you can see how the serialization/deserialization scheme works.

#### 5.2.2 Sending messages
#### Client -> Server:
The static class `ClientNetworkManager` on the client side invokes its `sendRequest(const client_request& request)` function with the `client_request` that you want to send. The server's response will arrive as an object of type `request_response` and the `ClientNetworkManager` will invoke the `Process()` function of that `request_response` object automatically.

#### Server -> Client:
All messages arriving at the server are being deserialized and then passed on to the `handle_request(client_request* req)` function of the `request_handler` singleton class. This function returns a pointer to an object of type `request_response` (a subclass of `server_response`), which is then automatically sent back to the requesting client. In your game implementation you should extend the `handle_request(client_request* req)` function of the `request_handler`, such that it can handle the `client_request` that you add to your game and return an object of type `request_response` with all parameters you want to send. 

If the `client_request` causes an update of the game_state you should also update all other players of that game about the game_state change. This happens in the `game_instance` class, here examplified at the case where a `start_game_request` calls the `start_game(...)` function on the respective `game_instance` on the server side:

```cpp
bool game_instance::start_game(player* player, std::string &err) {
    modification_lock.lock();   // make sure only one request can modify the game_state at a time

    // Try to start the game
    if (_game_state->start_game(err)) { 
        // create a full_state_response (subclass of server_response) with the full game_state inside
        full_state_response state_update_msg = full_state_response(this->get_id(), *_game_state);
        // BROADCAST new game_state to all other players
        server_network_manager::broadcast_message(state_update_msg, _game_state->get_players(), player);

        modification_lock.unlock(); // allow other threads to modify the game_state
        return true;
    }
    modification_lock.unlock();
    return false;
}
```

#### 5.2.3 Debugging Messages

By default, the server (specifically, the server_network_manager) will print every valid message that it receives to the console. In order for this to work in your project as well, you have to make sure that your CMake file contains a line, where the preprocessor variable PRINT_NETWORK_MESSAGES is defined for your server executable. 

```
target_compile_definitions(Blackjack-server PRIVATE PRINT_NETWORK_MESSAGES=1)
```

If a wrongly formatted message arrives at the server, it will print an error message with the received message string to the console. 

If you want to manually print one of your serialized messages (or any other serialized object for that matter), you can use the helper function `json_utils::to_string(const rapidjson::Value* json)` as follows. 

```cpp
#include "source/general/serialization/json_utils.h"
#include "rapidjson/include/rapidjson/document.h"

...
// Create a request to serialize
join_game_request* req = new join_game_request(player->get_id(), player->get_player_name());

// serialize the request object
rapidjson::Document* req_json = req->to_json();

// print serialization to the console.
std::cout << json_utils::to_string(req_json) << std::endl;
```


### 5.3 Game State

The `game_state` class stores all parameters that are required to represent the game on the client (resp. server) side. In order to synchronize this `game_state` among all players, the `game_state` can also be **serialized** and **deserialized**. If a `client_request` was successfully executed on the server, then the `request_response` that is sent back to the client contains a serialized version of the updated `game_state`. All other players receive the updated `game_state` at the same time through a `full_state_response`.

To serialize the `game_state`, the same `write_into_json(...)` function is used as for the `client_request`. 

```cpp
class game_state : public unique_serializable {
public:

...

private:
    std::vector<player*> _players;
    shoe* _shoe;
    hand* _dealers_hand;
    serializable_value<bool>* _is_started;
    serializable_value<bool>* _is_finished;
    serializable_value<int>* _round_number;
    serializable_value<int>* _current_player_idx;
    serializable_value<int>* _starting_player_idx; // very possible this variable is NOT needed

    // from_diff constructor
    game_state(std::string id);

    // deserialization constructor
    game_state(
            std::string id,
            std::vector<player*>& players,
            shoe* shoe,
            hand* dealers_hand,
            serializable_value<bool>* is_started,
            serializable_value<bool>* is_finished,
            serializable_value<int>* round_number,
            serializable_value<int>* current_player_idx,
            serializable_value<int>* starting_player_idx);

    // returns the index of 'player' in the '_players' vector
    int get_player_index(player* player) const;
public:
    game_state();

    ...
    // SERIALIZATION
    virtual void write_into_json(rapidjson::Value& json, rapidjson::Document::AllocatorType& allocator) const override;
    // DESERIALIZATION
    static game_state* from_json(const rapidjson::Value& json);
};
```

The `game_state` inherits from `unique_serializable`, which essentially requires the `write_into_json()` function and adds a unique `id` to the object, such that it can be uniquely identified. Similarly, each parameter nested inside the `game_state` (e.g. players, draw_pile, etc.) also inherit from `unique_serializable` and therefore have their own `id` and serialization, resp. deserialization functions.

On the client side, the new `game_state` is then passed to the `updateGameState(game_state*)` function of the `GameController` class, which performs a redraw of the GUI.


### 5.4 GUI with wxWidgets

The GUI of the project was built using the cross-platform GUI library [wxWidgets](https://www.wxwidgets.org/). In order to build a project using wxWidget elements, you will first need to install wxWidgets on your system (see Section 1.1 above). 

#### 5.4.1 Structure & Important Classes

Here is a list of the most important elements of our GUI. This is just meant as an overview, you will need to look up their correct usage in wxWidget's [documentation](https://docs.wxwidgets.org/3.0/index.html).

* __Application core__
    * __`wxIMPLEMENT_APP()`__: In order to properly interact with the operating system's GUI, wxWidgets takes over the control flow of your application. wxWidgets therefore has its own `main()` function, that you can reference with the macro `wxIMPLEMENT_APP(wxApp*)`.
    * __`wxApp`__: The core class of your application must inherit from the `wxApp` class. wxWidgets will call the `OnInit()` function when starting the application. You can find the example project's implementation in `src/client/app/Blackjack`.
* __Windows__
    * __`wxFrame`__: Each window of your application must inherit from the `wxFrame` class. The example project has one window which you can find here: `src/client/windows/GameWindow`
* __GUI elements__
    * __`wxPanel`__: Panels serve as containers for elements within a window. All panels must instantiate or inherit from the `wxPanel` class. A panel can contain one or more subpanels.
    * __`wxBoxSizer`__: Box sizers allow you to layout your panels within a window, either horizontally or vertically. By nesting box sizers, you can create complex layouts. Have a look at `src/client/panels/ConnectionPanel` for an example.
    * __`wxStaticText`__: This class displays text in your GUI.
    * __`wxButton`__: This class creates a clickable button in your GUI.
* __Pop-ups__
    * __`wxMessageBox()`__: You can use this function to display a small pop-up window with text in front of the your current main window. This is useful to display error or status messages.


#### 5.4.2 Events

Like in most GUI environments, objects in wxWidgets trigger __events__ when they are interacted with by the user. For instance, a button will trigger a `wxEVT_BUTTON` event when clicked. Similarly, a panel will trigger a `wxEVT_LEFT_UP` event when clicked. There are many other events that can be triggered - for example when a keyboard key is pressed, when a window is resized, or when the cursor moves over an element.

In order to make the GUI interactive, we must specify the effect of an event. The easiest way is to __bind__ an event to a lambda function. A lambda function is an unnamed function that can be used as an r-value.

#### 5.4.3 Positioning

There are two ways to position elements (panels, button, etc.) within a parent element (window, panel): 
* __Using sizers__: In this approach, you will only need to provide an element's size, but not its position. You can then add that element to a sizer, which will then determine the element's position based on the sizer's predefined behavior. The most common sizer is `wxBoxSizer`, which allows you to position a set of elements one after the other, vertically or horizontally. You can also allow sizers to change the size of elements depending on the available space. This means you can have your GUI adapt to the size of the user's window and/or screen.
* __Using absolute positioning__: In this approach, you need to provide a position (as `wxPoint`) for each element. This position refers to the offset of the top-left corner of this element from the top-left corner of its parent. Using absolute positioning gives you much more control over the layout of your GUI. However, it is also much more work, as you will need to calculate the position for every single element. This is especially difficult if you want to adapt to changing window sizes.

#### 5.4.4 Classes for reuse

This project provides two classes that can be reused without changing anything:

* __`ImagePanel`__: This behaves like a regular panel, but shows an image in the background. You should use this to display any image you need for your GUI.
* __`InputField`__: This provides a user input field with a label in front of it. Use `getValue()` to get the user's input.
