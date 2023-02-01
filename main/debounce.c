/*
 * debounce.c 
 *
 * Software-based pushbutton/switch debounce for ESP32. GPIO pin can be configured in menuconfig (default is GPIO1). 
 * Assumes that the pushbutton is connected to ground and enables the internal pullup resistor to prevent the input from floating.
 * 
 * Author: Michael Naish
 *
 */

#include <stdio.h>
#include "sdkconfig.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "debounce";

#define GPIO_BUTTON           CONFIG_GPIO_INPUT_0                            // select GPIO pin for button
#define GPIO_BUTTON_PIN_SEL   (1ULL << GPIO_BUTTON)                          // set bit for button GPIO
#define DEBOUNCE_INTERVAL     100                                            // set debounce interval in milliseconds
                                                                            
static TickType_t next = 0;                                                  // tick count for next allowable button press
static uint32_t number_presses = 0;                                          // counter for number of button presses
static bool pressed = false;                                                 // state variable to indicate "valid" button press
                                                                            
// Interrupt service routine                                                
static void IRAM_ATTR button_handler(void *arg)                             
{                                                                           
   TickType_t now = xTaskGetTickCountFromISR();                              // capture current ticks
   if (now > next) {                                                         // check whether enough time has passed to consider a valid press
      number_presses++;                                                      // increment button press counter
      pressed = true;                                                        // indicate valid button press state
      next = now + DEBOUNCE_INTERVAL / portTICK_PERIOD_MS;                   // set tick count for next allowable button press
   }                                                                        
}                                                                           
                                                                            
// Initialize button GPIO and ISR                                           
static void button_init(void)                                               
{                                                                           
   gpio_config_t io_conf;                                                    // configuration structure
                                                                            
   io_conf.pin_bit_mask = GPIO_BUTTON_PIN_SEL;                               // select pin for pushbutton
   io_conf.mode = GPIO_MODE_INPUT;                                           // configure as input
   io_conf.pull_up_en = GPIO_PULLUP_ENABLE;                                  // enable internal pullup resistor
   io_conf.pull_down_en = GPIO_PULLDOWN_DISABLE;                             // disable internal pulldown resistor
   io_conf.intr_type = GPIO_INTR_NEGEDGE;                                    // trigger interrupt on negative edge 
   ESP_ERROR_CHECK(gpio_config(&io_conf));                                   // configure pushbutton pin
   
   ESP_ERROR_CHECK(gpio_install_isr_service(ESP_INTR_FLAG_LEVEL1));          // allocate interrupt with lowest priority
   ESP_ERROR_CHECK(gpio_isr_handler_add(GPIO_BUTTON, button_handler, NULL)); // setup ISR for button 
}

void app_main(void)
{
   ESP_LOGI(TAG, "Initialize button");     
   button_init();
   
   while (1) {
      vTaskDelay(pdMS_TO_TICKS(100));                                        // slow down since nothing else is happening in loop
      if (pressed) {  
         ESP_LOGI(TAG, "Button has been pressed %lu times", number_presses); // output count with each button press
         pressed = false;
      }
   }
}
