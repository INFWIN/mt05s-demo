#include <OneWire.h>

// OneWire DS18S20, DS18B20, DS1822 Temperature Example
//
// http://www.pjrc.com/teensy/td_libs_OneWire.html
//
// The DallasTemperature library can do all this work for you!
// https://github.com/milesburton/Arduino-Temperature-Control-Library


//#define TEST_MT05S_ONEWIRE_SKIP_ROM             1
//#define TEST_MT05S_ONEWIRE_READ_ROM             1
//#define TEST_MT05S_ONEWIRE_MATCH_ROM            1
//#define TEST_MT05S_ONEWIRE_SEARCH_ROM           1
//#define TEST_MT05S_ONEWIRE_SET_ROMCODE          1
//#define TEST_MT05S_ONEWIRE_READ_POWER_SUPPLY    1
//#define TEST_MT05S_ONEWIRE_SCRATCHPAD_TEST1     1
//#define TEST_MT05S_ONEWIRE_SCRATCHPAD_TEST2     1
#define TEST_MT05S_ONEWIRE_CONVERT_T            1
//#define TEST_MT05S_ONEWIRE_MULTIPLE_SENSOR      1

// Define a 1-Wire signal pin (a 2.0K~5.1K pull-up resistor is necessary)
// Change according to your wiring and connection.
OneWire  ds(15);  

void setup(void) {
  Serial.begin(9600);
}

void loop(void) {
  Serial.println("//-------------------------");
  Serial.println("// Start Testing MT05S     ");
  Serial.println("//-------------------------");
  uint8_t i;

#ifdef TEST_MT05S_ONEWIRE_SKIP_ROM
  //--------------------------------------------
  // Tested - Skip ROM Command 
  // Attention: After a specific ROM command (Read Rom [33h], Match Rom [55H], Skip Rom [CCh]) 
  //            is issued by master device, the sensor starts waiting for a FUNCTION command in 500ms timeout checking period. 
  //            The sensor will discard RESET signal issued by master device in this period, and return to the standby status if timeout, 
  //            to wait another 1-Wire transaction issued by master device.
  //--------------------------------------------
  Serial.print("Test ROM COMMAND- Skip ROM Command [0xCC]: \n");
  byte data[9];
  //Send RESET
  ds.reset();
  //Send ROM Command-Skip ROM
  ds.write(0xCC);
  //Send Function Command-Read Scratchpad
  ds.write(0xBE); 
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  Serial.printf("Scratchpad= ");
  for ( i = 0; i < 9; i++) {
    Serial.printf("%.2X ",data[i]);
  }
  Serial.printf("CrcCalculated=%.2X\n",OneWire::crc8(data, 8));
  Serial.print("\n");
  delay(1000);
 #endif

#ifdef TEST_MT05S_ONEWIRE_READ_ROM
  //--------------------------------------------
  // Tested - Read ROM Command 
  // Attention: After a specific ROM command (Read Rom [33h], Match Rom [55H], Skip Rom [CCh]) 
  //            is issued by master device, the sensor starts waiting for a FUNCTION command in 500ms timeout checking period. 
  //            The sensor will discard RESET signal issued by master device in this period, and return to the standby status if timeout, 
  //            to wait another 1-Wire transaction issued by master device.
  //--------------------------------------------
  Serial.print("Test ROM COMMAND- Read ROM Command [0x33]: \n");
  byte romCodeRead[8];
  byte data[9];

  //Send RESET
  ds.reset();
  //Send ROM Command-Read ROM
  ds.write(0x33);
  for( i = 0; i < 8; i++) {
    romCodeRead[i] = ds.read();
  }
  //Send Function Command-Read Scratchpad
  ds.write(0xBE); 
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  Serial.printf("ROMCODE= ");
  for ( i = 0; i < 8; i++) {
    Serial.printf("%.2X ",romCodeRead[i]);
  }
  Serial.print("\n");

  Serial.printf("Scratchpad= ");
  for ( i = 0; i < 9; i++) {
    Serial.printf("%.2X ",data[i]);
  }
  Serial.printf("CrcCalculated=%.2X\n",OneWire::crc8(data, 8));
  Serial.print("\n");
  delay(1000);
 #endif

#ifdef TEST_MT05S_ONEWIRE_MATCH_ROM
  //--------------------------------------------
  // Tested - Match ROM Command
  // Attention: After a specific ROM command (Read Rom [33h], Match Rom [55H], Skip Rom [CCh]) 
  //            is issued by master device, the sensor starts waiting for a FUNCTION command in 500ms timeout checking period. 
  //            The sensor will discard RESET signal issued by master device in this period, and return to the standby status if timeout, 
  //            to wait another 1-Wire transaction issued by master device.
  //--------------------------------------------
  Serial.print("Test ROM COMMAND- Match ROM Command [0x55]: \n");
  Serial.print("ATTENTION!!!!!!!- To test Match ROM, You have to change ROMCODE defined in Array:romCodeToMatch[8] with your own!\n");
  byte data[9];
  byte romCodeToMatch[8]={ 0x28, 0x00, 0x14, 0x08, 0x0F, 0x00, 0x01, 0x60};
  Serial.print("ROMCODE to Match= ");
  for ( i = 0; i < 8; i++) {
    Serial.printf("%.2X ",romCodeToMatch[i]);
  }
  Serial.print("\n");
 
  //Send RESET
  ds.reset();
  //Send ROM Command-Match ROM
  ds.select(romCodeToMatch);
  //Send Function Command-Read Scratchpad
  ds.write(0xBE); 
  for ( i = 0; i < 9; i++) {
    data[i] = ds.read();
  }

  Serial.printf("Scratchpad= ");
  for ( i = 0; i < 9; i++) {
    Serial.printf("%.2X ",data[i]);
  }
  Serial.printf("CrcCalculated=%.2X\n",OneWire::crc8(data, 8));
  Serial.print("\n");
  delay(1000);
 #endif

#ifdef TEST_MT05S_ONEWIRE_SEARCH_ROM
  //--------------------------------------------
  // Tested - Search ROM Command
  // Attention: 
  //--------------------------------------------
  Serial.print("Test ROM COMMAND- Search ROM Command [0xF0]: \n");
  byte addr[8];

  if ( !ds.search(addr)) {
    Serial.print("No more addresses.\n");
    Serial.print("\n");
    ds.reset_search();
    delay(2000);
    return;
  }
  
  Serial.print("ROM Found=");
  for (uint8_t i = 0; i < 8; i++) {           // we need 8 bytes
    Serial.printf("%.2X ",addr[i]);
  }
  Serial.print("\n");
  Serial.printf("CrcCalculated=%.2X\n",OneWire::crc8(addr, 7));

  if (OneWire::crc8(addr, 7) != addr[7]) {
      Serial.print("CRC is not valid!\n");
      return;
  }
  Serial.print("\n");
 #endif

#ifdef TEST_MT05S_ONEWIRE_SET_ROMCODE
  //--------------------------------------------
  // Tested - Set ROMCODE and check result
  // Attention: 
  //--------------------------------------------
  Serial.print("Test Function Command- Set ROMCODE [0x0F]: \n");
  byte writeResult[2];
  byte romCodeToWrite[8]={ 0x28, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0xFF};
  romCodeToWrite[7]=OneWire::crc8(romCodeToWrite, 7);
  Serial.print("ROM To Set=");
  for( i = 0; i < 8; i++) {
    Serial.printf("%.2X ",romCodeToWrite[i]);
  }
  Serial.print("\n");
  
  ds.reset();//Send RESET
  ds.write(0xCC);//Send ROM Command-Skip ROM
  ds.write(0x0F);//function command- Set RomCode
  ds.write(0xF0);//Flag data(Fixed)
  ds.write(0x00);//Flag data(Fixed)
  ds.write(romCodeToWrite[0]);//new romcode
  ds.write(romCodeToWrite[1]);
  ds.write(romCodeToWrite[2]);
  ds.write(romCodeToWrite[3]);
  ds.write(romCodeToWrite[4]);
  ds.write(romCodeToWrite[5]);
  ds.write(romCodeToWrite[6]);
  ds.write(romCodeToWrite[7]);
  delay(100);//Important!! Delay 100ms for eeprom write done
  writeResult[0] = ds.read();
  writeResult[1] = ds.read();
  if(writeResult[0]==0x00 && writeResult[1]==0x03)
  {
    Serial.printf("SET ROMCODE SUCCESSFULLY[%.2X,%.2X]\n",writeResult[0],writeResult[1]);
  }
  else if(writeResult[0]==0x00 && writeResult[1]==0xC0)
  {
    Serial.printf("SET ROMCODE FAILED[%.2X,%.2X]\n",writeResult[0],writeResult[1]);
  }
  else
  {
    Serial.printf("SET ROMCODE FAILED: Unknown Reason[%.2X,%.2X]\n",writeResult[0],writeResult[1]);
  }
  delay(1000);
#endif

#ifdef TEST_MT05S_ONEWIRE_READ_POWER_SUPPLY
  //--------------------------------------------
  // Tested - READ POWER SUPPLY
  // Attention: The MT05S sensor only support External Power Supply
  //--------------------------------------------
  Serial.print("Test Function Command- READ POWER SUPPLY [0xB4]: \n");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0xb4);//Send Function command- Read Power Supply
	if (ds.read_bit() == 0)
  {
    Serial.print("Parasite Power");
  }
  else
  {
    Serial.print("External Power");
  }
  Serial.println();
  delay(1000);
#endif

#ifdef TEST_MT05S_ONEWIRE_SCRATCHPAD_TEST1
  //--------------------------------------------
  // Tested -Write Scratchpad, Copy Scratchpad and then Read Scratchpad
  // Attention: 
  //--------------------------------------------
  Serial.print("Test Function Command- -Write Scratchpad[0x4E], Copy Scratchpad[0x48] , Recall EE[0xB8], and Read Scratchpad [0xBE]: \n");
  byte config0=0x08,config1=0x00,configDummy=0x00;
  byte scratchpadRead[9];

  //Write Scratchpad
  Serial.print("Write Scratchpad [0x4E]\n");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0x4E);//Send Function command- write scratchpad
  ds.write(config0);//write 3 byte to sensor(including a dummy byte)
  ds.write(config1);
  ds.write(configDummy);

  delay(10);//Sometime for sensor to finish the operation.

  //Copy Scratchpad
  Serial.print("Copy Scratchpad [0x48]\n");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0x48);//Send Function command-Copy scratchpad

  delay(10);//Sometime for sensor to finish the operation.

  //Recall EE
  Serial.print("Recall EE [0xB8]: ");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0xB8);//Send Function command-Recall EE
  while(ds.read_bit() == 0)//Wait for operation done
  {
    delay(10);
    Serial.print(".");
  }
	Serial.print("Done\n");

  //Read Scratchpad
  Serial.print("Read Scratchpad [0xBE]: ");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0xBE);//Send Function command- read scratchpad
  for ( int i = 0; i < 9; i++) { 
    scratchpadRead[i] = ds.read();
  }
  for ( int i = 0; i < 9; i++) { 
    Serial.printf("%.2X ",scratchpadRead[i]);
  }
  Serial.printf("CrcCalculated=%.2X\n",OneWire::crc8(scratchpadRead, 8));
  Serial.println();
  delay(1000);
#endif


#ifdef TEST_MT05S_ONEWIRE_SCRATCHPAD_TEST2
   //--------------------------------------------
  // Tested - Write Scratchpad and Read Scratchpad, then Recall EE and Read Scratchpad
  // Attention: 
  //--------------------------------------------
  Serial.print("Test Function Command- Write Scratchpad [0x4E]  & Read Scratchpad [0xBE]: \n");
  byte config0=0x08,config1=0x00,configDummy=0x00;
  byte scratchpadRead[9];

  //Write Scratchpad
  Serial.print("Write Scratchpad [0x4E]\n");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0x4E);//Send Function command- write scratchpad
  ds.write(config0);//write 3 byte to sensor(including a dummy byte)
  ds.write(config1);
  ds.write(configDummy);

  delay(10);//Sometime for sensor to finish the write scratchpad operation.

  //Read Scratchpad
  Serial.print("Read Scratchpad [0xBE]: ");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0xBE);//Send Function command- read scratchpad
  for ( int i = 0; i < 9; i++) { 
    scratchpadRead[i] = ds.read();
  }
  for ( int i = 0; i < 9; i++) { 
    Serial.printf("%.2X ",scratchpadRead[i]);
  }
  Serial.printf("CrcCalculated=%.2X",OneWire::crc8(scratchpadRead, 8));
  Serial.println();

  //Recall EE
  Serial.print("Recall EE [0xB8]: ");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0xB8);//Send Function command- Recall EE
  while(ds.read_bit() == 0)//Wait for operation done
  {
    delay(10);
    Serial.print(".");
  }
	Serial.print("Done\n");

  //Read Scratchpad
  Serial.print("Read Scratchpad [0xBE]: ");
  ds.reset();//Send RESET
  ds.write(0xcc);//Send ROM Command-Skip ROM
  ds.write(0xBE);//Send Function command- read scratchpad
  for ( int i = 0; i < 9; i++) { 
    scratchpadRead[i] = ds.read();
  }
  for ( int i = 0; i < 9; i++) { 
    Serial.printf("%.2X ",scratchpadRead[i]);
  }
  Serial.printf("CrcCalculated=%.2X",OneWire::crc8(scratchpadRead, 8));
  Serial.println();

  delay(1000);
#endif


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
    Serial.printf("%.2X ",scratchpad[i]);
  }
  Serial.printf("CrcCalculated=%.2X",OneWire::crc8(scratchpad, 8));
  Serial.println();

  if(OneWire::crc8(scratchpad, 8) == scratchpad[8])
  {
    int16_t temperature = makeWord(scratchpad[0],scratchpad[1]);
    int16_t moisture = makeWord(scratchpad[2],scratchpad[3]);
    int16_t conductivity = makeWord(scratchpad[4],scratchpad[5]);
    Serial.printf("TEMP=%.2f, MOISTURE=%.2f, CONDUCTIVITY=%d, CONFIG0=%.2X, CONFIG1=%.2X, CRC8=%.2X\n", temperature/100.00, moisture/100.00, conductivity, scratchpad[6],scratchpad[7],scratchpad[8] );
  }
  else
  {
    Serial.print("CRC ERROR!\n");
  }

  Serial.println();
  delay(1000);
#endif

#ifdef TEST_MT05S_ONEWIRE_MULTIPLE_SENSOR
  //--------------------------------------------
  // Tested - 2 Sensor on line, Search ROM and conversion, Read Scratchpad
  // Attention: 
  //--------------------------------------------
  Serial.print("Test - 2 Sensor on line, Search ROM and conversion, Read Scratchpad: \n");
  byte romCodeSearched[8];
  byte scratchpad[9];
  if ( !ds.search(romCodeSearched)) {
    Serial.println("No more addresses.");
    Serial.println();
    ds.reset_search();
    delay(250);
    return;
  }
  
  Serial.print("ROM Read=");
  for ( int i = 0; i < 8; i++) {           // we need 8 bytes
    Serial.printf("%.2X ",romCodeSearched[i]);
  }
  Serial.println();
  Serial.printf("CrcCalculated=%.2X",OneWire::crc8(romCodeSearched, 7));
  Serial.println();

  if (OneWire::crc8(romCodeSearched, 7) != romCodeSearched[7]) {
      Serial.println("CRC is not valid!");
      return;
  }
  Serial.println();

  Serial.print("Convert T : ");
  ds.reset();//Send RESET
  ds.select(romCodeSearched);//Send ROM Command - Match ROM
  ds.write(0x44);//Send Function command- convert T
  while(ds.read_bit() == 0)
  {
    delay(10);
    Serial.print(".");
  }
	Serial.print("Done");
  Serial.print("\n");

  Serial.print("Read Scratchpad : ");
  ds.reset();//Send RESET
  ds.select(romCodeSearched);//Send ROM Command - Match ROM
  ds.write(0xBE);//Send Function command- Read Scratchpad
  for (int i = 0; i < 9; i++) {           // we need 9 bytes
    scratchpad[i] = ds.read();
    Serial.printf("%.2X ",scratchpad[i]);
  }
  Serial.printf("CrcCalculated=%.2X",OneWire::crc8(scratchpad, 8));
  Serial.println();
  delay(1000);
#endif

}