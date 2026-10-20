
const int buttonPin = 2;
const int RledPin = 3;
const int GledPin = 4;
const int BledPin = 5;

int buttonState = 0;
bool buttonPressed = false;

int moodScore = 3;
const int maxScore = 10;
const int minScore = 0;

unsigned long lastPetTime = 0;
const unsigned long decayInterval = 1000;

unsigned long rainbowStartTime = 0;
bool isRainbowMode = false;
unsigned long lastColorChange = 0;
int colorStage = 0;

int pressCount = 0;
bool blinkState = false;
unsigned long lastBlinkTime = 0;
const unsigned long blinkInterval = 300;

void setup() {
  pinMode(RledPin, OUTPUT);
  pinMode(GledPin, OUTPUT);
  pinMode(BledPin, OUTPUT);

  pinMode(buttonPin, INPUT);

  Serial.begin(9600);
  lastPetTime = millis();
}

void loop() {
  unsigned long currentMillis = millis();
  buttonState = digitalRead(buttonPin);

  if (buttonState == HIGH && !buttonPressed) {
    buttonPressed = true;
    lastPetTime = currentMillis;

    if (!isRainbowMode) {
      if (moodScore == 2 && pressCount == 0) {
        moodScore = 3;
      } else {
        pressCount++;

        if (pressCount >= 3) {
          pressCount = 0;
          moodScore++;

          if (moodScore >= maxScore) {
            moodScore = maxScore;
            isRainbowMode = true;
            rainbowStartTime = currentMillis;
            lastColorChange = currentMillis;
            blinkState = false;
          }
        }
      }
    }

    Serial.print("Pet! Mood Score: ");
    Serial.println(moodScore);

  } else if (buttonState == LOW && buttonPressed) {
    buttonPressed = false;
  }

  if (!isRainbowMode &&
      currentMillis - lastPetTime >= decayInterval) {

    lastPetTime = currentMillis;

    if (moodScore > minScore) {
      moodScore--;
      pressCount = 0;

      Serial.print("Ignored... Mood Score: ");
      Serial.println(moodScore);
    }
  }

  if (isRainbowMode) {
    if (currentMillis - rainbowStartTime >= 3000) {
      isRainbowMode = false;
      moodScore = 9;
      pressCount = 0;
      lastPetTime = currentMillis;
    } else {
      if (currentMillis - lastColorChange >= blinkInterval) {
        lastColorChange = currentMillis;
        blinkState = !blinkState;
      }

      if (blinkState) {
        digitalWrite(RledPin, HIGH);
        digitalWrite(GledPin, LOW);
        digitalWrite(BledPin, HIGH);
      } else {
        digitalWrite(RledPin, HIGH);
        digitalWrite(GledPin, HIGH);
        digitalWrite(BledPin, HIGH);
      }
    }
  } else {
    if (moodScore >= 9) {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, HIGH);
    } else if (moodScore >= 6) {
      digitalWrite(RledPin, HIGH);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, LOW);
    } else if (moodScore >= 3) {
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, LOW);
      digitalWrite(BledPin, HIGH);
    } else if (moodScore >= 1) {
      digitalWrite(RledPin, LOW);
      digitalWrite(GledPin, HIGH);
      digitalWrite(BledPin, HIGH);
    } else {
      if (currentMillis - lastBlinkTime >= blinkInterval) {
        lastBlinkTime = currentMillis;
        blinkState = !blinkState;
      }

      if (blinkState) {
        digitalWrite(RledPin, LOW);
        digitalWrite(GledPin, HIGH);
        digitalWrite(BledPin, HIGH);
      } else {
        digitalWrite(RledPin, HIGH);
        digitalWrite(GledPin, HIGH);
        digitalWrite(BledPin, HIGH);
      }
    }
  }
}
