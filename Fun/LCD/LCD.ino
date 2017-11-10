#include <LiquidCrystal.h>

LiquidCrystal lcd(3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13);


byte customChar[8] = {
  0b00000,
  0b11111,
  0b00000,
  0b11111,
  0b00000,
  0b11111,
  0b00000,
  0b11111
};


void setup() {
  lcd.createChar(1,customChar);
  lcd.begin(16, 2);
  lcd.noAutoscroll();
}



void loop() {
  lcd.setCursor(0,1);
  lcd.write(1);
  lcd.write(1);

  
  
}
