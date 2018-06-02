#include <TinyGPS++.h>
#include <SoftwareSerial.h>
#include <NewPing.h>
#define Emergency_Button 10
#define FRONT_ULTRA 1
#define DEPTH_ULTRA 0

//Front sensor
#define TRIGGER_PIN  A5  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     A4  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

//depth sensor
#define TRIGGER_PIN_2  A3  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN_2     A2  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE_2 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.
#define buzzer 11

static const int RXPin = 6, TXPin = 7;
static const uint32_t GPSBaud = 9600;
TinyGPSPlus gps;
SoftwareSerial GPS(RXPin, TXPin);
float lat1 ,lng1;
void GSM_SENT(int x);
void GSM_MSG_MODE_ACTIVE();
void GSM_Call();
void GPS_Check();

NewPing sonar1(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE);
NewPing sonar2(TRIGGER_PIN_2, ECHO_PIN_2, MAX_DISTANCE_2);

unsigned char FrontDistance=0,DepthDistance=0;
unsigned near_object_count=0;
boolean obstacal_detect,depth_detect,Emergency_detect;
void read_distance(unsigned char sensor);
void check_emergency();
void sound();
void Diplay();

void tone_generate(unsigned int d);
void tone_generate(unsigned int d)
{
  for(unsigned int i=0;i<d;i++)
  {
    delay(1);
    check_emergency();
  }
}
void setup()
{
   Serial.begin(115200);
   pinMode(buzzer,OUTPUT);
   digitalWrite(buzzer,LOW);
   pinMode(Emergency_Button,INPUT_PULLUP);
   GPS.begin(GPSBaud);
   delay(1000);
   GSM_Call();
   delay(2000);
   GSM_MSG_MODE_ACTIVE();

   obstacal_detect = false;
   depth_detect    = false;
}

void loop()
{
   read_distance(FRONT_ULTRA);
   read_distance(DEPTH_ULTRA);
   check_emergency();
   GPS_Check();

 //  Diplay();
   sound();
}


void check_emergency()
{
  if(digitalRead(Emergency_Button) == LOW && Emergency_detect == false)
  {
   // while(digitalRead(Emergency_Button) ==LOW)
     Emergency_detect = true;
     Serial.println("----------EMERGENY ----------");
     Serial.println("-------Send Message----------");
     GSM_SENT(1);
     Emergency_detect = false;
  }
}
void read_distance(unsigned char sensor)
{
   if(sensor == FRONT_ULTRA)
   {
    for(int i=0;i<10;i++)
      FrontDistance = (sonar1.ping_cm());

      if(FrontDistance >=10 && FrontDistance <= 70)
      {
         obstacal_detect = true;
         if(FrontDistance >= 10 && FrontDistance < 20)
            near_object_count = 50;
         else if(FrontDistance >= 20 && FrontDistance< 30)
            near_object_count = 200;
         else if(FrontDistance >= 30 && FrontDistance< 40)
            near_object_count = 500;
         else if(FrontDistance >= 40 && FrontDistance <50)
            near_object_count = 1000;
         else if(FrontDistance >=50)
            near_object_count = 1500;

      }
   }
   else
   {
     for(int i=0;i<10;i++)
      DepthDistance = (sonar2.ping_cm());

      if(DepthDistance >= 40 ) // change here
      {
         depth_detect = true;
      }
   }
}

void sound()
{
  if(obstacal_detect)
  {
    //for(int i=0;i<=5;i++)
    {
       digitalWrite(buzzer,HIGH);
       tone_generate(near_object_count);
       digitalWrite(buzzer,LOW);
       tone_generate(near_object_count);
    }
    obstacal_detect = false;
  }
  else if(depth_detect)
  {
       digitalWrite(buzzer,HIGH);
       tone_generate(500);
       digitalWrite(buzzer,LOW);
       tone_generate(500);
       depth_detect = false;
  }
}
void Diplay()
{
   Serial.print("FrontDistance:\t\t"); Serial.println(FrontDistance);
   Serial.print("DepthDistance:\t\t"); Serial.println(DepthDistance);
   Serial.print("latitude"); Serial.print(lat1); Serial.print("  longitude:  "); Serial.println(lng1);
}
