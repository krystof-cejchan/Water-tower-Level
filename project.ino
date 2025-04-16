#include <LiquidCrystal_I2C.h>
#include <PacketSerial.h>

LiquidCrystal_I2C lcd(0x3F, 16, 2);
PacketSerial ps;

const int trigPin = 9;
const int echoPin = 10;
const int buzzerPin = 6;

const byte distLimitCM = 30;

void setup() {
  pinMode(buzzerPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  lcd.init();
  lcd.backlight();
  ps.begin(115200);
}

void loop() {
  ps.update();
  double dist = distance();
  lcd.clear();
  lcdPrint(0, 1, dist + " cm");

  if (dist > distLimitCM) {
    //hladina je příliš daleko
    tone(buzzerPin, 1000);
    lcdPrint(0, 2, "Hladina je nízko!");

  }
  else {
    //hladina je ok
    noTone(buzzerPin);
    lcdPrint(0, 2, "Hladina je OK");
  }

  sendPacket(distance);
  delay(500);
}

void sendPacket(double distance) {
  ps.send(reinterpret_cast<const uint8_t*>(&distance), sizeof(distance));
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
