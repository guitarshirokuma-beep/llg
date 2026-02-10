#include <filesystem>
#include <chrono>
#include <iomanip>
#include <sstream>
#include "params.hpp"

std::string make_run_dir()
{
    auto now = std::chrono::system_clock::now();
    auto t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << "runs/run_" << std::put_time(std::localtime(&t), "%Y%m%d_%H%M%S");

    std::filesystem::create_directories(ss.str());
    return ss.str();
}

#include <cstdio>
#include <memory>
#include <array>

inline std::string get_git_hash()
{
    std::array<char, 128> buffer;
    std::string result;

    FILE *pipe = popen("git rev-parse --short HEAD", "r");
    if (!pipe)
        return "unknown";

    while (fgets(buffer.data(), buffer.size(), pipe) != nullptr)
        result += buffer.data();

    pclose(pipe);

    // 改行除去
    if (!result.empty() && result.back() == '\n')
        result.pop_back();

    return result;
}

#include <fstream>

void save_params(const Params &p, const std::string &dir)
{
    std::ofstream f(dir + "/params.txt");

    f << "Lx " << p.Lx << "\n";
    f << "N_steps " << p.N_steps << "\n";
    f << "lam " << p.lam << "\n";
    f << "h_app_static " << p.h_app_static << "\n";
    f << "sigma_x " << p.sigma_x << "\n";
    f << "sigma_step " << p.sigma_step << "\n";
    f << "pulse_norm " << p.pulse_norm << "\n";
    f << "time_pulse_center " << p.time_pulse_center << "\n";
    f << "local_pulse_center " << p.local_pulse_center << "\n";
    f << "J " << p.J << "\n";
    f << "delta " << p.delta << "\n";
    f << "dt " << p.dt << "\n";
    f << "gamma " << p.gamma << "\n";
    f << "git_hash " << get_git_hash() << "\n";
}
