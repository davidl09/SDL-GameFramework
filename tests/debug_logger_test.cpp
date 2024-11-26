#include <gtest/gtest.h>
#include "debug/debug_logger.h"
#include <fstream>
#include <string>
#include <thread>
#include <filesystem>
#include <regex>

class DebugLoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        logFilename = "test.log";
        // Ensure clean state
        if (std::filesystem::exists(logFilename)) {
            std::filesystem::remove(logFilename);
        }
        DebugLogger::Instance().Initialize(logFilename);
    }

    void TearDown() override {
        DebugLogger::Instance().Shutdown();
        if (std::filesystem::exists(logFilename)) {
            std::filesystem::remove(logFilename);
        }
    }

    std::string ReadLogFile() {
        std::ifstream file(logFilename);
        std::string content((std::istreambuf_iterator<char>(file)),
                           std::istreambuf_iterator<char>());
        return content;
    }

    std::string logFilename;
};

// TEST_F(DebugLoggerTest, LogLevels) {
//     LOG_DEBUG("Debug message");
//     LOG_INFO("Info message");
//     LOG_WARNING("Warning message");
//     LOG_ERROR("Error message");
//     LOG_FATAL("Fatal message");

//     std::string content = ReadLogFile();
//     EXPECT_TRUE(content.find("[DEBUG]") != std::string::npos);
//     EXPECT_TRUE(content.find("[INFO]") != std::string::npos);
//     EXPECT_TRUE(content.find("[WARNING]") != std::string::npos);
//     EXPECT_TRUE(content.find("[ERROR]") != std::string::npos);
//     EXPECT_TRUE(content.find("[FATAL]") != std::string::npos);
// }

// TEST_F(DebugLoggerTest, LogFormatting) {
//     LOG_INFO("Test message");
//     std::string content = ReadLogFile();

//     // Check timestamp format: YYYY-MM-DD HH:MM:SS.mmm
//     std::regex timestamp_regex(R"(\d{4}-\d{2}-\d{2} \d{2}:\d{2}:\d{2}\.\d{3})");
//     EXPECT_TRUE(std::regex_search(content, timestamp_regex));
// }

// TEST_F(DebugLoggerTest, MultipleArguments) {
//     LOG_INFO("Value1: ", 42, " Value2: ", 3.14, " Text: ", "test");
//     std::string content = ReadLogFile();
//     EXPECT_TRUE(content.find("Value1: 42 Value2: 3.14 Text: test") != std::string::npos);
// }

// TEST_F(DebugLoggerTest, ConcurrentLogging) {
//     const int numThreads = 10;
//     const int numLogsPerThread = 100;
//     std::vector<std::thread> threads;

//     for (int i = 0; i < numThreads; ++i) {
//         threads.emplace_back([i, numLogsPerThread]() {
//             for (int j = 0; j < numLogsPerThread; ++j) {
//                 LOG_INFO("Thread ", i, " Log ", j);
//             }
//         });
//     }

//     for (auto& thread : threads) {
//         thread.join();
//     }

//     std::string content = ReadLogFile();
//     int logCount = 0;
//     std::string::size_type pos = 0;
//     while ((pos = content.find("[INFO]", pos)) != std::string::npos) {
//         ++logCount;
//         pos += 6;
//     }

//     EXPECT_EQ(logCount, numThreads * numLogsPerThread + 1); // +1 for initialization log
// }

TEST_F(DebugLoggerTest, ScopeTimer) {
    {
        SCOPE_TIMER("TestScope");
        std::this_thread::sleep_for(std::chrono::milliseconds(10));
    }

    std::string content = ReadLogFile();
    EXPECT_TRUE(content.find("TestScope took") != std::string::npos);
    EXPECT_TRUE(content.find("microseconds") != std::string::npos);
}

TEST_F(DebugLoggerTest, NestedScopeTimers) {
    {
        SCOPE_TIMER("OuterScope");
        {
            SCOPE_TIMER("InnerScope1");
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
        {
            SCOPE_TIMER("InnerScope2");
            std::this_thread::sleep_for(std::chrono::milliseconds(10));
        }
    }

    std::string content = ReadLogFile();
    EXPECT_TRUE(content.find("OuterScope took") != std::string::npos);
    EXPECT_TRUE(content.find("InnerScope1 took") != std::string::npos);
    EXPECT_TRUE(content.find("InnerScope2 took") != std::string::npos);
}

TEST_F(DebugLoggerTest, LoggerReinitialization) {
    LOG_INFO("First log");
    DebugLogger::Instance().Shutdown();
    
    // Create new log file
    std::string newLogFile = "test2.log";
    DebugLogger::Instance().Initialize(newLogFile);
    LOG_INFO("Second log");
    
    // Check first log file
    std::string content1 = ReadLogFile();
    EXPECT_TRUE(content1.find("First log") != std::string::npos);
    
    // Check second log file
    std::ifstream file2(newLogFile);
    std::string content2((std::istreambuf_iterator<char>(file2)),
                        std::istreambuf_iterator<char>());
    EXPECT_TRUE(content2.find("Second log") != std::string::npos);
    
    // Cleanup
    DebugLogger::Instance().Shutdown();
    if (std::filesystem::exists(newLogFile)) {
        std::filesystem::remove(newLogFile);
    }
}

TEST_F(DebugLoggerTest, LoggerPerformance) {
    const int numLogs = 10000;
    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < numLogs; ++i) {
        LOG_INFO("Performance test log ", i);
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    
    // Log rate should be at least 1000 logs per second
    EXPECT_LT(duration.count(), numLogs); // Less than 1ms per log
}
