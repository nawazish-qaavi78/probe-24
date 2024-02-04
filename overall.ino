#include <ESP8266WiFi.h>
#include <Servo.h>

const char* ssid = "narzo 50";
const char* password = "brokenow";

WiFiServer server(80);
Servo servo;


int mode=1; // mode1 is gamepad, 2 is hand following, 3 is collision avoidance
int inputDirection = 0; // 0 is for stop, 1 is for forward, 2 is for back, 3 is for left, 4 is for right

int speed=16; //D0
int speed2 = 5; //D1

int in1= 4; //D2  //left motor (YELLOW WIRES) //assuming clockwise
int in2= 0; // D3

int in3= 2; //D4 //right motor (RED WIRES)
int in4= 14; // D5

int trig=12; //D6
int echo=13; //D7

// CHECK 
int leftir=9;
int rightir=10;

int rightdis=0, leftdis=0, middis=0;

void webpage(WiFiClient client){
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/html");
  client.println(""); 
  
  client.println("<!DOCTYPE html>");
    client.println("<html lang=\"en\">");
    client.println("<head>");
      client.println("<meta charset=\"UTF-8\">");
      client.println("<meta name=\"viewport\" content=\"width=device-width, initial-scale=1.0\">");
    
      client.println("<title>IoT</title>");

      client.println("<style>");
        client.println("body {");
            client.println("display: flex;");
            client.println("flex-direction: column;");
            client.println("align-items: center;");
            client.println("justify-content: center;");
            client.println("min-height: 100vh;");
            client.println("margin: 0;");
        client.println("}");

        client.println("div {");
            client.println("text-align: center;");
        client.println("}");

        client.println("#gamepad {");
            client.println("margin-top: 20px;");
        client.println("}");

        client.println("#gamepad a {");
            client.println("margin: 5px;");
            client.println("padding: 10px;");
            client.println("color: #000;");
            client.println("border: 1px solid #000;");
        client.println("}");

        
        client.println("a {");
            client.println("text-decoration: none;");
        client.println("}");
    client.println("</style>");
    
  client.println("</head>");
  client.println("<body>");
    client.println("<div>");
        client.println("<h1>Modes</h1>");
        client.println("<a href=\"/mode1\"> <button>Gamepad</button> </a>");
        client.println("<a href=\"/mode2\"> <button>Hand Following</button> </a>");
        client.println("<a href=\"/mode3\"> <button>Collision Avoidance</button> </a>");
    client.println("</div>");
    client.println("<div id=\"gamepad\">");
            client.println("<a href=\"/up\">▲</a>");
            client.println("<div style=\"margin: 30px;\">");
                client.println("<a href=\"/left\">◄</a>");
                client.println("<a href=\"/stop\">◯</a>");
                client.println("<a href=\"/right\">►</a>");
            client.println("</div>            ");
            client.println("<a href=\"/down\">▼</a>");
    client.println("</div>");
  client.println("</body>");
  client.println("</html>");
}

void forward(){
  digitalWrite(speed,HIGH);
  digitalWrite(speed2,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in3,HIGH);
  digitalWrite(in2,LOW);
  digitalWrite(in4,LOW);
  Serial.println("FORWARD");
}

void backward(){
  digitalWrite(speed,HIGH);
  digitalWrite(speed2,HIGH);
  digitalWrite(in2,HIGH);
  digitalWrite(in4,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in3,LOW);
  Serial.println("BACKWARD");
}

void rightturn(){
  digitalWrite(speed,HIGH);
  digitalWrite(speed2,HIGH);
  digitalWrite(in1,LOW);
  digitalWrite(in3,HIGH);
  digitalWrite(in2,HIGH);
  digitalWrite(in4,LOW);
  Serial.println("RIGHT");
}

void leftturn(){
  digitalWrite(speed,HIGH);
  digitalWrite(speed2,HIGH);
  digitalWrite(in1,HIGH);
  digitalWrite(in3,LOW);
  digitalWrite(in2,LOW);
  digitalWrite(in4,HIGH);
  Serial.println("LEFT");
}

void stop(){
  digitalWrite(speed, LOW);
  digitalWrite(speed2,LOW);
  // Serial.println("STOP");
}

int distance(){
  digitalWrite(trig,LOW);
  delayMicroseconds(10);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig,LOW);
  int tt=pulseIn(echo, HIGH);
  int d =(0.0343 * tt) / 2; // speed = 0.0343 cm/microsecond
  Serial.print("distance =");
  Serial.print(d);
  Serial.println(" cm");
  return d;
}

void readInput(String request){

  if (request.indexOf("stop") != -1){ 
    inputDirection=0;
  }

  if (request.indexOf("mode1") != -1){ 
      mode=1;
  }
  else if (request.indexOf("mode2") != -1){ 
    mode =2;
  }
  else if (request.indexOf("mode3") != -1){ 
    mode = 3;
  }
  
  if(mode==1){
    if (request.indexOf("up") != -1){ 
      inputDirection=1;
    }
    else if (request.indexOf("down") != -1){ 
      inputDirection=2;
    }
    else if (request.indexOf("left") != -1){ 
      inputDirection=3;
    }
    else if (request.indexOf("right") != -1){ 
      inputDirection=4;
    } 
  }   
}

void handFollow(){
  Serial.println("Handfollow");
  int leftSensor = digitalRead(leftir);  // Read sensor values directly
  int rightSensor = digitalRead(rightir);
  if (leftSensor == HIGH && rightSensor == LOW) {
    leftturn();
  } else if (leftSensor == LOW && rightSensor == HIGH) {
    rightturn();
  } else if (leftSensor == HIGH && rightSensor == HIGH) {
    int action = distance();
    if (action >= 15) {
      forward();
    } else if (action <= 10) {
      backward();
    } else {
      stop();
    }
  } else {
    stop();
  }
}

void collisonAvoidance(){
  Serial.println("collisonAvoidance");
  servo.write(90);
  delay(500);
  middis=distance();
  if(middis <=15){
    stop();
    delay(500);
    servo.write(10);
    delay(500);
    leftdis=distance();
    delay(250);
    servo.write(180);
    delay(500);
    rightdis=distance();
    delay(250);
    servo.write(90);
    delay(1000);
    if(rightdis > leftdis){
      rightturn();
      delay(250);
    }
    else if(leftdis > rightdis){
      leftturn();
      delay(250);
    }
    else if((rightdis <=15) || (leftdis <=15)){
      backward();
      }
  }
  else{
    forward();
  }
}

void move(){
  switch(inputDirection){
    case 0:
      stop();
      break;
    case 1:
      forward();
      break;
    case 2:
      backward();
      break;
    case 3:
      leftturn();
      break;
    case 4:
      rightturn();
      break;
    default:
      inputDirection=0;
      stop();
  }
}

void setup() {
 
  
  Serial.begin(115200);
  
  
  while(!Serial);

  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("NodeMCU is connected to WiFi");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  server.begin();
  delay(3000);
  pinMode(in3, OUTPUT);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(speed,OUTPUT); //l293d driver had only one common enable pin
  pinMode(speed2, OUTPUT);
  pinMode(leftir, INPUT);
  pinMode(rightir, INPUT);
  servo.attach(5);
}

void loop() {
  WiFiClient client;
  client = server.available();


  switch(mode){
    case 1:
      move();
      break;
    case 2: 
      handFollow();
      break;
    case 3:
      collisonAvoidance();
      break;
    default:
      inputDirection=0;
      stop();
      break;
  }

  

 if (client == 1) {
    String request = client.readStringUntil('\n');
    client.flush();
    Serial.println(request);
    readInput(request);

    switch(mode){
      case 1:
        move();
        break;
      case 2: 
        // handFollow();
        break;
      case 3:
        collisonAvoidance();
        break;
      default:
        inputDirection=0;
        stop();
        break;
    }
    
    webpage(client);   
       
  }
}
