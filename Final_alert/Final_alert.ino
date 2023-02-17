#include <ESP8266WiFi.h>

const char* ssid = "homenet";
const char* password = "homenet2002";
const char* host = "maker.ifttt.com";

const int trigPin = D2;
const int echoPin = D4;
const int buzzer = D5;

long duration;
int distance;
int safetyDistance;

void setup() {

    pinMode(trigPin, OUTPUT); // Sets the trigPin as an Output
    pinMode(echoPin, INPUT); // Sets the echoPin as an Input
    pinMode(buzzer, OUTPUT);
    
    Serial.begin(115200);

    Serial.println("Email from Node Mcu");
    delay(100);

    Serial.print("Connecting to ");
    Serial.println(ssid);
    
    WiFi.begin(ssid, password);
    
    while (WiFi.status() != WL_CONNECTED) {
      delay(500);
      Serial.print(".");
    }
  
    Serial.println("");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());  

            
    delay(1000);


}

void loop() {  
      

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
          Serial.println("d=");
          Serial.println(safetyDistance);
          
           WiFiClient client; 
           const int httpPort = 80;  
            if (!client.connect(host, httpPort)) {  
                  Serial.println("connection failed");  
            return;} 
          //if(Serial.available()){
            
            //char a=Serial.read();
            
            if (safetyDistance < 20){
    
                    digitalWrite(buzzer, HIGH);
          
                    String url = "/trigger/sornar_alert/json/with/key/d4P1HGmYfxOIGRLqmPvdZN"; 

          
                    Serial.print("Requesting URL: ");
                    Serial.println(url);
                 
                     client.print(String("GET ") + url + " HTTP/1.1\r\n" + 
                                    "Host: " + host + "\r\n" +   
                                           "Connection: close\r\n\r\n");    
                                    }  
          
          
          else{
            Serial.println("Object not Detected");
            digitalWrite(buzzer, LOW);
          }
          //}
          
          delay(5000);
          
      
}
