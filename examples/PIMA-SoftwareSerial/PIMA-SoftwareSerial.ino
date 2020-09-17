//
// PIMA-SoftwareSerial.ino
//
// PIMA Decoder from SoftwareSerial and shows on the Serial Monitor.
//

#include <PIMA.h>
#include <SoftwareSerial.h>

#define TX 3
#define RX 2

SoftwareSerial mySerial(RX, TX); // RX, TX

void setup() {
  Serial.begin(115200);
  Serial.println("PIMA Decoder");

  pinMode(RX, INPUT_PULLUP);
  mySerial.begin(2400);
  PIMA.begin(&mySerial);
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
