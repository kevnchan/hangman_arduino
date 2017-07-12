#include <LiquidCrystal.h>
#include <Arduino.h>
#include "custom_characters.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
static bool buttonPushed = false;
int button = 2; // input pin

const String answer = "applesauce";
String displayWord = "";
int errorCount = 0;
char selectorCharacter = 97; // starts at a and will iterate in loop
static bool playing = false;

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
  attachInterrupt(0, buttonPush, RISING);
  // Remember to include interrupts
}

void loop() 
{
  checkIfPlaying();
  // prints selector character in top right
  // uncomment this if you want iterator

  lcd.setCursor(15, 0);
  lcd.print(selectorCharacter);
  delay(1500);

  //comment this if you want to iterator
  /*while (Serial.available() < 1) {}
  selectorCharacter = Serial.readString()[0];*/
  if(buttonPushed)
  {
    checkInput();
    buttonPushed = false;
  }

  lcd.setCursor(2, 1);
  lcd.print(displayWord);

  // Print error count to serial
  Serial.print("Error count: ");
  Serial.print(errorCount);
  Serial.println("");

// iterates character
// uncomment this if you want iterator

  if (selectorCharacter < 122)
    selectorCharacter++;
  else if (selectorCharacter == 122)
    selectorCharacter = 97;

  checkIfGameOver();

}

// --------------------------------------------------------------------------


void checkIfPlaying() {

  if (!playing)
  {
    lcd.clear();
    lcd.print("Press select to");
    lcd.setCursor(0,1);
    lcd.print("play!");
    while(!buttonPushed){ delay(500); /*wtf is this garbage ass shit*/ }
    initializeGraphics();
    playing = true;
    buttonPushed = false;
  }

}

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
}

void buttonPush()
{
  buttonPushed = true;
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

void checkInput() {
  bool found = false;

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
}

void checkIfGameOver() {
  if (errorCount > 5) 
  {
    delay(1000);
    lcd.clear();
    playing == false;
    lcd.print("GAME OVER!");
    while (!buttonPushed){}
  } else if (displayWord == answer)
  {
    delay(1000);
    lcd.clear();
    playing == false;
    lcd.print("Lock unlocked!");
    while(!buttonPushed){}
  }
}


