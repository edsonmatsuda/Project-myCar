#include <SoftwareSerial.h>

// Connect the HC-05 TX to Arduino pin 2 RX. 
// Connect the HC-05 RX to Arduino pin 3 TX through a voltage divider.
SoftwareSerial BTserial(2, 3); // RX | TX

String potValues = "";

/*    Pot Direction   */
int forwardBackward = A3;       //Pot to forward and backward (Stop = 512 / Forward < 512 / Backward > 512)
int leftRight = A2;             //Pot to left and right (Center = 512 / Left < 512 / Right > 512)
int pot_FB = 0;                 //Pot value in digital
int pot_LR = 0;                 //Pot value in digital

void setup() {
  // put your setup code here, to run once:
  //Joystick inputs
  pinMode(forwardBackward, INPUT);
  pinMode(leftRight, INPUT);
  
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:

  //Get joystick input
  pot_FB = analogRead(forwardBackward);
  pot_LR = analogRead(leftRight);

  //Print the values of the sensors (distance and joystick) on serial monitor
  /*
  Serial.print("FBont&rear = ");
  Serial.println(pot_FB);
  Serial.print("left&right = ");
  Serial.println(pot_LR);
  delay(500);
  */

  potValues = toSend(pot_FB, pot_LR);
  
  BTSerial.write(potValues);
}

void toSend(int value1, int value2){
  String combined = String(value1) + ";" + String(value2);
  return combined;
}
