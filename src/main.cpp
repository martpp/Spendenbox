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
    pinMode(SD_CS, OUTPUT);
    digitalWrite(SD_CS, HIGH);
    SPI.begin(SD_SCK, SD_MISO, SD_MOSI);
    Serial.begin(115200);
    if (!SD.begin(SD_CS))
    {
        Serial.println("Error talking to SD card!");
        while (true)
            ; // end program
    }
    audio.setPinout(MAX98357A_BCLK, MAX98357A_LRC, MAX98357A_DOUT);
    audio.setVolume(15); // 0...21
    audio.connecttoFS(SD, "/nggyu.mp3");
}

void loop() { audio.loop(); }