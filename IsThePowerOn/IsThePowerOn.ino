#define LED_BUILTIN 4
#define POWER_ON_OPTOCOUPLER_MODULE  34

int optocouplerModulePast = -1;

// The setup function runs once when you press reset or power the board.
void setup() {
  Serial.begin(115200);
  
  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(LED_BUILTIN, OUTPUT);

  // Initialize digital pin LED_BUILTIN as an output.
  pinMode(POWER_ON_OPTOCOUPLER_MODULE , INPUT);
}

// The loop function runs over and over again forever.
void loop() {
  int optocouplerModuleCurrent = digitalRead(POWER_ON_OPTOCOUPLER_MODULE);
  if (optocouplerModulePast != optocouplerModuleCurrent) {
    if (optocouplerModuleCurrent == LOW) { // The optocoupler module signal is inverse. Low means power is on.
      // Power is on.
      Serial.println("The power is on.");
      digitalWrite(LED_BUILTIN, HIGH);   // Turn the LED on (HIGH is the voltage level).
    } else {
      // Power is off.
      Serial.println("The power is off.");
      digitalWrite(LED_BUILTIN, LOW);    // Turn the LED off by making the voltage LOW.
    }

    optocouplerModulePast = optocouplerModuleCurrent;
    delay(10); // Prevents debounce.
  }
}
