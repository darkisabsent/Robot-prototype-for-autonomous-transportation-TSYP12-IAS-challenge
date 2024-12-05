#include "IR_sensor.h"
#include "motor_control.h"


IR_sensor::IR_sensor(){ 
    // previous_time = millis();

}
IR_sensor::~IR_sensor(){ 
    delete pid;
}
void IR_sensor::IR_sensor_init(const uint8_t* sensor_p, uint8_t emit_pin ){ 
    // _IR_pins= sensor_p; 

    pinMode(MR_IR,INPUT); 
    pinMode(ML_IR,INPUT);
    pinMode(D_IR,INPUT);
    qtr.setTypeAnalog(); 
    qtr.setSensorPins((const uint8_t []){26,25,33,32,35,34,39,36 } , 8); 
    qtr.setEmitterPin(16);
    qtr.setDimmingLevel(0);
}


void IR_sensor::Read_sensor(){ 
    ML_IR_Val_old=ML_IR_Val;
    MR_IR_Val_old = MR_IR_Val ; 
    ML_IR_Val=digitalRead(ML_IR);
    MR_IR_Val=digitalRead(MR_IR);
    D_IR_Val=digitalRead(D_IR);
   
    qtr.read(_IR_Value); 
    for (int  i = 0 ; i < IR_PIN_COUNT; i++ ){ 
       ( _IR_Value[i] >= THRESHOLD ? _IR_Value[i]=HIGH: _IR_Value[i]=LOW ) ;  
    }
    if ( MR_IR_Val== LOW && _IR_Value[0] == LOW &&
        _IR_Value[1] == LOW  && _IR_Value[2] == LOW && _IR_Value[3] == LOW  && _IR_Value[4] == LOW &&
        _IR_Value[5] ==LOW   && _IR_Value[6] == LOW &&
        _IR_Value[7] ==LOW && ML_IR_Val==LOW ){ 
            ML_IR_Val=ML_IR_Val_old;
            MR_IR_Val = MR_IR_Val_old ; 
        }

}

void IR_sensor::Print_sensor_values(){ 
    // debutg senor values ; 
    Serial.print(ML_IR_Val); 
    Serial.print('\t'); 
    for (uint8_t i = 0 ; i<IR_PIN_COUNT; i++){ 
            Serial.print(_IR_Value[i]); 
            Serial.print('\t'); 
    }
    Serial.print(MR_IR_Val); 
    Serial.print('\t'); 
    Serial.println();
}
//  set sensor pins 

void IR_sensor::init_pid(){ 
    pid = new  PID(&input, &output, &setPoint, Kp, Ki, Kd, DIRECT);
    pid->SetMode(AUTOMATIC);
    pid->SetOutputLimits(-120, 120 );
}
void IR_sensor::followLine(bool inverse, int bias_L, int bias_R) {
    // addd it to support the 8 IR sensors 
    int line = (inverse == true ? HIGH : LOW);
    // 1 black // 0  white 
             
    error =(MR_IR_Val == line ? (-7000 + bias_R) : 0) + 
           (_IR_Value[0] == line ? -4000  : 0) + 
           (_IR_Value[1] == line ? 0 : 0) + 
           (_IR_Value[2] == line ? -2000 : 0) + 
           (_IR_Value[3] == line ? -1000 : 0)+ 
           (_IR_Value[4] == line ? 1000 : 0) + 
           (_IR_Value[5] == line ? 2000 : 0) + 
           (_IR_Value[6] == line ? 0 : 0)+
           (_IR_Value[7] == line ?  4000  : 0)+
           (ML_IR_Val == line ? (7000 + bias_L) : 0) 
           ;
    
    input = error;
    pid->Compute();
    // output=  compute_pid(error);
    leftMotorSpeed = offset - output;
    rightMotorSpeed = offset+  output;
    int pwmL= constrain(leftMotorSpeed, 0, pwmMaxValue) ;  
    int pwmR= constrain(rightMotorSpeed, 0, pwmMaxValue) ; 
    moveForward(pwmL, pwmR);
}
int IR_sensor::compute_pid(int error){ 
    unsigned long now = millis();
    unsigned long dt= ( now - previous_time) / 1000 ; 
    out_proportional=   error; 
    out_derivative= (error - previous_error) / dt;  
    previous_error= error; 
    int out = (Kp * out_proportional)  + (Kd * out_derivative);
    return out;
}
void IR_sensor::print_pid_values(){ 
    Serial.print(" Kp ");
    Serial.print(Kp);
    Serial.print(" Ki ");
    Serial.print(Ki);
    Serial.print(" Kd ");
    Serial.print(Ki);
    Serial.println();


}
void IR_sensor::print_PID_output(){ 
    Serial.print( " Error: ");
    Serial.print(error);
    Serial.print(" PID output: "); 
    Serial.print(output); 
    Serial.print(" LEFT motor output: "); 
    Serial.print(constrain(leftMotorSpeed, 0, pwmMaxValue)); 
    Serial.print(" Right motor output: "); 
    Serial.print(constrain(rightMotorSpeed, 0, pwmMaxValue)); 
    Serial.println(); 

}

