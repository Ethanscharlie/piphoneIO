#pragma once

#include <functional>
#include <string>
#include <vector>

class Menu {
public:
  std::vector<std::pair<std::string, std::function<void()>>> options;
  int selection = 0;

  Menu() {}

  Menu(std::vector<std::pair<std::string, std::function<void()>>> options)
      : options(options) {}

  virtual void onSecondTick() {}
  virtual void onBButton() {}

  virtual void render();
  virtual void onAButton();
  virtual void onJoystick(int x, int y);
};
