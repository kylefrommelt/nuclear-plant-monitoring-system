#pragma once

#include "ISensorReader.h"
#include <memory>
#include <string>
#include <vector>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace Nuclear {

/**
 * @brief Modbus TCP/IP protocol handler for industrial sensor communication
 * 
 * Implements ISensorReader using Modbus protocol for nuclear plant equipment
 * Follows Open/Closed Principle - can be extended without modification
 */
class ModbusHandler : public ISensorReader {
private:
    struct ModbusConnection {
        std::string ipAddress;
        int port;
        SOCKET socket;
        bool connected;
    };

    std::vector<ModbusConnection> m_connections;
    mutable std::mutex m_connectionMutex;
    int m_transactionId;

    // Modbus function codes
    static constexpr uint8_t MODBUS_READ_HOLDING_REGISTERS = 0x03;
    static constexpr uint8_t MODBUS_READ_INPUT_REGISTERS = 0x04;
    
    // Sensor address mappings (would be configured from plant documentation)
    static constexpr int TEMPERATURE_BASE_ADDRESS = 0x1000;
    static constexpr int PRESSURE_BASE_ADDRESS = 0x2000;
    static constexpr int RADIATION_BASE_ADDRESS = 0x3000;

public:
    /**
     * @brief Constructor for Modbus handler
     */
    ModbusHandler();
    
    /**
     * @brief Destructor - ensures proper cleanup of connections
     */
    virtual ~ModbusHandler();
    
    /**
     * @brief Add Modbus device connection
     * @param ipAddress IP address of Modbus device
     * @param port Port number (typically 502 for Modbus TCP)
     * @return true if connection added successfully
     */
    bool AddDevice(const std::string& ipAddress, int port = 502);
    
    /**
     * @brief Connect to all configured Modbus devices
     * @return true if all connections successful
     */
    bool ConnectToDevices();
    
    /**
     * @brief Disconnect from all Modbus devices
     */
    void DisconnectFromDevices();
    
    // ISensorReader interface implementation
    double ReadTemperature(int sensorId) const override;
    double ReadPressure(int sensorId) const override;
    double ReadRadiationLevel(int sensorId) const override;
    bool IsSensorOnline(int sensorId) const override;
    std::vector<int> GetAvailableSensors() const override;

private:
    /**
     * @brief Send Modbus request and receive response
     * @param deviceIndex Index of device in connections vector
     * @param functionCode Modbus function code
     * @param address Register address to read
     * @param quantity Number of registers to read
     * @return Raw register value or -1 on error
     */
    int SendModbusRequest(int deviceIndex, uint8_t functionCode, uint16_t address, uint16_t quantity) const;
    
    /**
     * @brief Convert raw Modbus register value to engineering units
     * @param rawValue Raw 16-bit register value
     * @param sensorType Type of sensor for proper scaling
     * @return Converted value in engineering units
     */
    double ConvertToEngineeringUnits(int rawValue, const std::string& sensorType) const;
    
    /**
     * @brief Build Modbus TCP frame
     * @param transactionId Transaction identifier
     * @param unitId Unit identifier
     * @param functionCode Modbus function code
     * @param address Starting address
     * @param quantity Number of registers
     * @return Complete Modbus TCP frame
     */
    std::vector<uint8_t> BuildModbusFrame(uint16_t transactionId, uint8_t unitId, 
                                          uint8_t functionCode, uint16_t address, uint16_t quantity) const;
};

} // namespace Nuclear 