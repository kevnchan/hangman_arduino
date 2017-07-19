int numOfPins = 6;
int duration = 1000;

void setup() {
  Serial.begin(9600);

  for (int i = 4; i < 7; i++)
    pinMode(i, OUTPUT);

  for (int i = 8; i < 11; i++)
    pinMode(i, OUTPUT);

  pinMode(2, INPUT);
}

void loop() {
  if (digitalRead(2)) {
    printY();
  } else {
    clearLED();
  }

  Serial.print("Pin 2: ");
  Serial.println(digitalRead(2));
}

void clearLED() {
  PORTD = 0;
  PORTB = 0;
}

void alternateLights() {
  int gap = 5;
  delay(gap);
}

void printY() {
  int startTime = millis();
  while(millis() - startTime < duration) {
    clearLED();
    digitalWrite(10, HIGH);
    digitalWrite(5, HIGH);
    alternateLights();
  
    clearLED();
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    alternateLights();
  }
  clearLED();
  
}

void printN() {
  
  int startTime = millis();
  while(millis() - startTime < duration) {
    clearLED();
    digitalWrite(10, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(8, HIGH);
    digitalWrite(5, HIGH);
    alternateLights();
  
    clearLED();
    digitalWrite(10, HIGH);
    digitalWrite(6, HIGH);
    digitalWrite(4, HIGH);
    alternateLights();
  }
  clearLED();
}

