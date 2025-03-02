#include "uart.hpp"

void setupUART2() {
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE
    };

    // Configure UART parameters
    uart_param_config(UART_NUM, &uart_config);
    // Set UART2 pins (TX=17, RX=16)
    uart_set_pin(UART_NUM, ESP32_UART2_TX, ESP32_UART2_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // Install UART driver with buffer size
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, 10, NULL, 0);

    Serial.print("UART2 setup complete");
}

void readUART2(float *voltage, float *current) {
    uint8_t data[BUF_SIZE];
    int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, READ_UART_DELAY_MS / portTICK_PERIOD_MS);

    if (len > 0) {
        data[len] = '\0';  // Null-terminate the received data

        // Split the received string into voltage and current using space as delimiter
        char *token = strtok((char *)data, " ");
        if (token != NULL) {
            *voltage = atof(token);  // Update the voltage pointer value

            token = strtok(NULL, "\n");
            if (token != NULL) {
                *current = atof(token);  // Update the current pointer value

                // Log the parsed values with Serial.print
                Serial.print("Updated Voltage: ");
                Serial.print(*voltage);
                Serial.print(" V, Current: ");
                Serial.print(*current);
                Serial.println(" A");
            } else {
                Serial.println("Failed to parse current");
            }
        } else {
                Serial.println("Failed to parse voltage");
        }
    }
}