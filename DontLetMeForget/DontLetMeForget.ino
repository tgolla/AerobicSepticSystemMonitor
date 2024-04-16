#include "Wire.h"
#include "uEEPROMLib.h"

// uEEPROMLib eeprom;
uEEPROMLib eeprom(0x57);

void setup() {
  Serial.begin(115200);
  delay(2500);

  Wire.begin();

  int inttmp = 32123;
  float floattmp = 3.1416;
  char chartmp = 'A';
  char c_string[] = "lastminuteengineers.com"; //23
  int string_length = strlen(c_string);

  Serial.println("Writing into memory...");
  
  // Write single char at address. 
  if (!eeprom.eeprom_write(8, chartmp)) {
  Serial.println("Failed to store char.");
  } else {
  Serial.println("char was stored correctly.");
  }

  // Write a long string of chars FROM position 33 which isn't aligned to the 32 byte pages of the EEPROM.
  if (!eeprom.eeprom_write(33, (byte *) c_string, strlen(c_string))) {
  Serial.println("Failed to store string.");
  } else {
  Serial.println("string was stored correctly.");
  }

  // Write an int.
  if (!eeprom.eeprom_write(0, inttmp)) {
    Serial.println("Failed to store int.");
  } else {
    Serial.println("int was stored correctly.");
  }

  // write a float.
  if (!eeprom.eeprom_write(4, floattmp)) {
    Serial.println("Failed to store float.");
  } else {
    Serial.println("float was stored correctly.");
  }

  Serial.println("");
  Serial.println("Reading memory...");
  
  Serial.print("int: ");
  eeprom.eeprom_read(0, &inttmp);
  Serial.println(inttmp);

  Serial.print("float: ");
  eeprom.eeprom_read(4, &floattmp);
  Serial.println((float) floattmp);

  Serial.print("char: ");
  eeprom.eeprom_read(8, &chartmp);
  Serial.println(chartmp);

  Serial.print("string: ");
  eeprom.eeprom_read(33, (byte *) c_string, string_length);
  Serial.println(c_string);

  Serial.println();
}

void loop() {
}