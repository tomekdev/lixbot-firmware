/* PWM pins: 3, 5, 6, 9, 10, 11 */
#define pm pinMode
#define dw digitalWrite
#define dr digitalRead
#define aw analogWrite
#define ar analogRead

#include <Servo.h>

#define ENA_pin 5
#define IN1_pin 6 /* First 2 motors */
#define IN2_pin 7 /* First 2 motors */
#define IN3_pin 8 /* Next 2 motors */
#define IN4_pin 9 /* Nest 2 motors */
#define ENB_pin 10
#define EYE_pin 3
#define ECHO_pin 13
#define TRIG_pin 12

#define OUT OUTPUT
#define IN INPUT
#define debug Serial.print

Servo eyes;
void setup() {
  Serial.begin(9600);
  Serial.setTimeout(100);
//  Serial.println("Serial works");

  pm(IN1_pin, OUT);
  pm(IN2_pin, OUT);
  pm(IN3_pin, OUT);
  pm(IN4_pin, OUT);

  pm(ENA_pin, OUT);
  pm(ENB_pin, OUT);
  pm(EYE_pin, OUT);
  pm(ECHO_pin, IN);
  pm(TRIG_pin, OUT);
  eyes.attach(EYE_pin);
  eyes.write(49);
}

void forward()
{
  aw(ENA_pin, 255);
  dw(IN1_pin, LOW);
  dw(IN2_pin, HIGH);
  aw(ENB_pin, 255);
  dw(IN3_pin, LOW);
  dw(IN4_pin, HIGH);
}

void engine_stop()
{
  aw(ENA_pin, 0);
  aw(ENB_pin, 0);
}

void backward()
{
  aw(ENA_pin, 255);
  dw(IN1_pin, HIGH);
  dw(IN2_pin, LOW);
  aw(ENB_pin, 255);
  dw(IN3_pin, HIGH);
  dw(IN4_pin, LOW);
}

void right()
{
  aw(ENA_pin, 255);
  dw(IN1_pin, LOW);
  dw(IN2_pin, HIGH);
  aw(ENB_pin, 255);
  dw(IN3_pin, HIGH);
  dw(IN4_pin, LOW);
}

void left()
{
  aw(ENA_pin, 255);
  dw(IN1_pin, HIGH);
  dw(IN2_pin, LOW);
  aw(ENB_pin, 255);
  dw(IN3_pin, LOW);
  dw(IN4_pin, HIGH);
}

void forward_left()
{
  aw(ENA_pin, 80);
  dw(IN1_pin, LOW);
  dw(IN2_pin, HIGH);
  aw(ENB_pin, 255);
  dw(IN3_pin, LOW);
  dw(IN4_pin, HIGH);
}

void forward_right()
{
  aw(ENA_pin, 255);
  dw(IN1_pin, LOW);
  dw(IN2_pin, HIGH);
  aw(ENB_pin, 80);
  dw(IN3_pin, LOW);
  dw(IN4_pin, HIGH);
}

int pos = 0;
int st = 1;

int dist = 0;

long distance()
{
  long t = 0;
  dw(TRIG_pin, LOW);
  delayMicroseconds(2);
  dw(TRIG_pin, HIGH);
  delayMicroseconds(10);
  dw(TRIG_pin, LOW);

  t = pulseIn(ECHO_pin, HIGH);
  long dst = t / 58;
  return dst;
}

int prev_dist = 0;

char str[1];
bool manual_control = false;

void loop() {
  if(Serial.readBytes(&str[0],1) != 0)
  manual_control = true;

  dist = distance();
//  debug("Measured distance: ");
//  debug(distance());
//  debug('\n');
  if(dist <=50 && !manual_control)
  {
    {
      int i = 0;
      while(i < 1000)
      {
         right();
         i++;
      }
         if(pos >= 180)
      st = -5;
    if(pos <= 0)
      st = 5;

    pos = pos + st;
    eyes.write(pos);
    }
    if(pos > 90)
      right();
    else
      left();

    pos = 90;
    eyes.write(pos);
  }
  else
  {

      
    if(manual_control)
    {
      if(str[0] == 'F')
      {
        forward();
      } else if(str[0] == 'B')
      {
        backward();
      } else if(str[0] == 'L')
      {
        left();
      }
      else if(str[0] == 'R')
      {
        right();
      }
      else if(str[0] == 'G')
      {
        forward_left();
      }
      else if(str[0] == 'I')
      {
         forward_right();
      }
      else
      {
        engine_stop();
      }
    }
    else
    {
      forward();
    }
  }
}
