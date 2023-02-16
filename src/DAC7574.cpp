/*
  Arduino library for DAC7574, a 4-channel 12-bit DAC from TI with an I2C
  interface. Each channel can be set individually.
  
  The part also has a temporary data buffer for each channel that can be
  loaded ahead of time and then all 4 channels can be synchronously updated.
  
  Furthermore, the part supports a broadcast write which allows up to 4
  DAC7574 devices on the same   I2C bus to be synchronously updated.
 */

#include "Arduino.h"
#include "DAC7574.h"

DAC7574::DAC7574() {
}

// i2caddr need only be the two address bits for the part (0 .. 3).
// twire defaults to a pointer to Wire, but it can be a pointer to a
// different TwoWire instance, such as Wire1, in a system with more than
// one i2c bus.
void DAC7574::begin(uint8_t i2caddr, TwoWire *twire) {
  _i2caddr = DAC7574_I2CADDR | (i2caddr & 0x3);
  _wire = twire;
}

// Common code to write to control register and then data
void DAC7574::_writeCtrlData(uint8_t ctrl, uint16_t data) {
  _wire->beginTransmission(_i2caddr);
  _wire->write(ctrl);
  _wire->write(data >> 4); // Data[11:4]
  _wire->write(data << 4); // Data[3:0],X X X X
  _wire->endTransmission();
}

// Common code to broadcast to control register and then data
void DAC7574::_writeBcstData(uint8_t ctrl, uint16_t data) {
  _wire->beginTransmission(DAC7574_I2CBCST);
  _wire->write(ctrl);
  _wire->write(data >> 4); // Data[11:4]
  _wire->write(data << 4); // Data[3:0],X X X X
  _wire->endTransmission();
}

// directly set channel to data
void DAC7574::setData(uint16_t data, uint8_t channel) {
  // force channel into 2 bits in case channel is out of range
  channel = channel & 0x3;
  uint8_t ctrl = DIR_STORE | channel << 1 | PWR_NORMAL;
  _writeCtrlData(ctrl, data);
}

// set channel temporary buffer to data
void DAC7574::setBuffer(uint16_t data, uint8_t channel) {
  // force channel into 2 bits in case channel is out of range
  channel = channel & 0x3;
  uint8_t ctrl = BUF_STORE | channel << 1 | PWR_NORMAL;
  _writeCtrlData(ctrl, data);
}

// directly set channel to data and synchronously update the other 3 channels
// with the values stored in their temporary buffers
void DAC7574::setDataSync(uint16_t data, uint8_t channel) {
  // force channel into 2 bits in case channel is out of range
  channel = channel & 0x3;
  uint8_t ctrl = SYNC_STORE | channel << 1 | PWR_NORMAL;
  _writeCtrlData(ctrl, data);
}

// Synchronously set all 4 channel voltages
void DAC7574::setDataAll(uint16_t d0, uint16_t d1, uint16_t d2, uint16_t d3) {
  setBuffer(d0, 0);
  setBuffer(d1, 1);
  setBuffer(d2, 2);
  setDataSync(d3, 3);
}

// Broadcast to sync all DACs channels with buffer data
void DAC7574::setBcstSync() {
  uint8_t ctrl = BCST_UPDT | PWR_NORMAL;
  _writeBcstData(ctrl, 0);
}
