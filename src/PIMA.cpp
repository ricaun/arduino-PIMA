/**
  PIMA.cpp
*/

#include <Arduino.h>
#include "PIMA.h"

PIMAclass::PIMAclass()
{
}

PIMAclass::PIMAclass(Stream *_stream)
{
  stream = _stream;
}

void PIMAclass::begin(Stream *_stream)
{
  stream = _stream;
}

int PIMAclass::loop()
{
  unsigned long interval = 500;
  unsigned long currentMillis = millis();
  if (payload_len != 0)
  {
    if (currentMillis - previousMillis >= interval)
    {
      previousMillis = currentMillis;
      Serial.print("[PIMA] ");
      Serial.print(payload_len);
      Serial.println(" millis");
      clear();
    }
  }

  if (stream != nullptr)
  {
    while (available() > 0)
    {
      previousMillis = currentMillis;
      write(read());
    }
  }
  return 0;
}

int PIMAclass::processPIMA()
{
  if (payload_len == 1)
  {
    byte b = PIMA_PREAMBLE & 0x00ff;
    if (payload_buf[0] != b)
    {
      clear();
      Serial.print("[PIMA_PREAMBLE] ");
      Serial.println(payload_buf[0], HEX);
      return 0;
    }
  }
  else if (payload_len == 2)
  {
    uint16_t preamble = *((uint16_t *)(payload_buf));
    if (preamble != PIMA_PREAMBLE)
    {
      clear();
      Serial.print("[PIMA] ");
      Serial.print(preamble, HEX);
      Serial.println(" Preable error");
      return 0;
    }
  }
  else if (payload_len == 8)
  {
    pima_size = *((uint8_t *)(payload_buf + 2 + 5));
    if (pima_size > 10)
    {
      clear();
      Serial.print("[PIMA] ");
      Serial.println("pima_size");
      return 0;
    }
  }
  else if (payload_len == 10 + pima_size)
  {
    uint16_t crc = *((uint16_t *)(payload_buf + payload_len - 2));
    uint16_t calc_crc = calcula_crc16(payload_buf + 2, payload_len - 4);

    if (crc != calc_crc)
    {
      clear();
      Serial.print("[PIMA] ");
      Serial.println("CRC error");
      return 0;
    }

    pima.id = bcd2number(payload_buf + 2, 5);
    pima.index = *((uint16_t *)(payload_buf + 8));
    pima.value = bcd2number(payload_buf + 10, pima_size - 2);

    if (messageCallback)
    {
      messageCallback(pima);
    }

    if (messageCallback2)
    {
      messageCallback2(pima.id, pima.index, pima.value);
    }

    clear();

    return 1;
  }

  return 0;
}

void PIMAclass::onMessage(void (*cb)(PIMALayer pima))
{
  messageCallback = cb;
}

void PIMAclass::onMessage2(void (*cb)(uint32_t id, uint16_t index, uint32_t value))
{
  messageCallback2 = cb;
}

void PIMAclass::clear()
{
  pima_size = 0;
  payload_len = 0;
}

size_t PIMAclass::write(uint8_t c)
{
  payload_buf[payload_len++] = c;
  processPIMA();
  return 1;
};

size_t PIMAclass::write(const uint8_t *buffer, size_t size)
{
  size_t n = 0;
  for (size_t i = 0; i < size; i++)
  {
    n += write(*(buffer + i));
  }
  return n;
}

int PIMAclass::available()
{
  return (stream->available());
}

int PIMAclass::read()
{
  if (stream->available())
    return stream->read();
  return -1;
}

int PIMAclass::peek()
{
  if (stream->available())
    return stream->peek();
  return -1;
}

void PIMAclass::flush()
{
}

uint16_t PIMAclass::calcula_crc16(byte *buffer, byte length)
{
  uint16_t ret = 0;
  uint16_t crcpolinv = 0x4003;
  int i;
  int j;
  for (i = 0; i < length; i++)
  {
    ret ^= (buffer[i] & 0xff);
    for (j = 0; j < 8; j++)
    {
      if ((ret & 0x0001) != 0)
      {
        ret ^= crcpolinv;
        ret >>= 1;
        ret |= 0x8000;
      }
      else
      {
        ret >>= 1;
      }
    }
  }
  return ret;
}

uint32_t PIMAclass::bcd2number(byte *buffer, int length)
{
  uint32_t ret = 0;
  uint32_t mult = 1;
  for (int i = length - 1; i >= 0; i--)
  {
    uint32_t value = bcd2dec(buffer[i]);
    ret += value * mult;
    mult *= 100;
  }
  return ret;
}

uint8_t PIMAclass::bcd2dec(uint8_t n)
{
  return n - 6 * (n >> 4);
}

PIMAclass PIMA;
