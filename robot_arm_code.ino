#include <AccelStepper.h>

// Steppers
#define XIN1 2
#define XIN2 3
#define XIN3 4
#define XIN4 5

#define YIN1 6
#define YIN2 7
#define YIN3 8
#define YIN4 9

#define ZIN1 10
#define ZIN2 11
#define ZIN3 12
#define ZIN4 13

// Stop Buttons
#define XBUTTON A0
#define YBUTTON A1
#define ZBUTTON A2

AccelStepper stepperX(AccelStepper::HALF4WIRE, XIN1, XIN3, XIN2, XIN4);
AccelStepper stepperY(AccelStepper::HALF4WIRE, YIN1, YIN3, YIN2, YIN4);
AccelStepper stepperZ(AccelStepper::HALF4WIRE, ZIN1, ZIN3, ZIN2, ZIN4);
bool moving = false;

void setup() {
    pinMode(XIN1, OUTPUT);
    pinMode(XIN2, OUTPUT);
    pinMode(XIN3, OUTPUT);
    pinMode(XIN4, OUTPUT);

    pinMode(YIN1, OUTPUT);
    pinMode(YIN2, OUTPUT);
    pinMode(YIN3, OUTPUT);
    pinMode(YIN4, OUTPUT);

    pinMode(ZIN1, OUTPUT);
    pinMode(ZIN2, OUTPUT);
    pinMode(ZIN3, OUTPUT);
    pinMode(ZIN4, OUTPUT);

    pinMode(XBUTTON, INPUT_PULLUP);
    pinMode(YBUTTON, INPUT_PULLUP);
    pinMode(ZBUTTON, INPUT_PULLUP);

    Serial.begin(9600);  // Start Serial communication
    while (!Serial);

    stepperX.setMaxSpeed(500.0);  // Set max speed
    stepperX.setAcceleration(10000.0);  // Set acceleration

    stepperY.setMaxSpeed(500.0);  // Set max speed
    stepperY.setAcceleration(10000.0);  // Set acceleration
    
    stepperZ.setMaxSpeed(500.0);  // Set max speed  
    stepperZ.setAcceleration(10000.0);  // Set acceleration

    Serial.println("Setup complete");
}

void loop() {
    if (Serial.available()) {
        String command = Serial.readStringUntil('\n');  // Read one line
        command.trim();  // Remove spaces or newlines

        if (command.startsWith("STEP ")) {
          Serial.println(command);
          int x = extractValue(command, 'X');
          int y = extractValue(command, 'Y');
          int z = extractValue(command, 'Z');

          Serial.print("Moving x - ");
          Serial.println(x);

          Serial.print("Moving y - ");
          Serial.println(y);

          Serial.print("Moving z - ");
          Serial.println(z);

          stepperX.move(x);
          stepperY.move(y);
          stepperZ.move(z);
          moving = true;
        }
    }

    if (moving) {
        bool stillMoving = false;
        if (stepperX.distanceToGo() != 0) {
            // Check if X button is pressed and stop if necessary
            if (digitalRead(XBUTTON) == LOW) {
                stepperX.stop();  // Stop X motor if button is pressed
            }
            stepperX.run(); 
            stillMoving = true;
        }
        if (stepperY.distanceToGo() != 0) {
            // Check if Y button is pressed and stop if necessary
            if (digitalRead(YBUTTON) == LOW) {
                stepperY.stop();  // Stop Y motor if button is pressed
            }
            stepperY.run(); 
            stillMoving = true;
        }
        if (stepperZ.distanceToGo() != 0) {
            // Check if Z button is pressed and stop if necessary
            if (digitalRead(ZBUTTON) == LOW) {
                stepperZ.stop();  // Stop Z motor if button is pressed
            }
            stepperZ.run(); 
            stillMoving = true;
        }

        // If all motors have stopped, reset moving flag
        if (!stillMoving) 
        {
            sendAcknowledgement();
            moving = false;
        }
    }
    else 
    {
        sendAcknowledgement();
        delay(1000); // So we don't spam
    }
}

void sendAcknowledgement() {
    Serial.println("[ACK]"); // Send acknowledgement to show we're ready to receive messages
}

// Helper function to extract values like "X100"
int extractValue(String command, char axis) {
    int index = command.indexOf(axis);
    if (index != -1) {
        return command.substring(index + 1).toInt();
    }
    return 0;
}
