#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
const int trigPin = 9;
const int echoPin = 10;
const int buzzer = 6;

const byte distLimitCM = 30;

void setup() {
  pinMode(buzzer, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  Serial.begin(9600);
}

void loop() {
  double dist = distance();
  lcd.clear();
  lcdPrint(0, 1, dist + " cm");

  if (dist > distLimitCM) {
    //hladina je příliš daleko
    tone(buzzer, 1000);
    lcdPrint(0, 2, "Hladina je nízko!");

  }
  else {
    //hladina je ok
    noTone(buzzer);
    lcdPrint(0, 2, "Hladina je OK");
  }

  Serial.println(dist);
  delay(500);
}


double distance() {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  const double duration = pulseIn(echoPin, HIGH, 30000);

  return (duration * 0.0343) / 2;
}

void lcdPrint(int x, int y, String txt) {
  lcd.setCursor(x, y);
  lcd.print(txt);
}
