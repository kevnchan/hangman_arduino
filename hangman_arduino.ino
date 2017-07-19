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
volatile long millsec = 0;
volatile bool secPassed = 0;
int time = 61;

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

 
  
  pinMode(3, OUTPUT); // for printing Y
  digitalWrite(3, LOW);

  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= (1 << 2); // set frequency to clk/64 --> 250 kHz
  TCNT2 = 5;
  TIMSK2 |= (1 << 0); // enable overflow interrupts
  sei();
}

void loop() 
{

  checkIfPlaying();
  // prints selector character in top right

  //timer area
  if (secPassed && time > 0) {
    time--;
    printTime();    
    secPassed = false;
  }

  if (time == 0)
    errorCount = 6; //ends game

  //timer area

  lcd.setCursor(15, 0);
  lcd.print(alphabet[selectorCharacter]);

  if (buttonPushed && voltage > 920 && voltage < 924) { // left button

    cli();
    voltage = 0;
    if (selectorCharacter == 0)
      selectorCharacter = alphabet.length() - 1;
    else
      selectorCharacter--;
    
    lcd.setCursor(15, 0);
    lcd.print(alphabet[selectorCharacter]);
    buttonPushed = false;
    sei();
    delayCustom(500);
    
  } else if(buttonPushed && voltage > 1000 && voltage < 1004) { //right button 
    cli();
    voltage = 0;

    if (selectorCharacter == (alphabet.length() - 1) )
      selectorCharacter = 0;
    else
      selectorCharacter++;
      
    lcd.setCursor(15, 0);
    lcd.print(alphabet[selectorCharacter]);
    buttonPushed = false;
    sei();
    delayCustom(500);
  } else if (buttonPushed && voltage == 1023) {
    cli();
    voltage = 0;
 
    checkInput();
    alphabet.remove(selectorCharacter, 1);
    buttonPushed = false;
    if (selectorCharacter == alphabet.length()) {
      selectorCharacter = 0;
    }
    sei();
    delayCustom(500);
  }

  lcd.setCursor(2, 1);
  lcd.print(displayWord);

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
    while(!buttonPushed || (voltage < 1023)){ delayCustom(500); /*wtf is this garbage ass shit*/ }
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
  lcd.setCursor(8,0);
  lcd.print("Select:");


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

      digitalWrite(3, HIGH);
      
    }
  }

  if (!found) {
    errorCount++;
    drawHangman(errorCount);

    digitalWrite(3, LOW);
  }
}

void checkIfGameOver() {
  if (errorCount > 5) 
  {
    delayCustom(1000);
    lcd.clear();
    playing == false;
    lcd.print("Locked Out!");
    while (!buttonPushed){}
  } else if (displayWord == answer)
  {
    delayCustom(1000);
    lcd.clear();
    playing == false;
    lcd.print("Lock unlocked!");
    while(!buttonPushed){}
  }
}

ISR(TIMER2_OVF_vect) { 
  millsec++;
  if (millsec % 1000 == 0)
    secPassed = true;
  TCNT2 = 5;
}

void delayCustom(long duration) {
  long startTime = millsec;
  while (millsec - startTime < duration){}
}

void printTime() {
  int seconds = time % 60;
  int minutes = (time - seconds) / 60;
  lcd.setCursor(3, 0);
  lcd.print(minutes);
  lcd.print(":");
  if (seconds < 10)
    lcd.print("0");
  lcd.print(seconds);
}


