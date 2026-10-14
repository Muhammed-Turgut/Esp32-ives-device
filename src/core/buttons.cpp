#include "buttons.h"
Buttons buttons;

static bool okWas = false;
static bool leftWas =false;
static bool upWas = false;
static bool downWas = false;
static bool rightWas = false;
static bool backWas = false;

void buttonsBegin() {
  pinMode(BTN_OK, INPUT);
  pinMode(BTN_BACK, INPUT);
  pinMode(BTN_LEFT,INPUT);
  pinMode(BTN_RIGHT,INPUT);
  pinMode(BTN_UP,INPUT);
  pinMode(BTN_DOWN,INPUT);
}

void buttonsUpdate() {
  //Bu değerler buttonalra tıklanıp tıklanmadığını anlamak için

  bool ok = digitalRead(BTN_OK);
  buttons.okPressed = ok && !okWas;
  buttons.ok = ok;
  okWas = ok;

  bool left = digitalRead(BTN_LEFT);
  buttons.leftPressed = left && !leftWas;
  buttons.left = left;
  leftWas = left;

  bool right = digitalRead(BTN_RIGHT);
  buttons.rightPressed = right && !rightWas;
  buttons.right = right;
  rightWas = right;

  bool back = digitalRead(BTN_BACK);
  buttons.backPressed = back && !backWas;
  buttons.back = back;
  backWas = back;

  bool up = digitalRead(BTN_UP);
  buttons.upPressed = up && !upWas;
  buttons.up = up;
  upWas = up;

  bool down = digitalRead(BTN_DOWN);
  buttons.downPressed = down && !downWas;
  buttons.down = down;
  downWas = down;


  // diğer tuşlar aynı
}