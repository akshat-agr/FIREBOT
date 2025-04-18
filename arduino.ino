#include <LiquidCrystal.h>

// LCD Pins (RS, E, D4, D5, D6, D7)
LiquidCrystal lcd(12, 13, A1, A2, A3, A4);

// Sensor Pins
#define FLAME_SENSOR 2   // Digital Pin for Flame Sensor
#define GAS_SENSOR A0    // Analog Pin for Gas Sensor
#define ULTRASONIC_TRIG 6
#define ULTRASONIC_ECHO 7
#define LED_INDICATOR A5 // LED on A5
#define PUMP_MOTOR 3     // Pump motor on pin 3

// Line Following Sensors
#define IR_SENSOR_LEFT 8
#define IR_SENSOR_RIGHT 9

// Motor Driver Pins (L298N)
#define MOTOR_IN1 5
#define MOTOR_IN2 11
#define MOTOR_IN3 4
#define MOTOR_IN4 10

// Distance Thresholds
#define STOP_DISTANCE 15    // Stop 15cm before fire
#define OBSTACLE_DISTANCE 15 // Stop 5cm before obstacle
#define GAS_THRESHOLD 450   // Gas threshold value

// Timing for turns
#define TURN_DURATION 50    // Short burst for turning (adjust as needed, in ms)

void setup() {
    Serial.begin(9600); // Initialize Serial communication
    lcd.begin(16, 2);
    lcd.print("FireBot Ready!");
    delay(2000);

    // Sensor Inputs
    pinMode(FLAME_SENSOR, INPUT);
    pinMode(GAS_SENSOR, INPUT);
    pinMode(ULTRASONIC_TRIG, OUTPUT);
    pinMode(ULTRASONIC_ECHO, INPUT);
    pinMode(LED_INDICATOR, OUTPUT);
    pinMode(PUMP_MOTOR, OUTPUT);
    
    // Line Follower Sensors
    pinMode(IR_SENSOR_LEFT, INPUT);
    pinMode(IR_SENSOR_RIGHT, INPUT);

    // Motor Control Pins
    pinMode(MOTOR_IN1, OUTPUT);
    pinMode(MOTOR_IN2, OUTPUT);
    pinMode(MOTOR_IN3, OUTPUT);
    pinMode(MOTOR_IN4, OUTPUT);
    
    digitalWrite(PUMP_MOTOR, LOW);
    digitalWrite(LED_INDICATOR, LOW);
    moveForward(); // Start moving
}

void loop() {
    bool flameDetected = digitalRead(FLAME_SENSOR) == LOW;
    int gasLevel = analogRead(GAS_SENSOR);
    int distance = getDistance();
    gasLevel = gasLevel - gasLevel % 100;
    lcd.clear();

    // Define alert code
    int alertCode;
    if (flameDetected) {
        alertCode = 1; // Fire detected
    } else if (gasLevel > GAS_THRESHOLD) {
        alertCode = 2; // Gas leak
    } else if (distance <= OBSTACLE_DISTANCE) {
        alertCode = 3; // Obstacle
    } else {
        alertCode = 0; // Normal operation
    }

    // Send data to Serial Monitor in format: "{alertcode} {flame} {gaslevel} {distance}"
    Serial.print(alertCode);
    Serial.print(" ");
    Serial.print(flameDetected ? 1 : 0);
    Serial.print(" ");
    Serial.print(gasLevel);
    Serial.print(" ");
    Serial.println(distance);

    if (flameDetected) {
        stopMotors();
        digitalWrite(PUMP_MOTOR, HIGH);
        lcd.setCursor(0, 0);
        lcd.print("Fire!!!");
        lcd.setCursor(0, 1);
        lcd.print("Pump ON");
        
        while (flameDetected) {
            flameDetected = digitalRead(FLAME_SENSOR) == LOW;
            digitalWrite(LED_INDICATOR, !digitalRead(LED_INDICATOR));
            delay(100);
            // Update Serial data during loop
            Serial.print("1 ");
            Serial.print(flameDetected ? 1 : 0);
            Serial.print(" ");
            Serial.print(gasLevel);
            Serial.print(" ");
            Serial.println(getDistance());
        }
        
        digitalWrite(PUMP_MOTOR, LOW);
        moveForward();
    }
    else if (gasLevel > GAS_THRESHOLD) {
        stopMotors();
        lcd.setCursor(0, 0);
        lcd.print("Gas Leak!!!");
        lcd.setCursor(0, 1);
        lcd.print("Alert Sent");
        
        while (gasLevel > GAS_THRESHOLD) {
            digitalWrite(LED_INDICATOR, !digitalRead(LED_INDICATOR));
            delay(500);
            gasLevel = analogRead(GAS_SENSOR);
            // Update Serial data during loop
            Serial.print("2 ");
            Serial.print(digitalRead(FLAME_SENSOR) == LOW ? 1 : 0);
            Serial.print(" ");
            Serial.print(gasLevel);
            Serial.print(" ");
            Serial.println(getDistance());
        }
        
        moveForward();
    }
    else if (distance <= OBSTACLE_DISTANCE) {
        stopMotors();
        lcd.setCursor(0, 0);
        lcd.print("Obstacle!!!");
        lcd.setCursor(0, 1);
        lcd.print("Bot Stop!!!");
        digitalWrite(LED_INDICATOR, HIGH);
        
        while (getDistance() <= OBSTACLE_DISTANCE) {
            delay(50);
            // Update Serial data during loop
            Serial.print("3 ");
            Serial.print(digitalRead(FLAME_SENSOR) == LOW ? 1 : 0);
            Serial.print(" ");
            Serial.print(analogRead(GAS_SENSOR));
            Serial.print(" ");
            Serial.println(getDistance());
        }
        
        digitalWrite(LED_INDICATOR, LOW);
        moveForward();
    }
    else {
        lcd.setCursor(0, 0);
        lcd.print("Flame: NO");
        lcd.setCursor(0, 1);
        lcd.print("Gas: ");
        lcd.print(gasLevel);
        digitalWrite(LED_INDICATOR, LOW);
        digitalWrite(PUMP_MOTOR, LOW);
        followLine(); // Follow line when no conditions are met
    }
    delay(20); // Minimal delay for stability
}

int getDistance() {
    digitalWrite(ULTRASONIC_TRIG, LOW);
    delayMicroseconds(2);
    digitalWrite(ULTRASONIC_TRIG, HIGH);
    delayMicroseconds(10);
    digitalWrite(ULTRASONIC_TRIG, LOW);
    long duration = pulseIn(ULTRASONIC_ECHO, HIGH);
    return duration * 0.034 / 2;
}

void moveForward() {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);
}

void stopMotors() {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, LOW);
}

void turnLeft() {
    digitalWrite(MOTOR_IN1, LOW);
    digitalWrite(MOTOR_IN2, HIGH);
    digitalWrite(MOTOR_IN3, HIGH);
    digitalWrite(MOTOR_IN4, LOW);
    delay(TURN_DURATION); // Brief turn
    stopMotors(); // Stop to recheck sensors
}

void turnRight() {
    digitalWrite(MOTOR_IN1, HIGH);
    digitalWrite(MOTOR_IN2, LOW);
    digitalWrite(MOTOR_IN3, LOW);
    digitalWrite(MOTOR_IN4, HIGH);
    delay(TURN_DURATION); // Brief turn
    stopMotors(); // Stop to recheck sensors
}

void followLine() {
    bool leftSensor = digitalRead(IR_SENSOR_LEFT);  // HIGH (1) = white, LOW (0) = black
    bool rightSensor = digitalRead(IR_SENSOR_RIGHT); // HIGH (1) = white, LOW (0) = black

    if (leftSensor == LOW && rightSensor == LOW) { // Both on white
        moveForward(); // Keep moving straight
    } 
    else if (leftSensor == HIGH && rightSensor == HIGH) { // Both on black
        stopMotors(); // Stop if both detect black (end of line or error)
    } 
    else if (leftSensor == HIGH && rightSensor == LOW) { // Left on black, right on white
        turnRight(); // Brief right turn to stay on white
    } 
    else if (leftSensor == LOW && rightSensor == HIGH) { // Left on white, right on black
        turnLeft(); // Brief left turn to stay on white
    }
}
