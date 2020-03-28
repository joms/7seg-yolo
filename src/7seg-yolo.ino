#include <SevSeg.h>

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

void setup()
{
  Particle.function("setstring", setString);
  sevseg.begin(COMMON_ANODE, numDigits, digitPins, segmentPins);
}

void loop()
{
  char p[displayCharacters.length()];

  int i;
  for (i = 0; i < sizeof(p); i++)
  {
    p[i] = displayCharacters[i];
  }

  sevseg.setChars(p);
  sevseg.refreshDisplay();
}

int setString(String newString)
{
  displayCharacters = newString;
  return 0;
}
