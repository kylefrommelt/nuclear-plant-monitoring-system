#pragma once

#include <string>
#include <vector>

namespace Nuclear {

/**
 * @brief Interface for security management in nuclear plant monitoring
 * 
 * Implements secure coding practices and encryption for sensitive data
 */
class ISecurityManager {
public:
    virtual ~ISecurityManager() = default;
    
    /**
     * @brief Encrypt sensitive data before transmission or storage
     * @param data Plain text data to encrypt
     * @return Encrypted data as string
     */
    virtual std::string EncryptData(const std::string& data) = 0;
    
    /**
     * @brief Decrypt encrypted data
     * @param encryptedData Encrypted data to decrypt
     * @return Decrypted plain text data
     */
    virtual std::string DecryptData(const std::string& encryptedData) = 0;
    
    /**
     * @brief Validate input data for security vulnerabilities
     * @param input Input string to validate
     * @return true if input is safe, false if potentially malicious
     */
    virtual bool ValidateInput(const std::string& input) const = 0;
    
    /**
     * @brief Sanitize input data to prevent injection attacks
     * @param input Input string to sanitize
     * @return Sanitized safe string
     */
    virtual std::string SanitizeInput(const std::string& input) const = 0;
    
    /**
     * @brief Generate secure hash for data integrity verification
     * @param data Data to hash
     * @return SHA-256 hash of the data
     */
    virtual std::string GenerateHash(const std::string& data) const = 0;
    
    /**
     * @brief Verify data integrity using hash comparison
     * @param data Original data
     * @param hash Expected hash
     * @return true if hash matches, false otherwise
     */
    virtual bool VerifyHash(const std::string& data, const std::string& hash) const = 0;
};

} // namespace Nuclear 