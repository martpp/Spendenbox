#include <Arduino.h>
#include <Audio.h>
#include <SD.h>
#include <SPI.h>
#include <SparkFun_VL6180X.h>
#include <WiFi.h>
#include <Wire.h>

#include "config.h"

// Define pin for built-in LED
#define LED_BUILTIN 2

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

// Method to get random filename from SD card in directory
String getRandomFilename(char* directory)
{
    File root = SD.open(directory);
    File file = root.openNextFile();
    int count = 0;
    while (file)
    {
        count++;
        file = root.openNextFile();
    }
    root.close();
    root = SD.open(directory);
    int random = rand() % count;
    count = 0;
    file = root.openNextFile();
    while (file)
    {
        if (count == random)
        {
            root.close();
            return file.name();
        }
        count++;
        file = root.openNextFile();
    }
    root.close();
    return "";
}

// Method to play random audio file from SD card in directory
void playRandomAudio(char* directory)
{
    if (audio.isRunning())
        return;
    String filename = getRandomFilename(directory);
    if (filename != "")
    {
        Serial.print("Playing ");
        Serial.println(filename);
        // Check if file ends with .wav
        if (filename.endsWith(".mp3"))
        {
            // append directory to filename (e.g. /smallmp3/1.mp3)
            filename = String(directory) + "/" + filename;
            audio.connecttoFS(SD, filename.c_str());
        }
        else
        {
            Serial.println("File is not .mp3");
        }
    }
}

void setup()
{
    // Initialize SD card
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    Serial.begin(115200);
    if (!SD.begin(SD_CS))
    {
        Serial.println("Error talking to SD card!");
        while (true)
            ;
    }

    // Initialize audio board
    audio.setPinout(MAX98357A_BCLK, MAX98357A_LRC, MAX98357A_DOUT);
    audio.setVolume(21);

    // Initialize VL6180X sensor
    Wire.begin();
    delay(100);

    if (sensor.VL6180xInit() != 0)
    {
        Serial.println("Failed to initialize. Freezing...");
        while (1)
            ;
    }

    sensor.VL6180xDefautSettings();
}

void loop()
{
    if (sensor.getDistance() < 20.) // big money
    {
        digitalWrite(LED_BUILTIN, HIGH);
        playRandomAudio("/bigmp3");
    }
    else if (sensor.getDistance() < 55.) // small money
    {
        digitalWrite(LED_BUILTIN, HIGH);
        playRandomAudio("/smallmp3");
    }
    else
        digitalWrite(LED_BUILTIN, LOW);

    audio.loop();
}