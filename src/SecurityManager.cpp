#include "SecurityManager.h"
#include <algorithm>
#include <regex>
#include <random>
#include <iomanip>
#include <sstream>
#include <cstring>

namespace Nuclear {

SecurityManager::SecurityManager() : m_stats{0, 0, 0, 0} {
    // Initialize security configuration with safe defaults
    m_config.maxInputLength = 1024;
    m_config.enableInputSanitization = true;
    m_config.enableHashVerification = true;
    
    // Define blocked patterns for injection prevention
    m_config.blockedPatterns = {
        "DROP TABLE", "DELETE FROM", "INSERT INTO", "UPDATE SET",
        "<script", "javascript:", "vbscript:", "onload=", "onerror=",
        "../", "..\\", "%2e%2e", "passwd", "/etc/", "\\system32"
    };
    
    // Generate secure encryption key
    m_encryptionKey = GenerateRandomKey();
}

SecurityManager::~SecurityManager() {
    // Securely clear sensitive data
    SecureClear(m_encryptionKey);
}

std::string SecurityManager::EncryptData(const std::string& data) {
    std::lock_guard<std::mutex> lock(m_encryptionMutex);
    
    if (data.empty()) {
        return "";
    }
    
    try {
        // In production, use proper encryption like AES-256
        std::string encrypted = XorEncryptDecrypt(data, m_encryptionKey);
        
        // Convert to hex for safe transmission
        std::stringstream hexStream;
        for (unsigned char c : encrypted) {
            hexStream << std::hex << std::setw(2) << std::setfill('0') << static_cast<int>(c);
        }
        
        UpdateStats("encryption");
        return hexStream.str();
        
    } catch (const std::exception&) {
        UpdateStats("encryption_error");
        return "";
    }
}

std::string SecurityManager::DecryptData(const std::string& encryptedData) {
    std::lock_guard<std::mutex> lock(m_encryptionMutex);
    
    if (encryptedData.empty() || encryptedData.length() % 2 != 0) {
        return "";
    }
    
    try {
        // Convert from hex back to binary
        std::string binaryData;
        for (size_t i = 0; i < encryptedData.length(); i += 2) {
            std::string byteString = encryptedData.substr(i, 2);
            unsigned char byte = static_cast<unsigned char>(std::stoi(byteString, nullptr, 16));
            binaryData.push_back(byte);
        }
        
        // Decrypt the data
        std::string decrypted = XorEncryptDecrypt(binaryData, m_encryptionKey);
        
        UpdateStats("decryption");
        return decrypted;
        
    } catch (const std::exception&) {
        UpdateStats("decryption_error");
        return "";
    }
}

bool SecurityManager::ValidateInput(const std::string& input) const {
    // Check for basic length limits
    if (ExceedsSafeLength(input)) {
        UpdateStats("validation_failure");
        return false;
    }
    
    // Check for SQL injection patterns
    if (ContainsSqlInjection(input)) {
        UpdateStats("sql_injection_blocked");
        return false;
    }
    
    // Check for script injection patterns
    if (ContainsScriptInjection(input)) {
        UpdateStats("script_injection_blocked");
        return false;
    }
    
    // Check against blocked patterns
    std::string upperInput = input;
    std::transform(upperInput.begin(), upperInput.end(), upperInput.begin(), ::toupper);
    
    for (const auto& pattern : m_config.blockedPatterns) {
        std::string upperPattern = pattern;
        std::transform(upperPattern.begin(), upperPattern.end(), upperPattern.begin(), ::toupper);
        
        if (upperInput.find(upperPattern) != std::string::npos) {
            UpdateStats("blocked_pattern");
            return false;
        }
    }
    
    return true;
}

std::string SecurityManager::SanitizeInput(const std::string& input) const {
    if (!m_config.enableInputSanitization) {
        return input;
    }
    
    std::string sanitized = input;
    
    // Remove dangerous characters
    sanitized = RemoveDangerousChars(sanitized);
    
    // Escape special characters
    sanitized = EscapeSpecialChars(sanitized);
    
    // Truncate to safe length
    if (sanitized.length() > m_config.maxInputLength) {
        sanitized = sanitized.substr(0, m_config.maxInputLength);
    }
    
    return sanitized;
}

std::string SecurityManager::GenerateHash(const std::string& data) const {
    return CalculateSHA256(data);
}

bool SecurityManager::VerifyHash(const std::string& data, const std::string& hash) const {
    if (!m_config.enableHashVerification) {
        return true;  // Skip verification if disabled
    }
    
    std::string calculatedHash = GenerateHash(data);
    
    // Constant-time comparison to prevent timing attacks
    if (calculatedHash.length() != hash.length()) {
        return false;
    }
    
    volatile int result = 0;
    for (size_t i = 0; i < calculatedHash.length(); ++i) {
        result |= (calculatedHash[i] ^ hash[i]);
    }
    
    return result == 0;
}

bool SecurityManager::InitializeWithKey(const std::vector<uint8_t>& keyData) {
    std::lock_guard<std::mutex> lock(m_encryptionMutex);
    
    if (keyData.size() < 16) {  // Minimum key size
        return false;
    }
    
    // Securely clear old key
    SecureClear(m_encryptionKey);
    
    // Copy new key
    m_encryptionKey = keyData;
    
    return true;
}

SecurityManager::SecurityStats SecurityManager::GetSecurityStats() const {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    return m_stats;
}

void SecurityManager::UpdateConfiguration(const SecurityConfig& config) {
    m_config = config;
}

// Private methods implementation

std::vector<uint8_t> SecurityManager::GenerateRandomKey(size_t keySize) {
    std::vector<uint8_t> key(keySize);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, 255);
    
    for (size_t i = 0; i < keySize; ++i) {
        key[i] = static_cast<uint8_t>(dis(gen));
    }
    
    return key;
}

std::string SecurityManager::XorEncryptDecrypt(const std::string& data, const std::vector<uint8_t>& key) const {
    if (key.empty()) {
        return data;
    }
    
    std::string result;
    result.reserve(data.length());
    
    for (size_t i = 0; i < data.length(); ++i) {
        result.push_back(data[i] ^ key[i % key.size()]);
    }
    
    return result;
}

bool SecurityManager::ContainsSqlInjection(const std::string& input) const {
    // Common SQL injection patterns
    std::vector<std::regex> sqlPatterns = {
        std::regex(R"(\b(SELECT|INSERT|UPDATE|DELETE|DROP|CREATE|ALTER|UNION)\b)", std::regex_constants::icase),
        std::regex(R"((\-\-|\#|\/\*|\*\/))"),  // SQL comments
        std::regex(R"((\;|\||\&))"),  // Command separators
        std::regex(R"((\bOR\b|\bAND\b).*(\=|\<|\>))", std::regex_constants::icase),  // Boolean injections
        std::regex(R"('.*(\bOR\b|\bAND\b).*')", std::regex_constants::icase)  // String injections
    };
    
    for (const auto& pattern : sqlPatterns) {
        if (std::regex_search(input, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool SecurityManager::ContainsScriptInjection(const std::string& input) const {
    // Common script injection patterns
    std::vector<std::regex> scriptPatterns = {
        std::regex(R"(<script.*>)", std::regex_constants::icase),
        std::regex(R"(javascript:)", std::regex_constants::icase),
        std::regex(R"(vbscript:)", std::regex_constants::icase),
        std::regex(R"(on\w+\s*=)", std::regex_constants::icase),  // Event handlers
        std::regex(R"(<.*\s+src\s*=)", std::regex_constants::icase)  // External resources
    };
    
    for (const auto& pattern : scriptPatterns) {
        if (std::regex_search(input, pattern)) {
            return true;
        }
    }
    
    return false;
}

bool SecurityManager::ExceedsSafeLength(const std::string& input) const {
    return input.length() > m_config.maxInputLength;
}

std::string SecurityManager::RemoveDangerousChars(const std::string& input) const {
    std::string result;
    result.reserve(input.length());
    
    for (char c : input) {
        // Allow alphanumeric, spaces, and common safe punctuation
        if (std::isalnum(c) || c == ' ' || c == '.' || c == '-' || c == '_' || c == '@') {
            result.push_back(c);
        }
    }
    
    return result;
}

std::string SecurityManager::EscapeSpecialChars(const std::string& input) const {
    std::string result;
    result.reserve(input.length() * 2);  // Reserve extra space for escaping
    
    for (char c : input) {
        switch (c) {
            case '<':
                result += "&lt;";
                break;
            case '>':
                result += "&gt;";
                break;
            case '&':
                result += "&amp;";
                break;
            case '"':
                result += "&quot;";
                break;
            case '\'':
                result += "&#x27;";
                break;
            default:
                result.push_back(c);
                break;
        }
    }
    
    return result;
}

std::string SecurityManager::CalculateSHA256(const std::string& data) const {
    // Simplified hash implementation for demo purposes
    // In production, use proper crypto library like OpenSSL
    std::hash<std::string> hasher;
    size_t hashValue = hasher(data);
    
    std::stringstream ss;
    ss << std::hex << hashValue;
    return ss.str();
}

void SecurityManager::SecureClear(std::vector<uint8_t>& data) {
    if (!data.empty()) {
        // Use volatile to prevent compiler optimization
        volatile uint8_t* ptr = data.data();
        for (size_t i = 0; i < data.size(); ++i) {
            ptr[i] = 0;
        }
        data.clear();
    }
}

void SecurityManager::UpdateStats(const std::string& operation) const {
    std::lock_guard<std::mutex> lock(m_statsMutex);
    
    if (operation == "encryption") {
        ++m_stats.encryptionOperations;
    } else if (operation == "decryption") {
        ++m_stats.decryptionOperations;
    } else if (operation.find("validation") != std::string::npos || 
               operation.find("blocked") != std::string::npos ||
               operation.find("injection") != std::string::npos) {
        ++m_stats.validationFailures;
        ++m_stats.blockedInputs;
    }
}

} // namespace Nuclear 