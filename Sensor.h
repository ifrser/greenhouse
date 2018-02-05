/****************************************************************************/
//  Function: Header file for #!!!
//  Hardware: #!!!
//  Arduino IDE: Arduino-1.8.5
//  Author:  SI
//  Date:    Dec 2017
//  Version: v0.1
//  by Noosphere
/****************************************************************************/
#ifndef SENSOR_H_
#define SENSOR_H_


//////// Внешние библиотеки с подсказками ////////
// шина IIC https://geektimes.ru/post/255812/
#include <Wire.h>             // шина I²C

// LCD      http://wiki.amperka.ru/Схемы-подключения:подключение-текстового-экрана
#include <LiquidCrystal.h>    // LCD

// часы     http://wiki.amperka.ru/продукты:troyka-rtc
#include "TroykaRTC.h"        // часы

// датчик   http://wiki.amperka.ru/продукты:troyka-proximity
#include <SparkFun_VL6180X.h> // датчик приближения-освещенности VL6180X

// датчик   http://wiki.amperka.ru/продукты:troyka-dht11
#include "TroykaDHT11.h"      // датчик температуры-влажности B045

// датчик   http://amperka.ru/product/ir-receiver
#include <IRremote.h>         // инфракрасный приемник


//////// системные настройки устройств ////////
void setupLiquidCrystal(LiquidCrystal* pDevice);
void setupRTC(RTC* pDevice);
void setupVL6180xIIC(VL6180x* pDevice);
void setupDHT11(DHT11* pDevice);
void setupIR(IRrecv* pDevice);
// #!!! use "pinMode(relay1, OUTPUT);" for setupWaterControl
// #!!! use "pinMode(relay2, OUTPUT);" for setupLightControl


//////// классы дисплеев и сенсоров ////////
class ScreenOutput {
  // interp. 2x16 text streen
  private:
  LiquidCrystal* lcd;
  public:
  ScreenOutput(LiquidCrystal* pDevice);
  void printLine1(String pText);  // print text to the first line of the screen
  void printLine2(String pText);  // print text to the second line of the screen
  void printLines(String pLine1, String pLine2);  // print both lines to the screen
}
;
class DatetimeSensor {
  // interp. source of current time
  private:
  RTC* timekeeper;
  public:
  DatetimeSensor(RTC* pDevice);
  int readYear();
  int readMonth();
  int readDay();
  int readHour();
  int readMinute();
  int readSecond();
  String readDate();  // String -- current date
  String readTime();  // String -- current time
  String read();  // measurement in String format
}
;
class LuminositySensor {
  //interp. luminosity sensor
  private:
  VL6180x* device;
  public:
  LuminositySensor(VL6180x* pDevice);
  double readDouble(); // Double[>=0] -- current luminosity in lux
  String read();  // measurement in String format
}
;
class ProximitySensor {
  // interp. proximity sensor
  private:
  VL6180x* device;
  public:
  ProximitySensor(VL6180x* pDevice);
  int readInt();  // Int[0,255] -- current proximity in mm
  String read();  // measurement in String format
}
;
class TemperatureSensor {
  // interp. temperature sensor
  private:
  DHT11* device;
  public:
  TemperatureSensor(DHT11* pDevice);
  int readInt();  // Int[0,50] -- current temperature in 'C
  String read();  // measurement in String format
}
;
class HumiditySensor {
  // interp. humidity sensor
  private:
  DHT11* device;
  public:
  HumiditySensor(DHT11* pDevice);
  int readInt();  // Int[20,90] -- current humidity in %
  String read();  // measurement in String format
}
;
enum RemoteControllerButton {
  // intrp. a button of the remote controller
  PRESSED, // the previous button is still pressed
  POWER,
  MINUS,
  PLUS,
  RED,
  GREEN,
  BLUE,
  CROSS,
  SQUARE,
  TRIANGLE,
  HOME,
  UP,
  PAGEUP,
  LEFT,
  MIDDLE,
  RIGHT,
  END,
  DOWN,
  PAGEDOWN,
  X,
  Y,
  Z,
  OTHER // unrecognized signal
}
;
class InfraredSensor {
  // interp. infrared reciever
  private:
  IRrecv* device;
  decode_results recievedSignal;
  public:
  InfraredSensor(IRrecv* pDevice);
  void wait();    // wait for next signal
  boolean isSignalRecieved();  // check for a signal, true if found
  int readInt();  // unsigned long -- current signal value
  RemoteControllerButton readButton(); // RemoteControllerButton -- a button pressed
  String read();  // measurement in String format
}
;
enum SwitchCondition {
  // intrp. condition of a switch
  OFF,
  ON
}
;
class WaterControl {
  // interp. a controlled pomp
  private:
  int pin;
  SwitchCondition currentCondition;
  public:
  WaterControl(int pPin);
  void setOn();  // set 5v on the OutputPin
  void setOff();  // set 0v on the OutputPin
  String read();  // current condition in String format
}
;
class LightControl {
  // interp. a light switch
  private:
  int pin;
  SwitchCondition currentCondition;
  public:
  LightControl(int pPin);
  void setOn();  // set 5v on the OutputPin
  void setOff();  // set 0v on the OutputPin
  String read();  // current condition in String format
}
;


#endif //////// SENSOR_H_
