# Range Finder Based on Laser-m01-esp32
Integration of a high-precision laser distance measurement module with **ESP32** development boards.

Fork of the repository by [Andres-ros](https://github.com/Andres-ros/laser-m01-esp32).

## Overview
This repository provides a minimal and functional example for reading distances via the **UART protocol** using:

- **Laser Range Module M01 (6-pin board)**
- **Any ESP32-based development board**

Serial Monitor functions:  
- **Q** → Measure  
- **L** → Turn laser ON  
- **K** → Turn laser OFF  
- **R** → Reset the module  

---

## Safety

* The laser operates in the **infrared spectrum (≈905–940 nm)**.  
* Do not point it at eyes or reflective surfaces at close range.

---

## Required Materials
- **Laser Ranging Module M01 – 50 m** (Liancheng Electronics – AliExpress)  
- **Any ESP32 development board**  
- **3.3 V power supply** (recommended minimum: 150 mA)  
- **Dupont wires or soldering tools**

---

## Module Pinout (top to bottom)

1. **MIN (3V3)** – 3.3 V power  
2. **ENA** – Enable (active high)  
3. —  
4. **GND** – Ground  
5. **RXD** – UART input of the module  
6. **TXD** – UART output of the module  
7. **NC** – Not connected  
8. —

### Wire colors (according to the manufacturer)
- **Red** → MIN (3V3)  
- **Black** → GND  
- **Green** → RXD (module)  
- **Yellow** → TXD (module)

**Important:**  
- Module TX → ESP32 RX  
- Module RX ← ESP32 TX  

(Cross connection required)

---

## Recommended Connection (ESP32)
| Function | M01 Module | ESP32 |
|---------|-------------|--------|
| Power | Red (MIN) | 3V3 |
| Ground | Black (GND) | GND |
| TXD (module) | Yellow | GPIO33 (RX) |
| RXD (module) | Green | GPIO32 (TX) |
| ENA | ENA | GPIO5 HIGH or directly to 3V3 |

If your cable has only 4 wires (red/black/green/yellow), **ENA is often internally tied high**.  
If so, it does not need to be connected.

---

## Usage
1. Upload the sketch to the ESP32 and open the **Serial Monitor at 115200 baud**.  
2. Aim the module at a matte surface (2–4 m), preferably with a dark background.  
3. Use the following keys in the terminal:
   - **L** → Turns the laser ON  
   - **Q** → Quick measurement (returns `X.XXX`)  
   - **K** → Turns the laser OFF  
   - **R** → Resets the module (ENA cycle)

**Note:**  
The emitter is **infrared**, so the dot is not visible to the naked eye.  
However, it is usually visible through a smartphone camera.

---

## Quick Diagnostics (if no data is received on RX)
- Check wire crossing:  
  - Yellow (module TXD) → ESP32 RX  
  - Green (module RXD) ← ESP32 TX  
- Ensure **both devices share the same GND**.  
- Verify that **ENA is set high** (GPIO5 HIGH or 3V3).  
- **Quick echo test:**  
  - Short green ↔ yellow at the module connector.  
  - Anything transmitted by the ESP32 should be received back identically.  
  - If no echo → inspect solder joints and continuity between the pins and the connector.

- Some Laser M01 units are configured for **115200 baud**.  
  If necessary, change:

```cpp
LZR.begin(9600, …)
````

to:

```cpp
LZR.begin(115200, …)
```

---

## Acknowledgments

To the author [Andres-ros](https://github.com/Andres-ros/laser-m01-esp32).

Special thanks to **Liancheng Electronics (Shenzhen) Co., Ltd. Store** for providing the cable color scheme and assisting with RX/TX wiring issues.

**Purchase link:**
[https://www.aliexpress.com/store/1104805174?spm=a2g0o.order_list.order_list_main.8.21ef194dfoSHNu](https://www.aliexpress.com/store/1104805174?spm=a2g0o.order_list.order_list_main.8.21ef194dfoSHNu)
