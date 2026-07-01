#include <SoftwareSerial.h>
#include <math.h>

// Bluetooth pins
#define BT_RX 12
#define BT_TX 13
SoftwareSerial BT(BT_RX, BT_TX);

// Joystick pins
#define JOYSTICK_X A3
#define JOYSTICK_Y A1
#define JOYSTICK_K 4   // Joystick button

int joystick[2];
int key_pressed = 0;
char gerak = ' ';

void setup()
{
  pinMode(JOYSTICK_K, INPUT_PULLUP);

  Serial.begin(9600);   // Debug
  BT.begin(9600);       // Bluetooth

  Serial.println("Bluetooth Joystick Ready");
}

void loop()
{
  joystick[0] = analogRead(JOYSTICK_X);
  joystick[1] = analogRead(JOYSTICK_Y);

  // Center joystick
  joystick[0] -= 512;
  joystick[1] = 512 - joystick[1];

  float alpha = atan2(joystick[1], joystick[0]);
  float power = sqrt((float)(joystick[0] * joystick[0] + joystick[1] * joystick[1]));

  // Direction detection
  if (power > 50)
  {
    if (alpha > -PI / 4 && alpha <= PI / 4)
      gerak = 'R';
    else if (alpha > PI / 4 && alpha <= 3 * PI / 4)
      gerak = 'F';
    else if (alpha > -3 * PI / 4 && alpha <= -PI / 4)
      gerak = 'B';
    else
      gerak = 'L';

    BT.write(gerak);
    Serial.println(gerak);
  }

  // Joystick button (press)
  key_pressed = 0;
  while (digitalRead(JOYSTICK_K) == LOW)
  {
    delayMicroseconds(2);
    if (key_pressed++ > 10)
    {
      gerak = 'A';     // Action / Start / Stand
      BT.write(gerak);
      Serial.println(gerak);
      break;
    }
  }

  gerak = ' ';
  delay(100);
}
