/*
 * File    : ETT_24LC16B.H
 * Author  : ETT CO.,LTD 
 *         : Modify From Source -> blins@yandex.ru
 * Update  : October 2018        
 * Purpose : Library 24LC16B For I2C Interface Arduino
 * Support : ESP32
 *         : ESP8266
 *         : MEGA32U4
 */

#ifndef ETT_24LC16B_H
#define	ETT_24LC16B_H

#include <inttypes.h>

class ETT_24LC16B
{
protected:  
  uint8_t _i2caddr;
  
private:
  uint8_t get_HighAddress(int address);
  uint8_t get_LowAddress(int address);
  uint8_t makeDevAddress(int address);
    
public:
  ETT_24LC16B(uint8_t addr);
  
  void write(int address, uint8_t data);
  void write(int address, uint8_t* buffer, int length);
    
  uint8_t read_byte(int address);
  int read_int(int address);
  void read_buffer(int address, uint8_t* buffer, int length);  
};

#endif	/* ETT_24LC16B_H */

