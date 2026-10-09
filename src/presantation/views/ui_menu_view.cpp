#include "../ui_common.h"

void selecteMenuView (int screen){
     switch (screen) {
    case 0:
      drawGpsView();
      break;
    case 1:
      drawWifiView();
      break;

    case 2:
      tft.fillScreen(0x0); //burda bunu kullanma sabebimiz bluthoot ekranın sadece bir kez çizilmesi buda üstü üste piksel bime sorunu yaratıyor  bu fonksiyon sayesinde artık bluetooth çizilmeden nce ekran bir kere temizlenecek.
      drawBluetoothView();
    break;

    case 3:
      drawTestView();
      break;
    case 4:
      drawSettingsView();
      break;
    default:
      break;
  }
}

void selecteMenuAnimations(int screen){
     switch (screen) {
    case 0:
      tickGpsAnim();
      break;
      
    case 1:
      tickWifiAnim();
      break;

    case 2:
      drawBluetoothView();
    break;

    case 3:
      tickTestAnim();
      break;
    case 4:
      tickSettingsAnim();
      break;
    default:
      break;
  }
}