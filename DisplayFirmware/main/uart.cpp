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

String cleanUARTData(const char* data) {
    String dataStr = String(data);  // Convert the char* data to a String
    int firstLineEnd = dataStr.indexOf('\n');  // Find the first newline character

    if (firstLineEnd != -1) {
        // Skip the first line by removing everything up to and including the first newline
        dataStr = dataStr.substring(firstLineEnd + 1);
    }

    // Now find the first voltage current pair
    int spacePos = dataStr.indexOf(' ');  // Find the space between the voltage and current

    if (spacePos != -1) {
        // Extract the voltage and current pair
        String voltage = dataStr.substring(0, spacePos);
        String current = dataStr.substring(spacePos + 1, dataStr.indexOf('\n')); // Get current until next newline
        return voltage + " " + current;  // Return the voltage and current as a string
    }

    return "";  // Return an empty string if no valid pair is found
}

void readUART2() {
    uint8_t data[BUF_SIZE];
    int len = uart_read_bytes(UART_NUM, data, BUF_SIZE - 1, READ_UART_DELAY_MS / portTICK_PERIOD_MS);

    //TODO: Handle case where first message is clean if data is sent at a slower rate
    if (len > 0) {
        data[len] = '\0'; // Null-terminate the received data
        LOG(LOG_LEVEL_TRACE, String("Received UART Data: ") + (char *)data + '\n');

        // Clean the UART data by extracting the first valid "voltage current" pair
        String cleanedData = cleanUARTData((char *)data);

        if (cleanedData != "") {
            // Split the cleaned data by space to get voltage and current
            char *token = strtok((char *)cleanedData.c_str(), " ");
            if (token != NULL) {
                voltage = atof(token);  // Convert the voltage to a float

                token = strtok(NULL, "\n");
                if (token != NULL) {
                    current = atof(token);  // Convert the current to a float

                    // Log the parsed values
                    LOG(LOG_LEVEL_TRACE, "Updated Voltage : " + String(voltage) + " V,");
                    LOG(LOG_LEVEL_TRACE, " Current: " + String(current) + " A\n");
                } else {
                    Serial.println("Failed to parse current");
                }
            }
        } else {
            Serial.println("No valid data found");
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