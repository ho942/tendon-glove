#include <Encoder.h>
#include <AutoPID.h>

// pin setting
const int m1_en = 9;  // Speed control in PWM
const int m1_in1 = 12; // Rotation direction
const int m1_in2 = 13; // Rotation direction

// encoder setting
Encoder knob1(0, 1);

long target_m1 = 3000;   // target position for experiment <<<< what you can change for experiment
double ssp = 25; //pwm 64 is 25.1% <<<< what you can change for experiment
double msp = 50; //pwm 128 is 50.2% <<<< what you can change for experiment
double hsp = 75; //pwm 191 is 74.9% <<<< what you can change for experiment
long target = 3000;   // target position for motor
long initial_pos = 0;   // initial position for encoder
bool mact = false;      // state of motor
int motorSpeed = 0;      // PWM speed of the motor

const long stopThreshold = 5; //threshold for operation

// PID variables
double Setpoint = 0;  // target position
double Input = 0;     // current position
double Output = 0;    // PID output (motor speed)

// PID constant
double Kp = 1.5, Ki = 0.1, Kd = 1;

// AutoPID object
AutoPID pid(&Input, &Output, &Setpoint, Kp, Ki, Kd, 0, 255);

void setup() {
  Serial.begin(115200);

  pinMode(m1_en, OUTPUT);
  pinMode(m1_in1, OUTPUT);
  pinMode(m1_in2, OUTPUT);

  stopMotor(m1_en, m1_in1, m1_in2);  // initial state of motor
  knob1.write(0);  // initialize encoder as 0

  // PID controller initialization
  pid.setOutputRange(0, 255);  // Motor speed output limit (modified from setOutputLimits to setOutputRange)
}

void loop() {
  // receive the command
  receiveCommand();

  // read the current encoder position
  long currentPos = knob1.read();
  long thresHold = abs(target - currentPos);

  // Update Input for PID calculation
  Input = currentPos;
  Setpoint = target;

  // PID calculation
  pid.run();

  // Control motor based on PID output
  if (mact) {
    controlMotor(currentPos, thresHold);
  } else {
    stopMotor(m1_en, m1_in1, m1_in2);  // stop motor
  }

  sendEncoderValues(currentPos, thresHold);
}


//percentage of speed
int calculatePWM(int percentage) {
  float baseSpeed = 26.32;
  float targetSpeed = baseSpeed * (percentage / 100.0);
  int pwmValue = map(targetSpeed, 0, baseSpeed, 0, 255);
  return pwmValue;

}


// Receive command and set motor behavior
void receiveCommand() {
  if (Serial.available()) {
    String cmd = Serial.readStringUntil('\n');
    cmd.trim();

    if (cmd.length() > 0) {
      char command = cmd.charAt(0);

      // valid command option
      if (command == 'q' || command == 'w' || command == 'e') {
        mact = true;

        // Set target position and motor speed based on the command
        if (command == 'q') {
          motorSpeed = calculatePWM(ssp); // Low speed 
        } else if (command == 'w') {
          motorSpeed = calculatePWM(msp); // Medium speed 
        } else if (command == 'e') {
          motorSpeed = calculatePWM(hsp); // High speed 
        }

        target = target_m1;  // set the target (target position)
        Serial.print("Command received: Motor activated with speed ");
        Serial.println(motorSpeed);
      } else if (command == 'r') {
        mact = true;
        target = initial_pos;  // set the target(initial position)
        Serial.println("Command received: Motor reversing to initial position.");
      } else if (command == 's') {
        mact = false;  
        Serial.println("Command received: Motor stopped.");
      } else {
        Serial.println("Invalid command! Use 'q', 'w', 'e', 'r', or 's'.");
      }
    }
  }
}
void controlMotor(long currentPos, long thresHold) {
  long distance = abs(target - currentPos);

  // If the threshold is reached, stop the motor
  if (thresHold <= stopThreshold) {
    stopMotor(m1_en, m1_in1, m1_in2);
    Serial.println("Motor reached target position.");
  } else {
    // Use PID output for motor speed control
    int pidOutput = constrain(Output, 40, 255);  // Minimum speed set to 40 to avoid 0 PWM
    
    // Adjust the motor speed based on the motorSpeed input to approach the target motor speed
    int adjustedSpeed = pidOutput + (motorSpeed - pidOutput) * 0.1; // Proportional adjustment

    // Ensure the adjusted speed is within the valid range
    adjustedSpeed = constrain(adjustedSpeed, 40, 255);

    // Send pidOutput and adjustedSpeed to Serial Plotter
    Serial.print("pidOutput: ");
    Serial.print(pidOutput);
    Serial.print("\t");

    Serial.print("adjustedSpeed: ");
    Serial.println(adjustedSpeed);

    // Move motor depending on the direction
    if (target > currentPos) {
      moveMotor(m1_en, m1_in1, m1_in2, true);  // Move forward
    } else {
      moveMotor(m1_en, m1_in1, m1_in2, false);  // Move backward
    }

    analogWrite(m1_en, adjustedSpeed);  // Set motor speed
  }
}



// Move the motor in the given direction
void moveMotor(int pwmPin, int dirPin1, int dirPin2, bool forward) {
  digitalWrite(dirPin1, forward ? HIGH : LOW);
  digitalWrite(dirPin2, forward ? LOW : HIGH);
}

// Stop the motor
void stopMotor(int pwmPin, int dirPin1, int dirPin2) {
  digitalWrite(dirPin1, LOW);
  digitalWrite(dirPin2, LOW);
  analogWrite(pwmPin, 0);  // Stop the motor by setting PWM to 0
}

// Send encoder values for monitoring
void sendEncoderValues(long currentPos, long thresHold) {
  Serial.print("Current Position: ");
  Serial.print(currentPos);
  Serial.print(" | Target Position: ");
  Serial.println(target);
  Serial.print(" | Threshold: ");
  Serial.println(thresHold);
}
