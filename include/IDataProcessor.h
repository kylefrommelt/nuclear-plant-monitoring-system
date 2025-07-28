#pragma once

#include <vector>
#include <string>

namespace Nuclear {

struct SensorReading {
    int sensorId;
    double value;
    std::string timestamp;
    std::string sensorType;
};

struct ProcessedData {
    std::vector<SensorReading> readings;
    bool alertTriggered;
    std::string alertMessage;
    double averageTemperature;
    double averagePressure;
    double averageRadiation;
};

/**
 * @brief Interface for processing nuclear plant sensor data
 * 
 * Follows Single Responsibility Principle - only handles data processing
 */
class IDataProcessor {
public:
    virtual ~IDataProcessor() = default;
    
    /**
     * @brief Process raw sensor readings and generate alerts if needed
     * @param readings Vector of raw sensor readings
     * @return Processed data with calculated averages and alerts
     */
    virtual ProcessedData ProcessReadings(const std::vector<SensorReading>& readings) = 0;
    
    /**
     * @brief Set safety thresholds for alert generation
     * @param maxTemperature Maximum safe temperature in Celsius
     * @param maxPressure Maximum safe pressure in PSI
     * @param maxRadiation Maximum safe radiation in mSv/h
     */
    virtual void SetSafetyThresholds(double maxTemperature, double maxPressure, double maxRadiation) = 0;
    
    /**
     * @brief Validate sensor reading for data integrity
     * @param reading Sensor reading to validate
     * @return true if reading is valid, false otherwise
     */
    virtual bool ValidateReading(const SensorReading& reading) const = 0;
};

} // namespace Nuclear 