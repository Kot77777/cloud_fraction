#pragma once

#include <vector>
#include <filesystem>

namespace cloud_fraction {
    [[nodiscard]] std::vector<double> read_data(const std::filesystem::path &path);
}
