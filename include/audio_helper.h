#define I2S_DOUT 17 // Используем GPIO18 для Data Out
#define I2S_BCLK 18 // Используем GPIO17 для Bit Clock
#define I2S_LRC 16  // Используем GPIO3 для Left/Right Clock (Word Select)


void playWav(char *fname);
void initAudio();