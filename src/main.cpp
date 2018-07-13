
#include <Arduino.h>
#include <RFTransmitter.h>
#include <Adafruit_Sensor.h>
#include <DHT.h>

#define NODE_ID          1
#define OUTPUT_PIN       1
#define LED_BUILTIN      13



// Send on digital pin 11 and identify as node 1
RFTransmitter transmitter(OUTPUT_PIN, NODE_ID);
unsigned long start, finished, elapsed;


//Constants
#define DHTPIN 2     // what pin we're connected to
#define DHTTYPE DHT22   // DHT 22  (AM2302)
DHT dht(DHTPIN, DHTTYPE); //// Initialize DHT sensor for normal 16mhz Arduino
//Variables
int chk;
float hum;  //Stores humidity value
float temp; //Stores temperature value


void setup() {
  pinMode(LED_BUILTIN, OUTPUT);
  // Serial.begin(9600);
  dht.begin();
}

void sendMessage(bool resend) {

  //Read data and store it to variables hum and temp
  hum = dht.readHumidity();
  temp= dht.readTemperature();

  String fullString = "Temp: " + String(temp) + "°C Humitidty: " + String(hum);
  char msg[50];
  fullString.toCharArray(msg, 50);
  // Serial.println("sending...");
  digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)

  start=millis();
  if(resend) {
    transmitter.resend((byte *)msg, strlen(msg) + 1);
  }
  else {
    transmitter.send((byte *)msg, strlen(msg) + 1);
  }

  finished=millis();
  elapsed=finished-start;
  // if (resend){
  //   Serial.print("re");
  // }
  // Serial.print("send:");
  // Serial.print(elapsed);
  // Serial.println(" milliseconds elapsed");
  delay(500);                       // wait for a second
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
  delay(500);

}

void loop() {
  sendMessage(false);

  delay(4000);

  sendMessage(true);
}
