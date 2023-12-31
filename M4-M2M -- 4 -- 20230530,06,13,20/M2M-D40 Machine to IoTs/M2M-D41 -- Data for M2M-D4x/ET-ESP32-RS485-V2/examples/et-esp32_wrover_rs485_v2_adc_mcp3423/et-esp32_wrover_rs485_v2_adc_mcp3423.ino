/*******************************************************************************
 * ET-ESP32(WROVER) RS485 V2 
 * Tools->Board:"ESP32 Wrover Module"
 *******************************************************************************
 * I2C Interface & I2C Bus
 * -> IO22                = I2C_SCL
 * -> IO21                = I2C_SDA
 * -> I2C RTC:DS3231      = I2C Address : 0x68:1100100(x)
 * -> I2C EEPROM 24LC16   = I2C Address : 0x50:1010000(x)
 * -> I2C ADC MCP3423     = I2C Address : 0x6D:1100101(x)
 * -> I2C Sensor:BME280   = I2C Address : 0x76:1110110(x)
 * -> I2C Sebsor:SHT31    = I2C Address : 0x44:1000100(x)/0x45:1010101(x)
 * SPI Interface SD Card
 * -> SD_CS               = IO4
 * -> SPI_MISO            = IO19
 * -> SPI_MOSI            = IO23
 * -> SPI_SCK             = IO18
 * UART2 RS485 Half Duplex Auto Direction
 * -> IO26                = RX2
 * -> IO27                = TX2
 * User Switch
 * -> IO36                = USER_SW
 * RTC Interrupt
 * -> IO39                = RTC_INT#
 *******************************************************************************/
 
//=================================================================================================
#include <Wire.h> 
//=================================================================================================

//=================================================================================================
// Start of Default Hardware : ET-ESP32(WROVER) RS485 V2
//=================================================================================================
// Remap Pin USART -> C:\Users\Admin\Documents\Arduino\hardware\espressif\esp32\cores\esp32\HardwareSerial.cpp
//                    C:\Users\Admin\AppData\Local\Arduino15\packages\esp32\hardware\esp32\1.0.0\cores\esp32\HardwareSerial.cpp
//=================================================================================================
#include <HardwareSerial.h>
//=================================================================================================
#define SerialDebug           Serial                                                              // USB Serial(Serial0)
//=================================================================================================
#define SerialRS485_RX_PIN    26
#define SerialRS485_TX_PIN    27
#define SerialRS485           Serial2                                                             // Serial2(IO27=TXD,IO26=RXD)
//=================================================================================================
#define SerialLora_RX_PIN     14
#define SerialLora_TX_PIN     13
#define SerialLora            Serial1                                                             // Serial1(IO13=TXD,IO14=RXD)
//=================================================================================================
#define LORA_RES_PIN          33                                                                  // ESP32-WROVER :IO33(LoRa-RESET)
#define LORA_RES_PRESS        LOW
#define LORA_RES_RELEASE      HIGH
//=================================================================================================
#define I2C_SCL_PIN           22                                                                  // ESP32-WROVER : IO22(SCL1)
#define I2C_SDA_PIN           21                                                                  // ESP32-WROVER : IO21(SDA1)
//=================================================================================================
#define LED_PIN               2                                                                   // ESP-WROVER  : IO2
#define LedON                 1
#define LedOFF                0
//=================================================================================================
#define USER_SW_PIN           36                                                                  // ESP32-WROVER :IO36
#define SW_PRESS              LOW
#define SW_RELEASE            HIGH 
//=================================================================================================
#define RTC_INT_PIN           39                                                                  // ESP32-WROVER :IO39
#define RTC_INT_ACTIVE        LOW
#define RTC_INT_DEACTIVE      HIGH 
//=================================================================================================
// End of Default Hardware : ET-ESP32(WROVER) RS485 V2
//=================================================================================================

//=================================================================================================
#include "ETT_MCP342x.h"
//=================================================================================================
// MCP3423
// CH1 = 4..20mA Sensor
// CH2 = 4..20mA Sensor
// ADR1 = Float, ADR0 = Low : Slave ID = 1101001x = 0x69
// ADR1 = Float, ADR0 = High: Slave ID = 1101101x = 0x6D
//=================================================================================================
ETT_MCP342X mcp3423(0x6D);
//=================================================================================================
int16_t vbat_value;
float vbat_voltage;
float vbat_level;
//=================================================================================================
int16_t sensor_value;
float sensor_voltage;
float sensor_level;
//=================================================================================================
bool mcp3423_status;
//=================================================================================================

//=================================================================================================
unsigned long lastGetI2CSensorTime = 0;
//=================================================================================================

void setup() 
{
  //===============================================================================================
  // Start of Initial Default Hardware : ET-ESP32(WROVER) RS485 V2
  //===============================================================================================
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LedOFF);
  //===============================================================================================
  pinMode(USER_SW_PIN,INPUT_PULLUP);
  pinMode(RTC_INT_PIN,INPUT_PULLUP);
  //===============================================================================================
  Wire.begin(I2C_SDA_PIN,I2C_SCL_PIN);                                                      
  //===============================================================================================
  SerialDebug.begin(115200);
  while(!SerialDebug);
  //===============================================================================================
  SerialRS485.begin(9600, SERIAL_8N1, SerialRS485_RX_PIN, SerialRS485_TX_PIN);
  while(!SerialRS485);
  //===============================================================================================
  // End of Initial Default Hardware : ET-ESP32(WROVER) RS485 V2
  //===============================================================================================

  //===============================================================================================
  SerialDebug.println();
  SerialDebug.println("ET-ESP32(WROVER)RS485 V2.....Ready");
  //===============================================================================================
  
  //===============================================================================================
  SerialDebug.println();
  SerialDebug.println("================================================================");
  SerialDebug.println();
  SerialDebug.println("Initial MCP3423...Start");
  //===============================================================================================
  mcp3423_status = mcp3423.testConnection();
  //===============================================================================================
  SerialDebug.println(mcp3423_status ? "Initial MCP3423...Complete" : "Initial MCP3423...Error");
  //===============================================================================================  
}

void loop() 
{
  if((millis() - lastGetI2CSensorTime) > 1000ul)                                                  // 1-Second
  {
    //=============================================================================================
    // Start of  I2C ADC(MCP3423) Service
    //=============================================================================================
    mcp3423.setConfig(MCP342X_CHANNEL_1 |
                      MCP342X_MODE_CONTINUOUS |
                      MCP342X_SIZE_16BIT |
                      MCP342X_GAIN_1X);
    //=============================================================================================
    SerialDebug.print("CH1 Config = "); 
    SerialDebug.println(mcp3423.getConfigRegShdw(), HEX);                                         // 0x18(0011 1000=0:Update Reg,00:CH1,1:Continue,10:16Bit,00:1X)     
    //=============================================================================================
    mcp3423.startConversion(MCP342X_CHANNEL_1);                                                   // Start Conversion(-32768 to +32767)
    mcp3423.getResult(&vbat_value);      
    //=============================================================================================           
    
    //============================================================================================= 
    vbat_voltage = ((2.048/65535.0) * (float)vbat_value) * 2;
    vbat_level = map(vbat_voltage, 0.0, 1.65, 0.0, 100.0);                                        // 3.5-3.3V -> 0-100%
    //=============================================================================================
    SerialDebug.print("+VBAT measure  = ");
    SerialDebug.print(vbat_value,BIN);
    SerialDebug.print(" VBAT Level = ");
    SerialDebug.print((vbat_voltage*2),3);
    SerialDebug.print(" Volt(");
    SerialDebug.print(vbat_level,1);
    SerialDebug.println("%)");
    //=============================================================================================   
    
    //=============================================================================================
    mcp3423.setConfig(MCP342X_CHANNEL_2 |
                      MCP342X_MODE_CONTINUOUS |
                      MCP342X_SIZE_16BIT |
                      MCP342X_GAIN_1X);
    //============================================================================================= 
    SerialDebug.print("CH2 Config = "); 
    SerialDebug.println(mcp3423.getConfigRegShdw(), HEX);                                         // 0x38(0011 1000=0:Update Reg,01:CH2,1:Continue,10:16Bit,00:1X)   
    //=============================================================================================
    mcp3423.startConversion(MCP342X_CHANNEL_2);                                                   // Start Conversion(-32768 to +32767)
    mcp3423.getResult(&sensor_value);    
    //=============================================================================================                  
    sensor_voltage = (2.048/65535.0) * (float)sensor_value * 2;                                   // 4..20mA : 0.4...2.0V)
    //=============================================================================================  
    SerialDebug.print("Sensor measure = ");
    SerialDebug.print(sensor_value,BIN);  
    SerialDebug.print(" Sensor(4..20mA : 0.4..2.0V) = ");
    SerialDebug.print(sensor_voltage,3);
    SerialDebug.println(" Volt");
    //============================================================================================= 
    //=============================================================================================
    // End of  I2C ADC(MCP3423) Service
    //=============================================================================================

    //=============================================================================================
    lastGetI2CSensorTime = millis();
    //=============================================================================================
  }
}

