
// DHT22 Sensor Task
#define DHT22_TASK_STACK_SIZE 4096
#define DHT22_TASK_PRIORITY 4
#define DHT22_TASK_CORE_ID 1

// #define DHT_GPIO			25
#define DHT_GPIO GPIO_NUM_3 // GPIO pin connected to the DHT22

#define DHT_OK 0
#define DHT_CHECKSUM_ERROR -1
#define DHT_TIMEOUT_ERROR -2

/**
 * Starts DHT22 sensor task
 */
// void DHT22_task_start(void);

#ifdef __cplusplus
extern "C"
{
#endif

        void DHT22_task_start(void);

#ifdef __cplusplus
}
#endif

// == function prototypes =======================================

void setDHTgpio(int gpio);
void errorHandler(int response);
int readDHT();
float getHumidity();
float getTemperature();
int getSignalLevel(int usTimeOut, bool state);
