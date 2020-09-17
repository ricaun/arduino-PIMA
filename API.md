# PIMA API

## Include Library

```arduino
#include <PIMA.h>
```

### Begin

Initialize the library with a Stream input.

```arduino
PIMA.begin(&stream);
```

### Loop

Loop the stream and try to decode the packet

```arduino
PIMA.loop();
```

### Register callback

Register a callback function for when a valid packet finish.

```arduino
PIMA.onMessage(pimaMessage);

void pimaMessage(PIMALayer pima)
{
  Serial.print("id : ");
  Serial.println(pima.id);
  Serial.print("index : ");
  Serial.println(pima.index , HEX);
  Serial.print("value : ");
  Serial.println(pima.value);
}
```

### Writing

Write data to the decoder.

```arduino
LoRa.write(byte);

LoRa.write(buffer, length);
```
* `byte` - single byte to write to packet

or

* `buffer` - data to write to packet
* `length` - size of data to write

Returns the number of bytes written.

## Defines

The library has the Standard Packages

| Defines | Value | 
| :- | -: |
| PIMA_INDEX_ACTIVE | 0x0A02 |
| PIMA_INDEX_INDUCTIVE | 0x0A07 |
| PIMA_INDEX_CAPACITIVE | 0x0A0C |
| PIMA_INDEX_REVERSE | 0x0A51 |