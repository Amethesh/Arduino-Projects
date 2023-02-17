#include <ESP8266WiFi.h>

const char* ssid = "homenet";
const char* password = "homenet2002";


const char* host = "maker.ifttt.com";


//int IRSensor = 8; // connect ir sensor to arduino pin 2
const int trigPin = D2;
const int echoPin = D4;
const int buzzer = D5;



long duration;
int distance;
int safetyDistance;


void setup() 
{
   // pinMode (IRSensor, INPUT); // sensor pin INPUT

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    pinMode(buzzer, OUTPUT);
    
    
    Serial.begin(115200);
    Serial.println("Email from Node Mcu");
    delay(100);
    delay(1000);

   connectWiFi();   
}

void loop()
{
      WiFiClient client; 
      const int httpPort = 80; 
      //
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
      
       
      if (!client.connect(host, httpPort)) 
      {  
         Serial.println("connection failed");  
         return;
      } 
      if (safetyDistance <= 20)
      {     
            digitalWrite(buzzer, HIGH);
            digitalWrite(ledPin, HIGH);        
            String url = "/trigger/sornar_alert/json/with/key/d4P1HGmYfxOIGRLqmPvdZN"; 
            Serial.print("Requesting URL: ");
            Serial.println(url);                 
            client.print(String("GET ") + url + " HTTP/1.1\r\n" + "Host: " + host + "\r\n" + "Connection: close\r\n\r\n");                    
      }  
      else
      {
           digitalWrite(buzzer, LOW);
           digitalWrite(ledPin, LOW);
           Serial.println("Object Not Detected");            
      }
          
      delay(5000);  
      Serial.print("Distance: ");
      Serial.println(distance);
  
    while((!(WiFi.status() == WL_CONNECTED)))
    {
      connectWiFi();
    } 
}


void connectWiFi()
{ 
  pinMode(2, OUTPUT);
  int i=0;
  WiFi.disconnect();
  //WiFi.mode(WIFI_STA);
  Serial.println("Connecting to wifi......");
 
  WiFi.begin(ssid, password);
  while ((!(WiFi.status() == WL_CONNECTED)))
  {
    digitalWrite(2,HIGH);
    delay(300);
    digitalWrite(2,LOW);
    delay(200);
    
    Serial.println(" - ");
    
    i++;
    if( i>10 )    
    { 
     return;
    }
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("NodeMCU Local IP is : ");
  Serial.print((WiFi.localIP()));

  
  digitalWrite(2,HIGH);
  delay(400);
  digitalWrite(2,LOW);

}
