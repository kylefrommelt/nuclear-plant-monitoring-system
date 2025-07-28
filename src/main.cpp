#include "PlantMonitor.h"
#include "ModbusHandler.h"
#include "DataProcessor.h"
#include "SecurityManager.h"
#include "SocketManager.h"
#include <iostream>
#include <memory>
#include <csignal>
#include <atomic>

#ifdef _WIN32
#include <windows.h>
#include <conio.h>
#else
#include <unistd.h>
#endif

using namespace Nuclear;

// Global variables for signal handling
std::atomic<bool> g_running{true};
std::unique_ptr<PlantMonitor> g_monitor;

/**
 * @brief Signal handler for graceful shutdown
 * @param signal Signal number
 */
void SignalHandler(int signal) {
    std::cout << "\nReceived signal " << signal << ". Initiating graceful shutdown...\n";
    g_running = false;
    if (g_monitor) {
        g_monitor->StopMonitoring();
    }
}

/**
 * @brief Display application banner
 */
void DisplayBanner() {
    std::cout << R"(
╔══════════════════════════════════════════════════════════════╗
║           Nuclear Plant Monitoring System v1.0              ║
║                                                              ║
║  Westinghouse Electric Co. - Commercial Nuclear Division    ║
║  Real-time monitoring with industrial-grade reliability     ║
╚══════════════════════════════════════════════════════════════╝
)" << std::endl;
}

/**
 * @brief Display help information
 */
void DisplayHelp() {
    std::cout << "\nNuclear Plant Monitor Commands:\n";
    std::cout << "  status  - Display current system status\n";
    std::cout << "  clients - Show connected monitoring clients\n";
    std::cout << "  config  - Display current configuration\n";
    std::cout << "  help    - Show this help message\n";
    std::cout << "  quit    - Shutdown monitoring system\n";
    std::cout << "\nPress Enter after typing command.\n\n";
}

/**
 * @brief Create and configure the monitoring system with dependency injection
 * @return Configured PlantMonitor instance
 */
std::unique_ptr<PlantMonitor> CreateMonitoringSystem() {
    try {
        // Create dependencies using SOLID principles (Dependency Inversion)
        auto sensorReader = std::make_unique<ModbusHandler>();
        auto dataProcessor = std::make_unique<DataProcessor>();
        auto securityManager = std::make_unique<SecurityManager>();
        auto socketManager = std::make_unique<SocketManager>(8080);
        
        // Configure Modbus devices (simulated for demo)
        auto modbusHandler = static_cast<ModbusHandler*>(sensorReader.get());
        modbusHandler->AddDevice("192.168.1.100");  // Primary reactor sensors
        modbusHandler->AddDevice("192.168.1.101");  // Secondary cooling sensors
        modbusHandler->AddDevice("192.168.1.102");  // Radiation monitoring sensors
        
        // Configure safety thresholds
        auto processor = static_cast<DataProcessor*>(dataProcessor.get());
        processor->SetSafetyThresholds(
            350.0,   // Max temperature (°C)
            2200.0,  // Max pressure (PSI)
            1.0      // Max radiation (mSv/h)
        );
        
        // Create main monitoring system with dependency injection
        std::string plantId = "WESTINGHOUSE_REACTOR_001";
        auto monitor = std::make_unique<PlantMonitor>(
            std::move(sensorReader),
            std::move(dataProcessor),
            std::move(securityManager),
            std::move(socketManager),
            plantId
        );
        
        return monitor;
        
    } catch (const std::exception& e) {
        std::cerr << "Failed to create monitoring system: " << e.what() << std::endl;
        return nullptr;
    }
}

/**
 * @brief Process user commands
 * @param command User input command
 * @return true to continue, false to exit
 */
bool ProcessCommand(const std::string& command) {
    if (command == "quit" || command == "exit") {
        return false;
    } else if (command == "status") {
        if (g_monitor) {
            std::cout << g_monitor->GetSystemStatus() << std::endl;
        }
    } else if (command == "clients") {
        if (g_monitor) {
            // Would display connected client information
            std::cout << "Connected monitoring clients: [Implementation would show client list]\n";
        }
    } else if (command == "config") {
        std::cout << "Plant ID: " << (g_monitor ? g_monitor->GetPlantId() : "Not initialized") << std::endl;
        std::cout << "Monitoring: " << (g_monitor && g_monitor->IsMonitoring() ? "ACTIVE" : "INACTIVE") << std::endl;
    } else if (command == "help") {
        DisplayHelp();
    } else if (!command.empty()) {
        std::cout << "Unknown command: " << command << ". Type 'help' for available commands.\n";
    }
    return true;
}

/**
 * @brief Main application entry point
 */
int main() {
    // Display application banner
    DisplayBanner();
    
    // Set up signal handlers for graceful shutdown
    std::signal(SIGINT, SignalHandler);
    std::signal(SIGTERM, SignalHandler);
    
#ifdef _WIN32
    // Windows-specific console setup
    SetConsoleOutputCP(CP_UTF8);
    std::signal(SIGBREAK, SignalHandler);
#endif
    
    try {
        // Create monitoring system
        std::cout << "Initializing Nuclear Plant Monitoring System...\n";
        g_monitor = CreateMonitoringSystem();
        
        if (!g_monitor) {
            std::cerr << "Failed to create monitoring system. Exiting.\n";
            return 1;
        }
        
        // Initialize the system
        if (!g_monitor->Initialize("config/plant_config.ini")) {
            std::cerr << "Failed to initialize monitoring system. Exiting.\n";
            return 1;
        }
        
        // Start monitoring operations
        std::cout << "Starting monitoring operations...\n";
        if (!g_monitor->StartMonitoring(1000)) {  // 1 second scan interval
            std::cerr << "Failed to start monitoring. Exiting.\n";
            return 1;
        }
        
        std::cout << "Nuclear Plant Monitoring System is now ACTIVE\n";
        std::cout << "Type 'help' for available commands or 'quit' to exit.\n\n";
        
        // Main command loop
        std::string command;
        while (g_running) {
            std::cout << "NPM> ";
            std::getline(std::cin, command);
            
            if (!ProcessCommand(command)) {
                break;
            }
        }
        
    } catch (const std::exception& e) {
        std::cerr << "Critical error: " << e.what() << std::endl;
        return 1;
    }
    
    // Cleanup
    std::cout << "\nShutting down Nuclear Plant Monitoring System...\n";
    if (g_monitor) {
        g_monitor->StopMonitoring();
        g_monitor.reset();
    }
    
    std::cout << "Shutdown complete. Goodbye.\n";
    return 0;
} 