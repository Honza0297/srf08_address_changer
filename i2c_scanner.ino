 // --------------------------------------
// i2c_scanner and SRF08 address changer
//
// Original code: https://playground.arduino.cc/Main/I2cScanner/
// 
// Author of the remix: Jan Beran
//
// NOTE: SRF08 used 8bit address, Arduino works with 7bit ones -> it discards the lowest bit
//       Example: 0xE8 (real SRF08 address) -> 0x74 (what will Arduino see)

#include <Wire.h>
int new_address = 0xE8; // front, 0x74
//int new_address = 0xE4; // front_right, 0x72
//int new_address = 0xE0; // back_right, 0x70
//int new_address = 0xE6; // back, 0x73
//int new_address = 0xEA; //back_left, 0x75
//int new_address = 0xE2; //front_left, 0x71




void change_address(int dst_addr)
{
  Wire.beginTransmission(dst_addr);
    Wire.write(0x00);
    Wire.write(0xA0);
    Wire.endTransmission();
    delay(60);
    
    Wire.beginTransmission(dst_addr);
    Wire.write(0x00);
    Wire.write(0xAA);
    Wire.endTransmission();
    delay(60);
    
    Wire.beginTransmission(dst_addr);
    Wire.write(0x00);
    Wire.write(0xA5);
    Wire.endTransmission();
    delay(60);
    
    Wire.beginTransmission(dst_addr);
    Wire.write(0x00);
    Wire.write(new_address);
    Wire.endTransmission();
}

void setup()
{
  Wire.begin();

  Serial.begin(9600);
  while (!Serial);             // Leonardo: wait for serial monitor
  Serial.println("\nI2C Scanner");
}

bool done = false;
void loop()
{
  byte error, address;
  int nDevices;

  Serial.println("Scanning...");

  nDevices = 0;
  for(address = 1; address < 127; address++ ) 
  {
    // The i2c_scanner uses the return value of
    // the Write.endTransmisstion to see if
    // a device did acknowledge to the address.
    Wire.beginTransmission(address);
    error = Wire.endTransmission();

    if (error == 0)
    {
      Serial.print("I2C device found at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println("  !");
      if(!done)
      {
        change_address(address);
        done = true;
      }
      
      
      nDevices++;
    }
    else if (error==4) 
    {
      Serial.print("Unknown error at address 0x");
      if (address<16) 
        Serial.print("0");
      Serial.println(address,HEX);
    }    
  }
  if (nDevices == 0)
    Serial.println("No I2C devices found\n");
  else
    Serial.println("done\n");

  delay(5000);           // wait 5 seconds for next scan
}
