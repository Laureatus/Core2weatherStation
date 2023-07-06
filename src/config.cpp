#ifndef CONFIG_H_
#define CONFIG_H_

// mqtt configuration (username, password not used)
const char *  mqtt_server = "cloud.tbz.ch";
const char *  clientId = "oliverholst";
const char *  username = "";
const char *  password = "";

// WiFi configuration
const char * ssid = "LERNKUBE";
const char * passphrase = "l3rnk4b3";

// MQTT topics
const char * m5stack_primary = "WeatherStation_1";
const char * m5stack_secondary = "WeatherStation_2";
const char * influxdb_topic = "WeatherStation_Main";

// Station Name
const char * second_station_label = "Station 2";

#endif // BASE_H