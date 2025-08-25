# Strain Gauge Reader 1.0 - BSP Layer

## EEPROM

### Memory map

| Address range | Data |
| ------------- | ---- |
| 0x00 - 0x27 | Wheatstone Bridge conf. |
| 0x28 | CAN bus freq. |
| 0x29 - 0x2C | CAN conf. message ID |
| 0x2D - 0x30 | CAN first frame ID |
| 0x31 - 0x34 | CAN message freqs. |
| 0x35 - 0x5C | Digital filters conf. |
| 0x5E - 0x7A | Strain gauge conf. |
