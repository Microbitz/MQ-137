#include <Wire.h>
#include "SSD1306Ascii.h"
#include "SSD1306AsciiAvrI2c.h"
#define I2C_ADDRESS 0x3C

SSD1306AsciiAvrI2c oled;

int ledr = 5; //LED pin
int ledb = 7; //LED pin
int ledg = 6; //LED pin
int relay = 3; //Buzzer pin
int gas_sensor = A0; //Sensor pin
float m = -0.263; //Slope
float b = 0.42; //Y-Intercept
float R0 = 2.19; //Sensor Resistance in fresh air from previous code

void setup() {
  Serial.begin(9600); //Baud rate
  Wire.begin();
  oled.begin(&Adafruit128x32, I2C_ADDRESS);
  oled.setFont(TimesNewRoman16_bold);
  oled.clear();
  oled.setCursor(0, 0 );
  oled.println("NH3 Sense MT");
  Serial.begin(9600);
  pinMode(ledr, OUTPUT); //Set LED as output
  pinMode(ledg, OUTPUT); //Set LED as output
  pinMode(ledb, OUTPUT); //Set LED as output
  digitalWrite(ledr, HIGH); //Turn LED off
  digitalWrite(ledg, LOW); //Turn LED off
  digitalWrite(ledb, HIGH); //Turn LED off
  pinMode(relay, OUTPUT); //Set buzzer as output
  digitalWrite(relay, LOW); // Turn buzzer off
  pinMode(gas_sensor, INPUT); //Set gas sensor as input
  oled.print("Calentando");
  delay(10000);
  digitalWrite(ledr, HIGH); //Turn LED off
  digitalWrite(ledg, HIGH); //Turn LED off
  digitalWrite(ledb, LOW); //Turn LED off
  oled.clear();
}

void loop() {
  oled.setCursor(0, 0 );
  float sensor_volt; //Define variable for sensor voltage
  float RS_gas; //Define variable for sensor resistance
  float ratio; //Define variable for ratio
  float sensorValue = analogRead(gas_sensor); //Read analog values of sensor
  sensor_volt = sensorValue * (5.0 / 1023.0); //Convert analog values to voltage
  RS_gas = ((5.0 * 10.0) / sensor_volt) - 10.0; //Get value of RS in a gas
  ratio = RS_gas / R0; // Get ratio RS_gas/RS_air

  double ppm_log = (log10(ratio) - b) / m; //Get ppm value in linear scale according to the the ratio value
  double ppm = pow(10, ppm_log); //Convert ppm value to log scale
  double percentage = ppm / 10000; //Convert to percentage
  int PotSP = analogRead(A1);
  int Zero = analogRead(A2);
  int Spam = analogRead(A3);
  double ppmsp;
  double zerotrim;
  zerotrim = map(Zero, 0, 1023, 0, 2);  
  ppmsp = map(PotSP, 0, 1023, 0, 5000);
  oled.print(ppm-zerotrim); //Load screen buffer with percentage value
  oled.println(" ppm             "); //Load screen buffer with "%"
  oled.println(" ppm             "); //Load screen buffer with "%"
  oled.print("SP. ");
  oled.print(ppmsp);
  oled.print(" ppm             ");
  delay(2000);
}
