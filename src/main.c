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
#define PIN_PUMP GPIO_NUM_35

/*
    Set bit: reg |= (1ULL << n); // ≈ bitSet(reg, n)

    Clear bit: reg &= ~(1ULL << n); // ≈ bitClear(reg, n)

    Toggle bit: reg ^= (1ULL << n); // ≈ bitToggle(reg, n)

    Test bit: (reg & (1ULL << n)) != 0 // ≈ bitRead(reg, n)

*/

//ADC
static adc_oneshot_unit_handle_t adc1_handle;
volatile int adc = 0 ;


void app_main(void) {

    bootInit();
    pumpTriggerInit();
    run_ADC();

    int adc_raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL, &adc_raw));
    int adc = adc_raw;

    

    
    
    while (1) {

    }
}

void interruptInit() {

}

void pumpTriggerInit() {
    
    gpio_config_t relayTrig = {
        .pin_bit_mask = (1ULL << PIN_PUMP),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = 0,
        .pull_down_en = 0,
        .intr_type = GPIO_INTR_DISABLE,

    };

}

void adc_oneShot_Init() {
    
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT_1,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
}

void adc_chan_init() {
    adc_oneshot_chan_cfg_t adc1 = {
        .atten = ADC_ATTEN_DB_12,
        .bitwidth = ADC_BITWIDTH_12,

    };
}

void bootInit() {
    ESP_LOGI(TAG, "boot");

}

void run_ADC() {
    
    int adc_raw = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL, &adc_raw));
    adc = adc_raw;

}

