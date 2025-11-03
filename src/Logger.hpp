#pragma once
#include <ctime>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <mutex>
#include <sstream>
#include <string>

// ANSI Color Codes
#define INFO_COLOR "\033[94m"     // Blue
#define WARN_COLOR "\033[93m"     // Yellow
#define ERROR_COLOR "\033[91m"    // Red
#define SUCCESS_COLOR "\033[92m"  // Green
#define RESET_COLOR "\033[0m"

const std::string LOG_DIRECTORY_PATH = "logs";

enum class LogLevel { DEBUG, INFO, SUCCESS, WARN, ERROR };

class Logger {
   public:
    // Initialize optional file output
    static void init(const std::string& filename = "") {
        // Always ensure logs directory exists
        std::filesystem::create_directories(LOG_DIRECTORY_PATH);

        // Default to logs/game.log (clears on each run)
        std::string logFile = filename.empty() ? (LOG_DIRECTORY_PATH + "/game.log") : filename;

        fileStream_.open(filename, std::ios::out | std::ios::trunc);

        if (fileStream_.is_open()) {
            std::lock_guard<std::mutex> lock(mutex_);
            fileStream_ << "=== Log started at " << timestamp() << " ===" << std::endl;
        }
    }

    static void setLevel(LogLevel level) { currentLevel_ = level; }

    template <typename... Args>
    static void log(LogLevel level, const std::string& message, Args&&... args) {
        if (level < currentLevel_) return;

        std::ostringstream formatted;
        ((formatted << std::forward<Args>(args)), ...);  // fold expression
        std::string content = message + formatted.str();

        std::ostringstream line;

        // Build formatted line
        // Example: [12:30:00] [<colored>INFO</colored>] Game started
        line << "[" << timestamp() << "] "
             << "[" << levelColor(level) << levelToString(level) << RESET_COLOR << "] " << content;

        const std::string output = line.str();

        {
            std::lock_guard<std::mutex> lock(mutex_);

            // Print with colorized brackets only
            std::cout << output << std::endl;

            // Write to log file (no ANSI color)
            if (fileStream_.is_open()) {
                // Strip color codes for file output
                std::string plain =
                    "[" + timestamp() + "] [" + levelToString(level) + "] " + content;
                fileStream_ << plain << std::endl;
            }
        }
    }

   private:
    static std::string timestamp() {
        auto t = std::time(nullptr);
        std::tm tm;
        localtime_r(&t, &tm);
        std::ostringstream oss;
        oss << std::put_time(&tm, "%H:%M:%S");
        return oss.str();
    }

    static const char* levelToString(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:
                return "DEBUG";
            case LogLevel::INFO:
                return "INFO";
            case LogLevel::SUCCESS:
                return "SUCCESS";
            case LogLevel::WARN:
                return "WARN";
            case LogLevel::ERROR:
                return "ERROR";
            default:
                return "UNKNOWN";
        }
    }

    static const char* levelColor(LogLevel level) {
        switch (level) {
            case LogLevel::DEBUG:
                return "\033[37m";  // gray
            case LogLevel::INFO:
                return INFO_COLOR;
            case LogLevel::SUCCESS:
                return SUCCESS_COLOR;
            case LogLevel::WARN:
                return WARN_COLOR;
            case LogLevel::ERROR:
                return ERROR_COLOR;
            default:
                return RESET_COLOR;
        }
    }

    inline static std::ofstream fileStream_;
    inline static std::mutex mutex_;
    inline static LogLevel currentLevel_ = LogLevel::DEBUG;
};

// Macros
#define LOG_DEBUG(...) Logger::log(LogLevel::DEBUG, __VA_ARGS__)
#define LOG_INFO(...) Logger::log(LogLevel::INFO, __VA_ARGS__)
#define LOG_SUCCESS(...) Logger::log(LogLevel::SUCCESS, __VA_ARGS__)
#define LOG_WARN(...) Logger::log(LogLevel::WARN, __VA_ARGS__)
#define LOG_ERROR(...) Logger::log(LogLevel::ERROR, __VA_ARGS__)
