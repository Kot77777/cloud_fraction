#include "cloud.hpp"

#include <cmath>
#include <numbers>
#include "read_data.hpp"

namespace cloud_fraction {
    cloud::cloud(const std::filesystem::path &path, const double bound) {
        data_ = read_data(path);
        bound_ = bound;
    }

    double cloud::cloud_fraction(double num_day, double width, double len) const {
        constexpr double pi = std::numbers::pi_v<double>;

        const std::size_t day_ind = std::floor(num_day);
        // const std::size_t width_ind = width == pi / 2 ? 0 : 89 - static_cast<std::size_t>(std::floor(width * 180 / pi));
        // const std::size_t len_ind = len == -pi ? 0 : 179 + static_cast<std::size_t>(std::ceil(len * 180 / pi));

        const std::size_t width_ind = width == -pi / 2 ? 179 : static_cast<std::size_t>(std::floor((0.5 - width / pi) * 180));
        const std::size_t len_ind = len == pi ? 359 : static_cast<std::size_t>(std::floor((len / pi + 1) * 180));

        return data_[360 * (180 * day_ind + width_ind) + len_ind];
    }


    bool cloud::operator()(const double num_day, const double width, const double len) const {
        return cloud_fraction(num_day, width, len) <= bound_;
    }
}
