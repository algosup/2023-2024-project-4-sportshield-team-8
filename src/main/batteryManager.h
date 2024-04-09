class BatteryManager {
private:
  const int batteryPin;  // Analog pin used to read battery voltage
  const float referenceVoltage;  // Voltage reference for the ADC
  const float voltageDividerRatio;  // Total voltage divider ratio
  const float lowBatteryThreshold;  // Voltage threshold to indicate low battery

public:
  BatteryManager(int pin, float refVoltage, float dividerRatio, float lowVoltageThreshold)
    : batteryPin(pin), referenceVoltage(refVoltage), voltageDividerRatio(dividerRatio), lowBatteryThreshold(lowVoltageThreshold) {
    pinMode(batteryPin, INPUT);
  }

  float readVoltage() {
    int sensorValue = analogRead(batteryPin);
    float voltage = (sensorValue * referenceVoltage / 1023.0) * voltageDividerRatio;
    return voltage;
  }

  bool isBatteryLow() {
    float voltage = readVoltage();
    return voltage <= lowBatteryThreshold;
  }
};
