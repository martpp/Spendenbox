#include <Arduino.h>
#include <Audio.h>
#include <SD.h>
#include <SPI.h>
#include <SparkFun_VL6180X.h>
#include <WiFi.h>
#include <Wire.h>

#include "config.h"

// Define pins for audio board (MAX98357A)
#define MAX98357A_DOUT 25
#define MAX98357A_BCLK 27
#define MAX98357A_LRC 26

// Definitions for VL6180X
#define VL6180X_SCL 22
#define VL6180X_SDA 21
#define VL6180X_ADDRESS 0x29

// Define pins for micro SD card adapter
#define SD_CS 5
#define SD_MOSI 23
#define SD_MISO 19
#define SD_SCK 18

// VL6180X sensor
VL6180x sensor(VL6180X_ADDRESS);

// Audio board
Audio audio;

// Method to print the measured distance of the VL6180X sensor
void printDistance()
{
    Serial.print("Distance (mm): ");
    Serial.println(sensor.getDistance());
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
    // Print files in the root directory
    while (true)
    {
        File entry = root.openNextFile();
        if (!entry)
        {
            // No more files
            break;
        }
        Serial.print(entry.name());
        if (entry.isDirectory())
        {
            Serial.println("/");
        }
        else
        {
            // Files have sizes, directories do not
            Serial.print("\t\t");
            Serial.println(entry.size(), DEC);
        }
        entry.close();
    }
    root.close();

    // Sensor initialization

    Wire.begin(); // Start I2C library
    delay(100);   // delay .1s

    if (sensor.VL6180xInit() != 0)
    {
        Serial.println("Failed to initialize. Freezing..."); // Initialize device and check for errors
        while (1)
            ;
    }

    sensor.VL6180xDefautSettings(); // Load default settings to get started.
}

void loop()
{
    // Print the measured distance
    printDistance();
    delay(1000);
}
