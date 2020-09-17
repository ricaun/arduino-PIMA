/**
 PIMA.h
*/

#ifndef PIMA_H_
#define PIMA_H_

#define PIMA_BUF_SIZE 32

#define PIMA_PREAMBLE 0x55AA

#define PIMA_INDEX_ACTIVE     0x020A
#define PIMA_INDEX_INDUCTIVE  0x070A
#define PIMA_INDEX_CAPACITIVE 0x0C0A
#define PIMA_INDEX_REVERSE    0x510A

typedef struct
{
	uint32_t id;
	uint16_t index;
	uint32_t value;
} PIMALayer;


class PIMAclass : public Stream
{
private:
  // ----------------------------------------------- //
  // 	buffer
  uint8_t payload_buf[PIMA_BUF_SIZE];
  uint8_t payload_len = 0;
  // ----------------------------------------------- //

  PIMALayer pima;
  uint8_t pima_size = 0;

  unsigned long previousMillis = 0;

  Stream *stream = nullptr;

public:

  PIMAclass();
  PIMAclass(Stream *_stream);

  void begin(Stream *_stream);

  int loop();
  int processPIMA();

  void clear();
  void onMessage(void (*cb)(PIMALayer pima));
  void onMessage2(void (*cb)(uint32_t id, uint16_t index, uint32_t value));

  size_t write(const uint8_t *buffer, size_t size) override;
  size_t write(uint8_t data) override;

  int available() override;
  int read() override;
  int peek() override;
  void flush() override;

private:

  void (*messageCallback)(PIMALayer pima);
  void (*messageCallback2)(uint32_t id, uint16_t index, uint32_t value);

  uint16_t calcula_crc16(byte *buffer, byte length);
  uint32_t bcd2number(byte *buffer, int length);
  uint8_t bcd2dec(uint8_t n);
};

extern PIMAclass PIMA;

#endif /* PIMA_H_ */
