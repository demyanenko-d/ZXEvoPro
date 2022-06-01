#include <Arduino.h>
#include "app_hal/io.hpp"
#include "app_hal/i2c.hpp"


void setup() {
  Serial.begin(115200);
  Serial.println("\nReady");

  //io_init();
  //i2c_init();

}

void loop() {
  // put your main code here, to run repeatedly:
  delay(1000);
  Serial.print("1");
}