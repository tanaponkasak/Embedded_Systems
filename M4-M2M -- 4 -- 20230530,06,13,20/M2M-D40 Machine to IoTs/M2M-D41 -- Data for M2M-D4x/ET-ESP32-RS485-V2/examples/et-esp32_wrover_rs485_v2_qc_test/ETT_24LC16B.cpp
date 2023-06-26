/*
 * File    : ETT_24LC16B.C
 * Author  : ETT CO.,LTD 
 *         : Modify From Source -> blins@yandex.ru
 * Update  : October 2018        
 * Purpose : Library 24LC16B For I2C Interface Arduino
 * Support : ESP32
 *         : ESP8266
 *         : MEGA32U4
 */

#include <stddef.h>
#include <Wire.h> 
#include "ETT_24LC16B.h"

ETT_24LC16B::ETT_24LC16B(uint8_t addr)
{
  _i2caddr = addr;
}

uint8_t ETT_24LC16B::get_HighAddress(int address)
{
  return (uint8_t)((address) >> 8);
}

uint8_t ETT_24LC16B::get_LowAddress(int address)
{
  return (uint8_t)((address & 0xFF));
}

uint8_t ETT_24LC16B::makeDevAddress(int address)
{
  return (uint8_t)((_i2caddr) | ((this->get_HighAddress(address)) & 0x07));
}

void ETT_24LC16B::write(int address, uint8_t data)
{
  Wire.beginTransmission(this->makeDevAddress(address));
  Wire.write(this->get_LowAddress(address));
  Wire.write(data);
  Wire.endTransmission();
}

void ETT_24LC16B::write(int address, uint8_t* buffer, int length)
{
  int page_i = 0;
  uint8_t c = 0;
  while (c < length)
  {
    Wire.beginTransmission(this->makeDevAddress(address + c));
    Wire.write(this->get_LowAddress(address + c));
    page_i = 0;
    while ((page_i < 16) && (c < length))
    {
      Wire.write(buffer[c]);
      page_i++;
      c++;
    }
    Wire.endTransmission();
    delay(5);
  }
}

uint8_t ETT_24LC16B::read_byte(int address)
{
  Wire.beginTransmission(this->makeDevAddress(address));
  Wire.write(this->get_LowAddress(address));
  Wire.endTransmission();
  uint8_t rdata = 0x00;
  Wire.requestFrom((this->makeDevAddress(address)), (uint8_t)1);
  if (Wire.available()) 
  {
    rdata = Wire.read();
  }
  return rdata;
}

int ETT_24LC16B::read_int(int address)
{
  int rdata = 0;
  this->read_buffer(address, (uint8_t*) rdata, sizeof(rdata));
  return rdata;
}

void ETT_24LC16B::read_buffer(int address, uint8_t* buffer, int length)
{
  Wire.beginTransmission(this->makeDevAddress(address));
  Wire.write(this->get_LowAddress(address));
  Wire.endTransmission();
  Wire.requestFrom(this->makeDevAddress(address),(uint8_t)length);
  int c = 0;
  for ( c = 0; c < length; c++ )
  if (Wire.available()) buffer[c] = Wire.read();
}

