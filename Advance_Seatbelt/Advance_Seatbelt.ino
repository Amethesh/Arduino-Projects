#define USE_ARDUINO_INTERRUPTS true    // Set-up low-level interrupts for most acurate BPM math.
#include <PulseSensorPlayground.h>     // Includes the PulseSensorPlayground Library.   

#define BLYNK_TEMPLATE_ID "TMPL3_p9T5jnL"
#define BLYNK_TEMPLATE_NAME "SmartLEDS"
#define BLYNK_AUTH_TOKEN "XVoGxQX2SixExD8HsHjno6IYukvBF1Vq"

#define BLYNK_PRINT Serial

#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>

//server status led
#define BlynkLED D5

//buttons and led pins
#define btn1 D1

#define led1 D7

//  Variables
const int PulseWire = 0;       // PulseSensor PURPLE WIRE connected to ANALOG PIN 0
const int LED13 = 13;          // The on-board Arduino LED, close to PIN 13.
int relay = 8;
int Threshold = 550;           // Determine which Signal to "count as a beat" and which to ignore.
const int buzzer = 11;
                               // Use the "Gettting Started Project" to fine-tune Threshold Value beyond default setting.
                               // Otherwise leave the default "550" value. 

// Relay state
bool relayState = false;

//led states
bool led1State = false;

// Server state | flag for online/offline mode
bool onlineMode = false;

// WiFi config
char ssid[] = "homenet";
char pass[] = "homenet2002";

const char *ifttt_ssid = "homenet";
const char *ifttt_pass = "homenet2002";
const char *host = "maker.ifttt.com";

BlynkTimer timer;
                               
PulseSensorPlayground pulseSensor;

//
void isServerConnected(){
  bool isConnected = Blynk.connected();
  if(isConnected == true){
    onlineMode = true;
    digitalWrite(BlynkLED, HIGH);
    Serial.println("Connected");
  }
  if(isConnected == false){
    if(onlineMode == true){
     onlineMode = false;
    } 
    Serial.println("Not Connected");
    digitalWrite(BlynkLED, LOW);
  }
}


//------------------------------------------------SETUP FUNCTION---------------------------------------------------------//
void setup() {   

  //Base setup 
  pinMode(buzzer, OUTPUT);
  pinMode(relay,OUTPUT);
  digitalWrite(relay,HIGH);
  Serial.begin(9600);          // For Serial Monitor

  //Setup blynk app
  timer.setInterval(2000L, isServerConnected);
  pinMode(BlynkLED, OUTPUT);
  pinMode(led1, OUTPUT);
  pinMode(btn1, INPUT_PULLUP); //For testing

  // Configure the PulseSensor object, by assigning our variables to it. 
  pulseSensor.analogInput(PulseWire);   
  pulseSensor.blinkOnPulse(LED13);       //auto-magically blink Arduino's LED with heartbeat.
  pulseSensor.setThreshold(Threshold);   

  // Double-check the "pulseSensor" object was created and "began" seeing a signal. 
   if (pulseSensor.begin()) {
    Serial.println("We created a pulseSensor Object !");  //This prints one time at Arduino power-up,  or on Arduino reset.  
  }

  //Blynk Link
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  //IFTTT Setup
  Serial.println("Email from Node Mcu");
  delay(100);

  Serial.print("Connecting to ");
  Serial.println(ifttt_ssid);

  WiFi.begin(ifttt_ssid, ifttt_pass);

  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());

  delay(1000);

}

//------------------------------------------------LOOP FUNCTION---------------------------------------------------------//
void loop() {

  //Blynk.run();
  timer.run();

  int myBPM = pulseSensor.getBeatsPerMinute();  // Calls function on our pulseSensor object that returns BPM as an "int".
                                               // "myBPM" hold this BPM value now. 

  digitalWrite(relay,LOW);              
  Serial.println("♥  A HeartBeat Happened ! "); // If test is "true", print a message "a heartbeat happened".
  Serial.print("BPM: ");                        // Print phrase "BPM: " 
  Serial.println(myBPM);                        // Print the value inside of myBPM. 
  digitalWrite(buzzer, LOW);


  if(myBPM <= 0){
    digitalWrite(buzzer,HIGH);
  }

  if(onlineMode){
    Blynk.run();
    // whenOnline(); //handles online functionalities
  }else{
    // whenOffline(); //handles offline functionalities
  }

  WiFiClient client;
  const int httpPort = 80;
  if (!client.connect(host, httpPort))
  {
    Serial.println("connection failed");
    return;
  }

  //Email notification

  // if (safetyDistance < 20){

    // digitalWrite(buzzer, HIGH);

    String url = "/trigger/watertank/json/with/key/dT_ncxZbU5WTEk2UA1edgSnhL5TNAw84v6nIFvvvGjW";

    Serial.print("Requesting URL: ");
    Serial.println(url);

    client.print(String("GET ") + url + " HTTP/1.1\r\n" +
                 "Host: " + host + "\r\n" +
                 "Connection: close\r\n\r\n");
  // }

  delay(20);                    // considered best practice in a simple sketch and testing

}

void updateLEDs(){
  digitalWrite(led1, led1State);
}

void updateBlynkServer(){
   Blynk.virtualWrite(V0, led1State); 
}


  BLYNK_WRITE(V0){
    led1State = param.asInt();
    Serial.println(led1State);
    updateLEDs();
  }
