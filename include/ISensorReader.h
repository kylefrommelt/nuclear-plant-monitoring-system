#pragma once

#include <memory>
#include <vector>
#include <string>

namespace Nuclear {

/**
 * @brief Interface for reading sensor data from nuclear plant equipment
 * 
 * Follows Interface Segregation Principle - focused only on sensor reading
 */
class ISensorReader {
public:
    virtual ~ISensorReader() = default;
    
    /**
     * @brief Read temperature sensor data
     * @param sensorId Unique identifier for the sensor
     * @return Temperature value in Celsius
     */
    virtual double ReadTemperature(int sensorId) const = 0;
    
    /**
     * @brief Read pressure sensor data
     * @param sensorId Unique identifier for the sensor
     * @return Pressure value in PSI
     */
    virtual double ReadPressure(int sensorId) const = 0;
    
    /**
     * @brief Read radiation level sensor data
     * @param sensorId Unique identifier for the sensor
     * @return Radiation level in mSv/h
     */
    virtual double ReadRadiationLevel(int sensorId) const = 0;
    
    /**
     * @brief Check if sensor is online and responding
     * @param sensorId Unique identifier for the sensor
     * @return true if sensor is operational, false otherwise
     */
    virtual bool IsSensorOnline(int sensorId) const = 0;
    
    /**
     * @brief Get list of all available sensor IDs
     * @return Vector of sensor identifiers
     */
    virtual std::vector<int> GetAvailableSensors() const = 0;
};

} // namespace Nuclear 