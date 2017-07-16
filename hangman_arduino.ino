#include <LiquidCrystal.h>
#include <Arduino.h>
#include "custom_characters.h"

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
static bool buttonPushed = false;
int button = 2; // input pin

const String answer = "applesauce";
String displayWord = "";
int errorCount = 0;
// char selectorCharacter = 97; // starts at a and will iterate in loop
String alphabet = "abcdefghijklmnopqrstuvwxyz";
int selectorCharacter = 0;
static bool playing = false;

volatile int voltage = 0;

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
  lcd.print(alphabet[selectorCharacter]);

  //comment this if you want to iterator
  /*while (Serial.available() < 1) {}
  selectorCharacter = Serial.readString()[0];*/
  if (buttonPushed && voltage > 920 && voltage < 924) { // left button

    voltage = 0;
    cli();
    if (selectorCharacter == 0)
      selectorCharacter = alphabet.length() - 1;
    else
      selectorCharacter--;
    
    lcd.setCursor(15, 0);
    lcd.print(alphabet[selectorCharacter]);
    buttonPushed = false;


    delay(500);
    sei();
    
  } else if(buttonPushed && voltage > 1000 && voltage < 1004) { //right button 

    voltage = 0;
    cli();

    if (selectorCharacter == (alphabet.length() - 1) )
      selectorCharacter = 0;
    else
      selectorCharacter++;
      
    lcd.setCursor(15, 0);
    lcd.print(alphabet[selectorCharacter]);
    buttonPushed = false;

    delay(500);
    sei();
  } else if (buttonPushed && voltage == 1023) {

    voltage = 0;
    cli();
    checkInput();
    alphabet.remove(selectorCharacter, 1);
    buttonPushed = false;
    if (selectorCharacter == alphabet.length()) {
      selectorCharacter = 0;
    }

    delay(500);
    sei();
  }

  lcd.setCursor(2, 1);
  lcd.print(displayWord);

  // Print error count to serial
  // Serial.print("Error count: ");
  // Serial.print(errorCount);
  // Serial.println("");

// iterates character
// uncomment this if you want iterator
/*
  if (selectorCharacter < alphabet.length() - 1)
    selectorCharacter++;
  else if (selectorCharacter == alphabet.length() - 1)
    selectorCharacter = 0; */

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
    while(!buttonPushed || (voltage < 1023)){ delay(500); /*wtf is this garbage ass shit*/ }
    voltage = 0;
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
  lcd.setCursor(5,0);
  lcd.print("Pick char:");


  //Add Kevins letterspacings to GUI on bottom row
  lcd.setCursor(2, 1);
  for (int i = 0; i < answer.length(); i++) 
    displayWord += "_";

  lcd.print(displayWord);
}

void buttonPush()
{
  buttonPushed = true;
  voltage = analogRead(1);
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
    if (alphabet[selectorCharacter] == answer[i]) {
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


