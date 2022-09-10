#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>
#include <Servo.h>
#include <LiquidCrystal.h>
LiquidCrystal lcd(13, 12, 11, 10, 9, A4);  // ( RS, E, D4, D5, D6, D7 )
Servo test;

RF24 radio(7, 8); // CE, CSN
const byte addresses[][6] = {"00001", "00002"};

char data[20] = "";
int X = 0;


int flag_1 = 0;   // Left function 
int flag_2 = 0;   // Right function 
int flag_3 = 0;   // Battery charge control
int flag_4 = 0;   // Camera direction control
int flag_5 = 0;   // Camera operation transfer control


void setup() 
{
  pinMode(15,OUTPUT);    // Enable pin
  pinMode(14,OUTPUT);    // Enable pin
  pinMode(16, OUTPUT);   // Transistor control pin 
  
  pinMode(4,OUTPUT);   // Motor control pin
  pinMode(5,OUTPUT);   // Motor control pin
  pinMode(6,OUTPUT);   // Motor control pin
  pinMode(A3,OUTPUT);  // Motor control pin

  test.attach(3); 
  lcd.begin(20, 4);
  lcd.setCursor (0, 0);
  lcd.print("    Welcome to...   ");
  lcd.setCursor (0, 1);
  lcd.print("      American      ");
  lcd.setCursor (0, 2);
  lcd.print("    International   ");
  lcd.setCursor (0, 3);
  lcd.print("Univ. of Bangladesh ");
  test.write(0);
  delay(2000);
  lcd.clear();
  
  radio.begin();
  //Serial.begin(9600);
  radio.openWritingPipe(addresses[0]); // 00002
  radio.openReadingPipe(1, addresses[1]); // 00001
  radio.setPALevel(RF24_PA_MAX);
}

void loop() 
{
  
  lcd.setCursor (0, 0);
  lcd.print(" Surveillance Robot ");
  lcd.setCursor(0, 2);
  lcd.print(" BAT Status :");
  
  digitalWrite(15,HIGH);   // Enable B
  digitalWrite(14,HIGH);   // Enable A



  

//---------------------------------------------------------------------------------Charge control loop-----------------------------------------------------------------------------------------


   {
     int pot_2 = analogRead(A5);                  // Read Battery level as ADC value
     int battery = map(pot_2, 0, 850, 0, 100);    // at ADC 850 - Battery level 100%
     int pot_3 = analogRead(A6);                  // Read Panel level as ADC value
     int panel = map(pot_3, 930, 1023, 90, 100);  // at ADC 930 - Panel level 90%
  
     if(battery >= 100)
     {
      lcd.setCursor(13, 2);
      lcd.print("100%");
      lcd.setCursor(0, 3);
      lcd.print("Solar Charging : OFF");
      digitalWrite(16, LOW);
      flag_3 = 0;  
     }
     
     else
     {
      lcd.setCursor(13, 2);
      lcd.print(battery);
      lcd.setCursor(15, 2);
      lcd.print("% ");

      if(panel >= 90)
      {
        if(battery > 75 && battery <= 99 && flag_3 == 0)
        {
         digitalWrite(16, LOW);
         lcd.setCursor(0, 3);
         lcd.print("Solar Charging : OFF");
        }
        
        if(battery <= 75 && flag_3 == 0)
        {
         flag_3 = 1; 
        }

        if(flag_3 == 1)
        {
         lcd.setCursor(0, 3);
         lcd.print(" Battery in Charging");
         digitalWrite(16, HIGH);
        }
      }
      else
      {
        digitalWrite(16, LOW);
        lcd.setCursor(0, 3);
        lcd.print("Solar Charging : OFF"); 
      }
     }
     
   }


//----------------------------------------------------------------------------- Camera control loop -----------------------------------------------------------------------------------------  
  {
    if(flag_5 == 0)
    {                 
      camera_Rotate();      
    }

    if(flag_5 == 1)
    {
      camera_control();
    }
  }
  
//--------------------------------------------------------------------------------Motor and Display control loop-------------------------------------------------------------------------------
  
{
  //delay(5);
  radio.startListening();
  
  String msg = "";
  if ( radio.available()) 
  {
      while( radio.available()) 
      {
       radio.read(&data, sizeof(data));
       Serial.println(data);
       msg = data;
       
       if(msg == "up")
       {
        forward();
        lcd.setCursor (0, 1);
        lcd.print("   Going Forward    ");
       }

       if(msg == "down")
       {
        backward();
        lcd.setCursor (0, 1);
        lcd.print("   Going Backward   ");
       }

       if(msg == "left")
       {
        lcd.setCursor (0, 1);
        lcd.print("     Going Left     ");
      
        if(flag_1 == 0)
        {
         left();
         delay(1000);
         flag_1 = 1;
        }     
        if(flag_1 == 1)
        {
         forward(); 
        } 
       }

       if(msg == "right")
       {
        lcd.setCursor (0, 1);
        lcd.print("    Going Right     ");
      
        if(flag_2 == 0)
        {
         right();
         delay(1000);
         flag_2 = 1;
        }
        if(flag_2 == 1)
        {
         forward(); 
        }   
       }

       if(msg == "start" )
       {
         flag_5 = 0;
       }
 
       if(msg == "select")
       {
         flag_5 = 1;
       }

       if(msg == "stop")
       {
        stopmotor();
        flag_1 = 0;    // Reset flag_1
        flag_2 = 0;    // Reset flag_2
        lcd.setCursor (0, 1);
        lcd.print(" Robot is now _ Stop");  
       }
       
       
       
     }        
  }
} 
   
  
  
}   // End of void loop





//-------------------------------------------------------------------------------Sub - Functions for Motor control---------------------------------------------------------------------------- 


void forward()
{
  digitalWrite(A3,HIGH);
  digitalWrite(6,LOW);  
  digitalWrite(5,HIGH);
  digitalWrite(4,LOW);
}


void backward()
{
  digitalWrite(A3,LOW);
  digitalWrite(6,HIGH);
  digitalWrite(5,LOW);
  digitalWrite(4,HIGH);   
}


void left()
{
  digitalWrite(A3,HIGH);
  digitalWrite(6,LOW);
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);  
}


void right()
{
  digitalWrite(A3,LOW);
  digitalWrite(6,LOW);
  digitalWrite(5,HIGH);
  digitalWrite(4,LOW);   
}


void stopmotor()
{
  digitalWrite(A3,LOW);
  digitalWrite(6,LOW);
  digitalWrite(5,LOW);
  digitalWrite(4,LOW);   
}


//------------------------------------------------------------------------------ joystick control ---------------------------------------------------------------------------------------------


void camera_control()
{
   radio.read(&X, sizeof(X));
   Serial.println(X);
   test.write(X);
   delay(5);
}


//------------------------------------------------------------------------------------Servo control loop---------------------------------------------------------------------------------------

void camera_Rotate()
{  
      int servo;
      int Time = millis();
      int value = abs(Time);
      {
        
        if( value > 0 && value <= 8000 && flag_4 == 0 )
        {
         servo = map(value, 0, 8000, 0, 180); 
         test.write(servo);
        }
        else if(value >= 8001 && value <= 16000 && flag_4 == 0 )
        {
         servo = map(value, 8001, 16000, 180, 0); 
         test.write(servo);
        }

        else if(value >= 16001 && value <= 24000 && flag_4 == 0 )
        {
         servo = map(value, 16001, 24000, 0, 180);
         test.write(servo); 
        }
      
        else if(value >= 24001 && value <= 32499 && flag_4 == 0 )
        {
         servo = map(value, 24001, 32499, 180, 0);
         test.write(servo);       
        }
     
     
        else if(value == 32500 && flag_4 == 0 )
        {
         test.write(0);   
         flag_4 = 1;
        }

     
        if(flag_4 == 1)
        {
         if( value <= 32499 && value >= 24001)
         {
          servo = map(value, 32499, 24001, 0, 180);
          test.write(servo); 
         }
         else if(value <= 24000 && value >= 16001)
         {
          servo = map(value, 24000, 16001, 180, 0);
          test.write(servo); 
         }

         else if(value <= 16000 && value >= 8001)
         {
          servo = map(value, 16000, 8001, 0, 180);
          test.write(servo);  
         }
      
         else if(value <= 8000 && value >= 1)
         {
          servo = map(value, 8000, 1, 180, 0);
          test.write(servo);  
         }
         else if(value == 0)
         {
          test.write(0); 
          flag_4 = 0; 
         }
        }
     
      }       
      
   }


