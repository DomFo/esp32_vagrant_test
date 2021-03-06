
/* Bus configuration */

// This MACROS are defined in "skdconfig.h" and set through 'menuconfig'.
// Can use to check which protocol has been selected.

#include "SPIbus.hpp"
static SPI_t& spi                     = vspi;  // hspi or vspi
static constexpr int MOSI             = 23;
static constexpr int MISO             = 19;
static constexpr int SCLK             = 18;
static constexpr int CS               = 5;
static constexpr uint32_t CLOCK_SPEED_LOW = 1*1000*1000;  // 1MHz
static constexpr uint32_t CLOCK_SPEED_HIGH = 10*1000*1000;  // 10MHz

static constexpr int LOG_PIN          = 33;


void mpu_spi_pre_transfer_callback(spi_transaction_t *t)
{
    gpio_set_level((gpio_num_t)CS, 0);
}

void mpu_spi_post_transfer_callback(spi_transaction_t *t)
{
    gpio_set_level((gpio_num_t)CS, 1);
}


/* MPU configuration */

static constexpr int kInterruptPin         = 34;  // GPIO_NUM
static constexpr uint16_t kSampleRate      = 500;  // Hz
static constexpr mpud::accel_fs_t kAccelFS = mpud::ACCEL_FS_4G;
static constexpr mpud::gyro_fs_t kGyroFS   = mpud::GYRO_FS_500DPS;
static constexpr mpud::dlpf_t kDLPF        = mpud::DLPF_98HZ;
static constexpr mpud::int_config_t kInterruptConfig{
    .level = mpud::INT_LVL_ACTIVE_HIGH,
    .drive = mpud::INT_DRV_PUSHPULL,
    .mode  = mpud::INT_MODE_PULSE50US,
    .clear = mpud::INT_CLEAR_STATUS_REG  //
};

// FIFO
constexpr uint16_t kFIFOPacketSize = 12;  // in Byte
constexpr uint16_t kFIFOSize = 512;  // in Byte

/*-*/

static const char* TAG = "MPU9250";

/* SD card configuation */

static constexpr int PIN_NUM_SD_CMD            = 15;
static constexpr int PIN_NUM_SD_D0             = 2;
static constexpr int PIN_NUM_SD_D1             = 4;
static constexpr int PIN_NUM_SD_D2             = 12;
static constexpr int PIN_NUM_SD_D3             = 13;

#define MOUNT_POINT "/sdcard"


/* Tasks */

static void mpuISR(void*);
static void mpuTask(void*);
static void printTask(void*);

static void mount_sd_card(void);

static const char* get_filename();

TaskHandle_t mpu_task_handle = NULL; 
TaskHandle_t print_task_handle = NULL; 
xQueueHandle data_queue; 