#pragma once

class Menu {
public:
  virtual void render() {}
  virtual void onSecondTick() {}

  virtual void onAButton() {}
  virtual void onBButton() {}
  virtual void onCButton() {}
  virtual void onJoystick(int x, int y) {}
};
