#include <ESP8266WiFi.h>

// Replace with your network credentials
const char* ssid     = "Your network name ";
const char* password = "Your network password";

// Set web server port number to 80
WiFiServer server(80);

// Variable to store the HTTP request
String header;

// Auxiliar variables to store the current output state
String pin_State = "off";

// Assign output variables to GPIO pins
const int pin_num = D5;




void setup() {
Serial.begin(115200);
// Initialize the output variables as outputs
pinMode(pin_num, OUTPUT);

// Set outputs to LOW
digitalWrite(pin_num, LOW);


// Connect to Wi-Fi network with SSID and password
Serial.print("Connecting to ");
Serial.println(ssid);
WiFi.begin(ssid, password);
while (WiFi.status() != WL_CONNECTED) {
  delay(500);
  Serial.print(".");
}
// Print local IP address and start web server
Serial.println("");
Serial.println("WiFi connected.");
Serial.println("IP address: ");
Serial.println(WiFi.localIP());
server.begin();
}

void loop(){
WiFiClient client = server.available();   // Listen for incoming clients

if (client) {                             // If a new client connects,
  Serial.println("New Client.");          // print a message out in the serial port
  String currentLine = "";                // make a String to hold incoming data from the client
 

  while (client.connected()) { // loop while the client's connected
    if (client.available()) {             // if there's bytes to read from the client,
      char c = client.read();             // read a byte, then
                                        // print it out the serial monitor
      header += c;
      if (c == '\n') {     
      

       // if the byte is a newline character
        // if the current line is blank, you got two newline characters in a row.
        // that's the end of the client HTTP request, so send a response:
    
   
        if (currentLine.length() == 0) {
          
          // turns the GPIOs on and off
          
          if (header.indexOf("GET /D5/on") >= 0) {
            Serial.println("pin D5 is on");
            pin_State = "on";
            digitalWrite(pin_num, HIGH);
          }
          else if (header.indexOf("GET /D5/off") >= 0) {
            Serial.println("pin D5 is off");
            pin_State = "off";
            digitalWrite(pin_num, LOW);
          } 
          
    
          // Display the HTML web page
          client.println("<!DOCTYPE html><html>");
          client.println("<head><meta name=\"viewport\" content=\"width=device-width, initial-scale=1\">");
          // CSS to style the on/off buttons 
          // Feel free to change the background-color and font-size attributes to fit your preferences
          client.println("<style>html { font-family: Helvetica; display: inline-block; margin: 0px auto; text-align: center;}");
          client.println(".button { background-color: #195B6A; border: none; color: white; padding: 16px 130px;");
          client.println("text-decoration: none; font-size: 30px; margin: 2px; cursor: pointer;}");
          client.println(".button2 {background-color: #77878A;}</style></head>");
          
          // Web Page Heading
          client.println("<body><h1>AZ Electronics Server</h1>");
          
          // Display current state, and ON/OFF buttons for GPIO D5 
          client.println("<p>Pin D5 - State " + pin_State + "</p>");
          // If the pin_State is off, it displays the ON button       
          if (pin_State=="off") {
            client.println("<p><a href=\"/D5/on\"><button class=\"button\">ON</button></a></p>");
          } else {
            client.println("<p><a href=\"/D5/off\"><button class=\"button button2\">OFF</button></a></p>");
          } 
             
         
           client.println("</body></html>");
          // The HTTP response ends with another blank line
          client.println();
          // Break out of the while loop
          break;
        } else { // if you got a newline, then clear currentLine
          currentLine = "";
        }
      } 
    }
  }
  // Clear the header variable
  header = "";
  // Close the connection
  client.stop();
  Serial.println("Client disconnected.");
  Serial.println("");
}
}
