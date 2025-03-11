#include "uart.hpp"


void uart_event_task(void *pvParameters)
{
    uart_event_t event;

    LOG(LOG_LEVEL_DEBUG,"In UART EVENT TASK");
    while (1)
    {

        if (xQueueReceive(uartQueue, (void *)&event, (TickType_t)portMAX_DELAY))
        {
            LOG(LOG_LEVEL_DEBUG,"Recieved a UART EVENT");
            switch (event.type)
            {
            case UART_DATA:
                LOG(LOG_LEVEL_DEBUG,"UART DATA Event");
                readUART2();
                xTaskNotify(displayTaskHandle, (1 << 1), eSetBits);
                // xTaskNotifyGive(displayTaskHandle);
                break;
            default:
                LOG(LOG_LEVEL_DEBUG,"NOT UART DATA Event");
                break;
            }
        }
    }
}

void setupUART2()
{
    uart_config_t uart_config = {
        .baud_rate = 9600,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE};

    // Configure UART parameters
    uart_param_config(UART_NUM, &uart_config);
    // Set UART2 pins (TX=17, RX=16)
    uart_set_pin(UART_NUM, ESP32_UART2_TX, ESP32_UART2_RX, UART_PIN_NO_CHANGE, UART_PIN_NO_CHANGE);
    // Install UART driver with buffer size
    uart_driver_install(UART_NUM, BUF_SIZE * 2, BUF_SIZE * 2, QUEUE_LENGTH, &uartQueue, 0);

    Serial.print("UART2 setup complete");
}

void readUART2()
{
    uint8_t data[BUF_SIZE];
    int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, READ_UART_DELAY_MS / portTICK_PERIOD_MS);

    if (len > 0)
    {
        data[len] = '\0'; // Null-terminate the received data

        // Split the received string into voltage and current using space as delimiter
        char *token = strtok((char *)data, " ");
        if (token != NULL)
        {
            voltage = atof(token); // Update the voltage pointer value

            token = strtok(NULL, "\n");
            if (token != NULL)
            {
                current = atof(token); // Update the current pointer value

                // Log the parsed values with Serial.print
                Serial.print("Updated Voltage: ");
                Serial.print(voltage);
                Serial.print(" V, Current: ");
                Serial.print(current);
                Serial.println(" A");
            }
            else
            {
                Serial.println("Failed to parse current");
            }
        }
        else
        {
            Serial.println("Failed to parse voltage");
        }
    }
}

void sendResistanceLevelUART2(uint8_t numRepeats)
{
    ScreenManager &screenManager = ScreenManager::getInstance();
    uint8_t resistanceLevel = screenManager.getResistanceLevel();

    for (int i = 0; i < numRepeats; ++i)
    {
        char asciiDigit = '0' + resistanceLevel;
        //Send raw byte
        LOG(LOG_LEVEL_TRACE,"SENT LEVEL TO ARDUINO");
        uart_write_bytes(UART_NUM, &asciiDigit, 1);
        uart_wait_tx_done(UART_NUM, pdMS_TO_TICKS(2));// Wait for transmission to complete
    }
}