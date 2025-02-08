
#define WIRE Wire

#define IMG_WIDTH 280
#define IMG_HEIGHT 240

#define DISPLAY_WIDTH 280

#define SLEEP_AFTER 15

#define DEEP_SLEEP_AFTER 120

// GYRO CONFIG

#define IMPACT_THRESHHOLD_ACC 0.6 // Пороговое значение изменений ускорения (низкое из-за минимальных изменений)
#define IMPACT_THRESHHOLD_GYR 1.8 // Пороговое значение изменений угловой скорости
#define LAST_IMPACT_MIN_PERIOD 2000

#define GYRO_ACTION_PERIOD 3000 // Время в миллисекундах


// BATTERY CONFIG

#define VREF  3.3    // Power supply voltage of ESP32-S3 (unit: volts)
#define BATTERY_R1  200000.0 // Resistance value of the first resistor (unit: ohms)
#define BATTERY_R2  100000.0
#define BATTERY_CHARGING_THR 0.1
#define BATTERY_CHARGING_V 3.95
#define BATTERY_CHARGING_V_MIN 3.9



// PIN CONFIG

#define XPOWERS_CHIP_AXP2101

#define LCD_DC 4
#define LCD_CS 5
#define LCD_SCK 6
#define LCD_MOSI 7
#define LCD_RST 8
#define LCD_BL 15
#define LCD_WIDTH 280
#define LCD_HEIGHT 240

#define IIC_SDA 11
#define IIC_SCL 10

#define TP_RST 13
#define TP_INT 14

// #define BUZZER_PIN 33
#define BUZZER_PIN 42
#define SYS_EN_PIN 41
#define SYS_OUT_PIN 40

// #define I2S_DOUT 17 
// #define I2S_BCLK 18 
// #define I2S_LRC 16  

#define I2S_DOUT 18 
#define I2S_BCLK 3 
#define I2S_LRC 17  

#define VOLTAGE_DIVEDER_PIN 1


// TOUCH CONFIG
#define LONGPRESSTHRESHOLD 800
#define DOUBLETAPTHRESHOLD 800