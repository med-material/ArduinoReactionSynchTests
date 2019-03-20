int inputButton = 2;
int testLED = 10;
int testVib = 3;

int deviceID;

//  ************** STUDENTS CHANGE VARIABLES BELOW

// Choose if the test should be used with vibration or not
bool useVibration = true;

//  ********************************************

void setup() {
  Serial.begin(9600);
  pinMode(inputButton, INPUT);
  pinMode(testLED, OUTPUT);
  pinMode(testVib, OUTPUT);
}

// Variables NOT TO BE CHANGED
// - Used for the software/test to function properly -
int highIntensityValue = 255;
int lowIntensityValue;

int minWaitTime = 1000;
int maxWaitTime = 4000;

int iterations = 5;

bool testRunning = false;
float timeUntilLED;
bool buttonClicked = false;


void loop() {
  // Reaction Time Test

  if(testRunning == false) {
    // Check for input to begin the test
    if(digitalRead(inputButton) == HIGH) {
      testRunning = true;
    }
  } else {
    // Countdown before test begins
    CountdownToTest();

    // Determines the order of the test and sets the needed values for the proper test
    int randomTest = random(0,2);
    int outputLeg = 0;
    int id = 0;
    
    if(useVibration) {
      outputLeg = testVib;
      lowIntensityValue = 150;
    } else {
      outputLeg = testLED;
      lowIntensityValue = 25;
    }
            
    // The actual test code
    for(int test = 0; test <= 1; test++) {
      for(int i = 0; i < iterations; i++) {
        // Test Number
        id++;
        // Logs the time at which the iteration started
        float testStartStamp = millis();
        float timeUntilLED = testStartStamp + random(minWaitTime, maxWaitTime);
    
        // Waits for the LED to light up and the user to press the button
        while(buttonClicked == false || millis() < timeUntilLED) {
          if(millis() >= timeUntilLED) {
            if(randomTest % 2 == 0) {
              analogWrite(outputLeg, lowIntensityValue);
            } else {
              analogWrite(outputLeg, highIntensityValue);
            }
          }
    
          if(buttonClicked == false && digitalRead(inputButton) == HIGH) {
            buttonClicked = true;
            int deltaTime = millis() - timeUntilLED;
            Serial.print(id);
            if(randomTest % 2 == 0) {
              if(useVibration) {
                Serial.print("\tERM\tLow\t");
              } else {
                Serial.print("\tLED\tLow\t"); // As long as LEDs are not on pwd ports
              }
            } else {
              if(useVibration) {
                Serial.print("\tERM\tHigh\t");
              } else {
                Serial.print("\tLED\tHigh\t");
              }
            }
            Serial.println(deltaTime);
          }
        }
    
        // Resets the button and LED for next iteration of the test
        analogWrite(outputLeg, 0);
        buttonClicked = false;

        // Waits for the user to release the button, before continuing
        WaitForButtonRelease();
        delay(200);
      }
      randomTest += 1;
    }
    id = 0;
    EndOfTest();
  }

}


void CountdownToTest() {
    Serial.println("Test begins in:");
    for(int i = 3; i > 0; i--) {
      Serial.println(i);
      analogWrite(testVib, highIntensityValue);
      analogWrite(testLED, highIntensityValue);
      delay(750);
      analogWrite(testVib, 0);
      analogWrite(testLED, 0);
      delay(250);
    }
    Serial.println("----- LOG BEGIN REACTION TIME -----");
    Serial.println("TrialNo\tModal\tIntens\tReactionTime");
}

void EndOfTest() {
    Serial.println("----- LOG END REACTION TIME -----");

    digitalWrite(testLED, LOW);
    digitalWrite(testVib, LOW);

    WaitForButtonRelease();
    testRunning = false;
    delay(100);
}

void WaitForButtonRelease() {
  while(digitalRead(inputButton) == HIGH) {
  }
}

