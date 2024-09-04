#include <stdint.h>
#ifndef __INIT_H
#define __INIT_H

#define SCK_PIN  14
#define MOSI_PIN 13
#define CS_PIN   15
#define RST_PIN  2
#define DC_PIN   4
#define BUSY_PIN 5

#include <Arduino.h>

// Display resolution
#define WIDTH      128
#define HEIGHT     296

#define DEV_Delay_ms(__xms) delay(__xms)

// Set up pins for SPI communication
void GPIOsetup();
// Write command using SPI pins
void writeCommand(uint8_t hex);
// Write DATA using SPI pins
void writeData(uint8_t hex);
// wait until screen is not busy
void BusyWait(void);
// Software reset
void InitReset(void);

void ClearScreen(void);

void Display(const uint8_t *blackimage);
// Write hex to MOSI
void SPIByteWrite(uint8_t data);
#endif