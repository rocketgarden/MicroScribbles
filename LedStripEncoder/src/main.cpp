// -----
// InterruptRotator.ino - Example for the RotaryEncoder library.
// This class is implemented for use with the Arduino environment.
//
// Copyright (c) by Matthias Hertel, http://www.mathertel.de
// This work is licensed under a BSD 3-Clause License. See http://www.mathertel.de/License.aspx
// More information on: http://www.mathertel.de/Arduino
// -----
// 18.01.2014 created by Matthias Hertel
// 04.02.2021 conditions and settings added for ESP8266
// 03.07.2022 avoid ESP8266 compiler warnings.
// 03.07.2022 encoder instance not static.
// -----

// This example checks the state of the rotary encoder using interrupts and in the loop() function.
// The current position and direction is printed on output when changed.

// Hardware setup:
// Attach a rotary encoder with output pins to
// * 2 and 3 on Arduino UNO. (supported by attachInterrupt)
// * A2 and A3 can be used when directly using the ISR interrupts, see comments below.
// * D5 and D6 on ESP8266 board (e.g. NodeMCU).
// Swap the pins when direction is detected wrong.
// The common contact should be attached to ground.
//
// Hints for using attachinterrupt see https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/

#include <main.h>
#include <Arduino.h>
#include <Encoder.h>
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <NeoPixelBus.h>
#include <OneButton.h>

//
// Colors
#define colorSaturation 128
RgbColor red(colorSaturation, 0, 0);
RgbColor green(0, colorSaturation, 0);
RgbColor blue(0, 0, colorSaturation);
RgbColor white(colorSaturation);
RgbColor black(0);

//
// Display

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library. 
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

//
// Rotary Encoder
#define RENC_P1 D5
#define RENC_P2 D6

Encoder encoder(RENC_P1, RENC_P2);

//
// Button

#define BUTTON_P1 D7

OneButton button = OneButton(BUTTON_P1, true, true);

//
// WLED
// esp8266 must use GPIO3 pin for this, aka RX pin
// This has much higher Mem usage than bit-bang (+400%) but is CPU cheap
// INVERTED So we can use basic transistor/mosfet as level shifter
// Arbitrary 1000 pixel length
NeoPixelBus<NeoRgbFeature, NeoEsp8266Ws2821InvertedMethod> strip(1000);

//
// STATE STATE STATE STATE STATE
bool isButtonPressed = false;
bool isLongPressConsumed = false;

void setup()
{
  Serial.begin(115200);
  while (!Serial)
    ;

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) {
    Serial.println(F("SSD1306 allocation failed"));
    for(;;); // Don't proceed, loop forever
  }

  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.println("Spin me!");
  display.display();
} // setup()


// Read the current position of the encoder and print out when changed.
void loop()
{
  static int pos = 0;


  int newPos = encoder.read()/4;
  if (pos != newPos) {
    if(newPos >= 0) {
      onEncoderChanged(pos, newPos);
      pos = newPos;
      displayStatus(pos);
    } else {
      encoder.readAndReset(); // if below zero, reset to 0
    }
  }
}

void displayStatus(int lightIndex) {
  display.clearDisplay();
    display.setCursor(0, 0); 
    display.print("pos:");
    display.println(lightIndex);
    display.display();
}


void onEncoderChanged(int oldIndex, int newIndex) {
  strip.SetPixelColor(oldIndex, black);
  strip.SetPixelColor(newIndex, red);
  // check if longpressed or not
}

void onPress() {

}

// bind to both onclick and onLongPressUp
void onButtonRelease() {

}