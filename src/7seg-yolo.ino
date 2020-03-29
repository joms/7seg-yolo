#include <SevSeg.h>
#include <string.h>

SevSeg sevseg;

byte digit1 = D5;
byte digit2 = D4;
byte digit3 = D3;
byte digit4 = D2;

byte a = D6;
byte b = D7;
byte c = D8;
byte d = A0;
byte e = A1;
byte f = A2;
byte g = A3;
byte dot = A4;

byte numDigits = 4;
byte digitPins[] = {digit1, digit2, digit3, digit4};
byte segmentPins[] = {a, b, c, d, e, f, g, dot};

String displayCharacters;

enum DisplayState
{
  IDLE,
  LOADING
};

enum BlinkState
{
  NOT_BLINKING = 3000,
  BLINKING = 100
};

String enumToString(DisplayState type)
{
  switch (type)
  {
  case IDLE:
    return "idle";
  case LOADING:
    return "loading";
  default:
    return "invalid";
  }
}

DisplayState currentState = IDLE;
BlinkState currentBlinkState = NOT_BLINKING;

int setState(String state);

unsigned long timer = millis();

#define SPINNER_LENGTH 12
byte spinnerSegments[SPINNER_LENGTH] = {0b00000001, 0b00000001, 0b00000001, 0b00000001, 0b00000010, 0b00000100, 0b00001000, 0b00001000, 0b00001000, 0b00001000, 0b00010000, 0b00100000};
int spinnerDecimal[SPINNER_LENGTH] = {0, 1, 2, 3, 3, 3, 3, 2, 1, 0, 0, 0};
unsigned int spinnerCurrentSegmentSelector = spinnerDecimal[0];

void setup()
{
  Particle.function("set_state", setState);
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
}

void loop()
{
  switch (currentState)
  {
    // TODO Support double blinking from time to time
    // TODO Adjust blink frequency
  case IDLE:
    switch (currentBlinkState)
    {
    case NOT_BLINKING:
      sevseg.setChars("0 . 0");
      if (millis() > (timer + NOT_BLINKING))
      {
        currentBlinkState = BLINKING;
        timer = millis();
      }
      break;

    case BLINKING:
      sevseg.setChars("- . -");
      if (millis() > (timer + BLINKING))
      {
        currentBlinkState = NOT_BLINKING;
        timer = millis();
      }
      break;
    }
    break;

  case LOADING:
    byte segments[4] = {0b00000000};
    segments[spinnerDecimal[spinnerCurrentSegmentSelector]] = spinnerSegments[spinnerCurrentSegmentSelector];
    sevseg.setSegments(segments);

    if (millis() > (timer + 100))
    {
      spinnerCurrentSegmentSelector++;
      if (spinnerCurrentSegmentSelector >= SPINNER_LENGTH)
      {
        spinnerCurrentSegmentSelector = 0;
      }
      timer = millis();
    }
    break;
  }

  sevseg.refreshDisplay();
}

int setState(String state)
{
  if (state.equals(enumToString(IDLE)))
  {
    currentState = IDLE;
    return 0;
  }
  else if (state.equals(enumToString(LOADING)))
  {
    currentState = LOADING;
    return 0;
  }

  return -1;
}
