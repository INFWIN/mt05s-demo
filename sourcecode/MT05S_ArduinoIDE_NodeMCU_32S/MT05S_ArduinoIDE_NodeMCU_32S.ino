#include <OneWire.h>

#define TEST_MT05S_ONEWIRE_CONVERT_T            1

// Define a 1-Wire signal pin (a 2.0K~5.1K pull-up resistor is necessary)
// Change according to your wiring and connection.
OneWire  ds(2);  

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  Serial.println("//-------------------------");
  Serial.println("// Start Testing MT05S     ");
  Serial.println("//-------------------------");
  uint8_t i;

#ifdef TEST_MT05S_ONEWIRE_CONVERT_T
  //--------------------------------------------
  // Tested - Convert T and Read Scratchpad
  // Attention: 
  //--------------------------------------------
  Serial.print("Test Function Command- Convert T [0x44]  & Read Scratchpad [0xBE]: \n");
  byte scratchpad[9];

  Serial.print("Convert T [0x44]: ");
  ds.reset();//Send RESET
  ds.skip();//Send ROM Command-Skip ROM
  ds.write(0x44);//Send Function command- convert T

  //Wait conversion done (Whether to pull down the DQ line during operation can be configured in scratchpad CONFIG0-Bit3- SensorPowerMode)
  while(ds.read_bit() == 0)
  {
    delay(10);
    Serial.print(".");
  }
  Serial.print("Conversion Done\n");

  Serial.print("Read Scratchpad [0xBE]: ");
  ds.reset();//Send RESET
  ds.skip();//Send ROM Command-Skip ROM
  ds.write(0xBE);//Send Function command- Read Scratchpad
  for (int i = 0; i < 9; i++) {// we need 9 bytes
    scratchpad[i] = ds.read();
    Serial.print("%.2X ",scratchpad[i]);
  }
  Serial.print("CrcCalculated=%.2X",OneWire::crc8(scratchpad, 8));
  Serial.println();
  delay(1000);
#endif
}
