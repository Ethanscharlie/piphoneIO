#pragma once

class Menu {
public:
  virtual void render() {}
  virtual void onInput(bool a, bool b, bool c, bool d) {}
  virtual void onSecondTick() {}
};
