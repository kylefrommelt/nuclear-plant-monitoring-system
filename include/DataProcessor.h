#pragma once

#include "IDataProcessor.h"
#include <memory>
#include <vector>
#include <mutex>
#include <chrono>

namespace Nuclear {

/**
 * @brief Concrete implementation of data processor for nuclear plant monitoring
 * 
 * Follows Single Responsibility Principle - handles only data processing
 * Implements thread-safe operations for real-time processing
 */
class DataProcessor : public IDataProcessor {
private:
    // Safety thresholds
    double m_maxTemperature;
    double m_maxPressure;
    double m_maxRadiation;
    
    // Thread safety
    mutable std::mutex m_thresholdsMutex;
    
    // Statistics tracking
    struct Statistics {
        size_t totalReadings;
        size_t alertCount;
        std::chrono::steady_clock::time_point lastProcessingTime;
        double processingTimeMs;
    };
    
    mutable Statistics m_statistics;
    mutable std::mutex m_statisticsMutex;

public:
    /**
     * @brief Constructor with default safety thresholds
     */
    DataProcessor();
    
    /**
     * @brief Destructor
     */
    virtual ~DataProcessor() = default;
    
    // IDataProcessor interface implementation
    ProcessedData ProcessReadings(const std::vector<SensorReading>& readings) override;
    void SetSafetyThresholds(double maxTemperature, double maxPressure, double maxRadiation) override;
    bool ValidateReading(const SensorReading& reading) const override;
    
    /**
     * @brief Get processing statistics
     * @return Statistics structure with performance metrics
     */
    Statistics GetStatistics() const;
    
    /**
     * @brief Reset processing statistics
     */
    void ResetStatistics();

private:
    /**
     * @brief Calculate average values for each sensor type
     * @param readings Vector of sensor readings
     * @return Calculated averages
     */
    std::tuple<double, double, double> CalculateAverages(const std::vector<SensorReading>& readings) const;
    
    /**
     * @brief Check if any readings exceed safety thresholds
     * @param readings Vector of sensor readings
     * @return Pair of (alertTriggered, alertMessage)
     */
    std::pair<bool, std::string> CheckSafetyThresholds(const std::vector<SensorReading>& readings) const;
    
    /**
     * @brief Validate reading value ranges
     * @param reading Sensor reading to validate
     * @return true if reading is within expected ranges
     */
    bool IsValueInRange(const SensorReading& reading) const;
    
    /**
     * @brief Generate timestamp string
     * @return Current timestamp in ISO format
     */
    std::string GenerateTimestamp() const;
    
    /**
     * @brief Filter out invalid or anomalous readings
     * @param readings Input readings
     * @return Filtered readings
     */
    std::vector<SensorReading> FilterReadings(const std::vector<SensorReading>& readings) const;
};

} // namespace Nuclear 