/*
 * Test program for DAC7574 connected to a arduino nano.
 * Connect the DAC7574 four channels to the nano's analog inputs as follows:
 * 
 * DAC           Arduino
 * A (pin1)  -> A0
 * B (pin2)  -> A1
 * C (pin 4) -> A2
 * D (pin 5) -> A3
 */

#include <Wire.h>
#include <DAC7574.h>

DAC7574 dac;

uint16_t val[4] = {0, 1024, 2048, 3072};
int apin[4] = {A0, A1, A2, A3};

void setup() {
  Wire.begin();
  Wire.setClock(400000L);

  // Assuming A1, A0 for the device are 00
  dac.begin(0); 
  Serial.begin(115200);
  while(!Serial)
  Serial.println("Ready.\n");
}

void loop() {
  char buf[48];
  uint16_t aval[4];
  // Set the 4 dac channels synchronously
  dac.setDataAll(val[0], val[1], val[2], val[3]);
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
