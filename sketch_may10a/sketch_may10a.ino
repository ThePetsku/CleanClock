#include "init.h"
#include "image.h"
#include "Interface.h"

Interface* clockInterface = (nullptr);

void setup() {
  // Wifi Information
  //Update these to match your WIFI credentials
  const char* SSID = "XXX";
  const char* PASSWORD = "XXX";


  // Set up GPIO pins for SPI
  GPIOsetup();

  clockInterface = new Interface(SSID, PASSWORD);

  //TESTING START
  //clockInterface->DrawRect();
  
  //TESTING STOP
  
  InitReset();
  // Power ON
  writeCommand(0x04);
  BusyWait();
  
  writeCommand(0x00);//panel setting
  writeData(0x0f);//LUT from OTP，128x296
  writeData(0x89);//Temperature sensor, boost and other related timing settings

  writeCommand(0x61);//resolution setting
  writeData (0x80);
  writeData (0x01);
  writeData (0x28);

  writeCommand(0X50);//VCOM AND DATA INTERVAL SETTING			
  writeData(0x77);
  
  ClearScreen();

  Serial.println("Setup Complete");

  // Display the first time on screen
  Display(clockInterface->GivePicture());
}

void loop() {
  
  // Update interface
  // Draw Interface
  // Set ESP to sleep for a minute
  if(clockInterface->UpdateTime())
  {
    Display(clockInterface->GivePicture());
  }
  

}
