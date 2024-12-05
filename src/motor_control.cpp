#include "motor_control.h"
void init_motors(){ 
      // Initialize motor pins
    pinMode(motorLeftPin1, OUTPUT);
    pinMode(motorLeftPin2, OUTPUT);
    pinMode(motorRightPin1, OUTPUT);
    pinMode(motorRightPin2, OUTPUT);

    // Initialize PWM channels
    ledcSetup(0, pwmFrequency, pwmResolution);
    ledcAttachPin(motorENA, 0); // left 
    ledcSetup(1, pwmFrequency, pwmResolution);
    ledcAttachPin(motorENB, 1);// right 

}
void moveForward(int pwmL, int pwmR) {
    ledcWrite(0, pwmL);
    ledcWrite(1, pwmR);
    digitalWrite(motorLeftPin1,0);     
    digitalWrite(motorLeftPin2,1);     
    digitalWrite(motorRightPin1,1);     
    digitalWrite(motorRightPin2,0);     

}

void moveLeft(int pwmL, int pwmR){ 
    ledcWrite(0, pwmR);
    ledcWrite(1, pwmL);
    digitalWrite(motorLeftPin1,0);     
    digitalWrite(motorLeftPin2,0);     
    digitalWrite(motorRightPin1,1);     
    digitalWrite(motorRightPin2,0);   
}
void moveRight(int pwmL, int pwmR){ 
    ledcWrite(0, pwmR);
    ledcWrite(1, pwmL);
    digitalWrite(motorLeftPin1,1);     
    digitalWrite(motorLeftPin2,0);     
    digitalWrite(motorRightPin1,0);     
    digitalWrite(motorRightPin2,0);
}
void stopMotors() {
    ledcWrite(0, 0);
    ledcWrite(1, 0);
}


void moveBackward(int pwmL, int pwmR) {
    ledcWrite(0, pwmL);
    ledcWrite(1, pwmR);
    digitalWrite(motorLeftPin1,1);     
    digitalWrite(motorLeftPin2,0);     
    digitalWrite(motorRightPin1,0);     
    digitalWrite(motorRightPin2,1);     

}
