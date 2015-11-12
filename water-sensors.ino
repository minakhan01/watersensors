#include <Bridge.h>
#include <HttpClient.h>

int sensorPin = A3;    // select the input pin for the potentiometer
int ledPin = 13;      // select the pin for the LED
int sensorValue = 0;  // variable to store the value coming from the sensor
int count = 0;
int buttonOn = 0;
int val = 0;
int inPin = 7;   // choose the input pin (for a pushbutton)
dht DHT;
void setup() {
  // Bridge takes about two seconds to start up
  // it can be helpful to use the on-board LED
  // as an indicator for when it has initialized
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  pinMode(inPin, INPUT);    // declare pushbutton as input
  Bridge.begin();   // Initialize the Bridge
}

void loop() {
  //This is the "heart" of the program.
  DHT.read11(dht_dpin);

    Serial.print("Current humidity = ");
    Serial.print(DHT.humidity);
    Serial.print("%  ");
    Serial.print("temperature = ");
    Serial.print(DHT.temperature); 
    Serial.println("C  ");
    
    val = digitalRead(inPin);  // read input value
  if (buttonOn == 0 && val == 1) {         // check if the input is HIGH (button released)
    buttonOn = 1;  // turn LED OFF
    digitalWrite(ledPin, HIGH);
  } else if (buttonOn == 1 && val == 1) {
    buttonOn = 0;  // turn LED ON
    digitalWrite(ledPin, LOW);
  }
  
  Serial.print("sensorValue ");
  Serial.println(sensorValue);
  // read the value from the sensor:
  sensorValue = analogRead(sensorPin);
  String sensorString = String(sensorValue);
  String buttonOnString = String(buttonOn);
  String countString = String(count);
  String curlURLCommand = "curl -k -X POST -d '{\"time\": {\".sv\": \"timestamp\"}, \"water_sensor\":"+sensorString + ", \"showering\":"+buttonOnString+", \"humidity\":"+DHT.humidity+", \"temperature\":"+DHT.temperature+"'} 'https://water-conservation.firebaseio.com/"+"data.json'";
  Serial.println("curlCommand");
  Serial.println(curlURLCommand);
  Process p;
  p.runShellCommand(curlURLCommand);
  Serial.flush();
  delay(500);
  count++;
}


