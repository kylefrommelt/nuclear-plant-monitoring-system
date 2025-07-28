#pragma once

#include <string>
#include <vector>
#include <functional>
#include <memory>
#include <thread>
#include <atomic>
#include <mutex>

#ifdef _WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")
#else
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

namespace Nuclear {

/**
 * @brief Windows socket manager for real-time nuclear plant data transmission
 * 
 * Handles TCP/UDP communication for monitoring data distribution
 * Implements secure socket communication with proper error handling
 */
class SocketManager {
public:
    using DataHandler = std::function<void(const std::string&, const std::string&)>;
    using ErrorHandler = std::function<void(const std::string&)>;

private:
    struct ClientConnection {
        SOCKET socket;
        std::string clientAddress;
        std::string clientId;
        bool authenticated;
        std::chrono::steady_clock::time_point lastActivity;
    };

    SOCKET m_serverSocket;
    std::vector<ClientConnection> m_clients;
    std::atomic<bool> m_running;
    std::unique_ptr<std::thread> m_acceptThread;
    std::unique_ptr<std::thread> m_heartbeatThread;
    std::mutex m_clientsMutex;
    
    int m_port;
    DataHandler m_dataHandler;
    ErrorHandler m_errorHandler;
    
    // Security settings
    static constexpr int MAX_CLIENTS = 10;
    static constexpr int HEARTBEAT_INTERVAL_MS = 30000; // 30 seconds
    static constexpr int CLIENT_TIMEOUT_MS = 60000;     // 60 seconds
    static constexpr int BUFFER_SIZE = 4096;

public:
    /**
     * @brief Constructor for Socket Manager
     * @param port Port to listen on for incoming connections
     */
    explicit SocketManager(int port);
    
    /**
     * @brief Destructor - ensures proper cleanup
     */
    ~SocketManager();
    
    /**
     * @brief Initialize Windows Socket subsystem
     * @return true if initialization successful
     */
    bool Initialize();
    
    /**
     * @brief Start the socket server
     * @return true if server started successfully
     */
    bool StartServer();
    
    /**
     * @brief Stop the socket server
     */
    void StopServer();
    
    /**
     * @brief Send data to all connected clients
     * @param data Data to send
     * @return Number of clients data was successfully sent to
     */
    int BroadcastData(const std::string& data);
    
    /**
     * @brief Send data to specific client
     * @param clientId Client identifier
     * @param data Data to send
     * @return true if data sent successfully
     */
    bool SendToClient(const std::string& clientId, const std::string& data);
    
    /**
     * @brief Set data handler callback
     * @param handler Function to call when data received from client
     */
    void SetDataHandler(DataHandler handler);
    
    /**
     * @brief Set error handler callback
     * @param handler Function to call when error occurs
     */
    void SetErrorHandler(ErrorHandler handler);
    
    /**
     * @brief Get number of connected clients
     * @return Current client count
     */
    int GetClientCount() const;
    
    /**
     * @brief Get list of connected client IDs
     * @return Vector of client identifiers
     */
    std::vector<std::string> GetConnectedClients() const;
    
    /**
     * @brief Check if server is running
     * @return true if server is active
     */
    bool IsRunning() const;

private:
    /**
     * @brief Accept incoming client connections (runs in separate thread)
     */
    void AcceptConnections();
    
    /**
     * @brief Handle client communication
     * @param client Client connection to handle
     */
    void HandleClient(ClientConnection& client);
    
    /**
     * @brief Send heartbeat to all clients (runs in separate thread)
     */
    void HeartbeatMonitor();
    
    /**
     * @brief Remove disconnected or timed-out clients
     */
    void CleanupClients();
    
    /**
     * @brief Authenticate client connection
     * @param client Client to authenticate
     * @return true if authentication successful
     */
    bool AuthenticateClient(ClientConnection& client);
    
    /**
     * @brief Generate unique client ID
     * @param clientAddress Client IP address
     * @return Unique client identifier
     */
    std::string GenerateClientId(const std::string& clientAddress);
    
    /**
     * @brief Cleanup Windows Socket resources
     */
    void Cleanup();
};

} // namespace Nuclear 