/*!
 * @file readACCurrent.
 * @n This example reads Analog AC Current Sensor.

 * @copyright   Copyright (c) 2010 DFRobot Co.Ltd (https://www.dfrobot.com)
 * @licence     The MIT License (MIT)
 * @get from https://www.dfrobot.com

 Created 2016-3-10
 By berinie Chen <bernie.chen@dfrobot.com>

 Revised 2019-8-6
 By Henry Zhao<henry.zhao@dfrobot.com>

 Modified for Aerobic Septic System Monitor beta testing 2024-27-3
 By Terence F. Golla
*/

#define LED_BUILTIN 4
#define AC_CURRENT_SENSOR 35  // Set arduino signal read pin.
#define ACTectionRange 20     // Set Non-invasive AC Current Sensor tection range (5A,10A,20A).

// VREF: Analog Reference
// For Arduino UNO, Leonardo and mega2560, etc. change VREF to 5
// For Arduino Zero, Due, MKR Family, ESP32, etc. 3V3 controllers, change VREF to 3.3
// For a more accurate reading, use a high-precision multimeter to measure the analog reference voltage of the controller (usually the same as the supply voltage). 
// More information refence https://wiki.dfrobot.com/Gravity_Analog_AC_Current_Sensor__SKU_SEN0211_
#define VREF 3.255

float readACCurrentValue()
{
  float ACCurrtntValue = 0;
  float peakVoltage = 0;  
  float voltageVirtualValue = 0;  // Vrms
  for (int i = 0; i < 5; i++)
  {
    peakVoltage += analogRead(AC_CURRENT_SENSOR);   // Read peak voltage.
    delay(1);
  }
  peakVoltage = peakVoltage / 5;   
  voltageVirtualValue = peakVoltage * 0.707;    // Change the peak voltage to the Virtual Value of voltage.

  // The circuit is amplified by 2 times, so it is divided by 2.
  // An ESP32 analog pin has a resolution of 12 bits verses an Arduino with 10 bits. Hence we divide by 4096 instead of 1024. 
  voltageVirtualValue = (voltageVirtualValue / 4096 * VREF ) / 2;  

  ACCurrtntValue = voltageVirtualValue * ACTectionRange;

  return ACCurrtntValue;
}

void setup() 
{
  Serial.begin(115200);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(AC_CURRENT_SENSOR, INPUT);
}

void loop() 
{
  float ACCurrentValue = readACCurrentValue(); // Read AC Current Value.
  Serial.print(ACCurrentValue);
  Serial.println(" A");

  // Pause the next reading for a second and make the LED blink.
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}