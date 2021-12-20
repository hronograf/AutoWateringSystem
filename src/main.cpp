
#include <Arduino.h>
#include "Controller.h"
// #include <Wire.h> 
// #include "LiquidCrystal_I2C.h" // Подключение библиотеки
//#include <LiquidCrystal_PCF8574.h> // Подключение альтернативной библиотеки

// MainModel mainModel = MainModel();

Controller controller = Controller();
void setup() {
  // Serial.begin(9600);
  // // initialize GDB stub
  // Serial.begin(9600);
  
  // Serial.println("Setup END");
  // Serial.println("Setup !!!!!!!!!!!!!!!!!");
  controller.setup();
}


void loop() {
  controller.loop();
  // MainModel();
  // mainModel.loopCallback();
  // Serial.println("---------");
  // delay(500);
}


// LiquidCrystal_I2C lcd = LiquidCrystal_I2C(0x27,20,4); // Указываем I2C адрес (наиболее распространенное значение), а также параметры экрана (в случае LCD 1602 - 2 строки по 16 символов в каждой 
// //LiquidCrystal_PCF8574 lcd(0x27); // Вариант для библиотеки PCF8574 
// MainModel main_model = MainModel();


// void setup() {
//   Serial.begin(9600);
//   lcd.init();                      // Инициализация дисплея  
//   lcd.backlight();                 // Подключение подсветки
// }

// void loop() {
//   Serial.println(main_model.a);

//   for (int i = 0; i < 4; i++) {
//     lcd.setCursor(0, i);
//     lcd.print(main_model.a);
//     delay(1000);
//   }
//   lcd.clear();
//   delay(1000);
// }

// class MainModel { // класс со всеми состояниями

// }


/////////////////////////////////////////////////////////////////////////////////////////////////////////

// void loop() {
//   Serial.println("Start");
//   // устанавливаем курсор в позицию (0,0):
//   lcd.setCursor(0, 0);
//   // выводим цифры от 0 до 9:
//   char chars[] = {'a', 'b', 'c', 'd', 'e', 'f'};
//   for (int thisChar = 0; thisChar < 10; thisChar++) {
//     lcd.print(chars[thisChar]);
//     delay(500);
//   }

//   Serial.println("medium");
//   // устанавливаем курсор в (16,1):
//   lcd.setCursor(20, 1);
//   // включаем автоматическую прокрутку
//   lcd.autoscroll();
//   // печатаем от 0 до 9:
//   for (int thisChar = 0; thisChar < 10; thisChar++) {
//     lcd.print(thisChar);
//     delay(500);
//   }
//   // выключаем автоматическую прокрутку
//   lcd.noAutoscroll();
//   delay(2000);

//   // очищаем экран для следующей итерации
//   lcd.clear();
//   Serial.println("end");
//   Serial.println("--------");
// }

//////////////////////////////////////////////////////////////////////

