#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Preferences.h>

// --- Configuration & Pin Definitions ---
#define BUTTON_PIN 13
#define BUZZER_PIN 32
#define MAX_LEVEL 10
#define UNCONNECTED_ANALOG_PIN 34 

// LED Matrix Pins
const int ROW_PINS[4] = {4, 18, 19, 23};   // Cathodes (-)
const int COL_PINS[4] = {25, 26, 27, 14};  // Anodes (+)

LiquidCrystal_I2C lcd(0x27, 16, 2);

Preferences preferences;

// --- Game State Variables ---
int birdRow = 0;
int birdCol = 0;
int score = 0;
int totalShots = 0;
int level = 1;
int currentSpeed = 700; 
int highScore = 0;

// --- Musical Notes ---
enum Notes {
    NOTE_C4 = 262, NOTE_D4 = 294, NOTE_E4 = 330, NOTE_G4 = 392,
    NOTE_A4 = 440, NOTE_B4 = 494, NOTE_C5 = 523, NOTE_D5 = 587,
    NOTE_E5 = 659, NOTE_G5 = 784, REST = 0
};

// --- Hardware Control Functions ---
void clearAll() {
    for (int i = 0; i < 4; i++) {
        digitalWrite(ROW_PINS[i], HIGH);
        digitalWrite(COL_PINS[i], LOW);
    }
}

void lightOne(int r, int c) {
    clearAll();
    digitalWrite(ROW_PINS[r], LOW);
    digitalWrite(COL_PINS[c], HIGH);
}

// --- Sound Effects ---
void playPointTune() {
    tone(BUZZER_PIN, NOTE_E4, 75); delay(75);
    noTone(BUZZER_PIN);
    tone(BUZZER_PIN, NOTE_G4, 150); delay(150);
    noTone(BUZZER_PIN);
}

void playLevelUpTune() {
    int melody[] = { NOTE_E5, NOTE_D5, NOTE_C5, NOTE_D5, NOTE_E5, NOTE_E5, NOTE_E5 };
    int durations[] = { 4, 4, 4, 4, 4, 4, 2 };

    for (int i = 0; i < 7; i++) {
        int duration = 1000 / durations[i];
        tone(BUZZER_PIN, melody[i], duration);
        delay(duration * 1.3);
        noTone(BUZZER_PIN);
    }
}

// --- UI & Game Logic ---
void updateLCD() {
    if (score > highScore) {
        highScore = score;
        preferences.putInt("highScore", highScore);
    }

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Hit:"); lcd.print(score);
    lcd.print(" Tot:"); lcd.print(totalShots);
    
    lcd.setCursor(0, 1);
    lcd.print("Lv:"); lcd.print(level);
    lcd.print(" HS:"); lcd.print(highScore);
}

void resetGame() {
    score = 0;
    totalShots = 0;
    level = 1;
    currentSpeed = 700;
    
    randomSeed(analogRead(UNCONNECTED_ANALOG_PIN));
    birdRow = random(0, 4);
    birdCol = random(0, 4);
    
    updateLCD();
}

void flashBirdPosition(int r, int c) {
    for (int i = 0; i < 3; i++) {
        lightOne(r, c);
        delay(200);
        clearAll();
        delay(200);
    }
}

void winEffect() {
    for (int i = 0; i < 3; i++) {
        for (int r = 0; r < 4; r++) digitalWrite(ROW_PINS[r], LOW);
        for (int c = 0; c < 4; c++) digitalWrite(COL_PINS[c], HIGH);
        delay(100);
        clearAll();
        delay(100);
    }
}

// --- Main Program ---
void setup() {
    for (int i = 0; i < 4; i++) {
        pinMode(COL_PINS[i], OUTPUT);
        pinMode(ROW_PINS[i], OUTPUT);
    }
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(BUZZER_PIN, OUTPUT);

    lcd.init();
    lcd.backlight();

    preferences.begin("game_data", false);

    if (digitalRead(BUTTON_PIN) == LOW) {
        preferences.remove("highScore");
        highScore = 0;
        lcd.print("Storage Cleared");
        while (digitalRead(BUTTON_PIN) == LOW);
        delay(1000);
    }

    highScore = preferences.getInt("highScore", 0);
    resetGame();
}

void loop() {
    bool birdHit = false;

    flashBirdPosition(birdRow, birdCol);
    delay(500);

    while (!birdHit) {
        int randRow = random(0, 4);
        int randCol = random(0, 4);

        lightOne(randRow, randCol);

        unsigned long startTime = millis();
        while (millis() - startTime < (unsigned long)currentSpeed) {
            if (digitalRead(BUTTON_PIN) == LOW) {
                totalShots++;

                if (randRow == birdRow && randCol == birdCol) {
                    birdHit = true;
                    score++;
                    playPointTune();
                    winEffect();

                    if (currentSpeed > 150) currentSpeed -= 20; 
                    if (score % 10 == 0) playLevelUpTune();
                    
                    level++;
                    updateLCD();

                    birdRow = random(0, 4);
                    birdCol = random(0, 4);
                } else {
                    // Feedback for a missed shot
                    tone(BUZZER_PIN, NOTE_A4, 50);
                    updateLCD();
                    delay(100); 
                }
                
                while (digitalRead(BUTTON_PIN) == LOW);
            }
        }
        clearAll();
        delay(50); 
    }
}