

#define GYRO_D_NONE 0
#define GYRO_D_RIGHT 1
#define GYRO_D_LEFT 2
#define GYRO_D_FORWARD 3
#define GYRO_D_BACKWARD 4
#define GYRO_D_UP 5
#define GYRO_D_DOWN 6
#define GYRO_D_ROTATE_RIGHT 7
#define GYRO_D_ROTATE_LEFT 8
#define GYRO_D_TILT_FORWARD 9
#define GYRO_D_TILT_BACKWARD 10
#define GYRO_D_TILT_UP 11
#define GYRO_D_TILT_DOWN 12

bool InitGyro();
void gyroAndAccelReadTask(void *params);
