/*
 * Test program for dual DAC7574s connected to a arduino nano.
 * DAC0 is at address A1A0=00 and DAC1 is at address A1A0=01
 * Connect the DAC7574 channels to the nano's analog inputs as follows:
 * 
 * DAC0         Arduino
 * A (pin1)  -> A0
 * B (pin2)  -> A1
 * C (pin 4) -> 
 * D (pin 5) -> 
 * DAC1         Arduino
 * A (pin1)  -> A2
 * B (pin2)  -> A3
 * C (pin 4) -> 
 * D (pin 5) -> 
 *
 * We're only using 4 analog inputs on the nano so we only use 2 channels
 * from each DAC.
 */

#include <DAC7574.h>

DAC7574 dac0, dac1;

uint16_t val[4] = {0, 1024, 2048, 3072};
int apin[4] = {A0, A1, A2, A3};

void setup() {
  Wire.begin();
  Wire.setClock(400000L);

  // Assuming A1, A0 for DAC0 are 00, and for DAC1 are 01
  dac0.begin(0);
  dac1.begin(1);
  Serial.begin(115200);
  while(!Serial)
  Serial.println("Ready.\n");
}

void loop() {
  char buf[48];
  uint16_t aval[4];
  // Set the 4 dac channels
  dac0.setBuffer(val[0], 0);
  dac0.setBuffer(val[1], 1);
  dac1.setBuffer(val[2], 0);
  dac1.setBuffer(val[3], 1);
  // Can broadcast on either dac0 or dac1 since all dacs on bus respond
  dac0.setBcstSync();
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
