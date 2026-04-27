# NKTg Ground Vehicle
A real-time robot vehicle controlled entirely by NKTg (mass-kinetic energy function) — physical sensors feed actual x, v, m, dm/dt data into the NKTg engine running at 100 Hz on an ESP32, which issues motor commands in real time.

## What This Is
This is a hardware implementation of NKTg control theory — not a simulation, not a demo. The vehicle collects live sensor data, computes NKTg_1 and NKTg_2 on-chip, and drives motors accordingly — all without a host computer once flashed.
Physical sensors → NKTg Engine → Motor control → Web monitor
(100 Hz)         (100 Hz)       (100 Hz)       (20 Hz)

## Hardware

| Component | Est. Price |
|-----------|-----------|
| ESP32 DevKit V1 | ~$6 |
| IMU MPU6050 | ~$2 |
| 600 PPR Encoder x 2 | ~$3 each |
| 5 kg Load Cell + HX711 module | ~$5 |
| BTS7960 Motor Driver | ~$6 |
| 12V DC Gear Motor x 2 + rubber wheels | ~$6 per set |
| 3S 11.1V 3000 mAh Li-ion battery | ~$10 |
| 2WD / 4WD aluminum or acrylic chassis | ~$6-$12 |
| **Total** | **~$50-$60 USD** |

Full bill of materials: `hardware/BOM.md`

## Quick Start

### 1. Clone
```bash
git clone https://github.com/your-username/nktg-ground-vehicle.git
cd nktg-ground-vehicle
```

### 2. Flash firmware
- Open `firmware/NKTg_ESP32_Controller/NKTg_ESP32_Controller.ino` in Arduino IDE
- Install libraries: `MPU6050` by Electronic Cats, `HX711` by Bogdan Necula
- Board: ESP32 Dev Module — Upload speed 921600
- Upload, then open Serial Monitor at 115200 baud

### 3. Open the dashboard
- Open `web/NKTg_System.html` in Chrome or Edge
- Connect via Web Serial, select the ESP32 COM port
- Watch NKTg_1 and NKTg_2 plotted in real time

### 4. Assemble hardware
See: `build/build_guide.md`

## NKTg Control Logic

| Function | Formula | Physical meaning |
|----------|---------|-----------------|
| NKTg_1 | x · (m · v) | Position energy — detects trajectory instability |
| NKTg_2 | (dm/dt) · (m · v) | Mass-change energy — detects sudden load loss |

Default thresholds (tunable in `Config.h`):

| Condition | Action |
|-----------|--------|
| NKTg_1 > 500 | Emergency brake (PWM = 0) |
| NKTg_2 < -50 | Reduce speed 50% |
| Otherwise | Normal operation |

## Repository Layout
nktg-ground-vehicle/
├── README.md
├── build/build_guide.md              # Step-by-step assembly guide
├── firmware/NKTg_ESP32_Controller/   # ESP32 firmware
│   ├── NKTg_ESP32_Controller.ino
│   ├── NKTg_Engine.h                 # Core NKTg math
│   ├── Sensors.h                     # MPU6050 + HX711 + Encoder
│   ├── MotorControl.h                # BTS7960 driver
│   └── Config.h                      # All tunable constants
├── hardware/                         # BOM and wiring diagram
├── web/NKTg_System.html              # Real-time dashboard (Web Serial)
├── system/                           # Architecture and data flow docs
├── validation/real_test.md           # Real hardware test results
└── NEXT_APPLICATIONS.md

## Operating Layers

| Layer | Rate | Task |
|-------|------|------|
| 1 — Sensing | 100 Hz | Encoder + Load Cell + MPU6050 |
| 2 — NKTg compute | 100 Hz | computeNKTg() on-chip |
| 3 — Motor control | 100 Hz | PWM to BTS7960 |
| 4 — Web monitor | 20 Hz | Serial to NKTg_System.html |

## 🚀 Want to Go Deeper? Get the Book

This repository implements one application of NKTg. The full theory, 11 programming languages, ESA-validated data, and dozens more applications are in the book.

---

### PROGRAMMING COSMIC DYNAMICS
#### *A Practical Guide in 11 Popular Languages and Experimental Verification from ESA Data*

Physics isn't broken. It's just never been programmed.

For 300 years, inertia was treated as a constant. What if it isn't? What if motion can be computed — not just described?

Introducing the NKTg Law by Nguyen Khanh Tung:
NKTg = f(x, v, m)
NKTg_1 = x * p
NKTg_2 = (dm/dt) * p

A physical law encoded into executable code.

Not a theory. A system. A programmable physical law that executes, not approximates.

- ✅ Built across 11 programming languages
- ✅ Tested with real data from the European Space Agency
- ✅ Error margin: 0.208%

**What this unlocks:**
- AI that reacts like physical systems
- Autonomous vehicles with real-time physics engines
- Space navigation beyond classical models
- Simulation that doesn't approximate — it executes

From Python to Assembly. From AI to Quantum Computing. One law. One engine. Infinite applications.

---

### 📚 Where to Get It

#### I. Leanpub
| # | Edition | Link |
|---|---------|------|
| 1 | English Edition | https://leanpub.com/NKTgLaw |
| 2 | Vietnamese Edition | https://leanpub.com/NKTgLaw-vi |
| 3 | Chinese Simplified Edition | https://leanpub.com/NKTgLaw-zh |
| 4 | Global Bundle (English + Vietnamese) | https://leanpub.com/b/NKTgLaw-global |

#### II. Google Play Books
| # | Edition | Link |
|---|---------|------|
| 1 | English Edition | https://play.google.com/store/books/details?id=CNLKEQAAQBAJ&pli |
| 2 | Vietnamese Edition | https://play.google.com/store/books/details?id=buDMEQAAQBAJ |
| 3 | Chinese Simplified Edition | https://play.google.com/store/books/details?id=OMDTEQAAQBAJ |
| 4 | Global Bundle (English + Chinese Simplified + Vietnamese) | https://play.google.com/store/books/series?id=Fa6gHAAAABC0sM |

#### III. Amazon
| # | Edition | Link |
|---|---------|------|
| 1 | English Edition | https://www.amazon.com/dp/B0GTSBBRYR |
| 2 | Traditional Chinese Edition | https://www.amazon.com/dp/B0GHZMSKYB |
| 3 | Global Bundle (English + Traditional Chinese) | https://www.amazon.com/dp/B0D6579Q3G |

#### IV. Payhip
| # | Edition | Link |
|---|---------|------|
| 1 | English Edition | https://payhip.com/b/fU8Xr |
| 2 | Vietnamese Edition | https://payhip.com/b/aU9Ex |
| 3 | Chinese Simplified Edition | https://payhip.com/b/6UJbN |
| 4 | Global Bundle (English + Chinese Simplified + Vietnamese) | https://payhip.com/b/a5l9k |
| — | Storefront | https://payhip.com/NKTgLaw |

#### V. Gumroad
| # | Edition | Link |
|---|---------|------|
| 1 | English Edition | https://nktglaw.gumroad.com/l/NKTgLaw |
| 2 | Vietnamese Edition | https://nktglaw.gumroad.com/l/NKTgLaw-vi |
| 3 | Chinese Simplified Edition | https://nktglaw.gumroad.com/l/NKTgLaw-zh |
| 4 | Global Bundle (English + Chinese Simplified + Vietnamese) | https://nktglaw.gumroad.com/l/NKTgLaw-global |

---

> This is the shift. Not better formulas. Not faster simulations. A new way to build reality in code.

---

## 🌐 Explore the Physics Deeper

To explore the physical foundations behind NKTg Law in depth, you can visit:
👉 **[traiphieu.com](https://traiphieu.com)**

---

## 🔗 Distributed Repository Presence

Through this distributed presence, the NKTg Law operates not merely as software, but as a persistent, verifiable, and universally accessible system of computational physics — capable of existing, executing, and being validated anywhere in the world, independent of centralized control.

| Platform | Link |
|----------|------|
| GitHub | https://github.com/NKTgLaw/NKTgLaw |
| GitLab | https://gitlab.com/NKTgLaw/NKTgLaw |
| Gitea | https://gitea.com/NKTgLaw/NKTgLaw |
| Codeberg | https://codeberg.org/NKTgLaw/NKTgLaw |
| Launchpad | https://launchpad.net/nktglaw |
| SourceForge | https://sourceforge.net/projects/nktglaw/ |
| Bitbucket | https://bitbucket.org/nktglaw/nktglaw/src/main/ |
| SourceHut | https://git.sr.ht/~nktglaw/NKTgLaw |
| Rocketgit | https://rocketgit.com/NKTgLaw/NKTgLaw |
| Framagit | https://framagit.org/NKTgLaw/NKTgLaw |
| Disroot | https://git.disroot.org/NKTgLaw/NKTgLaw |

---

## License

Open source: `LICENSE`  
Commercial use: `LICENSE-commercial`  
For commercial licensing inquiries: traiphieu.com@gmail.com
