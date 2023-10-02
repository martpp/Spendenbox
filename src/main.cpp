#include <Arduino.h>
#include <WiFi.h>
#include <Audio.h>
#include <Wire.h>
#include <SparkFun_VL6180X.h>

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

