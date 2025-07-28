#pragma once

#include "ISensorReader.h"
#include "IDataProcessor.h"
#include "ISecurityManager.h"
#include "SocketManager.h"
#include <memory>
#include <string>
#include <atomic>
#include <thread>
#include <chrono>

namespace Nuclear {

/**
 * @brief Main nuclear plant monitoring system
 * 
 * Follows SOLID principles:
 * - Single Responsibility: Orchestrates monitoring operations
 * - Open/Closed: Extensible through dependency injection
 * - Liskov Substitution: Uses interfaces for all dependencies
 * - Interface Segregation: Depends only on needed interfaces
 * - Dependency Inversion: Depends on abstractions, not concretions
 */
class PlantMonitor {
private:
    std::unique_ptr<ISensorReader> m_sensorReader;
    std::unique_ptr<IDataProcessor> m_dataProcessor;
    std::unique_ptr<ISecurityManager> m_securityManager;
    std::unique_ptr<SocketManager> m_socketManager;
    
    std::atomic<bool> m_running;
    std::unique_ptr<std::thread> m_monitoringThread;
    std::chrono::milliseconds m_scanInterval;
    
    // Configuration
    std::string m_plantId;
    std::string m_configFile;

public:
    /**
     * @brief Constructor with dependency injection
     * @param sensorReader Interface for reading sensor data
     * @param dataProcessor Interface for processing data
     * @param securityManager Interface for security operations
     * @param socketManager Socket manager for network communication
     * @param plantId Unique identifier for this plant
     */
    PlantMonitor(std::unique_ptr<ISensorReader> sensorReader,
                 std::unique_ptr<IDataProcessor> dataProcessor,
                 std::unique_ptr<ISecurityManager> securityManager,
                 std::unique_ptr<SocketManager> socketManager,
                 const std::string& plantId);
    
    /**
     * @brief Destructor - ensures proper shutdown
     */
    ~PlantMonitor();
    
    /**
     * @brief Initialize the monitoring system
     * @param configFile Path to configuration file
     * @return true if initialization successful
     */
    bool Initialize(const std::string& configFile = "");
    
    /**
     * @brief Start monitoring operations
     * @param scanIntervalMs Interval between sensor scans in milliseconds
     * @return true if monitoring started successfully
     */
    bool StartMonitoring(int scanIntervalMs = 1000);
    
    /**
     * @brief Stop monitoring operations
     */
    void StopMonitoring();
    
    /**
     * @brief Get current system status
     * @return JSON string containing system status
     */
    std::string GetSystemStatus() const;
    
    /**
     * @brief Perform emergency shutdown
     * @param reason Reason for emergency shutdown
     */
    void EmergencyShutdown(const std::string& reason);
    
    /**
     * @brief Check if monitoring is active
     * @return true if monitoring is running
     */
    bool IsMonitoring() const;
    
    /**
     * @brief Set monitoring scan interval
     * @param intervalMs New interval in milliseconds
     */
    void SetScanInterval(int intervalMs);
    
    /**
     * @brief Get plant identifier
     * @return Plant ID string
     */
    const std::string& GetPlantId() const;

private:
    /**
     * @brief Main monitoring loop (runs in separate thread)
     */
    void MonitoringLoop();
    
    /**
     * @brief Perform single monitoring cycle
     * @return true if cycle completed successfully
     */
    bool PerformMonitoringCycle();
    
    /**
     * @brief Load configuration from file
     * @param configFile Path to configuration file
     * @return true if configuration loaded successfully
     */
    bool LoadConfiguration(const std::string& configFile);
    
    /**
     * @brief Handle data received from network clients
     * @param clientId Client identifier
     * @param data Received data
     */
    void HandleClientData(const std::string& clientId, const std::string& data);
    
    /**
     * @brief Handle network errors
     * @param error Error message
     */
    void HandleNetworkError(const std::string& error);
    
    /**
     * @brief Generate monitoring report
     * @param processedData Processed sensor data
     * @return JSON formatted report
     */
    std::string GenerateMonitoringReport(const ProcessedData& processedData) const;
    
    /**
     * @brief Log system event
     * @param level Log level (INFO, WARNING, ERROR, CRITICAL)
     * @param message Log message
     */
    void LogEvent(const std::string& level, const std::string& message) const;
};

} // namespace Nuclear 