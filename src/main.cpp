#include <Arduino.h>
#include "view.h"
#include "networking.h"
#include "sideled.h"


void event_handler_checkbox(struct _lv_obj_t * obj, lv_event_t event);
void event_handler_button(struct _lv_obj_t * obj, lv_event_t event);
void init_gui_elements();
void mqtt_callback(char* topic, byte* payload, unsigned int length);

unsigned long next_lv_task = 0;

lv_obj_t * led;

lv_obj_t * red_checkbox;
lv_obj_t * blue_checkbox;

lv_obj_t * off_checkbox;
lv_obj_t * on_checkbox;
lv_obj_t * blink_checkbox;

lv_obj_t * left_button;
lv_obj_t * right_button;

void event_handler_checkbox(struct _lv_obj_t * obj, lv_event_t event) {
  if(event == LV_EVENT_VALUE_CHANGED ) {
    if(
      (obj == red_checkbox || obj == blue_checkbox) &&
      (lv_checkbox_is_checked(red_checkbox) || lv_checkbox_is_checked(blue_checkbox))
      ) {
      lv_checkbox_set_checked(blue_checkbox, obj == blue_checkbox ? lv_checkbox_is_checked(blue_checkbox) : false);
      lv_checkbox_set_checked(red_checkbox, obj == red_checkbox ? lv_checkbox_is_checked(red_checkbox) : false);
    }
    if(
      (obj == off_checkbox || obj == on_checkbox || obj == blink_checkbox) &&
      (lv_checkbox_is_checked(off_checkbox) || lv_checkbox_is_checked(on_checkbox) || lv_checkbox_is_checked(blink_checkbox))
      ) {
      lv_checkbox_set_checked(off_checkbox, obj == off_checkbox ? lv_checkbox_is_checked(off_checkbox) : false);
      lv_checkbox_set_checked(on_checkbox, obj == on_checkbox ? lv_checkbox_is_checked(on_checkbox) : false);
      lv_checkbox_set_checked(blink_checkbox, obj == blink_checkbox ? lv_checkbox_is_checked(blink_checkbox) : false);
    }
    bool is_ready = (
        ((
        (lv_checkbox_is_checked(red_checkbox) && !lv_checkbox_is_checked(blue_checkbox)) ||
        (!lv_checkbox_is_checked(red_checkbox) && lv_checkbox_is_checked(blue_checkbox))
      ) &&
      (
        (!lv_checkbox_is_checked(off_checkbox) && lv_checkbox_is_checked(on_checkbox) && !lv_checkbox_is_checked(blink_checkbox)) ||
        (!lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(on_checkbox) && lv_checkbox_is_checked(blink_checkbox))
      )) || (
        (lv_checkbox_is_checked(off_checkbox) && !lv_checkbox_is_checked(on_checkbox) && !lv_checkbox_is_checked(blink_checkbox))
      ));
    lv_obj_set_click(left_button, is_ready);
    lv_obj_set_click(right_button, is_ready);
    lv_obj_set_state(left_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
    lv_obj_set_state(right_button, is_ready ? LV_STATE_DEFAULT : LV_STATE_DISABLED);
  } 
}

void event_handler_button(struct _lv_obj_t * obj, lv_event_t event) {
  if(event == LV_EVENT_PRESSED) {
    uint8_t led_start = (obj == right_button ? 0 : 5);
    uint8_t led_end = (obj == right_button ? 5 : 10);
    CRGB color = lv_checkbox_is_checked(blue_checkbox) ? CRGB::Blue : CRGB::Red;
    uint8_t state = SIDELED_STATE_OFF;
    if(lv_checkbox_is_checked(on_checkbox)) state = SIDELED_STATE_ON;
    if(lv_checkbox_is_checked(blink_checkbox)) state = SIDELED_STATE_BLINK;
    set_sideled_color(led_start,led_end, color);
    set_sideled_state(led_start,led_end, state);
  }
}

void init_gui_elements() {
  add_label("1. Select Color", 10, 10);
  red_checkbox = add_checkbox("Red", 10, 40, event_handler_checkbox);
  blue_checkbox = add_checkbox("Blue", 120, 40, event_handler_checkbox);
  add_label("2. Select Mode", 10, 70);
  off_checkbox = add_checkbox("Off", 10, 100, event_handler_checkbox);
  on_checkbox = add_checkbox("On", 120, 100, event_handler_checkbox);
  blink_checkbox = add_checkbox("Blink", 200, 100, event_handler_checkbox);
  add_label("3. Apply Color to side:", 10, 140);
  left_button = add_button("Apply Left", event_handler_button, 0, 170, 150, 50);
  right_button = add_button("Apply Right", event_handler_button, 160, 170, 150, 50);

  lv_obj_set_click(left_button, false);
  lv_obj_set_click(right_button, false);
  lv_obj_set_state(left_button, LV_STATE_DISABLED);
  lv_obj_set_state(right_button, LV_STATE_DISABLED);
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

  if(String(topic) == "example") {
    if(payloadS == "on") {
      lv_led_on(led);
    }
    if(payloadS == "off") {
      lv_led_off(led);
    }
  }
}


// ----------------------------------------------------------------------------
// UI event handlers
// ----------------------------------------------------------------------------

String buffer = "";

void event_handler_num(struct _lv_obj_t * obj, lv_event_t event) {

}

lv_obj_t * mbox;


// ----------------------------------------------------------------------------
// MAIN LOOP
// ----------------------------------------------------------------------------

void loop() {
  if(next_lv_task < millis()) {
    lv_task_handler();
    next_lv_task = millis() + 5;
  }

  // Uncomment the following lines to enable MQTT
  // mqtt_loop();
}

// ----------------------------------------------------------------------------
// MAIN SETUP
// ----------------------------------------------------------------------------

void setup() {
  init_m5();
  init_display();
  Serial.begin(115200);
  // Uncomment the following lines to enable WiFi and MQTT
  //lv_obj_t * wifiConnectingBox = show_message_box_no_buttons("Connecting to WiFi...");
  //lv_task_handler();
  //delay(5);
  //setup_wifi();
  //mqtt_init(mqtt_callback);
  //close_message_box(wifiConnectingBox);
  init_gui_elements();
  init_sideled();
}