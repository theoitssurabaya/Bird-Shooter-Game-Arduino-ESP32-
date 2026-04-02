# Bird Shooter Game (ESP32)

A reaction-based game built with Arduino/ESP32 where players must time their shots to hit a "bird" moving across a 4x4 LED matrix. The game features persistent high scores, escalating difficulty, and synchronized sound effects.

## 🛠️ Hardware Requirements
* **Microcontroller:** ESP32
* **Display:** LCD 16x2 with I2C Module (0x27)
* **Matrix:** 4x4 LED Matrix
* **Input:** 1x Push Button
* **Sound:** 1x Passive Buzzer
* **Power:** USB or External Power Supply

## 📌 Pin Mapping

| Component | Pin (ESP32) | Role |
| :--- | :--- | :--- |
| **Button** | GPIO 13 | Game Trigger / Reset |
| **Buzzer** | GPIO 32 | SFX & Melodies |
| **LCD SDA** | GPIO 21 | Data |
| **LCD SCL** | GPIO 22 | Clock |
| **Matrix Rows** | 4, 18, 19, 23 | Cathodes (-) |
| **Matrix Cols** | 25, 26, 27, 14 | Anodes (+) |

---

## 🚀 Key Features
* **Persistent Scoring:** High scores are saved to the ESP32's flash memory using the `Preferences.h` library.
* **Dynamic Difficulty:** The LED "speed" increases (delay decreases) as your score climbs.
* **Level System:** Tracks progress through 10 levels with a special "Level Up" melody every 10 points.
* **Visual/Audio Feedback:** * Point Tune & Win Effect (Matrix Flash) when hit.
    * Miss Tune when timing is incorrect.
    * LCD updates in real-time showing Hits, Total Shots, Level, and High Score.
* **Factory Reset:** Hold the button during startup to wipe the stored High Score.

---

## 🎮 How to Play
1. **Memorize:** At the start of a round, the "Bird's" position will flash 3 times.
2. **Observe:** The matrix will start flashing LEDs at random positions.
3. **Shoot:** Press the button **only** when the LED lights up at the memorized position.
4. **Advance:** Each hit increases the speed. Try to beat the High Score!

---

## 📂 Setup Instructions
1. Open the code in the **Arduino IDE**.
2. Install the following libraries:
   * `LiquidCrystal_I2C`
   * `Wire`
   * `Preferences` (built-in for ESP32)
3. Select your board (e.g., **DOIT ESP32 DEVKIT V1**).
4. Connect the hardware according to the pin mapping table.
5. Upload and enjoy!

---

## 🖼️ Product Overview
![Bird Shooter Game Prototype](https://via.placeholder.com/800x400.png?text=Place+Your+Project+Photo+Here)
*The hardware setup featuring the ESP32, 16x2 LCD display, and the 4x4 LED Matrix interface.*

---

## 📜 License
This project was created for educational purposes. Feel free to use and modify.
