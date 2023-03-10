/*
 * Test program for DAC7574 connected to a arduino nano.
 * Connect the DAC7574 four channels to the nano's analog inputs as follows:
 * Start the 4 channels at different values and increment each of them 
 * every loop cycle. 
 * Read them back on 4 analog input pins.
 * Print the values output on the 4 DAC channels and the values input from the  
 * corresponding 4 analog pins.
 * 
 * NOTE: Ardfuino nano ADC has a 10-bit resolution, other microcontrollers
 * have analog inputs with highe resolution.
 * 
 * DAC           Arduino
 * A (pin1)  -> A0
 * B (pin2)  -> A1
 * C (pin4)  -> A2
 * D (pin5)  -> A3
 */

#include <DAC7574.h>

// If using an I2C bus that is different from default Wire,
// for example SDA2 = PB3, SCL2 = PB10 on STM32F411-based "black pill"
// TwoWire Wire2(PB3, PB10);

DAC7574 dac;

uint16_t val[4] = {0, 1024, 2048, 3072}; 
int apin[4] = {A0, A1, A2, A3};

void setup() {
  Wire.begin();
  Wire.setClock(400000L);
  
  // If using Wire2
  // Wire2.begin();
  // Wire2.setClock(400000L);
  
  // Where available, e.g on STM32 "blue pill" or "black pill", enable 12-bit resolution
  // analogReadResolution(12);

  // Assuming A1, A0 for the part are 0, can be 0..3
  dac.begin(0);
  // If using Wire2
  // dac.begin(0, &Wire2);
  Serial.begin(115200);
  while(!Serial)
  Serial.println("Ready.\n");
}

void loop() {
  char buf[48];
  uint16_t aval[4];
  // Set the 4 dac channels
  for (uint8_t i = 0; i < 4; i++) {
    dac.setData(val[i], i);
  }
  // Read the 4 adc channels
  for (uint8_t i = 0; i < 4; i++) {
    aval[i] = analogRead(apin[i]);
  }
  
  sprintf(buf, "%d %d %d %d : %d %d %d %d",
            val[0], val[1], val[2], val[3],
            aval[0], aval[1], aval[2], aval[3]);
  Serial.println(buf);
  // increment the vals
  for (uint8_t i = 0; i < 4; i++) {
    val[i] = val[i] + 1;
    if (val[i] >= 4096) {
      val[i] = 0;
    }
  }
  // delay(1);
}
