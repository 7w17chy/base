#pragma once

#include <string>
#include <vector>

namespace base
{
struct Arguments
{
    int argument_count;
    std::vector<std::string> arguments;
};

Err<int> entry_point(Arguments args);
} // namespace base

int main(int argc, char **argv)
{
    using namespace base;
    std::vector<std::string> args;
    args.reserve(argc);

    for (int i = 1; i < argc; i++)
    {
        auto str = std::string(argv[i]);
        args.push_back(std::move(str));
    }

    Arguments arguments = {
        .argument_count = argc,
        .arguments = std::move(args),
    };
    auto result = entry_point(std::move(arguments));
}
