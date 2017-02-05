// MD03A_Motor_basic + encoder

#define InA1            10                      // INA motor pin
#define InB1            11                      // INB motor pin 
#define PWM1            9                       // PWM motor pin

// Encoder variables
enum PinAssignments {
  encoderPinA = 2,
  encoderPinB = 3
};
boolean A_set = false;
boolean B_set = false;

#define LOOPTIME        100                     // PID loop time
#define FORWARD         1                       // direction of rotation
#define BACKWARD        2                       // direction of rotation

unsigned long lastMilli = 0;                    // loop timing 
unsigned long lastMilliPrint = 0;               // loop timing
volatile long count = 0;                                 // rotation counter
long countInit;
long tickNumber = 0;
boolean run = false;                                     // motor moves

void setup() {
 pinMode(InA1, OUTPUT);
 pinMode(InB1, OUTPUT);
 pinMode(PWM1, OUTPUT);
 pinMode(encoderPinA, INPUT_PULLUP); 
 pinMode(encoderPinB, INPUT_PULLUP); 
 digitalWrite(encoderPinA, HIGH);
 digitalWrite(encoderPinB, HIGH);
 attachInterrupt(digitalPinToInterrupt(encoderPinA), doEncoderA, CHANGE);
 attachInterrupt(digitalPinToInterrupt(encoderPinB), doEncoderB, CHANGE);
}

void loop() {
 moveMotor(FORWARD, 150, 720);                        // direction, PWM, ticks number
 delay(5000);
 moveMotor(BACKWARD, 150, 720);                        // direction, PWM, ticks number
 delay(5000);
}

void moveMotor(int direction, int PWM_val, long tick)  {
 countInit = count;
 tickNumber = tick;
 if(direction==FORWARD)          motorForward(PWM_val);
 else if(direction==BACKWARD)    motorBackward(PWM_val);
}

// Interrupt on A changing state
void doEncoderA(){
  // Test transition
  A_set = digitalRead(encoderPinA) == HIGH;
  // and adjust counter + if A leads B
  count += (A_set != B_set) ? +1 : -1;
  if(run)  
    if((abs(abs(count)-abs(countInit))) >= tickNumber)      motorBrake();
}

// Interrupt on B changing state
void doEncoderB(){
  // Test transition
  B_set = digitalRead(encoderPinB) == HIGH;
  // and adjust counter + if B follows A
  count += (A_set == B_set) ? +1 : -1;
  if(run)  
    if((abs(abs(count)-abs(countInit))) >= tickNumber)      motorBrake();
}

void motorForward(int PWM_val)  {
 analogWrite(PWM1, PWM_val);
 digitalWrite(InA1, LOW);
 digitalWrite(InB1, HIGH);
 run = true;
}

void motorBackward(int PWM_val)  {
 analogWrite(PWM1, PWM_val);
 digitalWrite(InA1, HIGH);
 digitalWrite(InB1, LOW);
 run = true;
}

void motorBrake()  {
 analogWrite(PWM1, 0);
 digitalWrite(InA1, HIGH);
 digitalWrite(InB1, HIGH);
 run = false;
}
