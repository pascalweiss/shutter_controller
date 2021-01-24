#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

float x = 0;

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";
void setup() {
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
  pinMode(A0, INPUT);
}
void loop() {
  x = (float)analogRead(A0)/1024;
  //x = random(0,100)/100.0;
  radio.write(&x, sizeof(x));
  Serial.println(x);
  delay(500);
}
