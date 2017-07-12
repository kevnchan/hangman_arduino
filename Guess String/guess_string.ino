#include <LiquidCrystal.h>
#include <Arduino.h>

LiquidCrystal lcd(8, 9, 4, 5, 6, 7);
const String answer = "applesauce";
String displayWord = "";
int errorCount = 0;


void setup() {
	Serial.begin(9600);
	lcd.begin(16, 2);
	for (int i = 0; i < answer.length(); i++) 
		displayWord += "_";

	lcd.print(displayWord);
	
}

void loop() {

	while (Serial.available() < 1) {}
	char input = Serial.readString()[0];
	bool found = false;
	
	for (int i = 0; i < answer.length(); i++) {

		if (input == answer[i]) {
			displayWord[i] = answer[i];
			found = true;
		}
		
	}

	if (!found)
		errorCount++;

	lcd.clear();
	lcd.setCursor(0,0);
	lcd.print(displayWord);
	lcd.setCursor(0,1);
	lcd.print("Error count: ");
	lcd.print(errorCount);
}

