#include <LiquidCrystal.h>
#include <Arduino.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
String alpha = "abcdefghijklmnopqrstuvwxyz";
static bool buttonPushed = false;
int button = 2;
volatile int lcd_key = 0;
int adc_key_in = 0;

#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

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

void initializeGraphics()
{
  lcd.clear();
  lcd.write(byte(0));
  lcd.write(byte(1));
  lcd.setCursor(0, 1);
  lcd.print("l");
  lcd.setCursor(3,0);
  lcd.print("Pick a char:");
  //Add Kevins letterspacings to GUI on bottom row
  //Integrate iteration through alphabet in top right character space
}

void buttonPush()
{
  buttonPushed = true;
}

void setup() 
{
	lcd.createChar(0, post);
  lcd.createChar(1, noose);
  lcd.createChar(2, head);
  lcd.createChar(3, body);
  lcd.createChar(4, right_hand);
  lcd.createChar(5, left_hand);
  lcd.createChar(6, right_foot);
  lcd.createChar(7, left_foot);
  lcd.begin(16, 2);
  //Include interrupts
}

void loop() 
{
  static bool playing = false;
  
  if (!playing)
  {
    lcd.clear();
    lcd.print("Press select to");
    lcd.setCursor(0,1);
    lcd.print("play!");
    delay(1000);
    buttonPushed = true;

    if (buttonPushed)
    {
    initializeGraphics();
    playing = true;
    buttonPushed = false;
    }
  }
  
  delay(1000);
  drawHangman(++errorCount);

  if (errorCount == 6) 
  {
    delay(1000);
    lcd.clear();
    playing == false;
    lcd.print("GAME OVER!");
    while (!buttonPushed){}
  }
}

void drawHangman(int errorCount) 
{

  if (errorCount == 1) {
    lcd.setCursor(1, 0);
    lcd.write(byte(2));
  } else if (errorCount > 1) {
    lcd.setCursor(1, 1);
    lcd.write(byte(errorCount + 1));
  }

}

