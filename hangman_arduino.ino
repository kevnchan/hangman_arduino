#include <LiquidCrystal.h>
#include <Arduino.h>
#include "custom_characters.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
static bool buttonPushed = false;
int button = 2; // input pin

const String answer = "applesauce";
String displayWord = "";
int errorCount = 0;
char selectorCharacter = 97;
static bool playing = false;


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

  lcd.setCursor(2, 1);
  for (int i = 0; i < answer.length(); i++) 
    displayWord += "_";

  lcd.print(displayWord);

  //Integrate iteration through alphabet in top right character space


}

void buttonPush()
{
  buttonPushed = true;
}

void setup() 
{
  Serial.begin(9600);
	lcd.createChar(0, post);
  lcd.createChar(1, noose);
  lcd.createChar(2, head);
  lcd.createChar(3, body);
  lcd.createChar(4, right_hand);
  lcd.createChar(5, left_hand);
  lcd.createChar(6, right_foot);
  lcd.createChar(7, left_foot);
  lcd.begin(16, 2);
  // Remember to include interrupts
}

void loop() 
{
  
  checkIfPlaying();

  bool found = false;

  lcd.setCursor(15, 0);
  lcd.print(selectorCharacter);

  for (int i = 0; i < answer.length(); i++) {

    if (selectorCharacter == answer[i]) {
      displayWord[i] = answer[i];
      found = true;
    }
    
  }

  if (!found) {
    errorCount++;
    drawHangman(errorCount);
  }

  // lcd.clear();
  lcd.setCursor(2, 1);
  lcd.print(displayWord);

  // Print error count to serial
  Serial.print("Error count: ");
  Serial.print(errorCount);

  if (selectorCharacter < 122)
    selectorCharacter++;

  delay(500);

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

void checkIfPlaying() {

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

}

