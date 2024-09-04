#include <stdint.h>
#include <Arduino.h>
#include "init.h"

void GPIOsetup()
{
    pinMode(BUSY_PIN,  INPUT);
    pinMode(RST_PIN , OUTPUT);
    pinMode(DC_PIN  , OUTPUT);
    
    pinMode(SCK_PIN, OUTPUT);
    pinMode(MOSI_PIN, OUTPUT);
    pinMode(CS_PIN , OUTPUT);

    digitalWrite(CS_PIN , HIGH);
    digitalWrite(SCK_PIN, LOW);

    Serial.begin(115200);
}

void SPIByteWrite(uint8_t data)
{
  digitalWrite(CS_PIN, 0);

    // Iterate through the 8 bits in the byte
    for (int i = 0; i < 8; i++)
    {
        // Set MOSI pin to correct state
        // KOSKA "&" MUODOSTAA YKKÖSEN VAIN JOS MOLEMMAT OVAT 1 ja koska 0x80=10000000 tarkastellaan vain most significant bittiä!
        if ((data & 0x80) == 0) digitalWrite(MOSI_PIN, 0); 
        else                    digitalWrite(MOSI_PIN, 1);

        //Bitshift to iterate through the BYTE
        data <<= 1;

        //Tick clock to send MOSI state to slave (The 1 or the 0)
        digitalWrite(SCK_PIN, 1);     
        digitalWrite(SCK_PIN, 0);
    }

    //SPI.transfer(data);
    digitalWrite(CS_PIN, 1);
    //SPI.endTransaction();	
}

void writeCommand(uint8_t hex)
{
  // Pin 0 to let device know command coming
  digitalWrite(DC_PIN, 0);
  // Activate transmission
  SPIByteWrite(hex);
  // End Transmission

}

void writeData(uint8_t hex)
{
  // Pin 1 to let device know DATA coming
  digitalWrite(DC_PIN, 1);
  // Activate transmission
  SPIByteWrite(hex);
  // End Transmission
}

void BusyWait()
{
    Serial.println("e-Paper busy\r\n");
    while(digitalRead(BUSY_PIN) == 0) {      //LOW: idle, HIGH: busy
        DEV_Delay_ms(100);
    }
    Serial.println("e-Paper release\r\n");
}

void InitReset()
{
  //Activate reset
  digitalWrite(RST_PIN, HIGH);
  DEV_Delay_ms(200);
  digitalWrite(RST_PIN, LOW);
  DEV_Delay_ms(5);
  digitalWrite(RST_PIN, HIGH);
  DEV_Delay_ms(200);
}

void ClearScreen()
{
  uint16_t Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);
  uint16_t Height = HEIGHT;

    //Set black data to white
    writeCommand(0x10);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            writeData(0xFF);
        }
    }

    //Set red data to white
    writeCommand(0x13);
    for (uint16_t j = 0; j < Height; j++) {
        for (uint16_t i = 0; i < Width; i++) {
            writeData(0xFF);
        }
    }
    
    writeCommand(0x12);
    BusyWait();
}

void Display(const uint8_t *image)
{
    uint16_t Width, Height;
    Width = (WIDTH % 8 == 0)? (WIDTH / 8 ): (WIDTH / 8 + 1);
    Height = HEIGHT;

    //Display start, Transmission 1
    writeCommand(0x10);

    // Send pixels
    for (uint16_t i = 0; i < 4737; i++)
    {
      writeData(image[i]);
    }

    // Original driver has this, why?
    //writeCommand(0x92);

    writeCommand(0x12);
    BusyWait();
}