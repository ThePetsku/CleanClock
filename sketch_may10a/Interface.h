#include <unordered_map>
#ifndef __Interface_H
#define __Interface_H

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Arduino.h>
#include <map>

//ToDo: Kirjoita tuhoaja joka vapauttaa pointterit

class Interface
{
  private:

  // Map to link int numbers to the time pngs
  std::map<int, const unsigned char*> timeMap;
  unsigned char *InterfacePicture;
  char days[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

  //WiFi Credentials
  const char* SSID;
  const char* PASSWORD;

  // Online timeclient for this interface object
  NTPClient timeClient;
  WiFiUDP ntpUDP;

  // The up to date time
  int hour;
  int minutes;

  //
  void UpdateWeather();

  void WiFiSetup();

  // Set all pixels white as initiation
  void InitInterface();

  // Draw the wanted numbers starting from the wanted row to the interface picture memory location
  void DrawNumbers(int StartRow, const unsigned char* Number1, const unsigned char* Number2);

  // Updates the numbers on the interface memory location and draws them on the screen
  void UpdateInterfaceNumbers();

  // Modifies the array to include the 2 needed digits.
  // If time == 0, array is {0,0}
  void splitInt(int *arr, int time);

  public:
  // Set up the interface for the first time
  Interface(const char* SSID, const char* PASSWORD);

  // Updates interface with new updated values
  // if the minute or hour has changed from previous will return true 
  // otherwise will return false 
  bool UpdateTime();

  // Test Function to draw a rectrangle to the interface picture
  void DrawRect();

  // Return a pointer to the Interface Picture
  unsigned char* GivePicture();

};

#endif