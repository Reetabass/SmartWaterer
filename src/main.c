#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"

#define LED_PIN GPIO_NUM_18

void app_main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    vTaskDelay(pdMS_TO_TICKS(1000));
    printf("\n=== APPLICATION STARTING ===\n");
    printf("Booted successfully!\n");
    fflush(stdout);
    
    gpio_reset_pin(LED_PIN);
    gpio_set_direction(LED_PIN, GPIO_MODE_OUTPUT);
    printf("GPIO PIN %d initialized as output!\n", LED_PIN);
    printf("Starting LED blink loop...\n");
    fflush(stdout);

    
    while (1) {
        printf("LED OFF\n");
        gpio_set_level(LED_PIN, 0);
        vTaskDelay(pdMS_TO_TICKS(500));
        printf("LED ON\n");
        gpio_set_level(LED_PIN, 1);
        vTaskDelay(pdMS_TO_TICKS(500));
    }
}
