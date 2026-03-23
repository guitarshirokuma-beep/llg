#include <filesystem>
#include <chrono>
#include <format>
#include <fstream>
#include <array>
#include <cstdio>
#include <unistd.h>
#include <sys/wait.h>

#include "params.hpp"
#include "llg.hpp"

std::filesystem::path make_run_dir()
{
    using namespace std::chrono;

    auto now = system_clock::now();
    zoned_time zt(current_zone(), now);
    auto local = floor<seconds>(zt.get_local_time());

    auto dir = std::filesystem::path(
        std::format(
            "runs/run_{0:%Y%m%d}/run_{0:%Y%m%d_%H%M%S}",
            local));

    std::filesystem::create_directories(dir);
    return dir;
}

std::string get_git_hash()
{
    std::array<char, 128> buffer{};
    static std::string cached;

	if(!cached.empty())
		return cached;

    FILE *pipe = popen("git rev-parse --short HEAD", "r");
    if (!pipe)
        return "unknown";

    while (fgets(buffer.data(), buffer.size(), pipe))
        cached += buffer.data();

    pclose(pipe);

    // remove "\n"
    if (!cached.empty() && cached.back() == '\n')
        cached.pop_back();

    return cached;
}

void save_params(const Params &p, const std::filesystem::path &dir)
{
    write_params_file(p, dir);
    
    cout << "Lx = " << p.Lx << "\n";
    cout << "N_steps = " << p.N_steps << "\n";
    cout << "lam = " << p.lam << "\n";
    cout << "h_app_static = " << p.h_app_static << "\n";
    cout << "pulse_norm = " << p.pulse_norm << "\n";
    cout << "sin_norm = " << p.sin_norm << "\n";
    cout << "omega = " << p.omega << "\n";
    cout << "dt = " << p.dt << "\n";
    cout << "gamma = " << p.gamma << "\n";
    cout << "J = " << p.J << "\n";
    cout << "sigma_x = " << p.sigma_x << "\n";
    cout << "sigma_step = " << p.sigma_step << "\n";
    cout << "delta = " << p.delta << "\n";

    // for python loading
    ofstream ofs("llg_params.dat");
    ofs << p.Lx << "\n";
    ofs << p.N_steps << "\n";
    ofs << p.lam << "\n";
    ofs << p.h_app_static << "\n";
    ofs << p.pulse_norm << "\n";
    ofs << p.dt << "\n";
    ofs << p.gamma << "\n";
    ofs << p.J << "\n";
    ofs << p.sigma_x << "\n";
    ofs << p.delta << "\n";
    ofs << p.sigma_step << "\n";
    ofs << p.sin_norm << "\n";
    ofs << p.omega << "\n";
}

void write_params_file(const Params &p, const std::filesystem::path &dir){
    std::ofstream f(dir / "params.txt");

    f << "Lx " << p.Lx << "\n";
    f << "N_steps " << p.N_steps << "\n";
    f << "lam " << p.lam << "\n";
    f << "h_app_static " << p.h_app_static << "\n";
    f << "sigma_x " << p.sigma_x << "\n";
    f << "sigma_step " << p.sigma_step << "\n";
    f << "pulse_norm " << p.pulse_norm << "\n";
    f << "time_pulse_center " << p.time_pulse_center << "\n";
    f << "local_pulse_center " << p.local_pulse_center << "\n";
    f << "sin_norm " << p.sin_norm << "\n";
    f << "omega " << p.omega << "\n";
    f << "J " << p.J << "\n";
    f << "delta " << p.delta << "\n";
    f << "dt " << p.dt << "\n";
    f << "gamma " << p.gamma << "\n";
    f << "git_hash " << get_git_hash() << "\n";
};

void run_python(const std::filesystem::path &dir)
{
    pid_t pid = fork();

    if (pid < 0)
    {
        perror("fork failed");
        _exit(EXIT_FAILURE);
    }

    if (pid == 0)
    {
        execl(
            "/home/seukuu/cpp_source_code/venv/bin/python",
            "/home/seukuu/cpp_source_code/venv/bin/python",
            "/home/seukuu/cpp_source_code/llg.py",
            dir.c_str(),
            (char *)nullptr);

        perror("execl failed");
        _exit(EXIT_FAILURE);
    }

    int status;
    while (waitpid(pid, &status, 0) == -1)
    {
        if (errno == EINTR)
        {
            continue;
        }

        perror("waitpid failed");
        _exit(EXIT_FAILURE);
    }

    if (WIFEXITED(status))
    {
        int code = WEXITSTATUS(status);
        if (code != 0)
        {
            std::cerr << "Python script exited with status: " << code << "\n";
        }
    }

    else if (WIFSIGNALED(status))
    {
        std::cerr << "Python killed by signal: " << WTERMSIG(status) << "\n";
    }
}
