#pragma once
#include <string>
#include <fstream>
#include <sstream>
#include <chrono>
#include <iomanip>
#include <mutex>
#include <memory>

enum class LogLevel {
    Debug,
    Info,
    Warning,
    Error,
    Fatal
};

class DebugLogger {
public:
    static DebugLogger& Instance() {
        static DebugLogger instance;
        return instance;
    }

    DebugLogger(const DebugLogger&) = delete;
    DebugLogger& operator=(const DebugLogger&) = delete;

    void Initialize(const std::string& filename = "game.log") {
        std::lock_guard<std::mutex> lock(mutex);
        logFile.open(filename, std::ios::out | std::ios::app);
        Log(LogLevel::Info, "Debug Logger Initialized");
    }

    void Shutdown() {
        std::lock_guard<std::mutex> lock(mutex);
        if (logFile.is_open()) {
            Log(LogLevel::Info, "Debug Logger Shutdown");
            logFile.close();
        }
    }

    template<typename... Args>
    void Debug(Args&&... args) {
        Log(LogLevel::Debug, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Info(Args&&... args) {
        Log(LogLevel::Info, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Warning(Args&&... args) {
        Log(LogLevel::Warning, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Error(Args&&... args) {
        Log(LogLevel::Error, std::forward<Args>(args)...);
    }

    template<typename... Args>
    void Fatal(Args&&... args) {
        Log(LogLevel::Fatal, std::forward<Args>(args)...);
    }

private:
    DebugLogger() = default;
    ~DebugLogger() {
        Shutdown();
    }

    template<typename... Args>
    void Log(LogLevel level, Args&&... args) {
        std::lock_guard<std::mutex> lock(mutex);
        if (!logFile.is_open()) return;

        std::stringstream ss;
        (ss << ... << std::forward<Args>(args));

        auto now = std::chrono::system_clock::now();
        auto time = std::chrono::system_clock::to_time_t(now);
        auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(
            now.time_since_epoch()) % 1000;

        logFile << std::put_time(std::localtime(&time), "%Y-%m-%d %H:%M:%S")
                << '.' << std::setfill('0') << std::setw(3) << ms.count()
                << " [" << GetLevelString(level) << "] "
                << ss.str() << std::endl;

        // Also output to console in debug builds
#ifdef _DEBUG
        std::cout << GetLevelString(level) << ": " << ss.str() << std::endl;
#endif
    }

    const char* GetLevelString(LogLevel level) {
        switch (level) {
            case LogLevel::Debug:   return "DEBUG";
            case LogLevel::Info:    return "INFO";
            case LogLevel::Warning: return "WARNING";
            case LogLevel::Error:   return "ERROR";
            case LogLevel::Fatal:   return "FATAL";
            default:                return "UNKNOWN";
        }
    }

    std::ofstream logFile;
    std::mutex mutex;
};

// Convenience macros
#define LOG_DEBUG(...) DebugLogger::Instance().Debug(__VA_ARGS__)
#define LOG_INFO(...) DebugLogger::Instance().Info(__VA_ARGS__)
#define LOG_WARNING(...) DebugLogger::Instance().Warning(__VA_ARGS__)
#define LOG_ERROR(...) DebugLogger::Instance().Error(__VA_ARGS__)
#define LOG_FATAL(...) DebugLogger::Instance().Fatal(__VA_ARGS__)

// Scope timer for performance logging
class ScopeTimer {
public:
    ScopeTimer(const char* name) 
        : name(name), start(std::chrono::high_resolution_clock::now()) {}
    
    ~ScopeTimer() {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start);
        LOG_DEBUG(name, " took ", duration.count(), " microseconds");
    }

private:
    const char* name;
    std::chrono::time_point<std::chrono::high_resolution_clock> start;
};

#define SCOPE_TIMER(name) ScopeTimer timer##__LINE__(name)
