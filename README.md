# Nuclear Plant Monitoring System

**A Production-Grade Nuclear Plant Monitoring System**  

## 🎯 Project Overview

This project demonstrates a comprehensive, production-grade nuclear plant monitoring system built with modern C++ and web technologies. It showcases industrial-grade software engineering practices, secure coding, real-time data processing, and protocol implementation suitable for the commercial nuclear sector.

### Key Features

- **Real-time Sensor Monitoring**: Temperature, pressure, and radiation level tracking
- **Industrial Protocol Support**: Modbus TCP/IP implementation for device communication
- **Windows Socket Programming**: Multi-client network communication with authentication
- **Advanced Security**: Input validation, encryption, and secure coding practices
- **SOLID Design Principles**: Fully modular, extensible architecture
- **Comprehensive Testing**: Unit testing framework with extensive coverage
- **Professional Web Dashboard**: Real-time visualization with HTML5/CSS3/JavaScript
- **Production-Ready**: Exception handling, logging, and monitoring

## 🛠️ Technology Stack

### Core Technologies
- **C++17**: Modern C++ with RAII, smart pointers, and STL
- **CMake**: Cross-platform build system
- **Windows Sockets**: Network programming for real-time communication
- **Modbus Protocol**: Industrial device communication

### Web Technologies
- **HTML5/CSS3**: Modern responsive web interface
- **JavaScript ES6+**: Real-time dashboard with Chart.js visualization
- **WebSocket**: Real-time data streaming

### Security & Quality
- **Secure Coding**: Input validation, encryption, hash verification
- **Unit Testing**: Comprehensive test coverage
- **Documentation**: Doxygen-compatible documentation
- **Industrial Standards**: Nuclear safety compliance considerations

## 🏗️ Architecture

The system follows SOLID design principles with dependency injection:

```
├── Core Application (C++)
│   ├── PlantMonitor (Main orchestrator)
│   ├── ModbusHandler (Industrial protocol)
│   ├── SocketManager (Network communication)
│   ├── SecurityManager (Encryption & validation)
│   └── DataProcessor (Safety monitoring)
├── Web Dashboard
│   ├── Real-time visualization
│   ├── Alert management
│   └── Control interface
└── Testing Framework
    ├── Unit tests
    ├── Integration tests
    └── Security tests
```

## 🚀 Getting Started

### Prerequisites

**Windows Development Environment:**
- Visual Studio 2019+ or MinGW with C++17 support
- CMake 3.16+
- Windows 10+ for socket programming
- Web browser (Chrome/Edge recommended)

### Building the Project

1. **Clone the repository:**
   ```bash
   git clone <repository-url>
   cd westinghouse
   ```

2. **Configure and build:**
   ```bash
   mkdir build
   cd build
   cmake ..
   cmake --build . --config Release
   ```

3. **Run the application:**
   ```bash
   ./NuclearPlantMonitor
   ```

4. **Access the web dashboard:**
   - Open `web/index.html` in your browser
   - The dashboard will connect to the monitoring system on port 8080

### Running Tests

```bash
cd build
ctest --verbose
```

## 🔧 Configuration

### System Configuration

Create `config/plant_config.ini`:

```ini
[Plant]
PlantID=WESTINGHOUSE_REACTOR_001
Location=Warrendale, PA
ScanInterval=1000

[Safety]
MaxTemperature=350.0
MaxPressure=2200.0
MaxRadiation=1.0

[Network]
Port=8080
MaxClients=10

[Modbus]
Device1=192.168.1.100:502
Device2=192.168.1.101:502
Device3=192.168.1.102:502
```

### Security Configuration

The system includes multiple security layers:
- **Input Validation**: SQL injection and XSS prevention
- **Encryption**: Data encryption for network transmission
- **Authentication**: Client authentication for socket connections
- **Hash Verification**: Data integrity checking

## 📊 Features Demonstration

### 1. SOLID Design Principles

**Single Responsibility:**
- `SecurityManager`: Only handles security operations
- `DataProcessor`: Only processes sensor data
- `ModbusHandler`: Only handles Modbus communication

**Open/Closed:**
- New sensor types can be added without modifying existing code
- Protocol handlers can be extended through interfaces

**Liskov Substitution:**
- Any `ISensorReader` implementation can replace `ModbusHandler`
- Multiple data processors can implement `IDataProcessor`

**Interface Segregation:**
- Separate interfaces for reading, processing, and security
- Clients depend only on methods they use

**Dependency Inversion:**
- High-level modules depend on abstractions
- Dependencies injected through constructors

### 2. Windows Socket Programming

```cpp
// Multi-client server with authentication
class SocketManager {
    SOCKET m_serverSocket;
    std::vector<ClientConnection> m_clients;
    std::atomic<bool> m_running;
    
    void AcceptConnections();
    void HandleClient(ClientConnection& client);
    bool AuthenticateClient(ClientConnection& client);
};
```

### 3. Modbus Protocol Implementation

```cpp
// Industrial protocol handling
class ModbusHandler : public ISensorReader {
    std::vector<uint8_t> BuildModbusFrame(uint16_t transactionId, 
                                          uint8_t unitId, 
                                          uint8_t functionCode, 
                                          uint16_t address, 
                                          uint16_t quantity) const;
};
```

### 4. Secure Coding Practices

```cpp
// Input validation and encryption
class SecurityManager : public ISecurityManager {
    bool ValidateInput(const std::string& input) const override;
    std::string EncryptData(const std::string& data) override;
    bool ContainsSqlInjection(const std::string& input) const;
    bool ContainsScriptInjection(const std::string& input) const;
};
```

### 5. Comprehensive Testing

```cpp
// Professional unit testing
class SecurityManagerTest {
    void TestSqlInjectionDetection();
    void TestScriptInjectionDetection();
    void TestEncryptionRoundTrip();
    void TestInputSanitization();
};
```

## 🎨 Web Dashboard Features

- **Real-time Charts**: Live sensor data visualization
- **Alert System**: Safety threshold monitoring
- **Control Panel**: System control and configuration
- **Security Status**: Network security monitoring
- **Responsive Design**: Works on desktop and mobile
- **Professional UI**: Industrial-grade interface design

## 🔒 Security Features

### Input Validation
- SQL injection prevention
- XSS attack prevention
- Buffer overflow protection
- Pattern-based threat detection

### Encryption & Authentication
- Data encryption for transmission
- Hash-based integrity verification
- Client authentication
- Secure memory management

### Monitoring & Logging
- Security event tracking
- Failed authentication logging
- Threat attempt counting
- Audit trail generation

## 🧪 Testing Coverage

### Unit Tests
- **SecurityManager**: 15+ test cases covering all security functions
- **DataProcessor**: Threshold checking, data validation, statistics
- **ModbusHandler**: Protocol frame building, device communication
- **SocketManager**: Connection handling, authentication, data transmission

### Integration Tests
- End-to-end sensor reading
- Network communication
- Dashboard integration
- Security system integration

## 📈 Performance Characteristics

- **Real-time Processing**: Sub-second response times
- **Concurrent Clients**: Supports up to 10 simultaneous connections
- **Memory Efficient**: Smart pointer usage, RAII principles
- **Thread-Safe**: Mutex-protected critical sections
- **Scalable Architecture**: Easy to extend and modify

## 🏭 Industrial Applications

This system demonstrates capabilities directly applicable to:

- **Nuclear Plant Operations**: Real-time safety monitoring
- **Industrial Control Systems**: Modbus device integration
- **SCADA Systems**: Supervisory control and data acquisition
- **Safety Critical Systems**: Fault detection and emergency response
- **Network Security**: Industrial cybersecurity implementation

## 📚 Documentation

### Code Documentation
- Doxygen-compatible comments throughout
- Professional header documentation
- Inline code explanations
- Architecture decision records

### Technical Specifications
- System requirements
- Protocol specifications
- Security implementation details
- Performance benchmarks

## 🚀 Future Enhancements

- **OPC-UA Support**: Additional industrial protocol
- **Database Integration**: Historical data storage
- **Advanced Analytics**: Predictive maintenance algorithms
- **Mobile App**: iOS/Android monitoring application
- **Cloud Integration**: Azure/AWS connectivity
- **Regulatory Compliance**: Nuclear industry standards

## 👨‍💻 Professional Qualifications Demonstrated

This project showcases expertise in:

### Technical Skills
- ✅ **C/C++ Programming**: Advanced modern C++ practices
- ✅ **Windows Development**: Socket programming, Windows APIs
- ✅ **SOLID Principles**: Professional software architecture
- ✅ **Object-Oriented Design**: Interfaces, inheritance, polymorphism
- ✅ **Secure Coding**: Input validation, encryption, threat prevention
- ✅ **Unit Testing**: Comprehensive test coverage
- ✅ **Documentation**: Professional code documentation
- ✅ **Industrial Protocols**: Modbus TCP/IP implementation
- ✅ **Socket Programming**: Multi-client network architecture
- ✅ **Web Technologies**: HTML/CSS/JavaScript dashboard

### Industry Knowledge
- ✅ **Nuclear Sector**: Safety-critical system design
- ✅ **Industrial Control**: SCADA and monitoring systems
- ✅ **Cybersecurity**: Industrial security best practices
- ✅ **Regulatory Awareness**: Safety and compliance considerations
