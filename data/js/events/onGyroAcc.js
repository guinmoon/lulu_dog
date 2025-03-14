onGyroAcc = function (gyroX, gyroY, gyroZ, gyroDirection, accx, accy, accz,accDirection) {
    log_d("GYRO: " + gyroX + ", " + gyroY + ", " + gyroZ + ", " + gyroDirection + ", " + accx + ", " + accy + ", " + accz + ", " + accDirection);
    if (gyroDirection == 8 || gyroDirection == 10) {
        lulu_leftHand_long(4,6);    
    }
    if (gyroDirection == 8 || gyroDirection == 10) {
        lulu_rightHand_long(4,6);
    }
}