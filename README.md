# DAC7574
Arduino library for DAC7574 quad 12-bit digital to analog converter with I2C interface

[TI Part info](https://www.ti.com/product/DAC7574)

## Simple Usage
The simplest use case is when each channel is individually programmed using `dac.setData(v, channel)`.  This can be seen in the example `dac7574_nano_test`.

Declare an instance of the part using `DAC7574 dac`.
Initialize the default TwoWire instance Wire by calling `Wire.begin()` and optionally setting it's speed by calling `Wire.setClock(400000L)`.
Use `dac.begin(address)` to initialize the instance to use the I2C `address` and the default I2C channel, `Wire`. 
Note that only two bits of address corresponding to the settings of the A0, A1 pins for the part are required (i.e., `address` is in 0..3). The remaining 5 bits of the 7-bit I2C address are constant and are provided by the library.


## Synchronized Channel Setting
The `dac7574_nano_sync` example shows all four channels programmed synchronously using the `dac.setDataAll(v0, v1, v2, v3)` so that their outputs change simultaneously.

You can also do this using the `dac.setBuffer(v, channel)` to store the values of 3 channels individually to buffers in the part first and then using the `dac.setDataSync9v, channel)` function to set the value of the 4th channel and simultaneouly loading the previously programmed buffer values for the remaining 3 channels.

## Synchronized Setting on Multiple DAC7574 parts
The `dac7574_dual_nano` example shows how 2 DAC7574 parts on the same I2C bus can be programmed synchronously so that all 8 values change simultaneouly.
This is done using `dacx.setBuffer(v, channel)` for each of the 4 channels on both parts to set their buffer values, and then using `dac0.setBcstSync()` to broadcast the synchronizatoin command to both parts.  Note that you only need to call `setBcstSync()` on any one of the parts.

## Using Other Wire Channels
The Arduino Wire library declares a global instance of `TwoWire` named `Wire` and this library uses that by default.
But if you plan to use a different instance of TwoWire, you can pass that instance in when you call `dac.begin(address, twire)`.

For example, to use the 2nd I2C bus on a STM32F411-based "Black Pill" board, use:

`TwoWire Wire2(PB3, PB10); // SDA2 is PB3, SCL2 is PB10`

And in setup() call:

`Wire2.begin();`

`dac.begin(0, &Wire2); // A1,A0 = 0, can be 0..3`
