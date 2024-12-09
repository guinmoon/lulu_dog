

#define GYRO_D_NONE 0
#define GYRO_D_RIGHT 1
#define GYRO_D_LEFT 2
#define GYRO_D_FORWARD 3
#define GYRO_D_BACKWARD 3
#define GYRO_D_UP 3
#define GYRO_D_DOWN 3

bool gyroInit();
void gyroAndAccelReadTask(void *params);
