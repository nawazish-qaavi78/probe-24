#include <ESP8266WiFi.h>

const char* ssid = "narzo 50";
const char* password = "brokenow";

WiFiServer server(80);

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

    Serial.println("Client disonnected");
    Serial.println("------------------");
    Serial.println("                  ");
}

void setup() {
  pinMode(D1, OUTPUT);
  digitalWrite(D1, HIGH);
  
  Serial.begin(9600);
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
}

void loop() {
  WiFiClient client;
  client = server.available();

 if (client == 1) {
    String request = client.readStringUntil('\n');
    client.flush();
    Serial.println(request);

    if (request.indexOf("mode1") != -1)
    { 
      Serial.println("lol");
    }

    if (request.indexOf("mode2") != -1)
    { 
      Serial.println("Mode 2");
    }

    if (request.indexOf("mode3") != -1)
    { 
      Serial.println("Mode 3");
    }

    if (request.indexOf("up") != -1)
    { 
      Serial.println("Up");
    }
    if (request.indexOf("left") != -1)
    { 
      Serial.println("Left");
    }
    if (request.indexOf("right") != -1)
    { 
      Serial.println("right");
    }
    if (request.indexOf("down") != -1)
    { 
      Serial.println("down");
    }
    if (request.indexOf("stop") != -1)
    { 
      Serial.println("stop");
    }

 
    webpage(client);   
       
  }
}
