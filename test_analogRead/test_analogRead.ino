volatile bool buttonPushed = false;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  attachInterrupt(0, buttonPressed, RISING);
}

void loop() {
  // put your main code here, to run repeatedly:
  while (!buttonPushed){}
  if (buttonPushed) {
    Serial.println( analogRead(1) );
    buttonPushed = false;
  }
  
}

void buttonPressed() {
  buttonPushed = true;
}


