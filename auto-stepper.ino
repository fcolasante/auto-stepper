#include <AccelStepper.h>
#define dirPin 3  // DIR+
#define stepPin 4 // PUL+
#define motorInterfaceType 1 //DRIVER MODE


// PARAMETERS
#define STEP_REVOLUTION 800
#define STEP_SINGLE 133

/**
 * HP: step/sec 
 */
#define STEP_SPEED STEP_REVOLUTION * 2 // 1600 step/sec => 2 giri/sec
#define ACCELERATION 500 
#define DELAY_READ 2000 // WAIT UNTIL READ
#define INTERRUPT_PIN 2
#define SENSOR_PIN 5
volatile bool running = false;
volatile bool start = false;

AccelStepper stepper = AccelStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  pinMode(SENSOR_PIN, INPUT);
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  delay(1000);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), setStart, FALLING);

  stepper.setMaxSpeed(STEP_SPEED);
  stepper.setAcceleration(ACCELERATION);
}

void loop() {
  if(start && !running){
    check();
  }
  delay(500);
}

bool checkCorrectness(){
  delay(DELAY_READ);
  int value = digitalRead(SENSOR_PIN);
  Serial.print("read: "); Serial.println(value);
  return value == 1;
}

void moveMotor(int index){
  // 0,1,2,3 => 133
  // 4,5 => 134
  int currentStep = (index < 4) ? STEP_SINGLE : STEP_SINGLE + 1;
    // Set the target position:
  stepper.move(currentStep);
  // Run to target position with set speed and acceleration/deceleration:
  stepper.runToPosition();
}

void check(){
  start = false;
  if(running) return;
  running = true;
  bool correct = true;
  Serial.print("Checking...");
  Serial.print(start); Serial.println(running);
  for(int i = 0; i < 6; i++){
    correct = checkCorrectness();
    if(!correct){
      Serial.println("Pezzo errato");
      break;
    }
    moveMotor(i);
  }
  Serial.println("check completed");
  if(correct) Serial.println("Pezzo OK"); else Serial.println("Pezzo NC");
  Serial.flush();
  running = false;
  start = false;
}

void setStart(){
  start = true;
}