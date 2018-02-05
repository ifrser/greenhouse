/****************************************************************************/
//  Function: Header file for #!!!
//  Hardware: #!!!
//  Arduino IDE: Arduino-1.8.5
//  Author:  SI
//  Date:    Jan 2018
//  Version: v0.1
//  by Noosphere
/****************************************************************************/
#ifndef GROW_H_
#define GROW_H_

// применение схемы ухода за растениями согласно параметрам агротехнологии
#include "Sensor.h"
//
//#define VL6180X_ADDRESS 0x29  // адрес датчика приближения-освещенности по IIC
//
//
////////// устройства и способы подключения ////////
//LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  // LCD-экран (номера контактов соответствуют каналам RS, E, DB4, DB5, DB6, DB7)
//RTC timekeeper;                       // автономные часы
//VL6180x device1(VL6180X_ADDRESS);     // датчик приближения и освещенности
//DHT11 device2(9);                     // датчик температуры и влажности (номер контакта)
//IRrecv irreciever(10);                // ИК-приемник (номер контакта)
//int relay1 = 11;                      // управляемый контакт
//int relay2 = 12;                      // управляемый контакт
//
//
////////// программные объекты на основе устройств ////////
//ScreenOutput screen(&lcd);
//DatetimeSensor datetime(&timekeeper);
//LuminositySensor luminosity(&device1);
//ProximitySensor proximity(&device1);
//TemperatureSensor temperature(&device2);
//HumiditySensor humidity(&device2);
//InfraredSensor control(&irreciever);
//WaterControl pomp(relay1);
//LightControl light(relay2);
//
//////// логическая организация программы ////////
enum InformationType {
  // interp. type of information to show
  NONE,        // show nothing
  DATE,        // show today date
  TIME,        // show current time
  LUMINOSITY,  // show luminosity
  PROXIMITY,   // show proximity
  TEMPERATURE, // show temperature
  HUMIDITY,    // show humidity
  IRCODE,      // show infrared code recieved
  SWITCHCONTROL   // show current switch information
}
;
struct ScreenMode {
  // interp. information types showed on the screen
  InformationType line[2] = {IRCODE, TIME};
}
;
struct ControlCondition {
  // interp. condition of controllable devices
  SwitchCondition lightCondition;
  SwitchCondition pompCondtition;
  SwitchCondition blowerCondition;
}
;
struct DateTime {
  // interp. a date with time
  int year;
  int month;
  int day;
  int hour;
  int minute;
  int second;
}
;
struct SensorCondition {
  // interp. set of sensor data read //#!!! not precise types
  DateTime datetime;
  int luminosity;
  int moist;
  int humidity;
  int temperature;
  int waterLevel;
}
;
struct Agrotechnology {
  // interp. a set of rules to grow salad
  String label;
  int lightHoursPerDay;
  int lightLuminosity;
  int moistMin;
  int waterSupplyPerPlant;
  int humidityMin;
  int humidityMax;
}
;
class CommonPlace {
  //#!!! rename this class, it controls the program flow
  private:
  ScreenMode screenMode;
  int refreshDelay = 1000;
  int localScreenTime;
  ControlCondition controls;
  SensorCondtition readout;
  Agrotechnonlogy growScheme;
  public:
  CommonPlace(); //#!!! stub
  ControlCondition setControls(SensorCondtition currentCondition, Agrotechnology growScheme);
  SensorCondition readAllSensor(); // read sensor readouts available
  InformationType getNextInformationType(InformationType pInformationType);  // return next information type to show (in a cycle)
  String getInformation(InformationType pInformationType);  // read information from a sensor
  void refreshScreen(ScreenMode pScreenMode);  // refresh information on the screen
}
;


#endif //////// GROW_H_
