#include "SecurityManager.h"
#include <iostream>
#include <cassert>
#include <vector>
#include <string>

using namespace Nuclear;

class SecurityManagerTest {
private:
    SecurityManager* securityManager;
    int testsRun;
    int testsPassed;
    int testsFailed;

public:
    SecurityManagerTest() : securityManager(nullptr), testsRun(0), testsPassed(0), testsFailed(0) {}
    
    ~SecurityManagerTest() {
        delete securityManager;
    }
    
    void Setup() {
        securityManager = new SecurityManager();
    }
    
    void TearDown() {
        delete securityManager;
        securityManager = nullptr;
    }
    
    bool Assert(bool condition, const std::string& testName, const std::string& message) {
        testsRun++;
        if (condition) {
            testsPassed++;
            std::cout << "  [PASS] " << testName << std::endl;
            return true;
        } else {
            testsFailed++;
            std::cout << "  [FAIL] " << testName << ": " << message << std::endl;
            return false;
        }
    }
    
    void RunAllTests() {
        std::cout << "\n=== SecurityManager Unit Tests ===" << std::endl;
        
        Setup();
        
        // Test encryption/decryption functionality
        TestEncryptionDecryption();
        TestEncryptionWithEmptyData();
        TestEncryptionRoundTrip();
        
        // Test input validation
        TestSqlInjectionDetection();
        TestScriptInjectionDetection();
        TestInputLengthValidation();
        TestValidInputPassing();
        
        // Test input sanitization
        TestInputSanitization();
        TestSpecialCharacterEscaping();
        
        // Test hash generation and verification
        TestHashGeneration();
        TestHashVerification();
        
        // Test key management
        TestCustomKeyInitialization();
        
        // Test statistics tracking
        TestStatisticsTracking();
        
        TearDown();
        
        // Print summary
        std::cout << "\n=== Test Summary ===" << std::endl;
        std::cout << "Total Tests: " << testsRun << std::endl;
        std::cout << "Passed: " << testsPassed << std::endl;
        std::cout << "Failed: " << testsFailed << std::endl;
        std::cout << "Success Rate: " << (100.0 * testsPassed / testsRun) << "%" << std::endl;
        
        if (testsFailed == 0) {
            std::cout << "\n[PASSED] All SecurityManager tests completed successfully!" << std::endl;
        } else {
            std::cout << "\n[FAILED] Some SecurityManager tests failed!" << std::endl;
        }
    }

private:
    void TestEncryptionDecryption() {
        std::string testData = "Nuclear Plant Sensor Data: Temperature=350C, Pressure=2000PSI";
        std::string encrypted = securityManager->EncryptData(testData);
        
        Assert(!encrypted.empty(), "EncryptData_NonEmpty", "Encrypted data should not be empty");
        Assert(encrypted != testData, "EncryptData_Different", "Encrypted data should be different from original");
        
        std::string decrypted = securityManager->DecryptData(encrypted);
        Assert(decrypted == testData, "DecryptData_Roundtrip", "Decrypted data should match original");
    }
    
    void TestEncryptionWithEmptyData() {
        std::string empty = "";
        std::string encrypted = securityManager->EncryptData(empty);
        Assert(encrypted.empty(), "EncryptData_Empty", "Encrypting empty string should return empty");
        
        std::string decrypted = securityManager->DecryptData(encrypted);
        Assert(decrypted.empty(), "DecryptData_Empty", "Decrypting empty string should return empty");
    }
    
    void TestEncryptionRoundTrip() {
        std::vector<std::string> testStrings = {
            "Simple text",
            "Special chars: !@#$%^&*()",
            "Numbers: 12345",
            "Unicode: αβγδε",
            "Long string with multiple words and various characters 1234567890"
        };
        
        for (const auto& testStr : testStrings) {
            std::string encrypted = securityManager->EncryptData(testStr);
            std::string decrypted = securityManager->DecryptData(encrypted);
            Assert(decrypted == testStr, "EncryptDecrypt_Roundtrip_" + std::to_string(testStr.length()), 
                   "Round trip encryption should preserve data");
        }
    }
    
    void TestSqlInjectionDetection() {
        std::vector<std::string> sqlInjections = {
            "'; DROP TABLE users; --",
            "1' OR '1'='1",
            "admin'--",
            "'; INSERT INTO users VALUES ('hacker', 'password'); --",
            "1 UNION SELECT * FROM passwords",
            "test'; DELETE FROM sensors WHERE id=1; --"
        };
        
        for (const auto& injection : sqlInjections) {
            bool isValid = securityManager->ValidateInput(injection);
            Assert(!isValid, "SqlInjection_Blocked", "SQL injection should be blocked: " + injection);
        }
    }
    
    void TestScriptInjectionDetection() {
        std::vector<std::string> scriptInjections = {
            "<script>alert('XSS')</script>",
            "javascript:alert('XSS')",
            "<img src=x onerror=alert('XSS')>",
            "vbscript:msgbox('XSS')",
            "<iframe src=javascript:alert('XSS')></iframe>",
            "<div onload=alert('XSS')>content</div>"
        };
        
        for (const auto& injection : scriptInjections) {
            bool isValid = securityManager->ValidateInput(injection);
            Assert(!isValid, "ScriptInjection_Blocked", "Script injection should be blocked: " + injection);
        }
    }
    
    void TestInputLengthValidation() {
        // Test normal length input
        std::string normalInput(500, 'a');
        Assert(securityManager->ValidateInput(normalInput), "InputLength_Normal", "Normal length input should be valid");
        
        // Test excessive length input
        std::string longInput(2000, 'a');
        Assert(!securityManager->ValidateInput(longInput), "InputLength_Excessive", "Excessive length input should be invalid");
    }
    
    void TestValidInputPassing() {
        std::vector<std::string> validInputs = {
            "Normal sensor reading",
            "Temperature: 25.5°C",
            "Reactor status: NORMAL",
            "user@westinghouse.com",
            "Sensor_ID_001",
            "2023-12-07 14:30:00"
        };
        
        for (const auto& input : validInputs) {
            bool isValid = securityManager->ValidateInput(input);
            Assert(isValid, "ValidInput_Passing", "Valid input should pass validation: " + input);
        }
    }
    
    void TestInputSanitization() {
        std::string maliciousInput = "<script>alert('hack')</script>";
        std::string sanitized = securityManager->SanitizeInput(maliciousInput);
        
        Assert(sanitized.find("<script>") == std::string::npos, "Sanitization_ScriptRemoval", 
               "Sanitized input should not contain script tags");
        Assert(sanitized.find("&lt;") != std::string::npos || sanitized.find("script") == std::string::npos, 
               "Sanitization_EscapingOrRemoval", "Dangerous characters should be escaped or removed");
    }
    
    void TestSpecialCharacterEscaping() {
        std::string input = "<>&\"'";
        std::string sanitized = securityManager->SanitizeInput(input);
        
        // Check if dangerous characters are escaped
        bool hasEscaped = (sanitized.find("&lt;") != std::string::npos) ||
                         (sanitized.find("&gt;") != std::string::npos) ||
                         (sanitized.find("&amp;") != std::string::npos) ||
                         (sanitized.find("&quot;") != std::string::npos) ||
                         (sanitized.find("&#x27;") != std::string::npos);
        
        Assert(hasEscaped || sanitized.length() < input.length(), "SpecialChar_Escaping", 
               "Special characters should be escaped or removed");
    }
    
    void TestHashGeneration() {
        std::string data = "Nuclear Plant Data";
        std::string hash1 = securityManager->GenerateHash(data);
        std::string hash2 = securityManager->GenerateHash(data);
        
        Assert(!hash1.empty(), "Hash_NotEmpty", "Hash should not be empty");
        Assert(hash1 == hash2, "Hash_Consistent", "Same data should produce same hash");
        
        std::string differentData = "Different Data";
        std::string hash3 = securityManager->GenerateHash(differentData);
        Assert(hash1 != hash3, "Hash_Different", "Different data should produce different hash");
    }
    
    void TestHashVerification() {
        std::string data = "Test data for verification";
        std::string hash = securityManager->GenerateHash(data);
        
        Assert(securityManager->VerifyHash(data, hash), "HashVerify_Valid", "Valid hash should verify correctly");
        Assert(!securityManager->VerifyHash("tampered data", hash), "HashVerify_Invalid", "Invalid hash should fail verification");
        Assert(!securityManager->VerifyHash(data, "invalid_hash"), "HashVerify_WrongHash", "Wrong hash should fail verification");
    }
    
    void TestCustomKeyInitialization() {
        std::vector<uint8_t> customKey = {0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08,
                                         0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10};
        
        Assert(securityManager->InitializeWithKey(customKey), "CustomKey_Init", "Custom key initialization should succeed");
        
        // Test with too short key
        std::vector<uint8_t> shortKey = {0x01, 0x02, 0x03};
        Assert(!securityManager->InitializeWithKey(shortKey), "CustomKey_TooShort", "Too short key should fail");
    }
    
    void TestStatisticsTracking() {
        auto initialStats = securityManager->GetSecurityStats();
        
        // Perform some operations
        securityManager->EncryptData("test data");
        securityManager->ValidateInput("'; DROP TABLE users; --");
        
        auto updatedStats = securityManager->GetSecurityStats();
        
        Assert(updatedStats.encryptionOperations > initialStats.encryptionOperations, 
               "Stats_EncryptionCount", "Encryption count should increase");
        Assert(updatedStats.validationFailures > initialStats.validationFailures, 
               "Stats_ValidationFailures", "Validation failure count should increase");
    }
};

// Function to run security manager tests
void RunSecurityManagerTests() {
    SecurityManagerTest test;
    test.RunAllTests();
} 