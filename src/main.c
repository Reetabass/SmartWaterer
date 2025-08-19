#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_rom_sys.h"
#include "esp_log.h"

#define TAG "smartwater"

#define PIN_MOISTURE_ADC GPIO_NUM_9
#define ADC_CHANNEL ADC1_CHANNEL_1
#define PIN_EXTERNAL_INTER GPIO_NUM_8

//ADC
static adc_oneshot_unit_handle_t adc1_handle;


void app_main(void) {

    bootInit();
    
    
    while (1) {

    }
}

void interruptInit() {

}

void adcInit() {

}

void bootInit() {
    ESP_LOGI(TAG, "boot");

}

void run_ADC() {
    adc_oneshot_unit_handle_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));


}