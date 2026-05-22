#define MAX_LEVEL 100

const int ledPins[] = {8, 9, 10, 11};
const int buttonPins[] = {2, 3, 4, 5};
const int buzzerPin = 12;

int sequence[MAX_LEVEL];
int currentLevel = 0;
int step = 0;

// Állapotgép (FSM) állapotai
enum GameState { START, PLAY_SEQ, WAIT_INPUT, GAME_OVER };
GameState state = START;

void setup() {
  for (int i = 0; i < 4; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(buttonPins[i], INPUT); // Külső lehúzó ellenállást használunk
  }
  pinMode(buzzerPin, OUTPUT);
  randomSeed(analogRead(0)); // Véletlenszám-generátor inicializálása
}

void loop() {
  switch (state) {
    case START:
      currentLevel = 0;
      nextLevel();
      state = PLAY_SEQ;
      break;
      
    case PLAY_SEQ:
      delay(1000);
      playSequence();
      state = WAIT_INPUT;
      break;
      
    case WAIT_INPUT:
      if (checkPlayerInput()) {
        if (step == currentLevel) {
          delay(500);
          nextLevel();
          state = PLAY_SEQ;
        }
      } else {
        state = GAME_OVER;
      }
      break;
      
    case GAME_OVER:
      playGameOver();
      delay(2000);
      state = START;
      break;
  }
}

void nextLevel() {
  sequence[currentLevel] = random(0, 4);
  currentLevel++;
  step = 0;
}

void playSequence() {
  for (int i = 0; i < currentLevel; i++) {
    int pin = sequence[i];
    digitalWrite(ledPins[pin], HIGH);
    tone(buzzerPin, 200 + pin * 100, 300);
    delay(400);
    digitalWrite(ledPins[pin], LOW);
    delay(200);
  }
}

bool checkPlayerInput() {
  int expected = sequence[step];
  
  while (true) {
    for (int i = 0; i < 4; i++) {
      if (digitalRead(buttonPins[i]) == HIGH) {
        digitalWrite(ledPins[i], HIGH);
        tone(buzzerPin, 200 + i * 100, 200);
        
        while (digitalRead(buttonPins[i]) == HIGH); // Várakozás a gomb elengedésére
        
        digitalWrite(ledPins[i], LOW);
        delay(50);

        if (i == expected) {
          step++;
          return true;
        } else {
          return false; // Rossz gomb, játék vége
        }
      }
    }
  }
}

void playGameOver() {
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], HIGH);
  }
  tone(buzzerPin, 100, 1000); // Hibajelzés
  delay(1000);
  for (int i = 0; i < 4; i++) {
    digitalWrite(ledPins[i], LOW);
  }
}