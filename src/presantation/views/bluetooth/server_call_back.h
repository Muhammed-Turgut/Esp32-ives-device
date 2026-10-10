#ifndef SERVER_CALL_BACK_H
#define SERVER_CALL_BACK_H

#include <BLEDevice.h>

class ServerCallBack : public BLEServerCallbacks {
  void onConnect(BLEServer* s) override {}
  void onDisconnect(BLEServer* s) override {
    BLEDevice::startAdvertising();
  }
};

#endif