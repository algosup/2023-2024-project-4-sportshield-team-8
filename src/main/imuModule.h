class IMUModule {
private:
  LSM6DS3 imu;

public:
  IMUModule() : imu(I2C_MODE, 0x6A) {
    // Constructor code here
  }

  void setup() {
    // IMU setup code here
  }

  float readAcceleration() {
    // Code to read acceleration
  }

  float readGyro() {
    // Code to read gyroscope
  }
};
