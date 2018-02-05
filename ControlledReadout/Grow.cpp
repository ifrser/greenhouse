// вывод на экран показаний датчиков
#include "Sensor.h"

#define VL6180X_ADDRESS 0x29  // адрес датчика приближения-освещенности по IIC


//////// устройства и способы подключения ////////
LiquidCrystal lcd(2, 3, 4, 5, 6, 7);  // LCD-экран (номера контактов соответствуют каналам RS, E, DB4, DB5, DB6, DB7)
RTC timekeeper;                       // автономные часы
VL6180x device1(VL6180X_ADDRESS);     // датчик приближения и освещенности
DHT11 device2(9);                     // датчик температуры и влажности (номер контакта)
IRrecv irreciever(10);                // ИК-приемник (номер контакта)
int relay1 = 11;                      // управляемый контакт
int relay2 = 12;                      // управляемый контакт


//////// программные объекты на основе устройств ////////
ScreenOutput screen(&lcd);
DatetimeSensor datetime(&timekeeper);
LuminositySensor luminosity(&device1);
ProximitySensor proximity(&device1);
TemperatureSensor temperature(&device2);
HumiditySensor humidity(&device2);
InfraredSensor control(&irreciever);
WaterControl pomp(relay1);
LightControl light(relay2);

//////// логическая организация программы ////////
enum InformationType {
  // interp. type of information
  NONE,        // show nothing
  DATE,        // show today date
  TIME,        // show current time
  LUMINOSITY,  // show luminosity
  PROXIMITY,   // show proximity distance
  TEMPERATURE, // show temperature
  HUMIDITY,    // show humidity
  IRCODE,      // show infrared code recieved
  SWITCHCONTROL   // show current switch information
}
;
struct ScreenMode{
  // interp. type of information showed on the screen
  InformationType line[2] = {IRCODE, TIME};
}
;
ScreenMode screenMode;
int refreshDelay = 1000;
int localScreenTime;

InformationType getNextInformationType(InformationType pInformationType) {
  // return next type of information in a cycle
  switch (pInformationType) {
    case NONE: return DATE;
    case DATE: return TIME;
    case TIME: return LUMINOSITY;
    case LUMINOSITY: return PROXIMITY;
    case PROXIMITY: return TEMPERATURE;
    case TEMPERATURE: return HUMIDITY;
    case HUMIDITY: return DATE;
    case IRCODE: return DATE;
    case SWITCHCONTROL: return DATE;
    default: // not yet defined
      return NONE;
  }
}
;
String getInformation(InformationType pInformationType) {
  // read information from sensor
  switch (pInformationType) {
    case NONE: return "";
    case DATE: return datetime.readDate();
    case TIME: return datetime.readTime();
    case LUMINOSITY: return "Lum: " + luminosity.read() + "Lx";
    case PROXIMITY: return "d:   " + proximity.read() + "mm";
    case TEMPERATURE: return "T:   " + temperature.read() + "'C";
    case HUMIDITY: return "hmd: " + humidity.read() + "%";
    case IRCODE: return "btn: " + control.read();
    case SWITCHCONTROL: return "" + pomp.read() + "; "+ light.read(); //#!!!
    default: // not yet defined
      return "no information";
  }
}
;
void refreshScreen(ScreenMode pScreenMode) {
  // refresh information on the screen
  screen.printLines(
    getInformation(pScreenMode.line[0]),
    getInformation(pScreenMode.line[1])
  )
  ;
}
;

//////// последовательность действий ////////
void setup() {
  setupLiquidCrystal(&lcd);
  screen.printLine1("...");
  
  setupRTC(&timekeeper);
  setupVL6180xIIC(&device1);
  setupDHT11(&device2);
  setupIR(&irreciever);
  pinMode(relay1, OUTPUT); // setupWaterControl
  pinMode(relay2, OUTPUT); // setupLightControl
  screen.printLine2("hi!");
}

void loop() {

  if (control.isSignalRecieved()) {
    if (control.readButton() != PRESSED) {
      switch(control.readButton()) {
        case RED: // reset to date and time
          screenMode.line[0] = IRCODE;
          screenMode.line[1] = TIME;
          break;
        case GREEN: // change the second line
          screenMode.line[1] = getNextInformationType(screenMode.line[1]);
          break;
        case BLUE: // move the second line up
          screenMode.line[0] = screenMode.line[1];
          screenMode.line[1] = getNextInformationType(screenMode.line[1]);
          break;
        case PLUS: // set light on
          light.setOn();
          screenMode.line[1] = SWITCHCONTROL;
          break;
        case MINUS: // set light off
          light.setOff();
          screenMode.line[1] = SWITCHCONTROL;
          break;
        case POWER: // activate pomp for 2 seconds
          pomp.setOn();
          screenMode.line[1] = SWITCHCONTROL;
          delay(1000);
          pomp.setOff();
          screenMode.line[1] = SWITCHCONTROL;
          //#!!! do time control
          break;
        default: // no direct changes to the screen
          break;
      }
      refreshScreen(screenMode);
      localScreenTime = millis();
    }

    control.wait();
  }

  //#!!! it should be like "screen.refreshScreen(screenMode);"
  if (millis() - localScreenTime >= refreshDelay) {
    refreshScreen(screenMode);
    localScreenTime = millis();
  }
}

