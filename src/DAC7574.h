/*
  Arduino library for DAC7574, a 4-channel 12-bit DAC from TI with an I2C
  interface. Each channel can be set individually.
  
  The part also has a temporary data buffer for each channel that can be
  loaded ahead of time and then all 4 channels can be synchronously updated.
  
  Furthermore, the part supports a broadcast write which allows up to 4
  DAC7574 devices on the same   I2C bus to be synchronously updated.

 */
#ifndef DAC7574_h
#define DAC7574_h
#include "Arduino.h"
#include <Wire.h>

// Base I2C address
#define DAC7574_I2CADDR 0x4C
// I2C broadcast address
#define DAC7574_I2CBCST 0x48

// Definitions for the control byte.
// OR the various bits together to construct the byte
// MSB 7:4 bits
#define BUF_STORE 0x00 // Store data to temporary buffer
#define DIR_STORE 0x10 // Store data directly to selected DAC
#define SYNC_STORE 0x20 // Store data directly to selected DAC,
                        // synchronously update others from buffers
#define BCST_UPDT 0x30 // Broadcast update
// LSB 3:1 bits. Shouldn't need these,
// since we get the bits by channel << 1, if channel in 0..3
#define CHAN_A 0x00
#define CHAN_B 0x02
#define CHAN_C 0x04
#define CHAN_D 0x06
// LSB 0 bit
#define PWR_NORMAL 0x00
#define PWR_DOWN 0x01

// Call Wire.begin() and optionally Wire.setClock() before using this class.

class DAC7574{
 public:
  DAC7574();

  // i2caddr need only be the two address bits for the part (0 .. 3).
  // The second argument (twire) is optional and defaults to a pointer to
  // the standard i2c instance Wire.
  // If there are multiple I2C channels, you can pass in a pointer to a
  // a non-standard one (e.g, Wire1)
  void begin(uint8_t i2caddr, TwoWire *twire = &Wire);
  
  // directly set channel to data
  void setData(uint16_t data, uint8_t channel);
  
  // set channel temporary buffer to data
  void setBuffer(uint16_t data, uint8_t channel);
  
  // directly set channel to data and synchronously update the other 3 channels
  // with the values stored in their temporary buffers
  void setDataSync(uint16_t data, uint8_t channel);

  // Synchronously set all four channel voltages
  void setDataAll(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3);

  // Broadcast to sync all DACs channels with buffer data
  void setBcstSync();

 private:
  TwoWire *_wire;
  // XXX I think _i2caddr ought to be uint8_t but that gives
  // a compiler warning because it's apparently declared an int in TwoWire
  int _i2caddr;
  void _writeCtrlData(uint8_t ctrl, uint16_t data);
  void _writeBcstData(uint8_t ctrl, uint16_t data);
};

#endif // #ifndef DAC7574_h
