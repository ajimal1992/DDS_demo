#include <Wire.h>
#include "SparkFunISL29125.h"

int geo_pin1 = 4;
int geo_pin2 = 5;

int speeding_pin = 6;
int carpark_pin = 7;

int carpark_state = 0;
int speeding_state = 0;
int geo_location = 0;

// Declare sensor object
SFE_ISL29125 RGB_sensor;

int SHT_clockPin = 3;  // pin used for clock
int SHT_dataPin  = 2;  // pin used for data

const int ledPin =  13;      // the number of the LED pin

//variables for storing values
unsigned int s_red = 0;
unsigned int s_green = 0;
unsigned int s_blue = 0;

float tempC = 0.00;
float humidity = 0.00;
uint8_t lightState = 0;
uint8_t lightIntensity = 0;
int ledState = LOW;             // ledState used to set the LED
boolean CarparkState = LOW;
int count = 0;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200); // Open serial connection to report values to host
  if (RGB_sensor.init())
  { 
  }
  pinMode(ledPin, OUTPUT);
}

void loop() {
  // put your main code here, to run repeatedly:
  carpark_state = digitalRead(carpark_pin);
  speeding_state = digitalRead(speeding_pin);
  if(digitalRead(geo_pin1))
    geo_location = 1;
  if(digitalRead(geo_pin2))
    geo_location = 2;
  readSensor();
  readRGBSensor();
  if (count >40) readCPSensor();
  printOut();
  //delay(250);
  count++;
}

void readSensor()
{
  // Read values from the sensor
  tempC = getTemperature();
  humidity = getHumidity();
}

void readRGBSensor()
{
  // Read sensor values (16 bit integers)
  unsigned int red = RGB_sensor.readRed();
  unsigned int green = RGB_sensor.readGreen();
  unsigned int blue = RGB_sensor.readBlue();
  s_red = red/257;
  s_green = green/257;
  s_blue = blue/257;
}

void readCPSensor()
{
    CarparkState = !CarparkState;
    count = 0;
}
//-------------------------------------------------------------------------------------------
void printOut()
{
  lightIntensity = (s_red + s_green + s_blue)/ 3;
  if(lightIntensity >= 40)
  {
    lightState = 3;
    ledState = LOW;
  }
  else if(lightIntensity >= 13)
  {
    lightState = 2;
    ledState = LOW;
  }
  else                         
  {
    lightState = 1;
    ledState = HIGH;
  }

  // set the LED with the ledState of the variable:
  digitalWrite(ledPin, ledState);
  
  Serial.print(tempC);
  Serial.print("t");
  Serial.print(humidity); 
  Serial.print("h");
  Serial.print(lightState); 
  Serial.print("i");
  Serial.print(speeding_state); 
  Serial.print("s");
  Serial.print(geo_location); 
  Serial.print("g");
  Serial.print(carpark_state); 
  Serial.println("c");
}

float getTemperature(){
  //Return Temperature in Celsius
  SHT_sendCommand(B00000011, SHT_dataPin, SHT_clockPin);
  SHT_waitForResult(SHT_dataPin);

  int val = SHT_getData(SHT_dataPin, SHT_clockPin);
  SHT_skipCrc(SHT_dataPin, SHT_clockPin);
  return (float)val * 0.01 - 40; //convert to celsius
}

float getHumidity(){
  //Return  Relative Humidity
  SHT_sendCommand(B00000101, SHT_dataPin, SHT_clockPin);
  SHT_waitForResult(SHT_dataPin);
  int val = SHT_getData(SHT_dataPin, SHT_clockPin);
  SHT_skipCrc(SHT_dataPin, SHT_clockPin);
  return -4.0 + 0.0405 * val + -0.0000028 * val * val; 
}

void SHT_sendCommand(int command, int dataPin, int clockPin){
  // send a command to the SHTx sensor
  // transmission start
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, LOW);
  digitalWrite(clockPin, HIGH);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, LOW);

  // shift out the command (the 3 MSB are address and must be 000, the last 5 bits are the command)
  shiftOut(dataPin, clockPin, MSBFIRST, command);

  // verify we get the right ACK
  digitalWrite(clockPin, HIGH);
  pinMode(dataPin, INPUT);

  if (digitalRead(dataPin)) Serial.println("ACK error 0");
  digitalWrite(clockPin, LOW);
  if (!digitalRead(dataPin)) Serial.println("ACK error 1");
}


void SHT_waitForResult(int dataPin){
  // wait for the SHTx answer
  pinMode(dataPin, INPUT);

  int ack; //acknowledgement

  //need to wait up to 2 seconds for the value
  for (int i = 0; i < 1000; ++i){
    delay(2);
    ack = digitalRead(dataPin);
    if (ack == LOW) break;
  }

  if (ack == HIGH) Serial.println("ACK error 2");
}

int SHT_getData(int dataPin, int clockPin){
  // get data from the SHTx sensor

  // get the MSB (most significant bits)
  pinMode(dataPin, INPUT);
  pinMode(clockPin, OUTPUT);
  byte MSB = shiftIn(dataPin, clockPin, MSBFIRST);

  // send the required ACK
  pinMode(dataPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(dataPin, LOW);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);

  // get the LSB (less significant bits)
  pinMode(dataPin, INPUT);
  byte LSB = shiftIn(dataPin, clockPin, MSBFIRST);
  return ((MSB << 8) | LSB); //combine bits
}

void SHT_skipCrc(int dataPin, int clockPin){
  // skip CRC data from the SHTx sensor
  pinMode(dataPin, OUTPUT);
  pinMode(clockPin, OUTPUT);
  digitalWrite(dataPin, HIGH);
  digitalWrite(clockPin, HIGH);
  digitalWrite(clockPin, LOW);
}