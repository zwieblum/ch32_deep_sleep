//----------------------------------------------------------------------------
// CH32 Deep Sleep Example - Dr. Nikolaus Klepp - office@klepp.biz
//----------------------------------------------------------------------------
// The autowakeup delay can be calculated by:
//
// 		T = BASE_MS * NUM
//
// where NUM is [1...63]. T max = 30.24 secs
//----------------------------------------------------------------------------

#include <CH32_Deep_Sleep.h>

// Adjust to your needs
#define LED PD0             

void setup() {
  pinMode(LED, OUTPUT);
  digitalWrite(LED,LOW);      // LED on
  Serial.println("Chance to program ...");
  // IMPORTANT: if you miss that, you'll have a hard time reprogramming the CV32V003
  delay(5000);                // NEVER REMOVE THIS DELAY
  digitalWrite(LED,HIGH);     // LED off

  DeepSleep.init();
  Serial.println("entering sleep loop\r\n");
}


// the loop function runs over and over again forever
void loop() {
  DeepSleep.sleep();          // Enter sleep
  Serial.println(millis());   // send live sign
  digitalWrite(LED, LOW);     // turn the LED on
  delay(1000);
  digitalWrite(LED, HIGH);    // turn the LED off
}
