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
