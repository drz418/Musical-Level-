#include <Arduino_LSM9DS1.h>
//IMU code based on example: simplegyroscope

//based on Serial Call and Response by Tom Igoe and Scott Fitzgerald



const int pot1 = A0;
const int pot2 = A1;

const int LED = 12;

const int smoothAmt = 50;

int inByte = 0;// incoming serial byte
int potVal1; //potentiameter 1
int potVal2; //potentiameter 1

//arrays for smoothing values
int smooth1[smoothAmt];
int smooth2[smoothAmt];

int av[2];

void setup() {

  Serial.begin(9600);

 if (!IMU.begin()) {
    Serial.println("Failed to initialize IMU!");
    while (1);
  }
  
  //wait for serial port to connect before proceeding
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  pinMode(LED,OUTPUT);

  //populate the array
  for(int s =0;s<smoothAmt;s++){
    smooth1[s]=0;
    smooth2[s]=0;
  }

  

}

void loop() {

potVal1 = analogRead(pot1);
potVal2 = analogRead(pot2);

float x, y, z;

  if (IMU.gyroscopeAvailable()) {
    IMU.readGyroscope(x, y, z);
  }

 //shift data
    for(int s =1;s<(smoothAmt);s++){
      smooth1[smoothAmt-s]=smooth1[smoothAmt-s-1];
      smooth2[smoothAmt-s]=smooth2[smoothAmt-s-1];
    }
    smooth1[0]=potVal1;
    smooth2[0]=potVal2;

    av[0]=0;
    av[1]=0;

    //average data
    for(int s =0;s<smoothAmt;s++){
      av[0]= av[0] + smooth1[s];
      av[1]= av[1] + smooth2[s];
    }

    av[0] = av[0]/smoothAmt;
     av[1] = av[1]/smoothAmt;

Serial.print(av[0]);
Serial.print(",");
Serial.print(av[1]);
Serial.print(",");
Serial.print(x);
Serial.print(",");
Serial.print(y);
Serial.print(",");
Serial.println(z);

  /*Serial.print(potVal1);
  Serial.print(",");
  Serial.println(potVal2);
*/
  //check if there is an data available in the buffer
  if (Serial.available() > 0) {
    inByte = Serial.read();

    if(inByte == 'l'){
      digitalWrite(LED, HIGH);
      delay(1000);
      digitalWrite(LED,LOW);
    }
  }
  
delay(10);
}
