#include <SPI.h>
#include <MCP23S17.h>
#include <XInput.h>

#define CS_PIN 10          // Your Chip Select pin
#define CHIP_ADDRESS 0     // A0-A1-A2 tied to GND

MCP23S17 MCP(CS_PIN, CHIP_ADDRESS);  // Modern constructor (address explicit)

void setup() {
  // put your setup code here, to run once:
  XInput.setAutoSend(false);
  XInput.setJoystickRange(0,1023);
  XInput.begin();
  pinMode(8, INPUT_PULLUP);

  Serial.begin(115200);
  while (!Serial);  // Wait for Serial Monitor (useful on Pro Micro)
  // Resetting IO extender before use
  pinMode(9, OUTPUT);
  digitalWrite(9, LOW);
  delay(100);
  digitalWrite(9, HIGH);
  Serial.println("After reseting pin");
  Serial.println("MCP23S17 input test starting...");

  SPI.begin();  // Initialize hardware SPI
  SPI.setClockDivider(SPI_CLOCK_DIV64);

  // begin(true) = set all pins to INPUT + enable internal pull-ups
  bool initOK = MCP.begin(true);
  Serial.print("begin() success: ");
  Serial.println(initOK ? "YES" : "NO");

  if (!initOK) {
    Serial.println("MCP23S17 not responding! Check wiring/power/SPI.");
    while (1);  // Halt
  }
  

  Serial.print("Uses hardware SPI: ");
  Serial.println(MCP.usesHWSPI() ? "YES" : "NO");

  
 // MCP.setPullup16(0x2222);
  //uint16_t value = 0;
 // MCP.getPullup16(value);
 // Serial.print("Should be 0x2222 | is ");
  //Serial.println(value, HEX);
  //delay(3000);

  //MCP.setPullup16(0xFFFF);

  //Serial.println("\nInitial GPIO state (should be 0xFFFF if no buttons pressed):");

}

void loop() {
  // put your main code here, to run repeatedly:
  uint16_t binvalue = MCP.read16(); // Binary number for button inputs
  byte val = (bitRead(binvalue, 15) ^ 1);

  // Left Trigger - 0
  XInput.setTrigger(TRIGGER_LEFT, (bitRead(binvalue, 14) * -255 + 255));
  // Left Button - 15
  XInput.setButton(BUTTON_LB, (bitRead(binvalue, 15) ^ 1));
  // Back Button - 8
  XInput.setButton(BUTTON_BACK, (bitRead(binvalue, 8) ^ 1));
  // Up DPad - 3 Down DPad - 5 Left DPad - 4 Right DPad - 6
  //XInput.setDpad(!digitalRead(3), !digitalRead(5), !digitalRead(4), !digitalRead(6));
  // Right Trigger - 5
  XInput.setTrigger(TRIGGER_RIGHT, (bitRead(binvalue, 5) * -255 + 255));
  // Right Button - 6
  XInput.setButton(BUTTON_RB, (bitRead(binvalue, 6) ^ 1));
  // B Button - 1
  XInput.setButton(BUTTON_B, (bitRead(binvalue, 1) ^ 1));
  // A Button - 2
  XInput.setButton(BUTTON_A, (bitRead(binvalue, 2) ^ 1));
  // Y Button - 4
  XInput.setButton(BUTTON_Y, (bitRead(binvalue, 4) ^ 1));
  // Menu Button - 7
  XInput.setButton(BUTTON_START, (bitRead(binvalue, 7) ^ 1));
  // X Button - 3
  XInput.setButton(BUTTON_X, (bitRead(binvalue, 3) ^ 1));
  // LSB Button - 16
  XInput.setButton(BUTTON_L3, !digitalRead(8));
  // RSB Button - 0
  XInput.setButton(BUTTON_R3, (bitRead(binvalue, 0) ^ 1));

  // A2, A3 LEFT
  //int leftXAxis = (analogRead(A3)-512) * 64;
  //int leftYAxis = ((analogRead(A2)-511)*-1) * 64;
  XInput.setJoystickX(JOY_LEFT, analogRead(A3), false); 
  XInput.setJoystickY(JOY_LEFT, analogRead(A2), true);
  // A0, A1 RIGHT
  //int rightXAxis = (analogRead(A0)-512) * 64;
  //int rightYAxis = ((analogRead(A1)-511)*-1) * 64;
  XInput.setJoystickX(JOY_RIGHT, analogRead(A0), false); 
  XInput.setJoystickY(JOY_RIGHT, analogRead(A1), true);

  XInput.send();
}
