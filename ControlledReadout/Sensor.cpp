/****************************************************************************/
//  Function: CPP file for #!!!
//  Hardware: #!!!
//  Arduino IDE: Arduino-1.8.5
//  Author:  SI
//  Date:    Dec 2017
//  Version: v0.1
//  by Noosphere
/****************************************************************************/
#include "Sensor.h"


//////// системные настройки устройств ////////
void setupLiquidCrystal(LiquidCrystal* pDevice) {
  pDevice->begin(16, 2);
}
;
void setupRTC(RTC* pDevice) {
  pDevice->begin();
  //pDevice->set(__TIMESTAMP__); // to change the current clock time
  //pDevice->start();            // to start the clock go autonomous
  //pDevice->stop();             // to stop the clock
}
;
void setupVL6180xIIC(VL6180x* pDevice) {
  while (pDevice->VL6180xInit()) {
    delay(500); // time in ms to wait for response
  }
  pDevice->VL6180xDefautSettings(); // настройки по умолочнию
}
;
void setupDHT11(DHT11* pDevice) {
  pDevice->begin();
}
;
void setupIR(IRrecv* pDevice) {
  pDevice->enableIRIn();
}
;

//////// классы дисплеев и сенсоров ////////

// ScreenOutput represents 2x16 text screen
  ScreenOutput::ScreenOutput(LiquidCrystal* pDevice) {
    lcd = pDevice;
  }
  void ScreenOutput::printLine1(String pText) {
    // print text to the first line of the screen
    lcd->setCursor(0, 0);
    lcd->print(pText + "                "); //#!!! make char[16] out of String
  }
  void ScreenOutput::printLine2(String pText) {
    // print text to the second line of the screen
    lcd->setCursor(0, 1);
    lcd->print(pText + "                "); //#!!! make char[16] out of String
  }
  void ScreenOutput::printLines(String pLine1, String pLine2) {
    // print both lines to the screen
    printLine1(pLine1);
    printLine2(pLine2);
  }

// DatetimeSensor is source of current time
  DatetimeSensor::DatetimeSensor(RTC* pDevice) {
    timekeeper = pDevice;
  }
  int DatetimeSensor::readYear() {
    timekeeper->read();
    return timekeeper->getYear();
  }
  int DatetimeSensor::readMonth() {
    timekeeper->read();
    return timekeeper->getMonth();
  }
  int DatetimeSensor::readDay() {
    timekeeper->read();
    return timekeeper->getDay();
  }
  int DatetimeSensor::readHour() {
    timekeeper->read();
    return timekeeper->getHour();
  }
  int DatetimeSensor::readMinute() {
    timekeeper->read();
    return timekeeper->getMinute();
  }
  int DatetimeSensor::readSecond() {
    timekeeper->read();
    return timekeeper->getSecond();
  }
  String DatetimeSensor::readDate() {
    // String -- current date
    char v_tmp[16];
    timekeeper->read();
    timekeeper->getDateStr(v_tmp);
    return v_tmp;
  }
  String DatetimeSensor::readTime() {
    // String -- current time
    char v_tmp[16];
    timekeeper->read();
    timekeeper->getTimeStr(v_tmp);
    return v_tmp;
  }
  String DatetimeSensor::read() {
    // measurement in String format
    char v1[16];
    char v2[16];
    timekeeper->read();
    timekeeper->getDateStr(v1);
    timekeeper->getTimeStr(v2);
    return (String) v1 + " " + v2;
  }  

// LuminositySensor represents luminosity sensor
  LuminositySensor::LuminositySensor(VL6180x* pDevice) {
    device = pDevice;
  }
  double LuminositySensor::readDouble() {
    // Double[>=0] -- current luminosity in lux
    return device->getAmbientLight(GAIN_1);
  }
  String LuminositySensor::read() {
    // measurement in String format
    return (String) readDouble();
  }

// ProximitySensor represents proximity sensor
  ProximitySensor::ProximitySensor(VL6180x* pDevice) {
    device = pDevice;
  }
  int ProximitySensor::readInt() {
    // Int[0,255] -- current proximity in mm
    return device->getDistance();
  }
  String ProximitySensor::read() {
    // measurement in String format
    return (String) readInt();
  }

// TemperatureSensor represents temperature sensor
  TemperatureSensor::TemperatureSensor(DHT11* pDevice) {
    device = pDevice;
  }
  int TemperatureSensor::readInt() {
    // Int[0,50] -- current temperature in 'C
    device->read();
    return device->getTemperatureC();
  }
  String TemperatureSensor::read() {
    // measurement in String format
    return (String) readInt();
  }

// HumiditySensor represents humidity sensor
  HumiditySensor::HumiditySensor(DHT11* pDevice) {
    device = pDevice;
  }
  int HumiditySensor::readInt() {
    // Int[20,90] -- current humidity in %
    device->read();
    return device->getHumidity();
  }
  String HumiditySensor::read() {
    // measurement in String format
    return (String) readInt();
  }

// InfraredSensor represents infrared reciever
  InfraredSensor::InfraredSensor(IRrecv* pDevice) {
    device = pDevice;
  }
  void InfraredSensor::wait() {
    // wait for next signal
    device->resume();
  }
  boolean InfraredSensor::isSignalRecieved() {
    // check for a signal
    // true if new signal recieved and saved (with recievedSignal)
    return device->decode(&recievedSignal);
  }
  int InfraredSensor::readInt() {
    // unsigned long -- current signal value
    return recievedSignal.value;
  }
  RemoteControllerButton InfraredSensor::readButton() {
    // RemoteControllerButton -- a button pressed
    switch(recievedSignal.value) {
      case 4294967295: return PRESSED;
      case 378130479: return POWER;
      case 378134559: return MINUS;
      case 378132519: return PLUS;
      case 378077439: return RED;
      case 378126399: return GREEN;
      case 378110079: return BLUE;
      case 378114159: return CROSS;
      case 378118239: return SQUARE;
      case 378093759: return TRIANGLE;
      case 378097839: return HOME;
      case 378101919: return UP;
      case 378099879: return PAGEUP;
      case 378081519: return LEFT;
      case 378091719: return MIDDLE;
      case 378116199: return RIGHT;
      case 378083559: return END;
      case 378124359: return DOWN;
      case 378085599: return PAGEDOWN;
      case 378089679: return X;
      case 378122319: return Y;
      case 378105999: return Z;
      default: return OTHER;      
    }
  }
  String InfraredSensor::read() {
    // measurement in String format
    return (String) readButton();
    /*switch(recievedSignal.value) {
      case 4294967295: return "PRESSED";
      case 378130479: return "BUTTON_POWER";
      case 378134559: return "BUTTON_MINUS";
      case 378132519: return "BUTTON_PLUS";
      case 378077439: return "BUTTON_RED";
      case 378126399: return "BUTTON_GREEN";
      case 378110079: return "BUTTON_BLUE";
      case 378114159: return "BUTTON_CROSS";
      case 378118239: return "BUTTON_SQUARE";
      case 378093759: return "BUTTON_TRIANGLE";
      case 378097839: return "BUTTON_HOME";
      case 378101919: return "BUTTON_UP";
      case 378099879: return "BUTTON_PAGEUP";
      case 378081519: return "BUTTON_LEFT";
      case 378091719: return "BUTTON_MIDDLE";
      case 378116199: return "BUTTON_RIGHT";
      case 378083559: return "BUTTON_END";
      case 378124359: return "BUTTON_DOWN";
      case 378085599: return "BUTTON_PAGEDOWN";
      case 378089679: return "BUTTON_X";
      case 378122319: return "BUTTON_Y";
      case 378105999: return "BUTTON_Z";
      default: return (String) recievedSignal.value;      
    }*/
  }

// WaterControl represents a controlled pomp
  WaterControl::WaterControl(int pPin) {
    pin = pPin;
    currentCondition = OFF;
  }
  void WaterControl::setOn() {
    // set 5v on the OutputPin
    digitalWrite(pin, HIGH);
    currentCondition = ON;
  }
  void WaterControl::setOff() {
    // set 0v on the OutputPin
    digitalWrite(pin, LOW);
    currentCondition = OFF;
  }
  String WaterControl::read() {
    // current condition in String format
    return "wtr: " + (String) currentCondition;
  }

// LightControl represents a light switch
  LightControl::LightControl(int pPin) {
    pin = pPin;
    currentCondition = OFF;
  }
  void LightControl::setOn() {
    // set 5v on the OutputPin
    digitalWrite(pin, HIGH);
    currentCondition = ON;
  }
  void LightControl::setOff() {
    // set 0v on the OutputPin
    digitalWrite(pin, LOW);
    currentCondition = OFF;
  }
  String LightControl::read() {
    // current condition in String format
    return "lht: " + (String) currentCondition;
  }


