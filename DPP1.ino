#include <math.h>

#define TMPR A0
const float T0 = 298.15;
const float R0 = 10000;
const float BETA = 3650;

void setup() {
  Serial.begin(115200);
  pinMode(TMPR, INPUT_ANALOG);
}

void loop() {
  Heat();
  delay(500);
}

void Heat() {
  int tv = analogRead(TMPR);
  float cels = 1.0 / (log(1.0 / (1023.0 / float(tv) - 1.0)) / BETA + 1.0 / T0) - 273.15;
  if (cels < 8.0) {
    Serial.println("Нагрев");
  }
}
