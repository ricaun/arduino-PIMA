//
// PIMA-Serial1-index.ino
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
  unsigned long id = pima.id;
  unsigned int index = pima.index;
  unsigned long value = pima.value;

  Serial.print("id : ");
  Serial.println(id);

  switch (index) {
    case PIMA_INDEX_ACTIVE:
      Serial.print("ACTIVE: ");
      Serial.println(value);
      break;
    case PIMA_INDEX_REVERSE:
      Serial.print("REVERSE: ");
      Serial.println(value);
      break;
    case PIMA_INDEX_INDUCTIVE:
      Serial.print("INDUCTIVE: ");
      Serial.println(value);
      break;
    case PIMA_INDEX_CAPACITIVE:
      Serial.print("CAPACITIVE: ");
      Serial.println(value);
      break;
  }
}
