#include <Joystick.h>

// Set the number of buttons and axes for the joystick
const int numButtons = 8;
const int numAxes = 2;

// Create the Joystick object
Joystick_ joystick(numButtons, 0, numAxes);

void setup() {
  // Initialize the Joystick library
  joystick.begin();

  // Set pins for mode switch and shifter axes
  int modePin = A0;
  int xAxisPin = A1;
  int yAxisPin = A2;

  // Set the mode switch pin as input with internal pullup resistor enabled
  pinMode(modePin, INPUT_PULLUP);

  // Set the shifter axis pins as input
  pinMode(xAxisPin, INPUT);
  pinMode(yAxisPin, INPUT);
}

void loop() {
  // Read the mode switch value
  int modeValue = analogRead(A0);

  // Determine the shifter mode based on the mode switch value
  int shifterMode = (modeValue < 512) ? 0 : 1;

  // Read the shifter axis values
  int xAxisValue = analogRead(A1);
  int yAxisValue = analogRead(A2);

  // Determine the button states based on the shifter mode and axis values
  int buttonStates[numButtons];

  if (shifterMode == 0) { // H-pattern mode
    // Determine the button state for each gear
    for (int i = 0; i < 8; i++) {
      if ((xAxisValue >= gearMin[i]) && (xAxisValue <= gearMax[i]) && (yAxisValue >= -512) && (yAxisValue <= -256)) {
        buttonStates[i] = 1; // Button pressed
      } else {
        buttonStates[i] = 0; // Button released
      }
    }
  } else { // Sequential mode
    // Determine the button state for shifting up and down
    if ((yAxisValue >= 512) && (yAxisValue <= 768)) {
      buttonStates[0] = 1; // Shift up button pressed
      buttonStates[1] = 0; // Shift down button released
    } else if ((yAxisValue >= -768) && (yAxisValue <= -512)) {
      buttonStates[0] = 0; // Shift up button released
      buttonStates[1] = 1; // Shift down button pressed
    } else {
      buttonStates[0] = 0; // Both buttons released
      buttonStates[1] = 0;
    }
  }

  // Update the Joystick button states
  for (int i = 0; i < numButtons; i++) {
    joystick.setButton(i, buttonStates[i]);
  }

  // Update the Joystick axis values
  joystick.setXAxis(xAxisValue);
  joystick.setYAxis(yAxisValue);

  // Send the Joystick data to the computer
  joystick.sendState();

  // Delay to prevent sending data too fast
  delay(20);
}

// Define the minimum and maximum axis values for each gear in H-pattern mode
const int gearMin[] = { 0, 196, 392, 588, 784, -196, -392, -588 };
const int gearMax[] = { 195, 391, 587, 783, 1023, -1, -197, -393 };
