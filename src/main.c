#include "mgos.h"
#include "mgos_gpio.h"
#include "mgos_wifi.h"
#include "mgos_uart.h"
#include "mgos_app.h"
#include "mgos_mqtt.h"
#include "mgos_system.h"
#include "mgos_timers.h"

int led_pin=2;
int pin_mode=1;
int time_on=3000;
int time_off=3000;
const char *msg="mqtt_test_msg";
const char *mqtt_topic="esp01s/test";
const char *ssid="ssid";
const char *pass="pass";

#define UART_NO 0

static void timer_cb(void *arg) {
  mgos_uart_printf(UART_NO,"mqtt_topic: %s\r\n", mqtt_topic);
  (void) arg;
}

enum mgos_app_init_result mgos_app_init(void) {
  struct mgos_uart_config ucfg;
  struct mgos_config_wifi_sta wcfg = {
    .enable=true,
    .ssid=ssid,
    .pass=pass,
  };

  mgos_gpio_set_mode(led_pin, pin_mode);

  mgos_uart_config_set_defaults(UART_NO, &ucfg);

  ucfg.baud_rate=115200;
  ucfg.rx_buf_size=1500;
  ucfg.tx_buf_size=1500;

  if(!mgos_wifi_setup_sta(&wcfg)) {
    LOG(LL_INFO, ("WIFI setup failed!________________"));
    return MGOS_APP_INIT_ERROR;
  } else {
    LOG(LL_INFO, ("WIFI setup successful!____________________"));
  }

  mgos_set_timer(2000, true, timer_cb, NULL);
  mgos_gpio_blink(led_pin, time_on, time_off);
  //mgos_mqtt_pub(mqtt_topic, msg, strlen(msg), 0, true);

  return MGOS_APP_INIT_SUCCESS;
}
