#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

int up_button = 2;
int down_button = 4;
int left_button = 5;
int right_button = 3;
int start_button = 6;
int select_button = 7;
int push_button = 8;
int x_axis = A0;
int y_axis = A1;
int buttons[] = {up_button, down_button, left_button, right_button, start_button, select_button, push_button};

RF24 radio(9, 10); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};
char msg[20] = "";

void setup() 
{
  for(int i = 0; i < 7; i++)
  {
    pinMode(buttons[i], INPUT);
    digitalWrite(buttons[i], HIGH); 
  } 
  Serial.begin(9600);
  radio.begin();
  radio.openWritingPipe(addresses[1]); // 00001
  radio.openReadingPipe(1, addresses[0]); // 00002
  radio.setPALevel(RF24_PA_MAX);
}

void loop() 
{
  radio.stopListening();
  
  if(digitalRead(up_button) == LOW)
  {
    char msg[] = "up";
    radio.write(&msg, sizeof(msg));
    delay(300);
    Serial.println("Up Button pressed");
  }

  else if(digitalRead(down_button) == LOW)
  {
    char msg[] = "down";
    radio.write(&msg, sizeof(msg));
    delay(300);
    Serial.println("Down Button pressed");
  }

  else if(digitalRead(left_button) == LOW)
  {
    char msg[] = "left";
    radio.write(&msg, sizeof(msg));
    delay(300);
    Serial.println("Left Button pressed");
  }

  else if(digitalRead(right_button) == LOW)
  {
    char msg[] = "right";
    radio.write(&msg, sizeof(msg));
    delay(300);
    Serial.println("Right Button pressed");
  }

  else if(digitalRead(start_button) == LOW)
  {
    char msg[] = "start";
    radio.write(&msg, sizeof(msg));
    delay(300);
    Serial.println("Start Button pressed");
  }

  else if(digitalRead(select_button) == LOW)
  {
    char msg[] = "select";
    radio.write(&msg, sizeof(msg));
    delay(300);
    Serial.println("Select Button pressed");
  }

  else if(digitalRead(push_button) == LOW)
  {
    char msg[] = "stop";
    radio.write(&msg, sizeof(msg));
    delay(300);
    Serial.println("Push Button pressed");
  }

  else
  {
   int adcValue = analogRead(x_axis);
   int X = map(adcValue, 0, 1023, 0, 180);
   radio.write(&X, sizeof(X)); 
   Serial.println(X); 
  }
  
}
