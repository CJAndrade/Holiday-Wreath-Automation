//Carmelito 12/21/2016 Created for the Holiday Wreath Automation project
#include <WiFi101.h>
#include <WiFiClient.h>
#include <WiFiServer.h>
#include <WiFiSSLClient.h>
#include <WiFiUdp.h>
#include <Adafruit_NeoPixel.h>

//Setup NeoPixels strip
#define PIXEL_COUNT 30 //number of NeoPixel part of you setup
#define PIXEL_PIN 6 //Connect the NeoPixel Data in to pin#6
#define PIXEL_TYPE NEO_GRBW + NEO_KHZ400 //https://www.adafruit.com/products/1460
Adafruit_NeoPixel strip = Adafruit_NeoPixel(30, 6, NEO_GRB + NEO_KHZ800);

String readString;
bool firstTime = true;

//Your Wifi router setup at home
char ssid[] = "XXXXXXXXXXX";   //your network SSID (aka WiFi name)
char pass[] = "XXXXXXXXXX"; //your network password
int status = WL_IDLE_STATUS;
WiFiServer server(80);


void setup() {
  Serial.begin(115200);
  strip.begin();
  strip.show();
 //while (!Serial) { //this is just for testing, disable this once you are done with you testing and have you Lipo connected
   //; // wait for serial port to connect. Needed for native USB port only
 //}

  if (WiFi.status() == WL_NO_SHIELD) {
       // In case of connection issues NeoPixels red.
        colorWipe(strip.Color(255, 0, 0), 50);
        delay(3000);
        return;
  }
      // attempt to connect to Wifi network
    while ( status != WL_CONNECTED) {
        colorWipe(strip.Color(0, 0,255), 50); //Blue color
        // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
        status = WiFi.begin(ssid, pass);
        if(status != WL_CONNECTED){
          // wait 10 seconds for connection:
        colorWipe(strip.Color(0, 255, 0), 50);
        delay(10000);
        }

    }
    server.begin();
    printWifiStatus();
   //Some example procedures showing how to display to the pixels, from NeoPixel Strandtest Example
   /*colorWipe(strip.Color(255, 0, 0), 50); // Red
   colorWipe(strip.Color(0, 255, 0), 50); // Green
   colorWipe(strip.Color(0, 0, 255), 50); // Blue
   colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
   //Send a theater pixel chase in...
   theaterChase(strip.Color(127, 127, 127), 50); // White
   theaterChase(strip.Color(127, 0, 0), 50); // Red
   theaterChase(strip.Color(0, 0, 127), 50); // Blue
   rainbow(20);
   rainbowCycle(20);
   theaterChaseRainbow(50);
  */
  colorWipe(strip.Color(0, 0, 0), 50); // stop NeoPixel color
}

void loop() {
  WiFiClient client = server.available();   // listen for incoming clients
  if (client)
  {
   while (client.connected()) {
      if (client.available()) {
        char c = client.read();
        //read one by one char HTTP request
        if (readString.length() < 100) {
          //storing characters to string
          readString += c;
         }
         //if HTTP request has ended
         if (c == '\n') {
           //Serial.println(readString);//comment out for deployment of the project
           client.println("HTTP/1.1 200 OK"); //send new page
           client.println("Content-Type: text/html");
           client.println("Connection: close");
           client.println();
           client.println("<!DOCTYPE HTML>");
           client.println("<html>");
           client.println("<head>");
           client.println("<title>Light up Holiday Wreath</title>");
           //css for the big buttons posted at https://github.com/CJAndrade/Holiday-Wreath-Automation
           client.println("<link href='https://rawgit.com/CJAndrade/Holiday-Wreath-Automation/master/buttonColors.css ' rel='stylesheet' type='text/css'>");
           client.println("</head>");
           client.println("<body>");
           client.println("<h1>Holiday Wreath Automation</h1>");
           client.println("<br/><table><tr><td><a href=\"/?rainbow\"\"><button>Rainbow</button></a></td>");
           client.println("<td><a href=\"/?chaseReed\"\"><button>Run Red</button></a></td>");
           client.println("<td><a href=\"/?chaseWhite\"\"><button>Run White</button></a></td></tr><tr>");
           client.println("<td><a href=\"/?red\"\"><button>Red</button></a></td>");
           client.println("<td><a href=\"/?green\"\"><button> &nbsp;Green&nbsp;</button></a></td>");
           client.println("<td><a href=\"/?blue\"\"><button>Blue</button></a></td>");
           client.println("</tr><tr><td></td><td>");
           client.println("<a href=\"/?stopAll\"\"><button>&nbsp;stop&nbsp;</button></a>");
           client.println("</td><td></td></table>");
           client.println("<br/>");
           client.println("</body>");
           client.println("</html>");
           delay(5);
           //stopping client
           client.stop();

           if (readString.indexOf("?red") >0){
            colorWipe(strip.Color(255, 0, 0), 50); //Red NeoPixel
            delay(100);
           }

           if (readString.indexOf("?green") >0){
             Serial.println("green");
             colorWipe(strip.Color(0, 255, 0), 50); //Green
             delay(100);
           }
           if (readString.indexOf("?blue") >0){
            colorWipe(strip.Color(0, 0, 255), 50); //Blue
            delay(100);
           Serial.println("blue");
           }
           if (readString.indexOf("?rainbow") >0){
            rainbowCycle(20);
            Serial.println("rainbow");
            delay(10);
            }
            if (readString.indexOf("?chaseReed") >0){
            theaterChase(strip.Color(127, 0, 0), 50); //Red theater pixel chase
            Serial.println("redChase");
            delay(10);
            }
            if (readString.indexOf("?chaseWhite") >0){
            theaterChase(strip.Color(127, 127, 127), 50); // White theater pixel chase
            Serial.println("whiteChase");
            delay(10);
            }
           if (readString.indexOf("?stopAll") >0){
            colorWipe(strip.Color(0, 0, 0), 50); //clear the pixels
            Serial.println("stopAll");
            delay(250);
           }
            //clearing string for next read
            readString="";
         }
       }
    }
  }

}

void printWifiStatus() {
    // print the SSID of the network you're attached to:
    Serial.print("SSID: ");
    Serial.println(WiFi.SSID());
    // print your WiFi shield's IP address:
    IPAddress ip = WiFi.localIP();
    Serial.print("IP Address: ");
    Serial.println(ip);
    //signal strength:
    long rssi = WiFi.RSSI();
    Serial.print("signal strength (RSSI):");
    Serial.print(rssi);
    Serial.println(" dBm");
    // print where to go in a browser:
    Serial.print("Your IP address to use in the browser: ");
    Serial.println(ip);
}


//This piece of code is from the Adafruit Strandtest example after adding the NeoPixel library
// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (uint16_t i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
