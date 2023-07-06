#include <Arduino.h>
#include "view.h"
#include "networking.h"
#include "sideled.h"
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>
#include "M5_ENV.h"



void event_handler_checkbox(struct _lv_obj_t * obj, lv_event_t event);
void event_handler_button(struct _lv_obj_t * obj, lv_event_t event);
void init_image_roller();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

unsigned long next_lv_task = 0;

float tmp = 0.0;
float hum = 0.0;
float pressure = 0.0;

String secondCoreTemp = "";
String secondCoreHum = "";

SHT3X sht30;
QMP6988 qmp6988;


void event_handler_button(struct _lv_obj_t * obj, lv_event_t event) {
  
}

lv_obj_t * label_temp;
lv_obj_t * label_hum;
lv_obj_t * label_station2temp;
lv_obj_t * label_station2hum;

void init_gui() {
  add_label("Temperatur:", 10, 10);
  label_temp = add_label("0 °C", 150, 10);
  add_label("Humidity:", 10, 50);
  label_hum = add_label("0 %", 150, 50);
  add_label("Station 2 Temp", 10, 90);
  label_station2temp = add_label("0 °C", 150, 90);
  add_label("Station 2 Hum", 10, 130);
  label_station2hum = add_label("0 %", 150, 130);
}

// ----------------------------------------------------------------------------
// MQTT callback
// ----------------------------------------------------------------------------

void mqtt_callback(char* topic, byte* payload, unsigned int length) {
  // Parse Payload into String
  char * buf = (char *)malloc((sizeof(char)*(length+1)));
  memcpy(buf, payload, length);
  buf[length] = '\0';
  String payloadS = String(buf);
  payloadS.trim();

  Serial.println(topic);
  Serial.println(payloadS);

  if(String(topic) == (String(m5stack_secondary) + "/temp").c_str()) {
    secondCoreTemp = payloadS;
  }
  if(String(topic) == (String(m5stack_secondary) + "/hum").c_str()) {
    secondCoreHum = payloadS;
  }
}


// ----------------------------------------------------------------------------
// UI event handlers
// ----------------------------------------------------------------------------

String buffer = "";
lv_obj_t * mbox;


// ----------------------------------------------------------------------------
// MAIN LOOP
// ----------------------------------------------------------------------------
unsigned long next_temp_send = 0;
unsigned long next_temp_show = 0;


void loop() {
  if(next_lv_task < millis()) {
    lv_task_handler();
    next_lv_task = millis() + 5;
  }
  if(next_temp_send < millis()) {
    if(tmp != 0) {
      char buf[32];
      sprintf(buf, "{\"temp\":%.2f, \"hum\":%.2f}", tmp, hum);
      mqtt_publish((String(m5stack_primary) + "/status").c_str(), buf);
      mqtt_publish((String(influxdb_topic) + "/status").c_str(), buf);
      snprintf (buf, 32, "%.2f", tmp);
      mqtt_publish((String(m5stack_primary) + "/temp").c_str(), buf);
      mqtt_publish((String(influxdb_topic) + "/temp").c_str(), buf);
      snprintf (buf, 32, "%.2f", hum);
      mqtt_publish((String(m5stack_primary) + "/hum").c_str(), buf);
      mqtt_publish((String(influxdb_topic) + "/hum").c_str(), buf);
    }
    next_temp_send = millis() + 5000;
  }
  if(next_temp_show < millis()) {
     pressure = qmp6988.calcPressure();
    if(sht30.get()==0){ //Obtain the data of shT30.  获取sht30的数据
      tmp = sht30.cTemp;  
      hum = sht30.humidity;
      lv_label_set_text(label_temp, (String(tmp, 2)+ " °C").c_str());
      lv_label_set_text(label_hum, (String(hum, 2)+ " %").c_str());
      lv_label_set_text(label_station2temp, ("Station 2 Temp: " + secondCoreTemp + " °C").c_str());
      lv_label_set_text(label_station2hum, ("Station 2 Hum: " + secondCoreHum + " %").c_str());
    }else{
      tmp=0,hum=0;
    }
    next_temp_show = millis() + 500;
  }
  // Uncomment the following lines to enable MQTT
  mqtt_loop();
} 

// ----------------------------------------------------------------------------
// MAIN SETUP
// ----------------------------------------------------------------------------

void init_networking() {
  lv_obj_t * wifiConnectingBox = show_message_box_no_buttons("Connecting to WiFi...");
  lv_task_handler();
  delay(5);
  setup_wifi();
  mqtt_init(mqtt_callback);
  close_message_box(wifiConnectingBox);
}

void init_sensors() {
  
  Wire.begin(); //Wire init, adding the I2C bus.  Wire初始化, 加入i2c总线
  qmp6988.init();
}

void setup() {
  init_m5();
  init_display();
  Serial.begin(115200);
  
  // Uncomment the following lines to enable WiFi and MQTT (and the mqtt_loop() line in the loop function)
  init_networking();

  init_gui();
  init_sideled();

  init_sensors();
}