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
  }
  for (int i = 0; i < 9; i++) {// we need 9 bytes
    Serial.print(scratchpad[i],HEX);
    Serial.print(" ");
  }
  Serial.print(" CrcCalculated=");
  Serial.print(OneWire::crc8(scratchpad, 8),HEX);
  Serial.println();

  if(OneWire::crc8(scratchpad, 8) == scratchpad[8])
  {
    int16_t temperature = makeWord(scratchpad[0],scratchpad[1]);
    int16_t moisture = makeWord(scratchpad[2],scratchpad[3]);
    int16_t conductivity = makeWord(scratchpad[4],scratchpad[5]);
    Serial.print("TEMP(C) = "); Serial.print(temperature/100.00);
    Serial.print("  MOISTURE(%) = "); Serial.print(moisture/100.00);
    Serial.print("  CONDUCTIVITY(ms/cm) = "); Serial.print(conductivity/1000.00);
    Serial.print("  CONFIG0 = "); Serial.print(scratchpad[6],HEX);
    Serial.print("  CONFIG1 = "); Serial.print(scratchpad[7],HEX);
    Serial.print("  CRC8 = "); Serial.print(scratchpad[8],HEX);
  }
  else
  {
    Serial.print("CRC ERROR!");
  }
  Serial.print("\n\n");
  
  delay(1000);
#endif
}