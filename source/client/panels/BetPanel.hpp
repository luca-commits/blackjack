#ifndef BETPANEL_HPP
#define BETPANEL_HPP


class GameController {

public:
  int getBetSize();
  int getPlayerMoney();

private:
  int betSize;
  int playerMoney;
};

#endif //BETPANEL_HPP