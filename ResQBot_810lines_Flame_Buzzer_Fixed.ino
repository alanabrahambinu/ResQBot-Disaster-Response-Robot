#include <DHT.h>
#include <Servo.h>
#include <Wire.h>
#include <U8g2lib.h>

// =====================================================
// PIN DEFINITIONS
// =====================================================

#define DHT_PIN       2
#define SERVO_PIN     3

#define TRIG_PIN      4
#define ECHO_PIN      5

#define FLAME_PIN     6
#define BUZZER_PIN    7

#define RED_LED       8
#define GREEN_LED     9

#define SWITCH_PIN    10

#define MQ2_PIN       A0


// =====================================================
// DHT11
// =====================================================

#define DHT_TYPE DHT11

DHT dht(DHT_PIN, DHT_TYPE);


// =====================================================
// SERVO
// =====================================================

Servo emergencyServo;

#define SERVO_CLOSED 0
#define SERVO_OPEN   90


// =====================================================
// ULTRASONIC
// =====================================================

#define ECHO_TIMEOUT 60000

const int obstacleThreshold = 100;


// =====================================================
// MQ-2
// =====================================================

// Prototype threshold only.
// This is NOT ppm.
const int gasThreshold = 400;


// =====================================================
// OLED
// =====================================================

// IMPORTANT:
// _1_ means PAGE BUFFER mode.
// Therefore the code uses firstPage()/nextPage().
U8G2_SSD1306_128X64_NONAME_1_HW_I2C display(
  U8G2_R0,
  U8X8_PIN_NONE
);


// =====================================================
// VARIABLES
// =====================================================

float temperature = 0;
float humidity = 0;

int gasValue = 0;

long distance = -1;

bool flameDetected = false;
bool gasDanger = false;
bool obstacleDanger = false;

bool alarm = false;

bool emergencySwitch = false;


// =====================================================
// SETUP
// =====================================================

void setup() {

  Serial.begin(9600);

  // -------------------------
  // DHT11
  // -------------------------
  dht.begin();


  // -------------------------
  // HC-SR04
  // -------------------------
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);


  // -------------------------
  // Flame sensor
  // -------------------------
  pinMode(FLAME_PIN, INPUT);


  // -------------------------
  // Buzzer
  // -------------------------
  pinMode(BUZZER_PIN, OUTPUT);
  noTone(BUZZER_PIN);


  // -------------------------
  // LEDs
  // -------------------------
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);

  digitalWrite(RED_LED, LOW);
  digitalWrite(GREEN_LED, LOW);


  // -------------------------
  // Emergency switch
  // -------------------------
  // Switch:
  // D10 ---- switch ---- GND
  //
  // OFF = HIGH
  // ON  = LOW
  pinMode(SWITCH_PIN, INPUT_PULLUP);


  // -------------------------
  // Servo
  // -------------------------
  emergencyServo.attach(SERVO_PIN);
  emergencyServo.write(SERVO_CLOSED);


  // -------------------------
  // OLED
  // -------------------------
  display.begin();


  // -------------------------
  // OLED startup screen
  // -------------------------

  display.firstPage();

  do {

    display.setFont(u8g2_font_6x10_tf);

    display.drawStr(35, 25, "ResQBot");
    display.drawStr(18, 42, "System Starting");

  } while (display.nextPage());


  delay(2000);


  Serial.println("================================");
  Serial.println("       ResQBot Starting");
  Serial.println("================================");
}


// =====================================================
// ULTRASONIC SINGLE READING
// =====================================================

long readUltrasonic() {

  digitalWrite(TRIG_PIN, LOW);
  delayMicroseconds(5);

  digitalWrite(TRIG_PIN, HIGH);
  delayMicroseconds(10);

  digitalWrite(TRIG_PIN, LOW);


  long duration = pulseIn(
    ECHO_PIN,
    HIGH,
    ECHO_TIMEOUT
  );


  // No echo
  if (duration == 0) {
    return -1;
  }


  long measuredDistance = duration * 0.0343 / 2;


  // Invalid reading
  if (measuredDistance < 2 ||
      measuredDistance > 450) {

    return -1;
  }


  return measuredDistance;
}


// =====================================================
// AVERAGE ULTRASONIC READING
// =====================================================

long getDistance() {

  long total = 0;

  int validReadings = 0;


  for (int i = 0; i < 3; i++) {

    long d = readUltrasonic();


    if (d > 0) {

      total += d;
      validReadings++;
    }


    delay(60);
  }


  if (validReadings == 0) {
    return -1;
  }


  return total / validReadings;
}


// =====================================================
// UPDATE LEDs
// =====================================================

void updateLEDs() {

  if (alarm) {

    // -------------------------
    // DANGER
    // -------------------------
    digitalWrite(RED_LED, HIGH);
    digitalWrite(GREEN_LED, LOW);

  }

  else {

    // -------------------------
    // SAFE
    // -------------------------
    digitalWrite(RED_LED, LOW);
    digitalWrite(GREEN_LED, HIGH);
  }
}


// =====================================================
// UPDATE SERVO
// =====================================================

void updateServo() {

  if (emergencySwitch) {

    // Emergency kit OPEN
    emergencyServo.write(SERVO_OPEN);

  }

  else {

    // Emergency kit CLOSED
    emergencyServo.write(SERVO_CLOSED);
  }
}


// =====================================================
// UPDATE BUZZER
// =====================================================

void updateBuzzer() {

  if (alarm) {

    // Buzzer ON
    // tone() is used because the standalone
    // flame + buzzer test was confirmed working.
    tone(BUZZER_PIN, 2000);

  }

  else {

    // Buzzer OFF
    noTone(BUZZER_PIN);
  }
}


// =====================================================
// OLED DISPLAY
// =====================================================

void updateOLED() {

  // ---------------------------------------------------
  // PAGE 1
  // Sensor values
  // ---------------------------------------------------

  display.firstPage();

  do {

    display.setFont(u8g2_font_6x10_tf);


    // TITLE
    display.drawStr(0, 9, "ResQBot - SENSORS");


    // -------------------------
    // Temperature
    // -------------------------

    display.setCursor(0, 20);
    display.print("Temp: ");

    if (isnan(temperature)) {

      display.print("--");

    }

    else {

      display.print(temperature, 1);
      display.print(" C");
    }


    // -------------------------
    // Humidity
    // -------------------------

    display.setCursor(0, 30);
    display.print("Hum : ");

    if (isnan(humidity)) {

      display.print("--");

    }

    else {

      display.print(humidity, 0);
      display.print(" %");
    }


    // -------------------------
    // Gas
    // -------------------------

    display.setCursor(0, 40);
    display.print("Gas : ");
    display.print(gasValue);


    // -------------------------
    // Flame
    // -------------------------

    display.setCursor(0, 50);
    display.print("Fire: ");

    if (flameDetected) {

      display.print("DANGER");

    }

    else {

      display.print("SAFE");
    }


    // -------------------------
    // Distance
    // -------------------------

    display.setCursor(0, 60);
    display.print("Dist: ");

    if (distance < 0) {

      display.print("--");

    }

    else {

      display.print(distance);
      display.print(" cm");
    }


  } while (display.nextPage());


  // ---------------------------------------------------
  // Wait before showing second page
  // ---------------------------------------------------

  delay(1500);


  // ---------------------------------------------------
  // PAGE 2
  // System status
  // ---------------------------------------------------

  display.firstPage();

  do {

    display.setFont(u8g2_font_6x10_tf);


    // TITLE
    display.drawStr(0, 9, "ResQBot - STATUS");


    // -------------------------
    // Fire
    // -------------------------

    display.setCursor(0, 20);
    display.print("Fire     : ");

    if (flameDetected) {

      display.print("DANGER");

    }

    else {

      display.print("SAFE");
    }


    // -------------------------
    // Gas
    // -------------------------

    display.setCursor(0, 30);
    display.print("Gas      : ");

    if (gasDanger) {

      display.print("DANGER");

    }

    else {

      display.print("SAFE");
    }


    // -------------------------
    // Obstacle
    // -------------------------

    display.setCursor(0, 40);
    display.print("Obstacle : ");

    if (obstacleDanger) {

      display.print("DANGER");

    }

    else {

      display.print("SAFE");
    }


    // -------------------------
    // Emergency Kit
    // -------------------------

    display.setCursor(0, 50);
    display.print("Kit      : ");

    if (emergencySwitch) {

      display.print("OPEN");

    }

    else {

      display.print("CLOSED");
    }


    // -------------------------
    // Overall Alarm
    // -------------------------

    display.setCursor(0, 60);
    display.print("ALARM    : ");

    if (alarm) {

      display.print("ON");

    }

    else {

      display.print("OFF");
    }


  } while (display.nextPage());
}


// =====================================================
// SERIAL MONITOR
// =====================================================

void printSerialData() {

  Serial.println("--------------------------------");

  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.println(" C");


  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println(" %");


  Serial.print("MQ-2 Gas: ");
  Serial.println(gasValue);


  Serial.print("Flame: ");

  if (flameDetected) {

    Serial.println("DETECTED");

  }

  else {

    Serial.println("NO");
  }


  Serial.print("Distance: ");

  if (distance < 0) {

    Serial.println("No reading");

  }

  else {

    Serial.print(distance);
    Serial.println(" cm");
  }


  Serial.print("Emergency Switch: ");

  if (emergencySwitch) {

    Serial.println("ON");

  }

  else {

    Serial.println("OFF");
  }


  Serial.print("Alarm: ");

  if (alarm) {

    Serial.println("ON");

  }

  else {

    Serial.println("OFF");
  }
}


// =====================================================
// MAIN LOOP
// =====================================================

void loop() {

  // ===================================================
  // READ DHT11
  // ===================================================

  temperature = dht.readTemperature();
  humidity = dht.readHumidity();


  // ===================================================
  // READ MQ-2
  // ===================================================

  gasValue = analogRead(MQ2_PIN);


  // ===================================================
  // READ FLAME SENSOR
  // ===================================================
  // Common flame modules:
  // LOW  = flame detected
  // HIGH = no flame

  if (digitalRead(FLAME_PIN) == LOW) {

    flameDetected = true;

  }

  else {

    flameDetected = false;
  }


  // ===================================================
  // READ ULTRASONIC
  // ===================================================

  distance = getDistance();


  // ===================================================
  // OBSTACLE CONDITION
  // ===================================================

  if (distance > 0 &&
      distance <= obstacleThreshold) {

    obstacleDanger = true;

  }

  else {

    obstacleDanger = false;
  }


  // ===================================================
  // GAS CONDITION
  // ===================================================

  if (gasValue >= gasThreshold) {

    gasDanger = true;

  }

  else {

    gasDanger = false;
  }


  // ===================================================
  // EMERGENCY SWITCH
  // ===================================================
  // INPUT_PULLUP:
  //
  // Switch OFF = HIGH
  // Switch ON  = LOW

  if (digitalRead(SWITCH_PIN) == LOW) {

    emergencySwitch = true;

  }

  else {

    emergencySwitch = false;
  }


  // ===================================================
  // ALARM CONDITION
  // ===================================================
  //
  // Buzzer ONLY for:
  // 1. Flame
  // 2. Gas
  // 3. Obstacle
  //
  // Emergency switch does NOT activate buzzer.

  alarm =
    flameDetected ||
    gasDanger ||
    obstacleDanger;


  // ===================================================
  // SERVO
  // ===================================================

  updateServo();


  // ===================================================
  // BUZZER
  // ===================================================

  updateBuzzer();


  // ===================================================
  // LEDs
  // ===================================================

  updateLEDs();


  // ===================================================
  // OLED
  // ===================================================

  updateOLED();


  // ===================================================
  // SERIAL MONITOR
  // ===================================================

  printSerialData();


  // ===================================================
  // SMALL LOOP DELAY
  // ===================================================

  delay(500);
}