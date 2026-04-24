# Bill of Materials — NKTg Ground Vehicle

## Component List

| # | Component | Qty | Est. Price (USD) | Notes |
|---|-----------|-----|-----------------|-------|
| 1 | ESP32 DevKit V1 | 1 | ~$6 | Central control brain — WiFi + USB Serial |
| 2 | IMU MPU6050 | 1 | ~$2 | 3-axis accelerometer + gyroscope |
| 3 | 600 PPR Encoder | 2 | ~$3 each | Mount on wheel axles |
| 4 | 5 kg Load Cell | 1 | ~$3 | Measures cargo mass |
| 5 | HX711 ADC module | 1 | ~$2 | 24-bit ADC for load cell |
| 6 | BTS7960 Motor Driver | 1 | ~$6 | 43 A peak current — suitable for autonomous vehicles |
| 7 | 12V DC Gear Motor + rubber wheel | 2 | ~$6 per set | Main drive system |
| 8 | 3S 11.1V 3000 mAh Li-ion battery | 1 | ~$10 | Powers the entire system |
| 9 | 2WD / 4WD aluminum or acrylic chassis | 1 | ~$6–$12 | Depending on type |
| 10 | Jumper wires, M3 screws, connectors | 1 set | ~$3 | |
| | **Total** | | **~$50–$60** | |

## Where to Buy

- **Shopee / Lazada** — search by exact component name
- **Amazon / AliExpress** — search `ESP32 DevKit V1`, `MPU6050`, `BTS7960`
- **Local electronics stores** — typically have ESP32, encoders, and motor drivers in stock

## Substitutions

| Original | Alternative | Trade-off |
|----------|------------|-----------|
| BTS7960 | L298N | Cheaper (~$3), but lower current limit (2 A vs 43 A) |
| 3S 3000 mAh battery | 2S 7.4 V or 12 V adapter | Lower motor voltage — reduce `NORMAL_SPEED` accordingly |
| 600 PPR encoder | 400 PPR encoder | Slightly lower velocity resolution |
| Aluminum chassis | Acrylic chassis | Lighter but less rigid |
