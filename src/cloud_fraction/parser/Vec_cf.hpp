#pragma once

#include <vector>
#include <filesystem>

namespace cloud_fraction {
    class Vec_cf {
        std::vector<double> data_;
        double bound_;

    public:

        Vec_cf(const std::filesystem::path &path, double bound);

        [[nodiscard]] double cloud_fraction(double num_day, double width, double len) const;

        [[nodiscard]] bool operator()(double num_day, double width, double len) const;
    };
}
