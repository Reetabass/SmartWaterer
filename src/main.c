#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/semphr.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "driver/adc.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_rom_sys.h"
#include "esp_log.h"
#include "esp_adc/adc_cali.h"
#include "esp_adc/adc_cali_scheme.h"
#include "esp_mac.h"
#include "hal/adc_types.h"

#define TAG "smartwater"

#define PIN_MOISTURE_ADC GPIO_NUM_9 // PIN A2 on pinout
#define ADC_CHANNEL ADC1_CHANNEL_8
#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_12


#define PIN_EXTERNAL_INTER GPIO_NUM_8 // A3 on pinout
#define PIN_PUMP GPIO_NUM_6 // SCL on pinout not using i2c

/*
    Set bit: reg |= (1ULL << n); // ≈ bitSet(reg, n)

    Clear bit: reg &= ~(1ULL << n); // ≈ bitClear(reg, n)

    Toggle bit: reg ^= (1ULL << n); // ≈ bitToggle(reg, n)

    Test bit: (reg & (1ULL << n)) != 0 // ≈ bitRead(reg, n)

*/

//function defintions

static void adc_task(void *args);


//ADC
static adc_oneshot_unit_handle_t adc1_handle;
volatile int adc = 0 ;



//ADC calibration
static adc_cali_handle_t adc_cali_handle = NULL;
static bool cali_enabled = false;


void app_main(void) {

    bootInit();

    adc_oneShot_Init();

    BaseType_t ok = XTackCreate(
        adc_task,
        "adc_task",
        3072,
        NULL,
        5,
        NULL
    );

    if (ok != pdPASS) {
        ESP_LOGE(TAG, "Failed to create Task");
    }

}

static void adc_task(void *args) {
    (void)args;
    for(;;) {
        run_ADC_calibrated();
        vTaskDelay(pdMS_TO_TICKS(500));
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

//https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/adc_oneshot.html
void adc_oneShot_Init() {
    
    adc_oneshot_unit_init_cfg_t init_config1 = {
        .unit_id = ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config1, &adc1_handle));
    adc_chan_init();
    adc_calibration_init(ADC_UNIT, ADC_ATTEN);
}

void adc_chan_init() {

    adc_oneshot_chan_cfg_t init_config1 = {
        .atten = ADC_ATTEN,
        .bitwidth = ADC_BITWIDTH_12,

    };
    ESP_ERROR_CHECK(adc_oneshot_config_channel(&adc1_handle, ADC_CHANNEL, &init_config1));

}

void bootInit() {
    ESP_LOGI(TAG, "boot");

}

void run_ADC_calibrated() {
    
    int adc_raw = 0;
    int voltage_mv = 0;

    ESP_ERROR_CHECK(adc_oneshot_read(adc1_handle, ADC_CHANNEL, &adc_raw));

    if (cali_enabled) {
        esp_err_t ret = adc_cali_raw_to_voltage(adc_cali_handle, adc_raw, &voltage_mv);

        if (ret == ESP_OK) {
            ESP_LOGI(TAG, "Moisture reading %d mV", voltage_mv);
        }
        else {
            ESP_LOGW(TAG, "Calibration conversion failed: %s", esp_err_to_name(ret));
        }

    } 
    else {
        ESP_LOGW(TAG, "Calibration not enabled, no voltage available");
    }

}


//https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/peripherals/adc_calibration.html#_CPPv4N21adc_cali_scheme_ver_t32ADC_CALI_SCHEME_VER_LINE_FITTINGE

static bool adc_calibration_init(adc_unit_t unit, adc_atten_t atten) {
    
    esp_err_t ret;
    adc_cali_scheme_ver_t scheme_mask;

    //checks supported calibration schemes

    ret = adc_cali_check_scheme(&scheme_mask);

    /*  Returns:
            ESP_OK: On success
            ESP_ERR_INVALID_ARG: Invalid argument
            ESP_ERR_NOT_SUPPORTED: No supported calibration scheme */
    //ret should return esp ok if there are calibration schemes supported

    if (ret != ESP_OK) {
        //logs return value of ret and prints error to logs
        ESP_LOGW(TAG, "Calibration check failed %s", esp_err_to_name(ret));
        return false;
    }

    // so there are two types of esp calibrations
    
    // Checks and initalizes curve fitting calibration
    if (scheme_mask &  ADC_CALI_SCHEME_VER_CURVE_FITTING) {
        
        ESP_LOGI(TAG, "calibration scheme version is %s", "Curve Fitting");
        
        adc_cali_curve_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ret = adc_cali_create_scheme_curve_fitting(&cali_config, &adc_cali_handle);

        if (ret == ESP_OK) {
            cali_enabled = true;
            ESP_LOGI(TAG, "ADC calibration : curve fitting");
            return true;
        }
        ESP_LOGW(TAG, "curve fitting created failed", esp_err_to_name(ret));
    }

    /*Line fitting is not supported on esp32s3 so commenting out to remove error*/
    
    /*else if (scheme_mask & ADC_CALI_SCHEME_VER_LINE_FITTING) {
        
        ESP_LOGI(TAG, "calibration scheme version is %s", "Line Fitting");
        adc_cali_line_fitting_config_t cali_config = {
            .unit_id = unit,
            .atten = atten,
            .bitwidth = ADC_BITWIDTH_DEFAULT,
        };
        ESP_ERROR_CHECK(adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle));
        ret = adc_cali_create_scheme_line_fitting(&cali_config, &adc_cali_handle);

        if (ret == ESP_OK) {
            cali_enabled = true;
            ESPLOGI(TAG, "ADC calibration : line fitting");
            return true;
        }
        ESPLOGW(TAG, "line fitting created failed", esp_err_to_name(ret));

    } */


    ESP_LOGW(TAG, "No supported ADC calibration Scheme");
    return false;


}