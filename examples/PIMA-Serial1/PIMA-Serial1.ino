//
// PIMA-Serial1.ino
//
// PIMA Decoder from Serial1 and shows on the Serial Monitor.
//

#include <PIMA.h>

#define RX1 19 // Arduino Mega

void setup() {
  Serial.begin(115200);
  Serial.println("PIMA Decoder");

  pinMode(RX1, INPUT_PULLUP);
  Serial1.begin(2400);
  PIMA.begin(&Serial1);
  PIMA.onMessage(pimaMessage);
}

void loop() {
  PIMA.loop();
}

void pimaMessage(PIMALayer pima)
{
  Serial.print("id : ");
  Serial.println(pima.id);
  Serial.print("index : ");
  Serial.println(pima.index , HEX);
  Serial.print("value : ");
  Serial.println(pima.value);
}
