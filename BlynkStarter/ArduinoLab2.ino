/* Comment this out to disable prints and save space */
#define BLYNK_PRINT Serial

#define LED 2

int LED_state;
int time_count = 0; // timer counter global variable
String content = "";  // null string constant ( an empty string )
const int freq = 5000;     // 5KHz frequency is more than enough. Remember we used 100 before.
const int ledChannel = 0;  // You can have up to 16 pwm channels (0 to 15)
const int resolution = 10; // 10 bit resolution for 1023. Freq should be greater than resolution.

int brightness ;
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
BlynkTimer timer ;

// You should get Auth Token in the Blynk App.
// Go to the Project Settings (nut icon).
char auth[] = "ATtLHmyS_cvavOEOnD1xlY0ry4J0Vm6c";

// Your WiFi credentials.
// Set password to "" for open networks.
// The EE IOT network is hidden. You might not be able to see it.
// But you should be able to connect with these credentials. 
char ssid[32] = "iPhone";
char pass[32] = "Samscott2013";

void setup()
{
  // Serial Monitor
  Serial.begin(115200);
  Blynk.begin(auth, ssid, pass);
    // configure LED PWM functionality
    ledcSetup(ledChannel, freq, resolution);

    // attach the channel to the GPIO to be controlled
    ledcAttachPin(LED, ledChannel);
    timer.setInterval(10L, myTimerEvent);
}

void loop()
{
  Blynk.run();
  timer.run();
}

// If virtual pin 2 controls fade value, 0 to 1023.
BLYNK_WRITE(V2)
{
    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    brightness = param.asInt(); // assigning incoming value from pin V2 to a variable
    if(LED_state ==1){
     ledcWrite(ledChannel, brightness); // Note that this takes ledChannel as an argument, NOT the pin! Set duty = val.
    }
    else{
      ledcWrite(ledChannel, 0);
     
    }
}
BLYNK_WRITE(V1)
{

    // param is a member variable of the Blynk ADT. It is exposed so you can read it.
    int pinValue = param.asInt(); // assigning incoming value from pin V1 to a variable

    // Because V1 is a button, pinValue will be a 0 or a 1.
    if (pinValue == 0) {
        ledcWrite(ledChannel, 0);
        LED_state = 0;
    }
    else {
        ledcWrite(ledChannel, brightness);
        LED_state = 1;
    }
}






void myTimerEvent() // Every 10 ms
{
    if (time_count % 25 == 0) { // every 250 ms
        // Do thing that needs to happen every 0.25 seconds
    }
    if (time_count == 100){
        // Do thing that needs to happen every 1 second
        time_count = 0; // reset time counter
    }
    else {
        // Send serial data to Blynk terminal
        char character;
        while(Serial.available()) { // Check if serial is available every 10 ms
            character = Serial.read();
            content.concat(character);
        }
        if (content != "") {
            Blynk.virtualWrite(3, content);
            content = ""; // Clear String
        }  
    }
    time_count += 1; // Increment on every tick
}

// This function is executed whenever Blynk pin V0 requests data from the ESP32
BLYNK_READ(V0) // Widget in the app READS Virtal Pin V0 with the certain frequency
{
    // This command writes Arduino's uptime in seconds to Virtual Pin V5
    Blynk.virtualWrite(0, millis() / 1000);
}
