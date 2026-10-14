#pragma once
#include <Arduino.h>
#define BTN_LEFT  34
#define BTN_RIGHT 39
#define BTN_UP    36
#define BTN_DOWN  14
#define BTN_OK    35
#define BTN_BACK  13



struct Buttons {
  bool left, right, up, down, ok, back;
  bool leftPressed, rightPressed, upPressed, downPressed, okPressed, backPressed;
};

extern Buttons buttons;

void buttonsBegin();
void buttonsUpdate();