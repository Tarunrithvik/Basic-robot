#include<ESP8266WiFi.h>

#define FWDA D5
#define REVA D6
#define FWDB D7
#define REVB D8
const char* ssid ="wifi_username";
const char* password="wifi_passwd";

WiFiServer server(80);

void setup() {
  // put your setup code here, to run once:
  pinMode(REVB, OUTPUT);
  pinMode(FWDB, OUTPUT);
  pinMode(REVA, OUTPUT);
  pinMode(FWDA, OUTPUT);
  Serial.begin(115200);
  WiFi.begin(ssid,password);
  while(WiFi.status()!=WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nWiFi connected");
  server.begin();
  Serial.println("server started");
  Serial.print("USe this  URL to connect : http://");
  Serial.print(WiFi.localIP());
  Serial.print("/");
}

const char html[] = R"(
  <html>
  <body>
  <p align = 'center'>
  <a href=\"/W\"><button>FORWARD</button></a>
  <a href=\"/A\"><button>LEFT</button></a>
  <a href=\"/D\"><button>RIGHT</button></a>
  <a href=\"/S\"><button>REVERSE</button></a>
  <a href=\"/X\"><button>STOP</button></a>
  </p>
  </body>
  </html>

)";


void loop() {
  // put your main code here, to run repeatedly:
  WiFiClient client = server.available();
  if (!client)
  {
    return;
  }
  Serial.println("new client");

  while(!client.available())
  {
    delay(1);
  }
  String request = client.readStringUntil('\r');
  Serial.println(request);
  client.flush();

  if(request.indexOf("/W") >= 0){
    Execute('W');
  }
  else if(request.indexOf("/A") >= 0){
    Execute('A');
  }
  else if(request.indexOf("/D") >= 0){
    Execute('D');
  }
  else if(request.indexOf("/S") >= 0){
    Execute('S');
  }
  else if(request.indexOf("/X") >= 0){
    Execute('X');
  }

  client.println(html);

}


void Execute(char M) {
  // Forward
  if (M=='W') {
    digitalWrite(FWDA, HIGH);  
    digitalWrite(FWDB, HIGH);  
    digitalWrite(REVA, LOW);
    digitalWrite(REVB, LOW);
  }
  // Backward
  else if(M=='S'){
    digitalWrite(FWDA, LOW);
    digitalWrite(FWDB, LOW); 
    digitalWrite(REVA, HIGH); 
    digitalWrite(REVB, HIGH); 
  }
  // Right
  else if (M == 'D') {
    digitalWrite(FWDA, HIGH); 
    digitalWrite(FWDB, LOW); 
    digitalWrite(REVA, LOW);
    digitalWrite(REVB, HIGH); 
  }
  // Left
  else if (M == 'A') {
    digitalWrite(FWDA, LOW);
    digitalWrite(FWDB, HIGH); 
    digitalWrite(REVA, HIGH); 
    digitalWrite(REVB, LOW);
  }
  // Stop
  else if (M == 'X') {
    digitalWrite(REVB, LOW);
    digitalWrite(FWDB, LOW);
    digitalWrite(REVA, LOW);
    digitalWrite(FWDA, LOW);
  }
}
