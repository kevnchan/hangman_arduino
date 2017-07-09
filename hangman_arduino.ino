#include <LiquidCrystal.h>
#include <Arduino.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// change here

byte post[8] = {
  B00111,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
  B00100,
};

byte noose[8] = {
  B11100,
  B00100,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte head[8] = {
  B11100,
  B00100,
  B00100,
  B01110,
  B10001,
  B10001,
  B10001,
  B01110,
};

byte body[8] = {
  B00100,
  B00100,
  B00100,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte right_hand[8] = {
  B00100,
  B00101,
  B00110,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte left_hand[8] = {
  B00100,
  B10101,
  B01110,
  B00100,
  B00000,
  B00000,
  B00000,
  B00000,
};

byte right_foot[8] = {
  B00100,
  B10101,
  B01110,
  B00100,
  B00010,
  B00001,
  B00000,
  B00000,
};

byte left_foot[8] = {
  B00100,
  B10101,
  B01110,
  B00100,
  B01010,
  B10001,
  B00000,
  B00000,
};

int errorCount = 0;

void setup() {
	lcd.createChar(0, post);
  lcd.createChar(1, noose);
  lcd.createChar(2, head);
  lcd.createChar(3, body);
  lcd.createChar(4, right_hand);
  lcd.createChar(5, left_hand);
  lcd.createChar(6, right_foot);
  lcd.createChar(7, left_foot);
  lcd.begin(16, 2);
  lcd.clear();
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print("l");
}

void loop() {
  delay(1000);
  drawHangman(++errorCount);

  while (errorCount == 6) {}
}

void drawHangman(int errorCount) {

  if (errorCount == 1) {
    lcd.setCursor(1, 0);
    lcd.write(byte(2));
  } else if (errorCount > 1) {
    lcd.setCursor(1, 1);
    lcd.write(byte(errorCount + 1));
  }

}
