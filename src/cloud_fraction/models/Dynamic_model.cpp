#include "Dynamic_model.hpp"

#include <cmath>
#include <algorithm>
#include <random>
#include "cloud_fraction/parser/read_data.hpp"

namespace cloud_fraction {
    Dynamic_model::Dynamic_model(const double step_t,
                                 const double delta,
                                 const double sigma,
                                 const double n,
                                 const std::filesystem::path &path) : step_t_(step_t), down_date_(-2 * n),
                                                                      delta_(delta), sigma_(sigma), n_(n),
                                                                      data_loc_(
                                                                          static_cast<std::size_t>(n / step_t) * 360 *
                                                                          180, 0),
                                                                      data_(read_data(path)) {
    }

    double Dynamic_model::get_value(const double t, const double width, const double len) const {
        constexpr double pi = std::numbers::pi_v<double>;
        const std::size_t time_ind = static_cast<std::size_t>(std::floor((t - down_date_) / step_t_)) - (
                                         t == down_date_ + n_);
        const std::size_t width_ind = 89 - static_cast<std::size_t>(std::floor(width * 180 / pi)) + (width == pi / 2);
        const std::size_t len_ind = 179 + static_cast<std::size_t>(std::ceil(len * 180 / pi)) + (len == -pi);

        return data_loc_[360 * (180 * time_ind + width_ind) + len_ind];
    }

    double Dynamic_model::time_bound(const double t) const {
        //
        const double max_time = 367 - step_t_;
        const double max_int_time = t + std::floor((max_time - t) / step_t_) * step_t_;

        return std::max(0., std::min(t - n_ / 2, max_int_time - n_));
    }

    double Dynamic_model::process(const double t, const double width, const double len) {
        //Создается генератор случайных чисел.
        std::random_device rd;
        std::mt19937 generator(rd());
        std::normal_distribution<double> e(0., sigma_);

        //Пересчитывается нижняя граница для времени t.
        down_date_ = time_bound(t);
        constexpr std::size_t step_ind = 180 * 360;

        const auto get = [step_ind, *this](const double t, const std::size_t ind) {
            const std::size_t t_ind = std::floor(t);
            return data_[step_ind * t_ind + ind];
        };

        for (std::size_t i = 0; i < step_ind; ++i) {
            data_loc_[i] = get(down_date_, i);
        }
        for (std::size_t i = step_ind; i < data_loc_.size(); ++i) {
            data_loc_[i] = std::clamp(delta_ * data_loc_[i - step_ind] +
                                      (1 - delta_) * get(down_date_ + i / step_ind * step_t_, i % step_ind), 0., 1.);
        }

        // //Создается два
        // for (std::size_t i = 0; i < 180; ++i) {
        //     for (std::size_t j = 0; j < 360; ++j) {
        //         const std::size_t data_ind = i * 360 + j;
        //         constexpr std::size_t step_ind = 180 * 360;
        //         const double width_ = pi / 2 - i * pi / 180;
        //         const double len_ = -pi + j * pi / 180;
        //         data_loc_[data_ind] = s_.cloud_fraction(down_date_, width_, len_);
        //         for (std::size_t k = 1; k < data_loc_.size() / step_ind; ++k) {
        //             const std::size_t ind = data_ind + k * step_ind;
        //             data_loc_[ind] = std::clamp(delta_ * data_loc_[ind - step_ind] +
        //                                         (1 - delta_) * mu(down_date_ + k * step_t_, width_, len_) +
        //                                         e(generator), 0., 1.);
        //         }
        //     }
        // }
        return get_value(t, width, len);
    }

    double Dynamic_model::cloud_fraction(const double t, const double width, const double len) {
        return down_date_ <= t && t <= down_date_ + n_ ? get_value(t, width, len) : process(t, width, len);
    }
}
