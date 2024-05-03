#include <WiFi.h>
#include <ESPAsyncWebSrv.h>
#include <SPIFFS.h>
#include <HTTPClient.h>
#include <ESP_Google_Sheet_Client.h>

const char* ssid = "wifi-campus";
const char* password = "uandes2200";

const char* ap_ssid = "esp32_AP";
const char* ap_password = NULL;

#define PROJECT_ID "prototipo-kinesiologia"
#define CLIENT_EMAIL "esp32-prueba@prototipo-kinesiologia.iam.gserviceaccount.com"
#define USER_EMAIL "thevia@miuandes.cl"
String spreadsheetId = "1P-6eQAs651u6GsCwG5cbRAcgvW-aAuDTiPex3Wk4yYU";
String spreadsheetURL = "https://docs.google.com/spreadsheets/d/1P-6eQAs651u6GsCwG5cbRAcgvW-aAuDTiPex3Wk4yYU/edit";
const char PRIVATE_KEY[] PROGMEM = "-----BEGIN PRIVATE KEY-----\nMIIEvAIBADANBgkqhkiG9w0BAQEFAASCBKYwggSiAgEAAoIBAQDBpYAW/A0o5tU2\nv/Q4DagzW93Gmg7+KEn25pQ/iDjyuy8feZJs6SRxKzKulXNlXRgJoA3V2yPywU6S\nL9hPk/j3v42gX8O0Gpk22F3hfLu9EVQpNoyVN/5XGA242GI8o5t6RuIzoaHH7U4t\nCiZFtxX9On8faDnnz4cnUhDfvaPHlC8SVoqG03hZgVkE3SZVvi80nOX0imuCnl4Z\nxnOep/zPaKTPAgNWk96zLDzAOCfLfEHFKWLGtCGwctcXz3jW+dw5h5/xvdGQgjpb\nKykXAh1T1Z+fqhdroow9pDIg4oHPSeic37+NeyXx2EuvnBevuSK5dLpZ4+6iXjDe\nlaOKayxVAgMBAAECggEAJxEB8eEgsIc9LYH1PZe3vsDCUa5D5w9uuJJVByouvv13\naMSofzNbyD78Rw9ci+8Hie2RzPqcWn2YIdLoM0m5eRXadyfKvht9nHxzGCoHYAHO\ndlAMdZApzh1Kq2dwDia9l6IUQXFSx5g+5MQ+62Kf24oBGm92+ya5b/vU7qZ+E6We\nImgLh8cMZil7ITWGoupsdyd3FptxSrvbRpc2W3jLk6yYz18sMSyW36wypqsp4Cze\ng9mSGwIUP0ABjgXulelWF/0pvlCvOJwGgexuSa77hhFzs2H9WXLWt+ah63QcICHT\nqeag7TGCG+rt/nQCFwu7XyQ+MZgnGCEOGeqcaXjAAwKBgQDmxmIXlHlBRVNyckfE\nmGbYAAUz/W/jh9rlSkmWBPCblJn68Z4svyRGu7rShSrlcPvLdT/uUXKZ5BASyfg8\n+OcrP9Z4JpEzRLFjOZZAWpCwZBl6/lJ+dydqmPHumDquNMlKk2jB/eUZoRomedn7\nVofKKy5SuHpe75m+01voZJxpxwKBgQDW0C1gLaN4KVXFmrtpnI85DAeYcbcn1QX7\nSPo1inmkT7SkCY5T6uvP6zlVzlDuD/sZlNSrlM2uJiAp9z4foWSLtz61n6ifyAfO\n1dtEm8YBlKUdsShWy3y0yv+A/4LXxXzk3rKBEGfLjy0TAvh6wijnz5n+2CcgEZAT\nTd3mA7KZAwKBgFdiZ2JmZkPA7Ig5ndNjog3IdC/rl6KgntKT/qk/1ZwT+Pp841gu\nSJ5AF83/fu7CiZXRIBQt4ydCpJN4xvbPiTlnQt7cO78eE81u0OYtZPxl6wAXrUUV\n+Wbz4yhlnFK6yj3i85gBe+7oge5v1YrGiUSUdUX9dNjeAWmwvuyQrGmhAoGAQvD2\n4vz17zE6aGzysFO4kjd8enYddI0+4vL3/g1cIOcxzNipgN85gz6zTFkSAU37mxo1\nOA8UFYwOA+XndyXtVljRcvoBK2HDhSJBxVyh1MZEuj8F6h5Zi2GjQ8jcNm91n0EC\n8ZNyNQ42JrGLS34MD0kznSYw8x5l/o4Kc259jo8CgYAVyeSkFYlUVGSf/eWcUWZ+\ngY1tRcI88x3ZKfmE3Mxz5j8ugOrTuwseGs+3Lw6LWMW3nkU4yLij1Ir6Z/v011+3\ntb2Xtww/l81QfKYHURLsC9XXerxwE6fkDKTkxl315cfvERAKxk7eOBpyeMb+UWPe\nEWnUpYbDdf8AUDUJrW+gEQ==\n-----END PRIVATE KEY-----\n";

String header;
char numberArray[20];
bool taskComplete = false;
bool grequest = false;
bool ready;
String username;

bool procedureStarted = false;
bool techo = false;
bool fondo = false;

int ZonaA;
int ZonaB;
int ZonaC;
int ZonaD;

char last = ' ';

String tiempoTotal;

const int pinEntrada = 32;
const int pinTecho = 35;
const int pinZonaAi = 27;
const int pinZonaAf = 17;
const int pinZonaBi = 18;
const int pinZonaBf = 19;
const int pinZonaCi = 26;
const int pinZonaCf = 33;
const int pinZonaDi = 25;
const int pinFondo = 16;

unsigned long startTime = 0;
unsigned long startTimeZonaA = 0;
unsigned long startTimeZonaB = 0;
unsigned long startTimeZonaC = 0;
unsigned long startTimeZonaD = 0;

unsigned long elapsedTime = 0;
unsigned long elapsedTimeZonaA = 0;
unsigned long elapsedTimeZonaB = 0;
unsigned long elapsedTimeZonaC = 0;
unsigned long elapsedTimeZonaD = 0;

AsyncWebServer server(80);

void googlesheetrequest() {
  Serial.println("test gs0");
  FirebaseJson response;
  bool success;
  struct tm timeinfo;
  char timeStringBuff[50];
  String fechahora;
  FirebaseJson valueRange;

  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  strftime(timeStringBuff, sizeof(timeStringBuff), "%A, %B %d %Y %H:%M:%S", &timeinfo);
  fechahora = timeStringBuff;
  fechahora.replace(" ", "-");

  valueRange.add("majorDimension", "ROWS");
  valueRange.set("values/[0]/[0]", fechahora);
  valueRange.set("values/[0]/[1]", username);
  valueRange.set("values/[0]/[2]", tiempoTotal);
  valueRange.set("values/[0]/[3]", ZonaA);
  valueRange.set("values/[0]/[4]", ZonaB);
  valueRange.set("values/[0]/[5]", ZonaC);
  valueRange.set("values/[0]/[6]", ZonaD);
  Serial.println("test gs1");
  success = GSheet.values.append(&response /* returned response */, spreadsheetId /* spreadsheet Id to append */, "Master" /* range to append */, &valueRange /* data range to append */);
  
  Serial.println("test gs2");
  if (success)
    response.toString(Serial, true);
  else
    Serial.println(GSheet.errorReason());
  Serial.println();
}

void handleLogin(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_POST) {
    username = request->arg("username");
    Serial.println(username);
    request->send(200, "text/plain", "Login successful");
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void handleStart(AsyncWebServerRequest *request) {
  ZonaA = 0;
  ZonaB = 0;
  ZonaC = 0;
  ZonaD = 0;
  if (request->method() == HTTP_POST) {
    procedureStarted = true;
    Serial.println("Start");
    request->send(200, "text/plain", "Procedure started");
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
}

void handleEnd(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_POST) {
    procedureStarted = false;
    tiempoTotal = request->arg("timerValue");
    Serial.println("End");
    request->send(200, "text/plain", "Procedure ended");
  } else {
    request->send(400, "text/plain", "Bad Request");
    Serial.println("test end");
  }
}

void handleResult(AsyncWebServerRequest *request) {
  if (request->method() == HTTP_GET) {
    ZonaA = elapsedTimeZonaA / 1000;
    ZonaB = elapsedTimeZonaB / 1000;
    ZonaC = elapsedTimeZonaC / 1000;
    ZonaD = elapsedTimeZonaD / 1000;
    String result = String(ZonaA)+","+String(ZonaB)+","+String(ZonaC)+","+String(ZonaD);
    request->send(200, "text/plain", result);
  } else {
    request->send(400, "text/plain", "Bad Request");
  }
  Serial.println(elapsedTimeZonaA);
  Serial.println(elapsedTimeZonaB);
  Serial.println(elapsedTimeZonaC);
  Serial.println(elapsedTimeZonaD);
  Serial.println("test result");
  googlesheetrequest();
}

void calculateTime() {
  unsigned long elapsedTime = millis() - startTime;
  startTime = millis();
  switch (last) {
    case 'A': elapsedTimeZonaA += elapsedTime; break;
    case 'B': elapsedTimeZonaB += elapsedTime; break;
    case 'C': elapsedTimeZonaC += elapsedTime; break;
    case 'D': elapsedTimeZonaD += elapsedTime; break;
  }
}

void setup() {
  Serial.begin(115200);

  if (!SPIFFS.begin(true)) {
    Serial.println("An Error has occurred while mounting SPIFFS");
    return;
  }

  WiFi.mode(WIFI_AP_STA);
  WiFi.softAP(ap_ssid, ap_password);
  Serial.print("Access Point IP: ");
  Serial.println(WiFi.softAPIP());

  WiFi.begin(ssid, password);
  Serial.print("Connecting to WiFi");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(100);
  }
  Serial.println();
  Serial.print("WiFi local IP: ");
  Serial.println(WiFi.localIP());

  server.serveStatic("/", SPIFFS, "/").setDefaultFile("login.html");

  server.on("/login", HTTP_POST, handleLogin);
  server.on("/start", HTTP_POST, handleStart);
  server.on("/end", HTTP_POST, handleEnd);
  server.on("/result", HTTP_GET, handleResult);

  server.begin();
  GSheet.setPrerefreshSeconds(10 * 60);

  GSheet.begin(CLIENT_EMAIL, PROJECT_ID, PRIVATE_KEY);

  pinMode(pinEntrada, INPUT_PULLUP);
  pinMode(pinTecho, INPUT_PULLUP);
  pinMode(pinZonaAi, INPUT_PULLUP);
  pinMode(pinZonaAf, INPUT_PULLUP);
  pinMode(pinZonaBi, INPUT_PULLUP);
  pinMode(pinZonaBf, INPUT_PULLUP);
  pinMode(pinZonaCi, INPUT_PULLUP);
  pinMode(pinZonaCf, INPUT_PULLUP);
  pinMode(pinZonaDi, INPUT_PULLUP);
  pinMode(pinFondo, INPUT_PULLUP);
}

void loop()
{
  ready = GSheet.ready();
  if (procedureStarted) {
    int sensorEntrada = digitalRead(pinEntrada);
    int sensorTecho = digitalRead(pinTecho);
    int sensorAi = digitalRead(pinZonaAi);
    int sensorAf = digitalRead(pinZonaAf);
    int sensorBi = digitalRead(pinZonaBi);
    int sensorBf = digitalRead(pinZonaBf);
    int sensorCi = digitalRead(pinZonaCi);
    int sensorCf = digitalRead(pinZonaCf);
    int sensorDi = digitalRead(pinZonaDi);
    int sensorFondo = digitalRead(pinFondo);

    if (sensorEntrada == HIGH) {
      Serial.println("Entrada");
      if (last != 'E' && last != ' ') {
        calculateTime();
      }
      last = 'E';
    }
    else if (sensorAi == HIGH) {
      Serial.println("Ai");
      if (last != 'A') {
        calculateTime();
      }
      last = 'A';
    }
    else if (sensorTecho == HIGH) {
      Serial.println("Techo");
      techo = true;
    }
    else if (sensorAf == HIGH) {
      Serial.println("Af");
      if (last != 'A') {
        calculateTime();
      }
      last = 'A';
    }
    else if (sensorBi == HIGH) {
      Serial.println("Bi");
      if (last != 'B') {
        calculateTime();
      }
      last = 'B';
    }
    else if (sensorBf == HIGH) {
      Serial.println("Bf");
      if (last != 'B') {
        calculateTime();
      }
      last = 'B';
    }
    else if (sensorCi == HIGH) {
      Serial.println("Ci");
      if (last != 'C') {
        calculateTime();
      }
      last = 'C';
    }
    else if (sensorCf == HIGH) {
      Serial.println("Cf");
      if (last != 'C') {
        calculateTime();
      }
      last = 'C';
    }
    else if (sensorDi == HIGH) {
      Serial.println("Di");
      if (last != 'D') {
        calculateTime();
      }
      last = 'D';
    }
    else if (sensorFondo == HIGH) {
      Serial.println("Fondo");
      if (last != 'F') {
        calculateTime();
      }
      last = 'F';
    }



    /* if (sensorAi == HIGH) {
      if (last != 'A') startTimeZonaA = millis();
      last = 'A';
    }
    else if (sensorBi == HIGH) { 
      if (last != 'B') {
        startTimeZonaB = millis();
        if (last == 'A') elapsedTimeZonaA += startTimeZonaB - startTimeZonaA;
      }
      last = 'B';
    }
    else if (sensorCi == HIGH) {
      if (last != 'C') {
        startTimeZonaC = millis();
        if (last == 'B') elapsedTimeZonaB += startTimeZonaC - startTimeZonaB;
      }
      last = 'C';
    }
    else if (sensorDi == HIGH) {
      if (last != 'D') {
        startTimeZonaD = millis();
        if (last == 'C') elapsedTimeZonaC += startTimeZonaD - startTimeZonaC;
      }
      last = 'D';
    }
    
    //VUELTA
    else if (sensorCf == HIGH) {
      if (last != 'C') {
        startTimeZonaC = millis();
        if (last == 'D') elapsedTimeZonaD += startTimeZonaC - startTimeZonaD;
      }
      last = 'C';
    }
    else if (sensorBf == HIGH) {
      if (last != 'B') {
        startTimeZonaB = millis();
        if (last == 'C') elapsedTimeZonaC += startTimeZonaB - startTimeZonaC;
      }
      last = 'B';
    }
    else if (sensorAf == HIGH) {
      if (last != 'A') {
        startTimeZonaA = millis();
        if (last == 'B') elapsedTimeZonaB += startTimeZonaA - startTimeZonaB;
      }
      last = 'A';
    }
    else if (sensorEntrada == HIGH) {
      if (last == 'A') elapsedTimeZonaA += millis() - startTimeZonaA;
      last = 'E';
    }
    
    //TECHO
    else if (sensorTecho == HIGH) {
      techo = true;
    }
    //FONDO
    else if (sensorFondo == HIGH) {
      fondo = true;
    } */
  }

  delay(10);
}

void tokenStatusCallback(TokenInfo info)
{
  if (info.status == token_status_error)
  {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
    GSheet.printf("Token error: %s\n", GSheet.getTokenError(info).c_str());
  }
  else
  {
    GSheet.printf("Token info: type = %s, status = %s\n", GSheet.getTokenType(info).c_str(), GSheet.getTokenStatus(info).c_str());
  }
}
