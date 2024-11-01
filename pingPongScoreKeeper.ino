// Include the library
#include <TM1637Display.h>

const uint8_t redWin[] = {
  SEG_E | SEG_G,                                   // r
  SEG_C,                                           // i
  SEG_D | SEG_E | SEG_G | SEG_F,                   // t
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G            // E
};

const uint8_t blueWin[] = {
  SEG_B | SEG_C,                                   // l
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,           // E
  SEG_A | SEG_E | SEG_F | SEG_G,                   // f
  SEG_D | SEG_E | SEG_G | SEG_F,                   // t
};

const int buttonBlue = 4;
const int buttonGreen = 2;
const int buttonRed = 3;
const int CLK = 6;
const int DIO = 5;
const int r = 8;
const int g = 9;
const int b = 10;
const int buzzer = 7;
int counterBlue = 0;
int counterRed = 0;
int victory[] = {1047, 1047, 1568};
int times[] = {250, 250, 750};

// Create a display object of type TM1637Display
TM1637Display display = TM1637Display(CLK, DIO);
// Create an array that turns all segments ON
const uint8_t allON[] = {0xff, 0xff, 0xff, 0xff};
// Create an array that turns all segments OFF
const uint8_t allOFF[] = {0x00, 0x00, 0x00, 0x00};

void updateDisp(int blue, int red, bool bBlue, bool bRed) { 
  display.clear();
  display.showNumberDec(((blue * 100) + red), true);
  tone(buzzer, 1047, 250);
  delay(250);
}

void turnBlue() {
  analogWrite(r, 0);
  analogWrite(g, 0);
  analogWrite(b, 255);
}

void turnRed() {
  analogWrite(r, 255);
  analogWrite(g, 0);
  analogWrite(b, 0);
}

void turnGreen() {
  analogWrite(r, 0);
  analogWrite(g, 255);
  analogWrite(b, 0);
}

void setup() {
  display.setBrightness(7);
  display.setSegments(allON);
  updateDisp(counterBlue, counterRed, false, false);

  pinMode(r, OUTPUT);
  pinMode(g, OUTPUT);
  pinMode(b, OUTPUT);
}

void loop() {

  if (((counterBlue + counterRed) / 2) % 2 == 0) {
    turnBlue();
  }
  else {
    turnGreen();
  }

  if (counterBlue == 11 || counterRed == 11) {
    if (counterBlue == 11) {
      display.clear();
      display.setSegments(blueWin);
    }
    else {
      display.clear();
      display.setSegments(redWin);
    }

    for (int i = 0; i < 3; i++) {
      tone(buzzer, victory[i], times[i]);
      delay(500);
    }
    while (digitalRead(buttonGreen) != HIGH) {
      turnRed();
    }
    counterBlue = 0;
    counterRed = 0;
  }

	if (digitalRead(buttonBlue) == HIGH) {
    counterBlue++;
    updateDisp(counterBlue, counterRed, true, false);
  }
  else if (digitalRead(buttonRed) == HIGH) {
    counterRed++;
    updateDisp(counterBlue, counterRed, false, true);
  }
  else if (digitalRead(buttonGreen) == HIGH) {
    counterBlue = 0;
    counterRed = 0;
    updateDisp(counterBlue, counterRed, false, false);
  }
}
