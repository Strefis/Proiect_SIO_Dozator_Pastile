#include <SSD1306.h>
#include <Wire.h>
#include <TimeLib.h>
#include <DS1307RTC.h>


  //Senzor de detectie
const int IN_A0 = A0; // analog input
int value_A0;
int valoare_prag_detectie = 700; //valoare temporara, trebuie testat irl

//Buttons
const int set_button_pin = 2;
const int panic_button_pin = 3;
const int reset_button_pin = 4;

//LEDS
const int red_led_pin = 5;
const int green_led_pin = 6;

SSD1306 display;
tmElements_t KT;

void scanI2C() {
  Serial.println("I2C scan:");
  for (byte a = 1; a < 127; a++) {
    Wire.beginTransmission(a);
    if (Wire.endTransmission() == 0) {
      Serial.print("  found 0x");
      Serial.println(a, HEX);
    }
  }
  Serial.println("scan done");
}

void setup() {
  Serial.begin(9600);
  Wire.begin();
  Wire.setClock(100000);   // slow the I2C bus to 100 kHz
  Serial.println("1. setup start");

  display.init();
  Serial.println("2. display init done");

  // Show something immediately, before touching the RTC
  display.clear();
  display.setScale(1);
  display.setCaret(0, 0);
  display.print("OLED OK");
  display.update();
  Serial.println("3. OLED test text sent");

  scanI2C();   // expect 0x3C (OLED) and 0x68 (DS1307)

  //Senzor de detectie
  pinMode (IN_A0, INPUT);
  
  //Butoane
  pinMode(set_button_pin, INPUT);
  pinMode(panic_button_pin, INPUT);
  pinMode(reset_button_pin, INPUT);

  //Led-uri  
  pinMode(red_led_pin, OUTPUT);
  pinMode(green_led_pin, OUTPUT);

  digitalWrite(red_led_pin, LOW);
  digitalWrite(green_led_pin, LOW);
}

void loop() {
  bool ok = RTC.read(KT);
  Serial.print("RTC.read = ");
  Serial.print(ok ? "OK " : "FAIL ");
  if (ok) {
    Serial.print(KT.Hour); Serial.print(":");
    Serial.print(KT.Minute); Serial.print(":");
    Serial.println(KT.Second);
  } else {
    Serial.println(RTC.chipPresent() ? "(chip present but clock stopped)" : "(chip not found)");
  }

  display.clear();
  display.setScale(1);
  display.setCaret(0, 0);
  display.print(ok ? "RTC OK" : "RTC FAIL");
  if (ok) {
    display.setScale(2);
    display.setCaret(0, 15);
    display.print(KT.Hour);
    display.print(":");
    display.print(KT.Minute);
    display.print(":");
    display.print(KT.Second);
  }

    //Senzor de detectie
    value_A0 = analogRead(IN_A0); // reads the analog input from the IR distance sensor
    Serial.println(value_A0);


  display.update();
  delay(1000);
  
  bool set_button = digitalRead(set_button_pin);
  bool panic_button = digitalRead(panic_button_pin);
  bool reset_button = digitalRead(reset_button_pin);

  if(set_button){
    digitalWrite(red_led_pin, HIGH);
  }else if( panic_button){
    digitalWrite(green_led_pin, HIGH);
  }else if (reset_button){
    digitalWrite(red_led_pin, HIGH);
    digitalWrite(green_led_pin, HIGH);
  }else{
    digitalWrite(red_led_pin, LOW);
    digitalWrite(green_led_pin, LOW);
  }

}
