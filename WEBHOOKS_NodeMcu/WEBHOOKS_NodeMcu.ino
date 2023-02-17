#include <ESP8266WiFi.h>

const char* ssid = "homenet";
const char* password = "homenet2002";


const int trigPin = D2;
const int echoPin = D4;
//const int buzzer = 11;
//const int ledPin = 13;

long duration;
int distance;
int safetyDistance;

const char* host = "https://maker.ifttt.com";

void setup() 
{

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT);
    
    Serial.begin(115200);
    Serial.println("Email from Node Mcu");
    delay(100);
    delay(1000);

   connectWiFi(); 

}

void loop() 
{                  
        

      digitalWrite(trigPin, LOW);
delayMicroseconds(2);
 
// Sets the trigPin on HIGH state for 10 micro seconds
digitalWrite(trigPin, HIGH);
delayMicroseconds(10);
digitalWrite(trigPin, LOW);
 
// Reads the echoPin, returns the sound wave travel time in microseconds
duration = pulseIn(echoPin, HIGH);
 
// Calculating the distance
distance= duration*0.034/2;
 
safetyDistance = distance;

      WiFiClient client; 
      const int httpPort = 80;

      
      if (!client.connect(host, httpPort)) 
      {  
         Serial.println("connection failed");  
         return;
      } 
      Serial.println(safetyDistance);
      
      Serial.println("sdsd");
      //Serial.println(echoPin);
      //if(Serial.available())
      //{           
         //char a=Serial.read();           
        
         
         if (safetyDistance <= 10)
        {    
            String url = "https://maker.ifttt.com/trigger/sornar_alert/json/with/key/d4P1HGmYfxOIGRLqmPvdZN"; 
            Serial.print("Requesting URL: ");
            Serial.println(url); 
            Serial.println(safetyDistance);                
            client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");    
         }                     
          else
          {
            Serial.println("Correct character not pressed.Try again");            
          }
       //}   

              
       delay(5000);  
    while(((WiFi.status() == WL_CONNECTED)))
    {
     connectWiFi();
    }              
}

void connectWiFi()
{ 
  int i=0;
  Serial.println("Connecting to WIFI");
  WiFi.begin(ssid, password);
  while((!(WiFi.status() == WL_CONNECTED)))
  {
    Serial.println(" - "); 
    i++;
    delay(300);
    if( i>10 )    
    { 
     connectWiFi();
    }
   }  
   Serial.println("");
   Serial.println("WiFi connected");
}
