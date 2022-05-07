#include <WiFi.h>

const char* ssid     = "EN20380496";

WiFiServer server(80);
WiFiClient client;
String header;

String output4State = "off";
String output2State = "off";

const int output4 = 4;
const int output2 = 2;

SemaphoreHandle_t xSemaphore;

void Hotspot(void *parameters) {
  WiFi.softAP(ssid);
  Serial.print("Connecting to ");
  IPAddress IP=WiFi.softAPIP();
  server.begin();
  Serial.print(ssid);
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(IP);

  Serial.print("Connected to hotspot on CORE ");
  Serial.println(xPortGetCoreID());

  vTaskDelete(NULL);
}

void Web(void *parameters) {
  
  while(1){
   WiFiClient client = server.available();   // Listen for incoming clients
   if (client) {                             // If a new client connects,
      Serial.println("Connected");            // print a message out in the serial port
      String currentLine = "";                // make a String to hold incoming data from the client
      while (client.connected()) {            // loop while the client's connected
        Serial.print("Connected to WebServer on CORE ");
        Serial.println(xPortGetCoreID());
         if (client.available()) {             // if there's bytes to read from the client,
             char c = client.read();             // read a byte, then
             Serial.write(c);                    // print it out the serial monitor
             header += c;
             if (c == '\n') {                    // if the byte is a newline character
             // if the current line is blank, you got two newline characters in a row.
             // that's the end of the client HTTP request, so send a response:
             if (currentLine.length() == 0) {
                 // HTTP headers always start with a response code (e.g. HTTP/1.1 200 OK)
                 // and a content-type so the client knows what's coming, then a blank line:
                 client.println("HTTP/1.1 200 OK");
                 client.println("Content-type:text/html");
                 client.println("Connection: close");
                 client.println();
            
                  // Display the HTML web page
                 client.println("<!DOCTYPE html><html>");
                 client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
                 client.println("<link rel=\"icon\" href=\"data:,\">");
            
                  // CSS to style the on/off buttons 
                  // Feel free to change the background-color and font-size attributes to fit your preferences
                  client.println("<style>html { font-family: Calibri; display: inline-block; margin: 100px auto; text-align: Center; text}");
                  client.println(".button { background-color: #4CFF50; border: none; color: white; padding: 20px 40px;");
                  client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
                  client.println(".button2 {background-color: #555555;}</style></head>");
            
                  // Web Page Heading
                  client.println("<body><h1>EN20380496 LAB 3</h2>");
                            
                  // turns the GPIOs on and off
                  if (header.indexOf("GET /4/on") >= 0) {
                      Serial.println(header);
                      Serial.println("GPIO 4 on");
                      output4State = "on";
                      digitalWrite(output4, HIGH);
                  } else if (header.indexOf("GET /4/off") >= 0) {
                      Serial.println("GPIO 4 off");
                      output4State = "off";
                      digitalWrite(output4, LOW);
                  } else if (header.indexOf("GET /2/on") >= 0) {
                      Serial.println("GPIO 2 on");
                      output2State = "on";
                      digitalWrite(output2, HIGH);
                  } else if (header.indexOf("GET /2/off") >= 0) {
                      Serial.println("GPIO 2 off");
                      output2State = "off";
                      digitalWrite(output2, LOW);
                  }
            
                  // If the output4State is off, it displays the ON button       
                  if (output4State=="off") {
                     client.println("<p><a href=\"/4/on\"><button class=\"button\">LED 4 ON</button></a></p>");
                  } else {
                     client.println("<p><a href=\"/4/off\"><button class=\"button button2\">LED 4 OFF</button></a></p>");
                  } 
               
                  // If the output2State is off, it displays the ON button       
                  if (output2State=="off") {
                     client.println("<p><a href=\"/2/on\"><button class=\"button\">LED 2 ON</button></a></p>");
                  } else {
                     client.println("<p><a href=\"/2/off\"><button class=\"button button2\">LED 2 OFF</button></a></p>");
                  }
                  client.println("</body></html>");
            
                  // The HTTP response ends with another blank line
                  client.println();
                  // Break out of the while loop
                  break;
                  } else { // if you got a newline, then clear currentLine
                       currentLine = "";
                  }
             } else if (c != '\r') {  // if you got anything else but a carriage return character,
                  currentLine += c;      // add it to the end of the currentLine
         }
      }
    }
    // Clear the header variable
    header = "";
    // Close the connection
    client.stop();
    Serial.println(header);
    Serial.println("Disconnected.");
    Serial.println("");
  }
   
}
}

void setup() {
  Serial.begin(115200);
  
  pinMode(output4, OUTPUT);
  pinMode(output2, OUTPUT);

  digitalWrite(output4, LOW);
  digitalWrite(output2, LOW);
  
  xSemaphore = xSemaphoreCreateMutex();
  if( xSemaphore == NULL )
  {
   
    xTaskCreatePinnedToCore(Web, "Web Client and LED control", 1024, NULL, 1, NULL, 1);
  }
  {
     xTaskCreatePinnedToCore(Hotspot, "Hotspot", 4000, NULL, 1, NULL, 0);
  }



}

void loop() { 
  
}
