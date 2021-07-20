#include <Stepper.h>
// PARAMETERS
#define STEP_REVOLUTION 2048
#define STEP_SINGLE 341

/**
 * HP: 5 sec / giro => 12 rpm
 */
#define STEP_SPEED 12

#define DELAY_READ 500
#define INTERRUPT_PIN 2
volatile bool running = false;
volatile bool start = false;
Stepper myStepper(STEP_REVOLUTION, 4, 5, 6, 7);

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(0));
  pinMode(INTERRUPT_PIN, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(INTERRUPT_PIN), setStart, FALLING);

  myStepper.setSpeed(STEP_SPEED);
}

void loop() {
  if(start && !running){
    check();
  }
}

bool checkCorrectness(){
  delay(DELAY_READ);
  int value = analogRead(0);
  Serial.print("read: "); Serial.println(value);
  return value > 10;
}

void moveMotor(int index){
  // 0,1,2,3 => 341
  // 4,5 => 342
  int currentStep = (index < 4) ? STEP_SINGLE : STEP_SINGLE + 1;
  myStepper.step(currentStep);
}

void check(){
  if(running) return;
  running = true;
  start = false;
  bool correct = true;
  Serial.println("Checking...");
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
}

void setStart(){
  start = true;
}