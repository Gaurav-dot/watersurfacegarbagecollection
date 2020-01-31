 /*
                            *
                            * Project Name:   <Water Surface Garbage Collector Bot>
                            * Author List:    <Gaurav>
                            * Filename:       <WSGCB_final>
                            * 
                             */

/*###################    <Header Files>    ################################*/

#include <ESP8266WiFi.h>
#include <Servo.h>

/*###################    SSID & Password    ##############################*/

const char* ssid     = "moto";
const char* password = "mkyubtau";

/*##################     Web server port    ####################################*/

WiFiServer server(80);


/*##################     Pin Configuration  ###################################*/

const int motorfin_R_1 = D0;           // Fin motors pins
const int motorfin_R_2 = D1;
const int motorfin_L_1 = D2;
const int motorfin_L_2 = D3;

const int Conveyor_Belt = D4;           // Conveyor belt motors

const int Battery_Indicator = A0;      // Battery indicator input pin

const int Capacity_TrigPin = D6;      // Ultrasonic sensor
const int Capacity_EchoPin = D7;

const int Camera_Servo = D5;          // Camera servo pwm pin
const int Door_Servo = D8;            // Door servo pwm pin
const int Floor_Servo = D9;           // Floor servo pwm pin

/*################## Global variables & constant #################################*/

String header;                         // Variable to store the HTTP request

String motor_status = "OFF";           // Motor Status
String Belt_status = "OFF";            // Belt Status
String Capacity = "Empty";             // Capacity filled
String Camera_status = "default";       //Camera status
String Battery_status = "100%";

unsigned long currentTime = millis();  // Current time
unsigned long previousTime = 0;        // previous time

const long timeoutTime = 2000;         // Define timeout time in milliseconds (example: 2000ms = 2s)

long distance = 0;                     // Ultrasonic distance

Servo Camera_servo, Door_servo, Floor_servo ;                           // Servo class object

/*#################### Prototype Function #################################*/
 /*
* Function Name:  Init
* Input:    None
* Output:   Inilize all the variables 
*/
void Init();
 /*
* Function Name:  Connecting_to_Wifi
* Input:    None
* Output:   Establish connection between Esp and local host
*
*/
void Connecting_to_Wifi();
/*
* Function Name:  Fin_Motor_OFF
* Input:    None
* Output:   Fin Motors off
*/

void Fin_Motor_OFF();

/*
* Function Name:  Fin_Motor_UP
* Input:    None
* Output:   Fin Motors forward
*/

void Fin_Motor_UP();
/*
* Function Name:  Fin_Motor_DOWN
* Input:    None
* Output:   Fin Motors downward
*/
void Fin_Motor_DOWN();

/*
* Function Name:  Fin_Motor_LEFT
* Input:    None
* Output:   Fin Motors left
*/
void Fin_Motor_LEFT();

/*
* Function Name:  Fin_Motor_RIGHT
* Input:    None
* Output:   Fin Motors right
*/
void Fin_Motor_RIGHT();

/*
* Function Name:  Belt_Start
* Input:    None
* Output:   Start conveyor belt
*/
void Belt_Start();

/*
* Function Name:  Belt_Stop
* Input:    None
* Output:   Stop conveyor belt
*/
void Belt_Stop();

/*
* Function Name:  Door
* Input:    None
* Output:   Opens Door
*/

void Door();

/*
* Function Name:  Floor
* Input:    None
* Output:   Levitate floor
*/
void Floor();

/*
* Function Name:  Camera_default
* Input:    None
* Output:   Set comera at default angle
*/
void Camera_default();

/*
* Function Name:  Camera_Left
* Input:    None
* Output:   Set comera at Left 
*/
void Camera_Left();

/*
* Function Name:  Camera_Right
* Input:    None
* Output:   Set comera at Right 
*/
void Camera_Right();

/*
* Function Name:  Capacity_filled
* Input:    None
* Output:   Checks capacity of garbage filled 
*/

void Capacity_filled();

/*
* Function Name:  Battery_percent
* Input:    None
* Output:   Checks power of battery
*/

void Battery_percent();


/*###############################  Set up function ###############################################*/
void setup() {

  Serial.begin(9600);                  // Baud rate for Serial transmission
  Init();                              // Define pins as input and output
  Connecting_to_Wifi();                // Connecting to Hotspot with provied SSID and password
}

/*###############################  Loop function   #################################################*/
void loop() {

  WiFiClient client = server.available();   // Listen for incoming clients

  if (client) {                             // If a new client connects,
    Serial.println("New Client.");          // print a message out in the serial port
    String currentLine = "";                // make a String to hold incoming data from the client
    currentTime = millis();
    previousTime = currentTime;
    while (client.connected() && currentTime - previousTime <= timeoutTime) { // loop while the client's connected
      currentTime = millis();
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

            // Capacity check
            Capacity_filled();
            // Battery percentage check
            Battery_percent();

            // turn the gpio on and off
            if (header.indexOf("GET /OFF") >= 0)  {

              Fin_Motor_OFF();

            }

            if (header.indexOf("GET /UP") >= 0) {

              Fin_Motor_UP();

            }

            if (header.indexOf("GET /DOWN") >= 0)  {

              Fin_Motor_DOWN();

            }

            if (header.indexOf("GET /LEFT") >= 0) {

              Fin_Motor_LEFT();

            }

            if (header.indexOf("GET /RIGHT") >= 0)  {

              Fin_Motor_RIGHT();

            }

            if (header.indexOf("GET /Belt_Start") >= 0) {

              Belt_Start();

            }

            if (header.indexOf("GET /Belt_Stop") >= 0) {

              Belt_Stop();

            }

            if (header.indexOf("GET /unload") >= 0) {

              Door();

            }

            if (header.indexOf("GET /Camera_default") >= 0) {

              Camera_default();

            }

            if (header.indexOf("GET /Camera_Left") >= 0) {

              Camera_Left();

            }

            if (header.indexOf("GET /Camera_Right") >= 0) {

              Camera_Right();

            }

            


            // HTML Web page
            // Display the HTML web page
            client.println("<!DOCTYPE html>");
            client.println("<html lang=\"en\" dir=\"ltr\">");
            client.println("<head> <meta charset=\"utf-8\">");
            client.println("<meta http-equiv=\"refresh\" content=\"10\">");
            client.println("<title>Water Surface Garbage Collection Bot</title>");
            client.println("</head>");
            client.println("<style>");
            client.println(".heading{text-align: center;}");
            client.println(".up{padding-left: 465px;;}");
            client.println(".sim{padding-left: 420px;}");
            client.println(".down{padding-left: 460px;}");
            client.println(".over100{padding-left: 460px;}");
            client.println(".over90{padding-left: 460px;}");
            client.println(".over80{padding-left: 460px;}");
            client.println(".over70{padding-left: 460px;}");
            client.println(".over60{padding-left: 460px;}");
            client.println(".over40{padding-left: 460px;}");
            client.println(".over30{padding-left: 460px;}");
            client.println(".over20{padding-left: 460px;}");
            client.println(".over10{padding-left: 460px;}");
            client.println(".over0{padding-left: 460px;}");
            client.println(".conveyor{padding-left: 440px;}");
            client.println(".unload{padding-left: 460px;}");
            client.println(".angle{padding-left: 440px;}");
            client.println("</style>");
            client.println("<body>");
            client.println("<h4 class=\"heading\">Directions</h4>");
            client.println("<div class=\"up\"><a href=\"UP\"><button type=\"button\" name=\"button\">UP</button></a></div>");
            client.println("<div class=\"sim\"><a href=\"LEFT\"><button type=\"button\" class=\"left\" name=\"button\">Left</button></a>");
            client.println("<a href=\"OFF\"><button type=\"button\" class=\"stop\" name=\"button\">Stop</button></a>");
            client.println("<a href=\"RIGHT\"><button type=\"button\" class=\"right\" name=\"button\">Right</button></a></div>");
            client.println("<div class=\"down\"><a href=\"DOWN\"><button type=\"button\" name=\"down\">Down</button></a></div>");
            client.println("<h4 class=\"heading\">Conveyor Belt</h4>");
            if (Belt_status == "OFF") {
              client.println("<div class=\"conveyor\"><a href=\"Belt_Stop\"><button class=\"button1\">Conveyor OFF</button></a></div>");
            }
            
            else if (Belt_status == "ON")  {
            client.println("<div class=\"conveyor\"><a href=\"Belt_Start\"><button class=\"button1\">Conveyor ON</button></a></div>");
            }
            
            client.println("<h4 class=\"heading\">Unload</h4>");
            client.println("<div class=\"unload\">");
            client.println("<a href=\"unload\"><button type=\"button\" name=\"button\">Unload</button></a></div>");
            
            client.println("<h4 class=\"heading\">Overload</h4>");
            
            if (Capacity == "Capacity100")        
            client.println("<div class=\"over100\"><button type=\"button\" name=\"button\">100</button></div>");

            if (Capacity == "Capacity80")
            client.println("<div class=\"over80\"><button type=\"button\" name=\"button\">80</button></div>");
            
            if (Capacity == "Capacity60")
            client.println("<div class=\"over60\"><button type=\"button\" name=\"button\">60</button></div>");
            
            if (Capacity == "Capacity40")
            client.println("<div class=\"over40\"><button type=\"button\" name=\"button\">40</button></div>");
            
            if (Capacity == "Capacity20")
            client.println("<div class=\"over20\"><button type=\"button\" name=\"button\">20</button></div>");
            
            if (Capacity == "Empty")
            client.println("<div class=\"over0\"><button type=\"button\" name=\"button\">0</button></div>");
            
            client.println("<h4 class=\"heading\">Battery Indicator</h4>");
            if(Battery_status == "100%"){
              client.println("<div class=\"over100\"><button type=\"button\" name=\"button\">100</button></div>");  
            }
            
            else if(Battery_status == "80%") {
              client.println("<div class=\"over80\"><button type=\"button\" name=\"button\">80</button></div>");  
            }
            

            else if(Battery_status == "60%"){
              client.println("<div class=\"over60\"><button type=\"button\" name=\"button\">60</button></div>");  
            }
            

            else if(Battery_status == "40%"){
              client.println("<div class=\"over40\"><button type=\"button\" name=\"button\">40</button></div>");  
            }
            
            
            else if(Battery_status == "20%"){
              client.println("<div class=\"over20\"><button type=\"button\" name=\"button\">20</button></div>");  
            }
            
            
            else if(Battery_status == "10%"){
              client.println("<div class=\"over0\"><button type=\"button\" name=\"button\">10</button></div>");  
            }
            
            
            else if(Battery_status == "0%"){
              client.println("<div class=\"over0\"><button type=\"button\" name=\"button\">Charge!!!</button></div>");  
            }
            
            
            client.println("<h4 class=\"heading\">Camera</h4>");
            client.println("<div class=\"angle\"><a href=\"Camera_Left\"><button type=\"button\" name=\"button\">0</button></a>");
            client.println("<a href=\"Camera_default\"><button type=\"button\" name=\"button\">90</button></a>");
            client.println("<a href=\"Camera_Right\"><button type=\"button\" name=\"button\">180</button></a></div>");
            client.println("</body>");
            client.println("</html>");
            // The HTTP response ends with another blank line
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
    Serial.println("Client disconnected.");
    Serial.println("");


  }


  /*###################################### Function definations *########################################*/

}

void Init() {

  pinMode(motorfin_R_1 , OUTPUT);
  pinMode(motorfin_R_2 , OUTPUT);
  pinMode(motorfin_L_1 , OUTPUT);
  pinMode(motorfin_L_2 , OUTPUT);
  pinMode(Conveyor_Belt , OUTPUT);
  pinMode(Battery_Indicator , INPUT);
  pinMode(Capacity_TrigPin , OUTPUT);
  pinMode(Capacity_EchoPin , INPUT);

  // Servos
  Camera_servo.attach(Camera_Servo);
  Door_servo.attach(Door_Servo);
  Floor_servo.attach(Floor_Servo);

  // Set outputs to LOW
  digitalWrite(motorfin_R_1, LOW);
  digitalWrite(motorfin_R_2, LOW);
  digitalWrite(motorfin_L_1, LOW);
  digitalWrite(motorfin_L_2, LOW);
  digitalWrite(Conveyor_Belt, LOW);
  digitalWrite(Capacity_TrigPin, LOW);

  // Default servo position
  Camera_servo.write(90);
  Door_servo.write(0);
  Floor_servo.write(180);
}

void Connecting_to_Wifi() {

  Serial.print("Connecting to ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
}

void Fin_Motor_OFF()  {

  Serial.println("OFF button is pressed");
  motor_status = "OFF";
  digitalWrite(motorfin_R_1 , LOW);
  digitalWrite(motorfin_R_2 , LOW);
  digitalWrite(motorfin_L_1 , LOW);
  digitalWrite(motorfin_L_2 , LOW);

}
void Fin_Motor_UP() {

  Serial.println("UP button is pressed");
  motor_status = "UP";
  digitalWrite(motorfin_R_1 , HIGH);
  digitalWrite(motorfin_R_2 , LOW);
  digitalWrite(motorfin_L_1 , HIGH);
  digitalWrite(motorfin_L_2 , LOW);

}

void Fin_Motor_DOWN() {

  Serial.println("DOWN button is pressed");
  motor_status = "DOWN";
  digitalWrite(motorfin_R_1 , 0);
  digitalWrite(motorfin_R_2 , 1);
  digitalWrite(motorfin_L_1 , 0);
  digitalWrite(motorfin_L_2 , 1);


}
void Fin_Motor_LEFT() {

  Serial.println("LEFT button is pressed");
  motor_status = "LEFT";
  digitalWrite(motorfin_R_1 , 0);
  digitalWrite(motorfin_R_2 , 1);
  digitalWrite(motorfin_L_1 , 1);
  digitalWrite(motorfin_L_2 , 0);

}


void Fin_Motor_RIGHT()  {

  Serial.println("RIGHT button is pressed");
  motor_status = "RIGHT";
  digitalWrite(motorfin_R_1 , 1);
  digitalWrite(motorfin_R_2 , 0);
  digitalWrite(motorfin_L_1 , 0);
  digitalWrite(motorfin_L_2 , 1);

}

void Belt_Start() {

  Serial.println("Conveyor Belt is on");
  Belt_status = "OFF";
  digitalWrite(Conveyor_Belt , 1);

}

void Belt_Stop()  {

  Serial.println("Conveyor Belt is off");
  Belt_status = "ON";
  digitalWrite(Conveyor_Belt , 0);

}

void Door() {

  Door_servo.write(90);
  //for (int i=0 ; i<=90 ;i++)
  //{Door_servo.write(i);
  //delay(100);}
  Serial.println("Door opened");
  delay(2000);
  Floor();
  Serial.println("Door closed");
  Door_servo.write(0);
  //for(int i=90; i>=0; i--)
  //{Door_servo.write(i);
  //delay(100);}
}

void Floor()  {

  Serial.println("Floor Lefting");
  Floor_servo.write(90);
  //for(int i=0; i<=120; i++)
  //{Floor_servo(i);
  //delay(100);}
  delay(3000);
  Serial.println("Floor Lefted");
  Floor_servo.write(180);
  //for(int i=120; i>=0; i--)
  //{Floor_servo(i);
  //delay(100);}
}


void Camera_default() {

  Serial.println("Camera at default position");
  Camera_servo.write(90);
  Camera_status = "default";

}
void Camera_Left()  {

  Serial.println("Camera at Left position");
  Camera_servo.write(0);
  Camera_status = "Left";

}
void Camera_Right() {

  Serial.println("Camera at Right position");
  Camera_servo.write(180);
  Camera_status = "Right";

}
void Capacity_filled()  {

  digitalWrite(Capacity_TrigPin, LOW);
  delayMicroseconds(2);

  // Sets the trigPin on HIGH state for 10 micro seconds
  digitalWrite(Capacity_TrigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(Capacity_TrigPin, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  long duration = pulseIn(Capacity_EchoPin, HIGH);

  // Calculating the distance
  int distance = duration * 0.034 / 2;

  // Prints the distance on the Serial Monitor
  //Serial.println(distance);
  if (distance <= 5) {

    Serial.println("Capacity at 100%");
    Capacity = "Capacity100";
  }
  else if (distance <= 10)  {

    Serial.println("Capacity at 80%");
    Capacity = "Capacity80";
  }
  else if (distance <= 15)  {

    Serial.println("Capacity at 60%");
    Capacity = "Capacity60";
  }
  else if (distance <= 20)  {

    Serial.println("Capacity at 40%");
    Capacity = "Capacity40";
  }
  else if (distance <= 25)  {

    Serial.println("Capacity at 20%");
    Capacity = "Capacity20";
  }

  else  {

    Serial.println("Empty");
    Capacity = "Empty";
  }

}
void Battery_percent() {
  
  int battery = analogRead(Battery_Indicator);
  if(battery < 800 && battery > 750)
  {
    Serial.println("Battery at 100 %");
    Battery_status = "100%";
  }
  else if (battery < 750 && battery > 700)
  {
    Serial.println("Battery at 80%");
    Battery_status = "80%";
  }

  else if (battery < 700 && battery > 675)
  {
    Serial.println("Battery at 60%");    
    Battery_status = "60%";
  }

  else if (battery < 675 && battery > 725)
  {
    Serial.println("Battery at 40%");    
    Battery_status = "40%";
  }
  
  else if (battery < 725 && battery > 700)
  {
    Serial.println("Battery at 20%");    
    Battery_status = "20%";
  }
  
  else if (battery < 700 && battery > 600)
  {
    Serial.println("Battery below 20%!!! it's recommended charging the battery");    
    Battery_status = "10%";
  } 
  else
  {
    Serial.println("Battery state critical!!!! charge immediatly");
    Battery_status = "0%";
  }
}
