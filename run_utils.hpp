#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>

std::string make_run_dir()
{
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "runs/run_" << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S");

    std::filesystem::create_directories(ss.str());
    return ss.str();
}