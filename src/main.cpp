#include <Arduino.h>
#include <WiFi.h>
#include <Audio.h>
#include <Wire.h>
#include <SparkFun_VL6180X.h>
#include <SPI.h>
#include <SD.h>

#include "config.h"

// Define pins for audio board (MAX98357A)
#define MAX98357A_DOUT 25
#define MAX98357A_BCLK 27
#define MAX98357A_LRC 26

// Define pins for VL6180X
#define VL6180X_SCL 22
#define VL6180X_SDA 21

// Define pins for micro SD card adapter
#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

// Audio board
Audio audio;

// VL6180X sensor
//VL6180x vl6180x();

// Method for printing all the files in the SD card
void printDirectory(File dir, int numTabs)
{
  while (true)
  {
    File entry = dir.openNextFile();
    if (!entry)
    {
      // no more files
      break;
    }
    for (uint8_t i = 0; i < numTabs; i++)
    {
      Serial.print('\t');
    }
    Serial.print(entry.name());
    if (entry.isDirectory())
    {
      Serial.println("/");
      printDirectory(entry, numTabs + 1);
    }
    else
    {
      // files have sizes, directories do not
      Serial.print("\t\t");
      Serial.println(entry.size(), DEC);
    }
    entry.close();
  }
}


void setup()
{
    Serial.begin(115200);

    // Print the list of files in the SD card
    Serial.println("Initializing SD card...");
    if (!SD.begin(SD_CS))
    {
        Serial.println("ERROR - SD card initialization failed!");
        return;
    }

    File root = SD.open("/");
    printDirectory(root, 0);
    root.close();

}

void loop()
{
}
