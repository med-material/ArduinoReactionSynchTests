int inputButton = 2;
int userLED = 11;
int userVib = 3;
int testLED = 10;
int testVib = 5;

//  ************** STUDENTS CHANGE VARIABLES BELOW

// Choose what musical Ability the participant has
String musicalAbility = "Low"; // Low or High

// Choose if the test should be used with vibration or not
bool useVibration = false; // false or true

//  ********************************************

void setup() {
  Serial.begin(9600);
  pinMode(inputButton, INPUT);
  pinMode(userLED, OUTPUT);
  pinMode(userVib, OUTPUT);
  pinMode(testLED, OUTPUT);
  pinMode(testVib, OUTPUT);

  digitalWrite(userLED, LOW);
  digitalWrite(userVib, LOW);
  digitalWrite(testLED, LOW);
  digitalWrite(testVib, LOW);
}





// Variables NOT TO BE CHANGED
// - Used for the software/test to function properly -
int highIntensityValue = 255;
int lowIntensityValue;

int iterations = 20;
int blinkInterval = 500;
int onTime = 50;

bool testRunning = false;
float timeUntilLED;
bool buttonClicked = false;


void loop() {
  // Syncing Test

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
      lowIntensityValue = 255;
    } else {
      outputLeg = testLED;
      lowIntensityValue = 25;
    }
            
    // The actual test code
    for(int test = 0; test <= 1; test++) {
      for(int i = 0; i < iterations; i++) {
        // Test number
        id++;
        // Logs the time at which the iteration started
        float testStartStamp = millis();
        float timeUntilLED = testStartStamp + blinkInterval / 2;
        float intervalTime = millis() + blinkInterval;
        bool logRegistered = false;

        if(useVibration) {
          if(randomTest % 2 == 0) {
            onTime = 65;
          } else {
            onTime = 150;
          }
        }
    
        // Waits for the LED to light up and the user to press the button
        while(millis() < intervalTime) {
          // Makes the LED/Vibrator activate
          if(millis() >= timeUntilLED && millis() < timeUntilLED + onTime) {
            if(randomTest % 2 == 0) {
              analogWrite(outputLeg, lowIntensityValue);
            } else {
              analogWrite(outputLeg, highIntensityValue);
            }
          } else {
            analogWrite(outputLeg, 0);
          }

          // Writes the output in the console
          if(buttonClicked == false && digitalRead(inputButton) == HIGH) {
            buttonClicked = true;
            logRegistered = true;
            int deltaTime = millis() - timeUntilLED;
            Serial.print(id);
            if(randomTest % 2 == 0) {
              if(useVibration) {
                Serial.print("\tERM\tLow\t");
              } else {
                Serial.print("\tLED\tLow\t");
              }
            } else {
              if(useVibration) {
                Serial.print("\tERM\tHigh\t");
              } else {
                Serial.print("\tLED\tHigh\t");
              }
            }
            Serial.print(deltaTime);
            Serial.print("\t");
            Serial.println(musicalAbility);
          }

          if(buttonClicked == true && digitalRead(inputButton) == LOW) {
            buttonClicked = false;
          }

          // Makes the user LED/Vibrator activate when button is pressed
          if(useVibration) {
            digitalWrite(userVib, digitalRead(inputButton));
          } else {
            digitalWrite(userLED, digitalRead(inputButton));
          }

        }

        // Registers a null data point if the user did not press the button in time
        if(logRegistered == false) {
          Serial.print(id);
          if(randomTest % 2 == 0) {
            if(useVibration) {
              Serial.print("\tERM\tLow\t");
            } else {
              Serial.print("\tLED\tLow\t");
            }
          } else {
            if(useVibration) {
              Serial.print("\tERM\tHigh\t");
            } else {
              Serial.print("\tLED\tHigh\t");
            }
          }
          Serial.print("NA\t");
          Serial.println(musicalAbility);
        }
        
        // Resets the button and LED for next iteration of the test
        analogWrite(outputLeg, 0);
        logRegistered = false;
      }
      randomTest += 1;

      // Sets a delay before the next session begins
      delay(2000);
    }
    id = 0;
    EndOfTest();
  }

}


void CountdownToTest() {
    Serial.println("Test begins in:");
    for(int i = 3; i > 0; i--) {
      Serial.println(i);
      delay(1000);
    }
    Serial.println("----- LOG BEGIN SYNC TEST (sep=tab, col=5, label=synch)-----");
    Serial.println("TrialNo\tModal\tIntens\tReactionTime\tMusicalAbility");
}


void EndOfTest() {
    Serial.println("----- LOG END SYNC TEST -----");

    digitalWrite(userLED, LOW);
    digitalWrite(userVib, LOW);
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
