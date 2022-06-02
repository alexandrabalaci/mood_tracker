#include <ESP8266wifi.h>
#include <Adafruit_MQTT.h>
#include <ArduinoHttpClient.h>
#include <SoftwareSerial.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_NeoPixel.h>

#define RX 2
#define TX 3

#define ECHO 9
#define TRIG 8
#define BUZZER 13
#define BUTTON_1 4
#define BUTTON_2 5
#define BUTTON_3 6

#define LEDSTRIP    10
#define LED_COUNT 27

Adafruit_NeoPixel strip(LED_COUNT, LEDSTRIP, NEO_RGB + NEO_KHZ800);

SoftwareSerial AT(RX, TX);

/************************/

String ssid     = "mircica"; //Wifi SSID
String password = "moisturiseme"; //Wifi Password

String apiKeyIn = "rrgBl0qtMEt4FcpQ25TSU5XoRouAdiyk";

// ASKSENSORS API host config
String host = "api.asksensors.com";  // API host name
String port = "80";      // port

/************************/
byte down[8] ={ 0b00000, 0b00000, 0b00000, 0b11111, 0b11111, 0b01110, 0b00100, 0b00000};
byte up[8] ={ 0b00000, 0b00100, 0b01110, 0b11111, 0b11111,  0b00000, 0b00000, 0b00000};
byte eh[8] ={0b00000, 0b00000, 0b00000, 0b11111, 0b11111,  0b00000, 0b00000, 0b00000};

LiquidCrystal_I2C lcd(0x27, 16, 2);

/************************/

int counter_1 = 0; // counter for the number of button presses
int counter_2 = 0;
int counter_3 = 0;

int state_1 = 0; // current state of the button
int state_2 = 0;
int state_3 = 0;

int prev_state_1 = 0;     // previous state of the button
int prev_state_2 = 0;
int prev_state_3 = 0;
int r = 0;
int g = 0;
int b = 0;

long duration;
int distance;
boolean commandRecv = false;
String url;
int countTimeCommand = 0;
boolean changed = false;

void setup() {
  Serial.begin(9600);
  AT.begin(115200);
  /* connect to wifi network*/
  sendCommand("AT+CIPMUX=1", 5, "OK");
  sendCommand("AT+CWMODE=1", 10, "OK");
  sendCommand("AT+CWJAP=\"" + ssid + "\",\"" + password + "\"", 10, "OK");
  pinMode(BUTTON_1, INPUT_PULLUP);
  pinMode(BUTTON_2, INPUT_PULLUP);
  pinMode(BUTTON_3, INPUT_PULLUP);
  pinMode(BUZZER, OUTPUT);
  pinMode(TRIG, OUTPUT);
  pinMode(ECHO, INPUT);

  lcd.init();
  lcd.backlight();

  /* create characters to be printed on the lcd*/
  lcd.createChar(0, down);
  lcd.createChar(1, up);
  lcd.createChar(2, eh);
  lcd.clear();

  lcd.setCursor(5, 0);
  lcd.print("Votes:");
  strip.begin();
}


void loop() {
  // --- First Button --- UP ---

  // read the pushbutton input pin:
  state_1 = digitalRead(BUTTON_1);
  state_2 = digitalRead(BUTTON_2);
  state_3 = digitalRead(BUTTON_3);

  if (state_1 != prev_state_1) {
    changed = true;
    if (state_1 == LOW) {
      // off to on:
      counter_1++;
      Serial.println("1 on");
      Serial.print("number of button 1 pushes: ");
      Serial.println(counter_1);
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
    } else {
      // on to off:
      Serial.println("1 off");
    }

    // save the current state as the last state, for next time through the loop
    prev_state_1 = state_1;

    lcd.setCursor(0, 1);
    lcd.print(counter_1);
    lcd.setCursor(2, 1);
    lcd.write(byte(1));

    if (counter_1 % 10 == 0 && counter_1 != 0) {
      Serial.print("button 1 pressed 20 times!");
      // send updated data to askSensors
      url = "GET /write/" + apiKeyIn + "?module3=" + counter_1;
      sendCommand("AT+CIPMUX=1", 5, "OK");
      sendCommand("AT+CIPSTART=0,\"TCP\",\"" + host + "\"," + port, 10, "OK");
      sendCommand("AT+CIPSEND=0," + String(url.length() + 4), 10, ">");

      Serial.println(counter_1);
      Serial.print("********** requesting URL: ");
      Serial.println(url);
      AT.println(url);
      sendCommand("AT+CIPCLOSE=0", 10, "OK");

      Serial.println("********** Close TCP Connection ");
      Serial.println("*****************************************************");
    }
  }

  // --- Second Button --- EH----
  if (state_2 != prev_state_2) {
    changed = true;
    if (state_2 == LOW) {
      // off to on:
      counter_2++;
      Serial.println("2 on");
      Serial.print("number of button 2 pushes: ");
      Serial.println(counter_2);
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);
    } else {
      // on to off:
      Serial.println("2 off");
      //digitalWrite(BUZZER, LOW);
    }
    // save the current state as the last state, for next time through the loop
    prev_state_2 = state_2;

    lcd.setCursor(6, 1);
    lcd.print(counter_2);
    lcd.setCursor(8, 1);
    lcd.write(byte(2));

    if (counter_2 % 10 == 0 && counter_2 != 0) {
      Serial.print("button 2 pressed 20 times!");
      // send updated data to askSensors
      url = "GET /write/" + apiKeyIn + "?module2=" + counter_2;
      sendCommand("AT+CIPMUX=1", 5, "OK");
      sendCommand("AT+CIPSTART=0,\"TCP\",\"" + host + "\"," + port, 10, "OK");
      sendCommand("AT+CIPSEND=0," + String(url.length() + 4), 10, ">");

      Serial.println(counter_2);
      Serial.print("********** requesting URL: ");
      Serial.println(url);
      AT.println(url);
      sendCommand("AT+CIPCLOSE=0", 10, "OK");

      Serial.println("********** Close TCP Connection ");
      Serial.println("*****************************************************");
    }
  }

  // --- Third Button --- DOWN ----
  if (state_3 != prev_state_3) {
    changed = true;
    if (state_3 == LOW) {
      // off to on:
      counter_3++;
      Serial.println("3 on");
      Serial.print("number of button 3 pushes: ");
      Serial.println(counter_3);
      digitalWrite(BUZZER, HIGH);
      delay(100);
      digitalWrite(BUZZER, LOW);


    } else {
      //  on to off:
      Serial.println("3 off");
      //digitalWrite(BUZZER, LOW);
    }
    // save the current state as the last state, for next time through the loop
    prev_state_3 = state_3;

    lcd.setCursor(12, 1);
    lcd.print(counter_3);
    lcd.setCursor(14, 1);
    lcd.write(byte(0));

    if (counter_3 % 10 == 0 && counter_3 != 0) {
      Serial.print("button 3 pressed 20 times!");
      // send updated data to askSensors
      url = "GET /write/";
      url += apiKeyIn;
      url +=  "?module1=";
      url += counter_3;
      sendCommand("AT+CIPMUX=1", 5, "OK");
      sendCommand("AT+CIPSTART=0,\"TCP\",\"" + host + "\"," + port, 10, "OK");
      sendCommand("AT+CIPSEND=0," + String(url.length() + 4), 10, ">");


      Serial.println(counter_3);
      Serial.print("********** requesting URL: ");
      Serial.println(url);
      AT.println(url);
      sendCommand("AT+CIPCLOSE=0", 10, "OK");

      Serial.println("********** Close TCP Connection ");
      Serial.println("*****************************************************");
    }

  }

  //Ultrasonic Sensor on/off
  digitalWrite(TRIG, LOW);
  delayMicroseconds(2);
  // Trigger position read every 10 seconds
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(ECHO, HIGH);

  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  //  Serial.print("Distance: ");
  //  Serial.print(distance);
  //  Serial.println(" cm");

  /* turn on the led band in case of action in its proximity*/
  if (distance < 80) {
    Serial.println("turning light on");
     Sparkle(0);
    //pulse(30);

  } else {
    for (int i = 0; i < LED_COUNT; i++) { 
      strip.setPixelColor(i, strip.Color(0, 0, 0));
    }
    strip.show();
  }

}

// sendCommand to asksensors
void sendCommand(String command, int maxTime, char readReplay[]) {
  Serial.print("AT command:");
  Serial.println(command);

  while (countTimeCommand < (maxTime * 1))
  {
    AT.println(command);//at+cipsend
    if (AT.find(readReplay)) //ok
    {
      commandRecv = true;
      break;
    }

    countTimeCommand++;
  }
  Serial.print("...Result:");
  if (commandRecv == true) {
    Serial.println("DONE");
    countTimeCommand = 0;
  }
  if (commandRecv == false) {
    Serial.println("FAILED");
    countTimeCommand = 0;
  }
  commandRecv = false;
}


  /*lights settings */
void Sparkle(int SpeedDelay) {
  for (int i = 0 ; i < 255; i++) {
    setColor();
    int Pixel = random(LED_COUNT);
    int Pixl = random(LED_COUNT);
    int Pix = random(LED_COUNT);
    setPixel(Pixel, r, g, b);
    setPixel(Pixl, r, g, b);
    setPixel(Pix, r, g, b);
    strip.show();
    delay(SpeedDelay);
    setPixel(Pixel, 0, 0, 0);
    setPixel(Pixl, 0, 0, 0);
    setPixel(Pix, 0, 0, 0);
  }
}

void setColor() {
  r = random(0, 255);
  g = random(0, 255);
  b = random(0, 255);
}
void setPixel(int Pixel, byte red, byte green, byte blue) {
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < LED_COUNT; i++ ) {
    setPixel(i, red, green, blue);
  }
  strip.show();
}

void pulse(int wait) {
  int i, j;
  int brightness = 255;

  for(i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(wait);
  for(j = 170; j >= 135; --j) {
    for(i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, wheel((i+j) & 255));
    }
    strip.show();
    brightness -= 6;
    strip.setBrightness(brightness);
    delay(wait);
  }

  for(j = 135; j < 1170; j++) {
    for(i = 0; i < LED_COUNT; i++) {
      strip.setPixelColor(i, wheel((i+j) & 255));
    }
    strip.show();
    brightness += 6;
    strip.setBrightness(brightness);
    delay(wait);
  }
  
  for(i = 0; i < LED_COUNT; i++) {
    strip.setPixelColor(i, strip.Color(255, 255, 255));
  }
  strip.show();
  delay(wait);

}

int wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}

void bounce(byte red, byte green, byte blue, int size, int SpeedDelay, int ReturnDelay){

  for(int i = 0; i < LED_COUNT-size-2; i++) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= size; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+size+1, red/10, green/10, blue/10);
    strip.show();
    delay(SpeedDelay);
  }
  delay(ReturnDelay);

  for(int i = LED_COUNT-size-2; i > 0; i--) {
    setAll(0,0,0);
    setPixel(i, red/10, green/10, blue/10);
    for(int j = 1; j <= size; j++) {
      setPixel(i+j, red, green, blue);
    }
    setPixel(i+size+1, red/10, green/10, blue/10);
    strip.show();
    delay(SpeedDelay);

  }
   delay(ReturnDelay);
}
