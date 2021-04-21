// Include Libraries
#include <Arduino.h>
#include "src/Lib/Buzzer.h"
#include "src/Lib/HCSR04.h"
#include "src/Lib/LED.h"
#include <Wire.h>
#include <SPI.h>
#include "src/Lib/Adafruit_SSD1306.h"
#include "src/Lib/Adafruit_MLX90614.h"
#include "src/Lib/Adafruit_GFX.h"
#include <Servo.h>

// Pin Definitions
#define BUZZER_PIN_SIG	0 // D3
#define HCSR04_PIN_TRIG	13 // D7
#define HCSR04_PIN_ECHO	2 // D4
#define LEDG_PIN_VIN	14 // D5
#define LEDR_PIN_VIN	12 // D6
#define OLED_RST	15 // D8
#define SERVO_PIN_SIG	16 // D0
#define SDA 4 // D2
#define SCL 5 //D1
// SDA = D2
// SCL = D1
// RX = D9
// TX = D10

// Global variables and defines
#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64    // OLED display dimensions
double temp_amb;
double temp_body;           // Vars for storing ambient and body temperature
double calibration = 2.36;  // Idk
double threshold = 38.0;    // Temperature threshold for denying access
int maxDistance = 10;       // Max distance for taking temperature
byte trigger = 't';         // Trigger

// object initialization
Buzzer buzzer(BUZZER_PIN_SIG);
HCSR04 ultrasonic(HCSR04_PIN_TRIG,HCSR04_PIN_ECHO);
Led ledG(LEDG_PIN_VIN);
Led ledR(LEDR_PIN_VIN);
Adafruit_SSD1306 display(OLED_RST);
Adafruit_MLX90614 mlx = Adafruit_MLX90614();
Servo servo;