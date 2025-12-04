

#include <ESP8266WiFi.h>
#include "secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros
#include <DHT.h>  // Include the DHT sensor library for temperature and humidity

#define DHTPIN D5        // The digital pin where DHT11 data pin is connected
#define DHTTYPE DHT22   // Specify the type of DHT sensor used
DHT dht(DHTPIN, DHTTYPE);  // Initialize DHT object with the defined pin and type

#define soilsensor A0 
#define led D6

char ssid[] = "Harshini.medagam";   // your network SSID (name) 
char pass[] = "harshi17";   // your network password
int keyIndex = 0;            // your network key Index number (needed only for WEP)
WiFiClient  client;

unsigned long myChannelNumber = 3017167;
const char * myWriteAPIKey = "05N8M79USPCEJZGV";

// Initialize our values
int number1 = 0;
int number2 = random(0,100);
int number3 = random(0,100);
int number4 = random(0,100);
String myStatus = "";

void setup() {
  Serial.begin(115200);  // Initialize serial
  while (!Serial) {
    ; // wait for serial port to connect. Needed for Leonardo native USB port only
  }
  
  WiFi.mode(WIFI_STA); 
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  dht.begin();  
  pinMode(soilsensor,INPUT); 
  pinMode(led,OUTPUT);
}

void loop() {

  // Connect or reconnect to WiFi
  if(WiFi.status() != WL_CONNECTED){
    Serial.print("Attempting to connect to SSID: ");
    Serial.println(SECRET_SSID);
    while(WiFi.status() != WL_CONNECTED){
      WiFi.begin(ssid, pass);  // Connect to WPA/WPA2 network. Change this line if using open or WEP network
      Serial.print(".");
      delay(5000);     
    } 
    Serial.println("\nConnected.");
  }
   float humidity = dht.readHumidity();

  float temperature = dht.readTemperature();

  // Read analog value from the LDR (light sensor)
  int Soilvalue = analogRead(soilsensor);

  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.print("°C, ");

  // Print humidity data
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.print("%, ");

  // Print moisture level (based on LDR reading)
  Serial.print("Soil Moisture (approx): ");
  Serial.print(Soilvalue);
  Serial.println("%");
  if(Soilvalue>200){
    digitalWrite(led,HIGH);
    delay(1000);
  }else{
    digitalWrite(led,LOW);
    delay(1000);
  }

  // Wait for 2 seconds before next reading
  delay(2000);

  // set the fields with the values
  ThingSpeak.setField(1, temperature);
  ThingSpeak.setField(2, humidity);
  ThingSpeak.setField(3, Soilvalue);


  // figure out the status message
  if(number1 > number2){
    myStatus = String("field1 is greater than field2"); 
  }
  else if(number1 < number2){
    myStatus = String("field1 is less than field2");
  }
  else{
    myStatus = String("field1 equals field2");
  }
  
  // set the status
  ThingSpeak.setStatus(myStatus);
  
  // write to the ThingSpeak channel
  int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
  if(x == 200){
    Serial.println("Channel update successful.");
  }
  else{
    +
    Serial.println("Problem updating channel. HTTP error code " + String(x));
  }
  
  // change the values
  number1++;
  if(number1 > 99){
    number1 = 0;
  }
  number2 = random(0,100);
  number3 = random(0,100);
  number4 = random(0,100);
  
  delay(20000); // Wait 20 seconds to update the channel again
}
