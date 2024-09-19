void setup() {

  pinMode(2, INPUT);
  pinMode(3, INPUT);
  pinMode(4, INPUT);
  pinMode(5, INPUT);
  pinMode(6, INPUT);
  pinMode(7, INPUT);
  pinMode(8, INPUT);
  pinMode(9, INPUT);
  

  Serial.begin(9600);
}

void loop() {

  Serial.print("Tanque al 0%"); Serial.println(digitalRead(2));
  Serial.print("Tanque al 14%"); Serial.println(digitalRead(3));
  Serial.print("Tanque al 28%"); Serial.println(digitalRead(4));
  Serial.print("Tanque al 42%"); Serial.println(digitalRead(5));
  Serial.print("Tanque al 57%"); Serial.println(digitalRead(6));
  Serial.print("Tanque al 71%"); Serial.println(digitalRead(7));
  Serial.print("Tanque al 85%"); Serial.println(digitalRead(8));
  Serial.print("Tanque al 100%"); Serial.println(digitalRead(9));
  
  delay(1000);
}