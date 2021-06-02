/*    Distance sensor  */
int distanceSensorPin = 0;      //Pin to distance sensor
int distanceValue = 0;          //Value of the distance sensor
int distanceLimit = 350;        //Value when the distance sensor will disable to move forward
int stopMove = 0;               //Flag to stop (1 = stop)


/*    Pot Direction   */
int forwardBackward = A3;       //Pot to forward and backward (Stop = 512 / Forward < 512 / Backward > 512)
int leftRight = A2;             //Pot to left and right (Center = 512 / Left < 512 / Right > 512)
int pot_FB = 0;                 //Pot value in digital
int pot_LR = 0;                 //Pot value in digital

/*    Enable Motor Drive    */
int enable_1 = 9;               //enable_1 = HIGH and enable_2 = LOW -> Forward (Left motor)
int enable_2 = 10;              //enable_1 = LOW and enable_2 = HIGH -> Reverse (Left motor)
int enable_3 = 11;              //enable_3 = HIGH and enable_4 = LOW -> Forward (Right motor)
int enable_4 = 12;              //enable_3 = LOW and enable_4 = HIGH -> Reverse (Right motor)

/*    PWM Motors    */
int PWM_1 = 5;                  //Output left motor
int PWM_2 = 6;                  //Output right motor

/*    Speed   */
int movingFast = 100;           //Speed to go forward & backward - Fast
int movingSlow = 75;            //Speed to go forward & backward - Slow
int curve = 100;                //Speed to go left & right
int carStop = 0;                //Speed to stop

/*    RGB LED   */
const int ledBlue = 4;          //Blue LED pin
const int ledRed =  3;          //Red LED pin
const int ledGreen =  2;        //Green LED pin

long currentMillis = 0;
long ledFlashing = 1000;        //Time of each flashing
long ledFlashingRed = 150;
long ledFlashingGreen = 300;
long ledFlashingYellow = 1000;

int ledStatus = 0;              //LED = 0 -> Vehicle stopped (Yellow), LED = 1 -> Vehicle moving (Green), LED = 2 -> Obstacule ahead (Red)
int ledON = 0;

void setup() {
  // put your setup code here, to run once:
  pinMode(forwardBackward, INPUT);
  pinMode(leftRight, INPUT);
  pinMode(enable_1, OUTPUT);
  pinMode(enable_2, OUTPUT);
  pinMode(enable_3, OUTPUT);
  pinMode(enable_4, OUTPUT);
  pinMode(PWM_1, OUTPUT);
  pinMode(PWM_2, OUTPUT);

  pinMode(ledBlue, OUTPUT);
  pinMode(ledGreen, OUTPUT);
  pinMode(ledRed, OUTPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Check if there is any object in front of the vehicle
  distanceValue = analogRead(distanceSensorPin);
    if(distanceValue < distanceLimit){
      stopMove = 0;
    }else{
      stopMove = 1;
    }

  //Get joystick input
  pot_FB = analogRead(forwardBackward);
  pot_LR = analogRead(leftRight);

  led(ledStatus, ledFlashing);

  //Print the values of the sensors (distance and joystick) on serial monitor
  /*
  Serial.print("FBont&rear = ");
  Serial.println(pot_FB);
  Serial.print("left&right = ");
  Serial.println(pot_LR);
  Serial.print("Distance Sensor = ");
  Serial.println(distanceValue);
  delay(500);
  */

  //Move the robot based on the value of the joystick
  if((pot_FB > 462 && pot_FB < 562) &&          //STOPPED
      (pot_LR > 462 && pot_LR < 562)){
    stoppedCar();
    ledStatus = 0;
    ledFlashing = ledFlashingYellow;
  }
  
  else if(pot_FB > 100 && pot_FB < 400){       //FORWARD - Slow
    if(stopMove == 0){
      forwardCar(movingSlow);
      ledStatus = 1;
      ledFlashing = ledFlashingGreen;
    }else{
      stoppedCar();
      ledStatus = 2;
      ledFlashing = ledFlashingRed;
    }
  }
  
  else if(pot_FB <= 100){                     //FORWARD - Fast
    if(stopMove == 0){
      forwardCar(movingFast);
      ledStatus = 1;
      ledFlashing = ledFlashingGreen;
    }else{
      stoppedCar();
      ledStatus = 2;
      ledFlashing = ledFlashingRed;
    }
  }
  
  else if(pot_FB > 600 && pot_FB < 900){      //REVERSE - Slow
    reverseCar(movingSlow);
    ledStatus = 1;
    ledFlashing = ledFlashingGreen;
  }

  else if(pot_FB >= 900){                    //REVERSE - Fast
    reverseCar(movingFast);
    ledStatus = 1;
    ledFlashing = ledFlashingGreen;
  }
  
  else if(pot_LR < 400){                     //LEFT
    leftCar(curve);
    ledStatus = 1;
    ledFlashing = ledFlashingGreen;
  }

  else if(pot_LR > 624){                     //RIGHT
    rightCar(curve);
    ledStatus = 1;
    ledFlashing = ledFlashingGreen;
  }

}

void stoppedCar(){
  digitalWrite(enable_1, HIGH);     //Left motor stopped
  digitalWrite(enable_2, HIGH);
  analogWrite(PWM_1, carStop);
    
  digitalWrite(enable_3, HIGH);     //Right motor stopped
  digitalWrite(enable_4, HIGH);
  analogWrite(PWM_2, carStop);
}

void forwardCar(int speedCar){
  digitalWrite(enable_1, LOW);     //Left motor forward
  digitalWrite(enable_2, HIGH);
  analogWrite(PWM_1, speedCar);

  digitalWrite(enable_3, LOW);     //Right motor forward
  digitalWrite(enable_4, HIGH);
  analogWrite(PWM_2, speedCar);
}

void reverseCar(int speedCar){
  digitalWrite(enable_1, HIGH);     //Left motor backward
  digitalWrite(enable_2, LOW);
  analogWrite(PWM_1, speedCar);

  digitalWrite(enable_3, HIGH);     //Right motor backward
  digitalWrite(enable_4, LOW);
  analogWrite(PWM_2, speedCar);
}

void leftCar(int turn){
  digitalWrite(enable_1, LOW);     //Left motor to left
  digitalWrite(enable_2, HIGH);
  analogWrite(PWM_1, turn);

  digitalWrite(enable_3, HIGH);     //Right motor to left
  digitalWrite(enable_4, LOW);
  analogWrite(PWM_2, turn);
}

void rightCar(int turn){
  digitalWrite(enable_1, HIGH);     //Left motor to right
  digitalWrite(enable_2, LOW);
  analogWrite(PWM_1, turn);

  digitalWrite(enable_3, LOW);     //Right motor to right
  digitalWrite(enable_4, HIGH);
  analogWrite(PWM_2, turn);
}

void led(int led, long ledFlashing){
  if(millis() - currentMillis > ledFlashing){
    currentMillis = millis();
    if(led == 0){
      if(ledON == 0){
        digitalWrite(ledRed, HIGH);
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledBlue, LOW);
        ledON = 1;
      }else{
        digitalWrite(ledRed, LOW);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledBlue, LOW);
        ledON = 0;
      }
    }else if(led == 1){
      if(ledON == 0){
        digitalWrite(ledRed, LOW);
        digitalWrite(ledGreen, HIGH);
        digitalWrite(ledBlue, LOW);
        ledON = 1;
      }else{
        digitalWrite(ledRed, LOW);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledBlue, LOW);
        ledON = 0;
        }
      }else if(led == 2){
        if(ledON == 0){
        digitalWrite(ledRed, HIGH);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledBlue, LOW);
        ledON = 1;
      }else{
        digitalWrite(ledRed, LOW);
        digitalWrite(ledGreen, LOW);
        digitalWrite(ledBlue, LOW);
        ledON = 0;
      }
    }
  }
}
