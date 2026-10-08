#include <Wire.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

// Define the initial status of all sensors
bool startGame = false;

// Define the pins used
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

// Define the MPU6050
Adafruit_MPU6050 mpu;

// Define the initial status of MPU Calibration
bool mpuCalibrated = false;

// Variables to store the baseline "default position" offsets
float accelX_offset = 0;
float accelY_offset = 0;
float accelZ_offset = 0;
float gyroX_offset = 0;
float gyroY_offset = 0;
float gyroZ_offset = 0;

// Define a time for calculating exactly the time elapsed between MPU6050 readings
unsigned long previousTime = 0;

// Variables of the "current position"
float currentXM = 0;
float currentYM = 0;
float currentZM = 0;
float currentXDeg = 0;
float currentYDeg = 0;
float currentZDeg = 0;

void setup() {
  Serial.begin(115200);
  
  // Wait until serial done initializing
  while (!Serial) delay(10);

  // Wait until mpu6050 connected properly
  if (!mpu.begin()) {
    Serial.println("Error: MPU6050 not found!");
    while (1) delay(10);
  }

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
  
  // Set SCL clock speed to 100,000 Hz
  Wire.setClock(100000); 

  // Configure sensor ranges
  mpu.setAccelerometerRange(MPU6050_RANGE_2_G); // reading up to ±2G of physical force
  mpu.setGyroRange(MPU6050_RANGE_500_DEG); // track rotation up to 500 degrees per second
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ); // cuts out high-frequency vibrations faster than 21 times per second
}

void loop() {
  // Reading commands from the game, all command strings can be replaced if required
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();

    if (command == "CALIBRATE" && !mpuCalibrated) {
      runMPUCalibration(); // Run the calibration when "CALIBRATE" command received
    }

    if (command == "START_GAME") {
      if (!mpuCalibrated) {
        Serial.println("{\"status\":\"error_calibration_not_started\"}");
      }
      else {
        startGame = true; // Starting reading values from sensors when "START_GAME" command received and MPU has been calibrated
        Serial.println("{\"status\":\"game_successfully_started\"}");
      }
    }
  }

  if (startGame) {
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

    // Find the actual angle and pressure value based on the resistance (in g and deg) (currently using "typical values", can be replaced with actual equation after testing)
    float thumbPressG = 1000*((1/thumbPressRes)-0.000053)/0.000474;
    float indexPressG = 1000*((1/indexPressRes)-0.000053)/0.000474; 
    float middlePressG = 1000*((1/middlePressRes)-0.000053)/0.000474;
    float ringPressG = 1000*((1/ringPressRes)-0.000053)/0.000474;
    float pinkyPressG = 1000*((1/pinkyPressRes)-0.000053)/0.000474;
    float thumbFlexDeg = (thumbFlexRes - 10000)/333.333;
    float indexFlexDeg = (indexFlexRes - 10000)/333.333;
    float middleFlexDeg = (middleFlexRes - 10000)/333.333;
    float ringFlexDeg = (ringFlexRes - 10000)/333.333;
    float pinkyFlexDeg = (pinkyFlexRes - 10000)/333.333;

   // Print in JSON format
    Serial.print("{\"thumbPress\":"); Serial.print(thumbPressG);
    Serial.print(",\"indexPress\":"); Serial.print(indexPressG);
    Serial.print(",\"middlePress\":"); Serial.print(middlePressG);
    Serial.print(",\"ringPress\":"); Serial.print(ringPressG);
    Serial.print(",\"pinkyPress\":"); Serial.print(pinkyPressG);
    Serial.print(",\"thumbFlex\":"); Serial.print(thumbFlexDeg);
    Serial.print(",\"indexFlex\":"); Serial.print(indexFlexDeg);
    Serial.print(",\"middleFlex\":"); Serial.print(middleFlexDeg);
    Serial.print(",\"ringFlex\":"); Serial.print(ringFlexDeg);
    Serial.print(",\"pinkyFlex\":"); Serial.print(pinkyFlexDeg);

    if (mpuCalibrated) {
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);

    // Subtract the baseline offset from the current raw reading
    float correctedAccelX = a.acceleration.x - accelX_offset;
    float correctedAccelY = a.acceleration.y - accelY_offset;
    float correctedAccelZ = (a.acceleration.z - accelZ_offset) + 9.81; // take account of gravitation
    // and convert from Radians to Degrees per Second (* 57.2958)
    float correctedGyroX = (g.gyro.x - gyroX_offset) * 57.2958;
    float correctedGyroY = (g.gyro.y - gyroY_offset) * 57.2958;
    float correctedGyroZ = (g.gyro.z - gyroZ_offset) * 57.2958;

    //Find the actual current position
    unsigned long currentTime = millis(); // Obtain the milliseconds of current time
    float deltaTime = (currentTime - previousTime) / 1000.0; // Convert to seconds
    currentXM += correctedAccelX * deltaTime;
    currentYM += correctedAccelY * deltaTime;
    currentZM += correctedAccelZ *deltaTime;
    currentXDeg += correctedGyroX * deltaTime;
    currentYDeg += correctedGyroY * deltaTime;
    currentZDeg += correctedGyroZ * deltaTime;

    previousTime = currentTime;

    // Print in JSON format
    Serial.print(",\"xM\":"); Serial.print(currentXM);
    Serial.print(",\"yM\":"); Serial.print(currentYM);
    Serial.print(",\"zM\":"); Serial.print(currentZM);
    Serial.print(",\"xDeg\":"); Serial.print(currentXDeg);
    Serial.print(",\"yDeg\":"); Serial.print(currentYDeg);
    Serial.print(",\"zDeg\":"); Serial.print(currentZDeg);
    }
    Serial.println("}");
  }
}

void runMPUCalibration() {
  // Print in JSON format
  Serial.println("{\"status\":\"calibrating_do_not_move_sensor\"}");
  delay(500); // Allow sensor to settle

  int samples = 200;
  float aX = 0, aY = 0, aZ = 0;
  float gX = 0, gY = 0, gZ = 0;
  sensors_event_t a, g, temp;

  // Take 200 readings of the sensor while it is stationary
  for (int i = 0; i < samples; i++) {
    mpu.getEvent(&a, &g, &temp);
    aX += a.acceleration.x;
    aY += a.acceleration.y;
    aZ += a.acceleration.z;
    gX += g.gyro.x;
    gY += g.gyro.y;
    gZ += g.gyro.z;
    delay(3); // Tiny delay between samples
  }

  // Calculate the average background noise/drift value
  accelX_offset = aX / samples;
  accelY_offset = aY / samples;
  accelZ_offset = aZ / samples;
  gyroX_offset = gX / samples;
  gyroY_offset = gY / samples;
  gyroZ_offset = gZ / samples;

  delay(1000);
  mpuCalibrated = true;
  // Print in JSON format
  Serial.println("{\"status\":\"calibration_successful\"}");
}