
#include <Arduino.h>
#include <Wire.h> 
#include "LiquidCrystal_I2C.h" // Подключение библиотеки
//#include <LiquidCrystal_PCF8574.h> // Подключение альтернативной библиотеки

#include "MainModel.h"


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
// #include "app_api.h" // only needed with flash breakpoints
// https://stackoverflow.com/questions/43499817/how-to-disable-then-re-enable-a-watchdog-interrupt-for-arduino
MainModel mainModel = MainModel();

void setup()
{
  Serial.begin(9600);
  // initialize GDB stub
  Serial.begin(9600);
  
  Serial.println("Setup END");
  // Serial.println("Setup !!!!!!!!!!!!!!!!!");
}


void loop() {
  // MainModel();
  mainModel.loopCallback();
  Serial.println("---------");
  delay(500);
}

// #include <functional-vlpp.h>

// using namespace vl;
// using Fn = Func<void()>;

// void myFunc(int a) {
  
// }

// const Func<void()>& f = Func<void()>(&setup);
// const Func<void()>& f2 = Curry<void(int)>(&myFunc)(2);

// using Fn = void (Foo::*)();
// class Foo {
//     void func2() {
//       Serial.println(a);
//         // cout<<"func_"<< a << endl;
//     }  
// public:
//     int a = 2;

//      Fn get_pointer() {
//       return &func2;
//       // return function_binding::Binding<void(Foo*)>::Binder(&Foo::func2, this);
      
//       // return function_binding::Binding<void(Foo*)>();
//         // return Binder<void(Foo*)>(&Foo::func2)(this);
//         // return std::bind(&Foo::func2, this);
//     }
// };

// void loop()
// {
//   Foo foo;
//   // Fn t = foo.get_pointer();
//   Fn a = &foo.get_pointer;
//   void (Foo::*)() t = foo.get_pointer();
//   t();
//   foo.a = 555;
//   t();
//   Serial.println("---------------");
//   delay(5000);
// }

