#include <PIMA.h>
#include <StreamString.h>

#define HTOI(c) ((c<='9')?(c-'0'):((c<='F')?(c-'A'+10):((c<='f')?(c-'a'+10):(0))))
#define TWO_HTOI(h, l) ((HTOI(h) << 4) + HTOI(l))
#define HEX_TO_BYTE(a, h, n) { for (int i = 0; i < n; i++) (a)[i] = TWO_HTOI(h[2*i], h[2*i + 1]); }

byte payload_buf[32];
byte payload_len = 0;

const char *real = "AA550003397125050A02000000E598";


//AA550103050709050A02022222B3D0AA550103050709050A510111110671

StreamString stream;

void setup() {
  Serial.begin(115200);
  Serial.println("Start PIMA");
  PIMA.begin(&stream);
  PIMA.onMessage(pimaMessage);

  payload_len = strlen(real) / 2;
  HEX_TO_BYTE(payload_buf, real, payload_len);

  for(int i = 0; i< payload_len; i++)
  {
    stream.write(payload_buf[i]);
  }
}

void loop() {
  PIMA.loop();
  
  //if (runEvery(1000))
  {
    if (Serial.available() > 1)
    {
      char buffer[64];
      int size = 0;
      while(Serial.available())
      {
        buffer[size++] = Serial.read();
      }
      //Serial.write(buffer, size);
      //Serial.println("");
      payload_len = size /2;
      HEX_TO_BYTE(payload_buf, buffer, payload_len);
      for(int i = 0; i< payload_len; i++)
      {
        stream.write(payload_buf[i]);
      }
    }
  }
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

boolean runEvery(unsigned long interval)
{
  static unsigned long previousMillis = 0;
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval)
  {
    previousMillis = currentMillis;
    return true;
  }
  return false;
}