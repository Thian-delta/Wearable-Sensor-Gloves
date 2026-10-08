// Defining the pins used
const int thumbPressPin = 32;    // G32 Pin for thumb pressure sensor
const int indexPressPin = 35;  // G35 Pin for index pressure sensor
const int middlePressPin = 34;   // G34 Pin for middle pressure sensor
const int ringPressPin = 39; // SN (G39) Pin for ring pressure sensor
const int pinkyPressPin = 36; // SP (G36) Pin for pinky pressure sensor
const int thumbFlexPin = 4; // G4 Pin for thumb flex sensor
const int indexFlexPin = 12; // G12 Pin for index flex sensor
const int middleFlexPin = 14; // G14 Pin for middle flex sensor
const int ringFlexPin = 27; // G27 Pin for ring flex sensor
const int pinkyFlexPin = 33; // G33 Pin for pinky flex sensor

void setup() {
  Serial.begin(115200);
  
  // Wait until serial done initializing
  while (!Serial) delay(10);

  // Initialize each pin
  pinMode(thumbPressPin, INPUT);
  pinMode(indexPressPin, INPUT);
  pinMode(middlePressPin, INPUT);
  pinMode(ringPressPin, INPUT);
  pinMode(pinkyPressPin, INPUT);
  pinMode(thumbFlexPin, INPUT);
  pinMode(indexFlexPin, INPUT);
  pinMode(middleFlexPin, INPUT);
  pinMode(ringFlexPin, INPUT);
  pinMode(pinkyFlexPin, INPUT);
}

void loop() {
  int thumbPressValue = analogRead(thumbPressPin);
  int indexPressValue = analogRead(indexPressPin);
  int middlePressValue = analogRead(middlePressPin);
  int ringPressValue = analogRead(ringPressPin);
  int pinkyPressValue = analogRead(pinkyPressPin);
  int thumbFlexValue = analogRead(thumbFlexPin);
  int indexFlexValue = analogRead(indexFlexPin);
  int middleFlexValue = analogRead(middleFlexPin);
  int ringFlexValue = analogRead(ringFlexPin);
  int pinkyFlexValue = analogRead(pinkyFlexPin);

  // Read each flex and pressure sensor ports and read the resistance (in ohm)
  float thumbPressRes = 1000*(33 - 10*((analogRead(thumbPressPin)*3.3)/4095))/((analogRead(thumbPressPin)*3.3)/4095);
  float indexPressRes = 1000*(33 - 10*((analogRead(indexPressPin)*3.3)/4095))/((analogRead(indexPressPin)*3.3)/4095);
  float middlePressRes = 1000*(33 - 10*((analogRead(middlePressPin)*3.3)/4095))/((analogRead(middlePressPin)*3.3)/4095);
  float ringPressRes = 1000*(33 - 10*((analogRead(ringPressPin)*3.3)/4095))/((analogRead(ringPressPin)*3.3)/4095);
  float pinkyPressRes = 1000*(33 - 10*((analogRead(pinkyPressPin)*3.3)/4095))/((analogRead(pinkyPressPin)*3.3)/4095);
  float thumbFlexRes = 1000*(49.5 - 15*((analogRead(thumbFlexPin)*3.3)/4095))/((analogRead(thumbFlexPin)*3.3)/4095);
  float indexFlexRes = 1000*(49.5 - 15*((analogRead(indexFlexPin)*3.3)/4095))/((analogRead(indexFlexPin)*3.3)/4095);
  float middleFlexRes = 1000*(49.5 - 15*((analogRead(middleFlexPin)*3.3)/4095))/((analogRead(middleFlexPin)*3.3)/4095);
  float ringFlexRes = 1000*(49.5 - 15*((analogRead(ringFlexPin)*3.3)/4095))/((analogRead(ringFlexPin)*3.3)/4095);
  float pinkyFlexRes = 1000*(49.5 - 15*((analogRead(pinkyFlexPin)*3.3)/4095))/((analogRead(pinkyFlexPin)*3.3)/4095);

  Serial.print("Thumb Pressure Sensor Value: "); Serial.print(thumbPressValue); Serial.print(" , Thumb Pressure Sensor Resistance: "); Serial.println(thumbPressRes);
  Serial.print("Index Pressure Sensor Value: "); Serial.print(indexPressValue); Serial.print(" , Index Pressure Sensor Resistance: "); Serial.println(indexPressRes);
  Serial.print("Middle Pressure Sensor Value: "); Serial.print(middlePressValue); Serial.print(" , Middle Pressure Sensor Resistance: "); Serial.println(middlePressRes);
  Serial.print("Ring Pressure Sensor Value: "); Serial.print(ringPressValue); Serial.print(" , Ring Pressure Sensor Resistance: "); Serial.println(ringPressRes);
  Serial.print("Pinky Pressure Sensor Value: "); Serial.print(pinkyPressValue); Serial.print(" , Pinky Pressure Sensor Resistance: "); Serial.println(pinkyPressRes);
  Serial.print("Thumb Flex Sensor Value: "); Serial.print(thumbFlexValue); Serial.print(" , Thumb Flex Sensor Resistance: "); Serial.println(thumbFlexRes);
  Serial.print("Index Flex Sensor Value: "); Serial.print(indexFlexValue); Serial.print(" , Index Flex Sensor Resistance: "); Serial.println(indexFlexRes);
  Serial.print("Middle Flex Sensor Value: "); Serial.print(middleFlexValue); Serial.print(" , Middle Flex Sensor Resistance: "); Serial.println(middleFlexRes);
  Serial.print("Ring Flex Sensor Value: "); Serial.print(ringFlexValue); Serial.print(" , Ring Flex Sensor Resistance: "); Serial.println(ringFlexRes);
  Serial.print("Pinky Flex Sensor Value: "); Serial.print(pinkyFlexValue); Serial.print(" , Pinky Flex Sensor Resistance: "); Serial.println(pinkyFlexRes);
}
