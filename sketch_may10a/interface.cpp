#include "Interface.h"
#include "image.h"

Interface::Interface(const char* SSID, const char* PASSWORD) :
  timeClient(ntpUDP, "pool.ntp.org", 3600)
{

  // Initialize time map
  timeMap[0] = zero;
  timeMap[1] = one;
  timeMap[2] = two;
  timeMap[3] = three;
  timeMap[4] = four;
  timeMap[5] = five;
  timeMap[6] = six;
  timeMap[7] = seven;
  timeMap[8] = eight;
  timeMap[9] = nine;

  this->SSID = SSID;
  this->PASSWORD = PASSWORD;

  //Allocate memory for the interface picture (192*296 pixels = 4736 bytes)
  unsigned char *InterfacePicture = (unsigned char *)malloc(4736);
  this->InterfacePicture = InterfacePicture;

  // Check for malloc fail
  if(this->InterfacePicture == NULL)
  {
    Serial.println("Memory Allocation Failed");
  }

  this->InitInterface();
  this->WiFiSetup();
  this->UpdateTime();
}

void Interface::InitInterface()
{
  for(size_t i; i < 4736; i++)
  {
    this->InterfacePicture[i] = 0xFF;
  }
}

void Interface::DrawNumbers(int StartRow, const unsigned char* Number1, const unsigned char* Number2)
{
  // 4736 = Size of picture in bytes
  int a = 0;
  int numberIterator1 = 0;
  int numberIterator2 = 0;

  //Draw first 2 numbers
  for(size_t i = StartRow; i < StartRow+1024; i++)
  {
    
    // First Number
    if(a <= 7)
    {
      this->InterfacePicture[i] = Number1[numberIterator1];
      numberIterator1++;
    }

    // Second Number
    else
    {
      this->InterfacePicture[i] = Number2[numberIterator2];
      numberIterator2++;
    }

    // Change of row, 16 bytes in one row
    if(a == 15)
    {
      a = -1;
    }

    a++;
  }
}

void Interface::DrawRect()
{
  // Draw First 2 numbers
  this->DrawNumbers(0, four, four);

  // Draw dots with a ten row gap
  for(size_t i = 1185; i < (1185+160); i++)
  {
    this->InterfacePicture[i] = dots[i-1185];
  }

  // Draw last 2 numbers
  this->DrawNumbers(1392, one, one);
}

void Interface::WiFiSetup()
{

  Serial.println(this->SSID);
  Serial.println(this->PASSWORD);

  WiFi.begin(this->SSID, this->PASSWORD);

  while ( WiFi.status() != WL_CONNECTED ) {
    delay ( 500 );
    Serial.print ( "." );
  }

  Serial.println("WiFi Connected");
  this->timeClient.begin();
  //Set timezone to GMT+2(FINLAND)
  this->timeClient.setTimeOffset(3600*3);
}

void Interface::UpdateInterfaceNumbers()
{
  // Update the hours
  if(this->hour < 10)
  {
    DrawNumbers(0, zero, timeMap[this->hour]);
  }
  else
  {
    int hour[2] = {0, 0};
    // Points to hour[0]
    int *hourPtr = hour;
    splitInt(hourPtr, this->hour);
    DrawNumbers(0, timeMap[hour[0]], timeMap[hour[1]]);
  }

  //Update the minutes
  if(this->minutes < 10)
  {
    DrawNumbers(1392, zero, timeMap[this->minutes]);
  }
  else
  {
    int minutes[2] = {0, 0};
    // Points to minutes[0]
    int *minutesPtr = minutes;
    splitInt(minutesPtr, this->minutes);
    DrawNumbers(1392, timeMap[minutes[0]], timeMap[minutes[1]]);
  }
}

bool Interface::UpdateTime()
{
  bool changed = false;

  this->timeClient.update();

  // Hours
  int newHour = timeClient.getHours();
  Serial.print(this->hour);

  if(this->hour != newHour)
  {
    this->hour = newHour;
    changed = true;
  }

  // Minutes
  int newMinutes = this->timeClient.getMinutes();
  if(this->minutes != newMinutes)
  {
    this->minutes = newMinutes;
    changed = true;
  }

  Serial.print(":");
  Serial.print(this->minutes);

  Serial.print(":");
  Serial.println(this->timeClient.getSeconds());
  delay(1000);

  // Update drawing if changes to time
  if(changed)
  {
    UpdateInterfaceNumbers();
    return true;
  }
  return false;
}

unsigned char* Interface::GivePicture()
{
  return this->InterfacePicture;
}

void Interface::splitInt(int *arr, int time)
{
  if(time == 0)
  {
    arr[0] = 0;
    arr[1] = 0;
    return;
  }

  // Can be used because if time<10 -> int casting gives 0 
  arr[0] = time/10;
  // Taking modulo 10 of number gives last digit
  arr[1] = time%10;

  Serial.println(arr[0]);
  Serial.println(arr[1]);
}