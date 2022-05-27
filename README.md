# Blackjack

This is a C++ implementation of a simplified version of Blackjack. The implementation features a client/server architecture for multiplayer scenarios.
It uses [wxWidgets](https://www.wxwidgets.org/) for the GUI, [sockpp](https://github.com/fpagliughi/sockpp) for the network interface, [rapidjson](https://rapidjson.org/md_doc_tutorial.html) for object serialization, and [googletest](https://github.com/google/googletest) for the unit tests.

## 1. Rules
In our version each player has to decide for a bet, the amount has to be at least a certain unit, in our case 1$. After the player places his bet he receives 2 cards. He then can decide to hit (draw another card) or stand (finish his turn). At the end of the turn the hand of the dealer is displayed. If the player has more than 21 points he loses his bet. If his points are closer to 21 than the dealer or the dealer busts and the player does not, the player wins his bet. The bets are payed out 2:1. The value of the cards are the corresponding numerical values for numerals, 10 for figures and 1 or 11 for aces. The dealer is automatic and he draws cards until he has at least 16 points or more then he stands.

<img src="./assets/final_logo.png" alt="Blackjack-logo" width="200"/>

This project is based upon a template project for the students of the course Software Engineering. The template can be found [here](https://gitlab.ethz.ch/hraymond/cse-lama-example-project)

## 2. Compile instructions
This project only works on UNIX systems (Linux / MacOS). We recommend using [Ubuntu](https://ubuntu.com/#download), as it offers the easiest way to setup wxWidgets. Therefore, we explain installation only for Ubuntu systems. The following was tested on a Ubuntu 20.4 system, but should also work for earlier versions of Ubuntu.

**Note:** If you create a virtual machine, we recommend to give the virtual machine **at least 12GB** of (dynamic) harddrive space (CLion and wxWidgets need quite a lot of space).

### 2.1 Prepare OS Environment

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


### 2.2 Compile Code
1. Open Clion
2. Click `File > Open...` and there select the **/sockpp** folder of this project
3. Click `Build > Build all in 'Debug'`
4. Wait until sockpp is compiled (from now on you never have to touch sockpp again ;))
5. Click `File > Open...` select the **/blackjack** folder
6. Click `Build > Build all in 'Debug'`
7. Wait until Blackjack-server, Blackjack-client and Blackjack-tests are compiled

## 3. Run the Game
1. Open a console in the project folder, navigate into "cmake-build-debug" `cd cmake-build-debug`
2. Run server `./Blackjack-server`
3. In new consoles run as many clients as you want players `./Blackjack-client`

## 4. Run the Unit Tests
1. CLion should automatically create a Google Test configuration Blackjack-tests which will run all tests. See [Google Test run/debug configuration﻿](https://www.jetbrains.com/help/clion/creating-google-test-run-debug-configuration-for-test.html#gtest-config) for more information.
2. From the list on the main toolbar, select the configuration Lama-tests.
3. Click ![run](https://resources.jetbrains.com/help/img/idea/2021.1/artwork.studio.icons.shell.toolbar.run.svg) or press `Shift+F10`.
   
You can run individual tests or test suites by opening the corresponding file in the **/unit-tests** folder and clicking ![run](https://resources.jetbrains.com/help/img/idea/2021.1/artwork.studio.icons.shell.toolbar.run.svg) next to the test method or class. For more information on testing in CLion read the [documentation](https://www.jetbrains.com/help/clion/performing-tests.html).


## 5. Code Documentation
The code for the game can be found in **/source**, where it is separated into following folders:
- **/client** contains only code that is used on the client side (e.g. UI, sending messages)
- **/general** contains code that is shared between server and client.
    - **/exceptions** contains the exception class used on server and client side.
    - **/game_state** contains the `game_state` that is synchronized between client and server. We use the [conditional pre-compile directive](https://www.cplusplus.com/doc/tutorial/preprocessor/) BLACKJACK_SERVER to enable certain parts of the code only on the server side. Namely, these are the state update functions, as they should only happen on the server. The client simply reflects the current game state as sent by the server without modifying it directly. 
    - **/network** contains all the messages that are being passed between client and server. We use the BLACKJACK_CLIENT pre-compile directive to make `server_repsonses` only executable on the client side (through the function `Process()`) .
    - **/serialization** contains base classes for serializing `game_state`, `client_request` and `server_response` objects. **Serialization** is the process of transforming an object instance into a string that can be sent over a network, where the receiver deserializes it, i.e. recreates the object from the string. If you are interested, [read me on Wikipedia](https://en.wikipedia.org/wiki/Serialization).
- **/server** contains only code that is relevant for the server (e.g. player management, game instance management, receiving messages)

The **/assets** folder stores all the images that are being used to render the GUI.

The **/unit-tests** folder contains all unit tests, which validate the correct behaviour of the functions written in the source code of the game. 
