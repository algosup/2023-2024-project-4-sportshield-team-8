class GPSModule {
private:
  Adafruit_GPS GPS;

public:
  GPSModule() : GPS(&Serial1) {
    // Constructor code here
  }

  void setup() {
    // GPS setup code here
  }

  void update() {
    // Code to read and update GPS data
  }

  bool isPositionAcquired() {
    // Return whether position is acquired
  }

  void sleep() {
    // Put GPS into low power mode
  }

  void wake() {
    // Wake GPS from low power mode
  }
};
