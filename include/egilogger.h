#ifndef EGILOGGER_H
#define EGILOGGER_H

#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <sstream>
#include <mutex>
#include <queue>
#include <variant>
#include <any>
// Define log levels
enum class LogLevel
{
    DEBUG,
    INFO,
    WARNING,
    ERROR
};

#include <iostream>
#include <tuple>
#include <utility>

#define RESET_COLOR "\033[0m"
#define RED_TEXT "\033[31m"
#define GREEN_TEXT "\033[32m"

#include <regex>
class log
{
public:
    log(const std::string &filename)
        : logFile(filename)
    {

        std::time_t now = std::time(nullptr);
        std::tm tm = *std::localtime(&now); // TODO is it destructing it self?
        std::ostringstream filenameWithTime;
        filenameWithTime << filename << "_" << std::put_time(&tm, "%Y-%m-%d_%H-%M-%S") << ".txt";
        // TODO add date to filename
        // or do smth to prevent fileoverriding
    };
    log(){};

    static void ds(std::string value)
    {
        std::cout << GREEN_TEXT << value << RESET_COLOR << std::endl;
    };

    // Display passed arguments
    template <class... Args>
    static void d(Args... args)
    {
        std::time_t now = std::time(nullptr);
        std::tm tm = *std::localtime(&now); // TODO is it destructing it self?

        std::ostringstream logStream;
        logStream << "[" << std::put_time(&tm, "%H:%M:%S") << "] - ";
        std::cout << GREEN_TEXT << logStream.str(); // Print to console

        std::string callerArgs(__PRETTY_FUNCTION__);
        // std::cout << GREEN_TEXT << "This is green text." << RESET_COLOR << std::endl;

        size_t openBracketPos = callerArgs.find("[");

        // Check if "[" was found
        if (openBracketPos != std::string::npos)
        {
            // Extract the substring starting from "[" position
            std::string extracted = callerArgs.substr(openBracketPos);

            // Output the extracted substring
            std::cout << extracted << std::endl;
        };

        ((std::cout << args << "\n"), ...);
        std::cout << RESET_COLOR << std::endl;
    };

    // Log a message with a specified log level
    void logg(LogLevel level, const std::string &message)
    {
        std::lock_guard<std::mutex> lock(mutex);

        std::string levelStr;
        switch (level)
        {
        case LogLevel::DEBUG:
            levelStr = "DEBUG";
            break;
        case LogLevel::INFO:
            levelStr = "INFO";
            break;
        case LogLevel::WARNING:
            levelStr = "WARNING";
            break;
        case LogLevel::ERROR:
            levelStr = "ERROR";
            break;
        }
        std::time_t now = std::time(nullptr);
        std::tm tm = *std::localtime(&now);

        std::ostringstream logStream;
        logStream << std::put_time(&tm, "%Y-%m-%d %H:%M:%S") << " [" << levelStr << "] " << message << std::endl;

        std::cout << logStream.str(); // Print to console
        logFile << logStream.str();   // Write to the log file
    }

private:
    std::ofstream logFile;
    std::mutex mutex;
    std::size_t counter; // every log event has que value, so later you can filter logs
};

#endif
