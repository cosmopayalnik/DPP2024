// Подключение LORA SX1278 к bluepill
// (pin SX1278 - pin STM32)
// ===========================
//   1, 2, 9, 16 (GND) - (GND)
//   3 (3.3V)  - (3.3V)
//   4 (RST)   - 18 (PB0)
//   12 (SCK)  - 15 (PA5)
//   13 (MISO) - 16 (PA6)
//   14 (MOSI) - 17 (PA7)
//   15 (NSS)  - 14 (PA4)
//   5 (DIO0)  - 11 (PA1)
// ===========================

#include <SPI.h>
#include <LoRa.h>

#define LORA_PORT 915E6

void setup() {
  Serial.begin(115200);
  while (!Serial);
  while (1) {
    if (!LoRa.begin(LORA_PORT)) {
      delay(1000);
    } else {
      break;
    }
  }
}

void loop() {
  int packetSize = LoRa.parsePacket();
  if (packetSize) {
    int key_flag = 0;
    int count = 0;
    int num_1 = 0;
    int num_2 = 0;
    while (LoRa.available()) {
      char symb = (char)LoRa.read();
      if (key_flag == 0 && symb == "k") {
        key_flag = 1;
        count = 1;
      } else if (count > 0 && count < 7 && int(symb) > 47 && int(symb) < 58) {
        if (count == 1) {
          num_1 = num_1 + (int(symb) - 48) * 100;
          count = count + 1;
        } else if (count == 2) {
          num_1 = num_1 + (int(symb) - 48) * 10;
          count = count + 1;
        } else if (count == 3) {
          num_1 = num_1 + (int(symb) - 48);
          count = count + 1;
        } else if (count == 4) {
          num_2 = num_2 + (int(symb) - 48) * 100;
          count = count + 1;
        } else if (count == 5) {
          num_2 = num_2 + (int(symb) - 48) * 10;
          count = count + 1;
        } else if (count == 6) {
          num_2 = num_2 + (int(symb) - 48);
          count = count + 1;
        }
      } else if (count > 0 && count < 7){
          key_flag = 0;
          count = 0;
          num_1 = 0;
          num_2 = 0;
      } else if (count == 7 and symb == "s") {
        Serial.println(num_1);
        Serial.println(num_2);
        key_flag = 2;
        count = 8;
      } else if (count == 7) {
          key_flag = 0;
          count = 0;
          num_1 = 0;
          num_2 = 0;
      }
    }
  }
}
