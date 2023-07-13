# Core2weatherStation

The goal of this project is to fetch data with MQTT from two M5Stack Core2 that are equiped with a ENV3 Sensor and send the data to an InfluxDB to then display the measured data on a grafana dashboard.

## Diagramm

![Screenshot 2023-06-29_15-17-12-313](https://github.com/Laureatus/Core2weatherStation/assets/47870802/2bb90c41-65e9-4def-bfb6-d0f4055818a2)

## Installation
- Clone repository
- Open repository with PlatformIO
- Adjust settings in src/config.cpp
  - clientId
  - m5stack_primary
  - m5stack_secondary
  - second_station_label
- Adjust serial port in platform.ini
- Build and upload


## License
<a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/"><img alt="Creative Commons License" style="border-width:0" src="https://i.creativecommons.org/l/by-nc-sa/4.0/88x31.png" /></a><br />This work is licensed under a <a rel="license" href="http://creativecommons.org/licenses/by-nc-sa/4.0/">Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License</a> and [GNU GENERAL PUBLIC LICENSE version 3](https://www.gnu.org/licenses/gpl-3.0.en.html). If there are any contradictions between the two licenses, the Attribution-NonCommercial-ShareAlike 4.0 International license governs. 
