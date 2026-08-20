# 📡🎯 Router Attack

**An ESP8266 micro-game where nearby Wi-Fi networks become the enemies.**

Router Attack turns the wireless networks around you into a physical game using a **Heltec WiFi Kit 8**, its integrated OLED display, a potentiometer, a push button, and a buzzer — all packed inside an **Altoids tin**.

The ESP8266 scans nearby access points and uses their **SSID, RSSI, Wi-Fi channel, and security type** to create game targets. The player studies the target position, estimates its coordinates, and fires.

> 🛡️ **Router Attack does not hack, disconnect, deauthenticate, modify, or compromise Wi-Fi networks.**
> It only reads information normally broadcast by nearby access points and uses that information as game data.

---

## ✨ Features

* 📡 Scans nearby Wi-Fi networks with the ESP8266
* 🎯 Turns real access points into game targets
* 📶 Uses RSSI as one target coordinate
* 📻 Uses Wi-Fi channel as the second coordinate
* 🔐 Uses network security type to modify target tolerance
* 🕹️ Potentiometer-based coordinate selection
* 🔘 Single-button game controls
* 🔊 Buzzer sound effects
* 📟 128×32 OLED graphics
* 💥 Shooting animation
* 🏆 Score and attempt tracking
* ⏱️ Running-time statistics
* 🔋 Portable battery-powered design
* 🥫 Fits inside an Altoids tin
* 🖨️ 3D-printable front panel

---

## 🧠 How it works

```text
       Nearby Wi-Fi Networks
                │
                ▼
       ┌─────────────────┐
       │   ESP8266 Scan  │
       │                 │
       │ SSID            │
       │ RSSI            │
       │ Channel         │
       │ Security type   │
       └────────┬────────┘
                │
                ▼
       ┌─────────────────┐
       │  Target screen  │
       │  128×32 OLED    │
       └────────┬────────┘
                │
          Player estimates
          RSSI + Channel
                │
                ▼
       ┌─────────────────┐
       │ Potentiometer   │
       │ + Push button   │
       └────────┬────────┘
                │
                ▼
       ┌─────────────────┐
       │ Distance check  │
       │ Hit or miss     │
       └─────────────────┘
```

The ESP8266 starts in station mode and scans the surrounding Wi-Fi environment using:

```cpp
WiFi.scanNetworks();
```

For every detected network, Router Attack obtains information including:

* SSID
* RSSI
* Wi-Fi channel
* encryption/security type

The player selects a target and is shown an approximate graphical representation of its position.

Then the player must estimate:

1. 📶 **RSSI**
2. 📻 **Wi-Fi channel**

The potentiometer selects each value and the button confirms it.

---

## 🎮 Gameplay

### 1. Scan

Router Attack scans nearby Wi-Fi access points.

### 2. Select a target

Each detected SSID appears on the OLED.

Press the button to choose one.

### 3. Study the target

The target is plotted using:

```text
X → RSSI
Y → Wi-Fi Channel
```

You get a short opportunity to study its approximate location.

### 4. Enter RSSI

Turn the potentiometer to select an estimated RSSI value.

The available range is approximately:

```text
-120 ... 0
```

Press the button to confirm.

### 5. Enter the channel

Turn the potentiometer again to select the Wi-Fi channel:

```text
1 ... 14
```

Press the button to fire.

### 6. Fire

Router Attack compares your coordinates with the real access-point coordinates.

The OLED displays a shooting animation followed by:

```text
destroyed
```

or:

```text
missed
```

### 7. Check your score

The game tracks:

* ⏱️ running time
* 🎯 attempts
* 🏆 successful hits

---

## 📐 Distance calculation

The game calculates the Euclidean distance between the real network coordinates and the coordinates selected by the player:

```cpp
z = sqrt(
    (currentRSSI - selectedRSSI) *
    (currentRSSI - selectedRSSI)
    +
    (currentChannel - selectedChannel) *
    (currentChannel - selectedChannel)
);
```

Conceptually:

```text
                 Target
                   ●
                  /|
                 / |
                /  |
               / z |
              /    |
             ●─────┘
           Fire
```

A target is considered hit when the calculated distance falls within its configured tolerance.

---

## 🔐 Security as a game mechanic

The network security type affects the size of the target.

Router Attack therefore converts the access point's security configuration into a **game difficulty parameter**.

Conceptually:

```text
Open network
████████████
Large tolerance
Easier target

Protected network
   ████
Small tolerance
Harder target
```

This does **not** attempt to break the network's encryption. The encryption information is only used to alter the game tolerance.

---

## 🧰 Hardware

| Component                  | Purpose                                    |
| -------------------------- | ------------------------------------------ |
| **Heltec WiFi Kit 8**      | ESP8266, Wi-Fi scanner and integrated OLED |
| **128×32 OLED**            | Integrated into the Heltec board           |
| **Potentiometer**          | Coordinate selection                       |
| **Push button**            | Confirm/select/fire                        |
| **Buzzer**                 | Sound effects                              |
| **Battery**                | Portable power                             |
| **Altoids tin**            | Enclosure                                  |
| **3D-printed front panel** | Holds the hardware inside the tin          |

The original build reused a battery from an old mobile phone.

---

## 🔌 Wiring

The OLED is already integrated into the Heltec WiFi Kit 8, so only a few external components are required.

### Push button

```text
D6 / GPIO 12 ─── Button ─── GND
```

### Buzzer

```text
D7 / GPIO 13 ─── Buzzer ─── GND
```

### Potentiometer

```text
             ┌── 3.3V
             │
Potentiometer├── A0
             │
             └── GND
```

The source defines the controls as:

```cpp
const int potPin = A0;
const int buttonPin = 12; // D6
const int buzzerPin = 13; // D7
```

---

## 📟 OLED

Router Attack uses the **U8g2** graphics library with the integrated SSD1306-compatible 128×32 display.

The display is configured in the source as:

```cpp
U8G2_SSD1306_128X32_UNIVISION_F_SW_I2C u8g2(
    U8G2_R0,
    5,
    4,
    16
);
```

U8g2:

**[github.com/olikraus/u8g2](https://github.com/olikraus/u8g2)**

---

## 📁 Repository structure

```text
routerattack/
├── README.md
└── routerattack.ino
```

### `routerattack.ino`

Contains the complete game:

* Wi-Fi scanning
* target selection
* OLED interface
* potentiometer input
* button handling
* buzzer effects
* coordinate conversion
* shooting animation
* hit calculation
* scoring
* game statistics

---

## 🚀 Installation

### 1. Clone the repository

```bash
git clone https://github.com/ronibandini/routerattack.git
cd routerattack
```

### 2. Install ESP8266 support

Configure the Arduino IDE with the **ESP8266 Arduino Core**:

**[github.com/esp8266/Arduino](https://github.com/esp8266/Arduino)**

### 3. Install U8g2

Install **U8g2** using the Arduino Library Manager or from:

**[github.com/olikraus/u8g2](https://github.com/olikraus/u8g2)**

### 4. Connect the hardware

Wire:

* potentiometer → `A0`
* button → `D6`
* buzzer → `D7`

### 5. Upload

Open:

```text
routerattack.ino
```

Select the appropriate ESP8266 board configuration and upload the sketch.

---

## 🥫 Altoids enclosure

One of the defining parts of Router Attack is its enclosure.

The complete electronics fit inside a standard Altoids tin using a custom 3D-printed front panel.

### Cults3D

**[Router Attack Altoids Tin Game — 3D model](https://cults3d.com/en/3d-model/game/router-attack-altoids-tin-game)**

### Thingiverse

The original enclosure was also published on Thingiverse:

**[Router Attack Heltec WiFi Kit 8 Altoids Case](https://www.thingiverse.com/thing:4460079)**

---

## 🎥 Demo

Watch Router Attack in action:

**[▶️ Router Attack demo on YouTube](https://youtu.be/M7V6PAmy-XY)**

---

## 📰 External references

Router Attack has been documented and referenced outside GitHub.

### Hackster.io

**[Battle Against Wireless Routers with this Altoids Tin Game](https://www.hackster.io/news/battle-against-wireless-routers-with-this-altoids-tin-game-9f74ff9d48ea)**

Hackster writer Cameron Coward featured Router Attack, describing how the ESP8266 converts nearby wireless networks into enemies using real Wi-Fi information while emphasizing that the game does not perform actual network hacking.

---

### Hackaday.io

**[Router Attack — Hackaday.io](https://hackaday.io/project/172448-router-attack)**

---

### Cults3D

**[Router Attack Altoids Tin Game](https://cults3d.com/en/3d-model/game/router-attack-altoids-tin-game)**

3D-printable enclosure page for the Altoids version of Router Attack, including links back to the project tutorial and demo video.

---

### Thingiverse

**[Router Attack Heltec WiFi Kit 8 Altoids Case](https://www.thingiverse.com/thing:4460079)**

Original 3D enclosure publication linked from the Router Attack repository and Hackaday project.

---

## 🔬 Ideas for extending the project

Some possible upgrades:

* 💾 EEPROM score storage
* 🏆 persistent high scores
* 📊 statistics by security type
* 📡 Wi-Fi spectrum visualization
* 📈 RSSI history
* 🎮 additional game modes
* ⏲️ timed challenges
* 🔊 improved sound effects
* 🖥️ larger OLED/TFT support
* 🔋 battery-level display
* 🏅 difficulty levels
* 📊 use the scanning code as the basis for a Wi-Fi visualization tool

---

## ⚠️ Networking note

Router Attack uses **passive Wi-Fi discovery information** exposed by the ESP8266 scanning API.

It does not:

* send deauthentication frames
* inject packets
* connect to the target access point
* guess passwords
* exploit routers
* interfere with network traffic

The word **Attack** is part of the game's theme.

---

## 🔗 You may also be interested in...

Other projects by **Roni Bandini** involving wireless systems, embedded hardware, unconventional interfaces, and RF experimentation.

### 🎧🚫 Reggaeton Be Gone

**Machine-learning audio classification combined with Bluetooth/RF experimentation.**

The system listens for reggaeton using an Edge ML model and triggers an experimental wireless routine when the genre is detected.

**[github.com/ronibandini/reggaetonBeGone](https://github.com/ronibandini/reggaetonBeGone)**

---

### 📻 Pocket Gone

**Portable ESP32-based Bluetooth diagnostic and experimental hardware platform.**

Another compact wireless hardware project centered around radio experimentation and physical controls.

**[github.com/ronibandini/tallerPocketGone](https://github.com/ronibandini/tallerPocketGone)**

---

### 📡 mmWave Alarm

**Human-presence detection through doors and walls using mmWave radar and an ESP32.**

Combines RF sensing with embedded hardware and Telegram notifications.

**[github.com/ronibandini/mmWaveAlarm](https://github.com/ronibandini/mmWaveAlarm)**

---

### ❤️ Heart & Respiration Monitor

**Contactless heart-rate and respiration monitoring using 60 GHz mmWave radar, ESP32-C6, and UNIHIKER.**

Another project exploring radio-frequency sensing and embedded visualization.

**[github.com/ronibandini/heartRespirationMonitor](https://github.com/ronibandini/heartRespirationMonitor)**

---

## 👤 Author

**Roni Bandini**

Maker, AI developer, electronic artist and writer.

* 🐙 GitHub: **[@ronibandini](https://github.com/ronibandini)**
* 📸 Instagram: **[@ronibandini](https://www.instagram.com/ronibandini/)**
* 🐦 X: **[@RoniBandini](https://x.com/RoniBandini)**
* 🛠️ Hackaday: **[hackaday.io/ronibandini](https://hackaday.io/ronibandini)**

Contributions, forks, modifications, and alternative builds are welcome.
