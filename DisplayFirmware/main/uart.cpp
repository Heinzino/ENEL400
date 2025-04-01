#include "uart.hpp"

void uart_event_task(void *pvParameters)
{
    uart_event_t event;

    LOG(LOG_LEVEL_TRACE, "In UART EVENT TASK");
    while (1)
    {

        if (xQueueReceive(uartQueue, (void *)&event, (TickType_t)portMAX_DELAY))
        {
            LOG(LOG_LEVEL_TRACE, "Recieved a UART EVENT");
            switch (event.type)
            {
            case UART_DATA:
                LOG(LOG_LEVEL_TRACE, "UART DATA Event");
                readUART2();
                xTaskNotify(displayTaskHandle, (1 << 1), eSetBits);
                break;
            default:
                LOG(LOG_LEVEL_TRACE, "NOT UART DATA Event");
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
        LOG(LOG_LEVEL_TRACE, String("Received UART Data: ") + (char *)data + '\n');

        float voltage = 0.0f, current = 0.0f;
        int rpm = 0;
        int headerIconFlag = 0;
        int highTemperatureFlag = 0;

        int parsedValues = sscanf((char *)data, "%f %f %d %d %d", &voltage, &current, &rpm, &headerIconFlag, &highTemperatureFlag);

        if (parsedValues == 5)
        {
            SensorData &sensorData = SensorData::getInstance();
            sensorData.setVoltage(voltage);
            sensorData.setCurrent(current);
            sensorData.setRPM(rpm);

            SystemStatus &status = SystemStatus::getInstance();
            status.setHeaderIconFlag(headerIconFlag);
            status.setHighTemperatureFlag(highTemperatureFlag);

            LOG(LOG_LEVEL_TRACE, "Updated Voltage: " + String(voltage) + " V\n");
            LOG(LOG_LEVEL_TRACE, "Updated Current: " + String(current) + " A\n");
            LOG(LOG_LEVEL_TRACE, "Updated RPM: " + String(rpm) + " rpm\n");
            LOG(LOG_LEVEL_TRACE, "Updated Header Icon Flag: " + String(headerIconFlag) + '\n');
            LOG(LOG_LEVEL_TRACE, "Updated High Temperature Flag: " + String(highTemperatureFlag) + '\n');

            sensorData.recordDataPoint();
        }
        else
        {
            LOG(LOG_LEVEL_TRACE, "Failed to parse UART data");
        }
    }
}

void sendResistanceLevelUART2(uint8_t numRepeats, char resistanceLevel)
{
    for (int i = 0; i < numRepeats; ++i)
    {
        LOG(LOG_LEVEL_TRACE, "SENT LEVEL TO ARDUINO" + String(resistanceLevel));
        uart_write_bytes(UART_NUM, &resistanceLevel, 1);
        uart_wait_tx_done(UART_NUM, pdMS_TO_TICKS(2)); // Wait for transmission to complete
    }
}