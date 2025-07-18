#include "Static_model.hpp"

#include <cmath>
#include <numbers>
#include "cloud_fraction/parser/read_data.hpp"

namespace cloud_fraction {
    Static_model::Static_model(const std::filesystem::path &path, const double bound) : data_(read_data(path)), bound_(bound){}

    double Static_model::cloud_fraction(const double num_day, const double width, const double len) const {
        constexpr double pi = std::numbers::pi_v<double>;

        const std::size_t day_ind = std::floor(num_day);

        //Индекс надо увеличить на 1, если width == pi / 2 и len == -pi в соответсвующих выражениях.
        const std::size_t width_ind = 89 - static_cast<std::size_t>(std::floor(width * 180 / pi)) + (width == pi / 2);
        const std::size_t len_ind = 179 + static_cast<std::size_t>(std::ceil(len * 180 / pi)) + (len == -pi);

        return data_[360 * (180 * day_ind + width_ind) + len_ind];
    }


    bool Static_model::operator()(const double num_day, const double width, const double len) const {
        return cloud_fraction(num_day, width, len) <= bound_;
    }
}
