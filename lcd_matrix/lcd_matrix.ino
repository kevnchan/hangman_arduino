int numOfPins = 6;
volatile long millsec = 0;  

void setup() {
  Serial.begin(9600);

  // LED Matrix pins
  for (int i = 4; i < 7; i++)
    pinMode(i, OUTPUT);

  for (int i = 8; i < 11; i++)
    pinMode(i, OUTPUT);

  // trigger from Hangman Game
  pinMode(2, INPUT);

  // Use timer2 for custom millis
  cli();
  TCCR2A = 0;
  TCCR2B = 0;
  TCCR2B |= (1 << 2); // set frequency to clk/64 --> 250 kHz
  TCNT2 = 5;
  TIMSK2 |= (1 << 0); // enable overflow interrupts
  sei();
}

void loop() {
  if (digitalRead(2)) {
    printY();
  } else {
    clearLED();
  }
}

ISR(TIMER2_OVF_vect) { 
  millsec++;
  TCNT2 = 5;
}

void delayCustom(long duration) {
  long startTime = millsec;
  while (millsec - startTime < duration){}
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
  int startTime = millsec;
  int duration = 200;
  while(millsec - startTime < duration) {
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
  delayCustom(200);
  while(millsec - startTime < duration) {
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

