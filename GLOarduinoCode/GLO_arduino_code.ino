
#include <CayenneMQTTESP8266.h>
#include <ESP8266WiFi.h>
#include <DHT.h>
#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif


#define DHTPIN 4
#define DHTTYPE DHT11

#define PIN D5
#define NUMPIXELS 12
#define buzzer D7



DHT dht(DHTPIN, DHTTYPE);
Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);


 
char ssid[] = "FoxLab"; 
char password[] = "foxlabmakerspace";
char username[] = "141dc600-82b0-11e7-b546-bf6ccbcd8710"; 
char mqtt_password[] = "267fd607a6a8ad4c40a6da49327b3c17ecfb62f6";
char client_id[] = "fd15e9e0-ac37-11e7-8802-49bbc5641530";


const int LDRpin = A0;
const int PIRpin = D0;
int LDRval = 0;
int PIRval = 0;
int temp;
int hum;

int r,g,b;
int Cmode;
int TURN;
int SAFE_MODE;


void setup() 
  { 
pinMode(LDRpin,INPUT);
pinMode(PIRpin,INPUT);
pinMode(buzzer,OUTPUT);
dht.begin();
Serial.begin(9600);
Cayenne.begin(username,mqtt_password,client_id,ssid,password); 
pixels.begin(); 
Serial.println("FIRE FLY BOOTED");
  
   }

  
void loop() 
{ 
   Cayenne.loop(); 
   
   if(Cmode==1)
  {
    buglerMode();
  }
  else if(Cmode<1){
    setLight();
    
  }
  
hum = dht.readHumidity();
temp = dht.readTemperature();
LDRval = analogRead(LDRpin);
PIRval = digitalRead(PIRpin); 
Serial.println(PIRval);
       Cayenne.virtualWrite(0,temp);
       Cayenne.virtualWrite(1,hum);
       Cayenne.virtualWrite(2,LDRval);
       Cayenne.virtualWrite(3,PIRval);
  
}

void setColor(int red,int green,int blue)
{
  for(int i =0;i<=NUMPIXELS;i++)
  {
    pixels.setPixelColor(i,pixels.Color(red,green,blue));
 
    pixels.show();
  }
}


void setLight()
{

if(TURN==1)
{
  manual();
}

  else
  {
     if(PIRval>0)
{
  setColor(r,g,b);
}
else if(LDRval<30)
{
  setColor(r,g,b);
}

else if(PIRval<1&&LDRval>10)
{
  setColor(0,0,0);
}

}
}


void buglerMode()
{

  if(SAFE_MODE==1)
  {
    digitalWrite(buzzer,LOW);
  }

  
  if(PIRval>0)

{
  
  digitalWrite(buzzer,HIGH);
  Serial.println("bugler on");
}

 
}


 void manual(){

   setColor(r,g,b);
 }


 
    CAYENNE_IN(4)
{

  r= getValue.asInt();
   
 
}
CAYENNE_IN(5)
{

  g= getValue.asInt();
  
   
}


 CAYENNE_IN(6)
{

  
   
    b= getValue.asInt();

}

CAYENNE_IN(7)
{
  Cmode =getValue.asInt();
Serial.println(Cmode);
}

CAYENNE_IN(8)
{
  TURN = getValue.asInt();
}

CAYENNE_IN(9)
{
  SAFE_MODE = getValue.asInt();
}

 

  
