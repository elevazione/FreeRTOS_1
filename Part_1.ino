//Define the LED_BUILTIN constant to pin 2 
//Pin 2 is internally connect to the built in LED in ESP32

#define LED_BUILTIN 2

void setup() {
  pinMode(LED_BUILTIN,OUTPUT); //Setup the builtin led as output 
}

void loop() {
  delayMicroseconds(1000000);    //delay 1 seconds
  digitalWrite(LED_BUILTIN,HIGH);//Set builtin led output as high           
  delayMicroseconds(1000000);    //delay 1 seconds
  digitalWrite(LED_BUILTIN,LOW); //Set builtin led output as low
}
