#include "Dynamic_model.hpp"

#include <cmath>
#include <algorithm>
#include <random>
#include "cloud_fraction/parser/read_data.hpp"

namespace cloud_fraction {
    //down_date_ инициализируется как -2*n, чтобы в начале программы обязательно шло моделирование, а не возвращалось значение.
    //(n / step_t) должно быть целым. По хорошему нужно бы вызывать ошибку, если это не так.
    Dynamic_model::Dynamic_model(const double step_t,
                                 const double delta,
                                 const double sigma,
                                 const double n,
                                 const std::filesystem::path &path) : step_t_(step_t), down_date_(-2 * n),
                                                                      delta_(delta), sigma_(sigma), n_(n),
                                                                      data_loc_((n / step_t + 1) * 360 * 180),
                                                                      data_(read_data(path)) {
    }

    //Эта функция нужна только для теста, чтобы считать данные.
    double Dynamic_model::get_data(const std::size_t i) const {
        return data_loc_[i];
    }

    // Функция находит нужное значение в массиве из промоделированных данных.
    double Dynamic_model::get_value(const double t, const double width, const double len) const {
        constexpr double pi = std::numbers::pi_v<double>;
        const auto time_ind = static_cast<std::size_t>(std::floor((t - down_date_) / step_t_));
        const std::size_t width_ind = 89 - static_cast<std::size_t>(std::floor(width * 180 / pi)) + (width == pi / 2);
        const std::size_t len_ind = 179 + static_cast<std::size_t>(std::ceil(len * 180 / pi)) + (len == -pi);

        return data_loc_[360 * (180 * time_ind + width_ind) + len_ind];
    }

    //Функция находит нижнюю границу окрестности. Верхняя очевидно down_date_ + n_.
    double Dynamic_model::time_bound(const double t) const {
        // Верхняя граница по времени: 367 - step_t_(в целом можно добавить в расчет еще и 367 день, тогда будет
        // просто 367, но тут вроде и так сойдет).
        const double max_time = 367 - step_t_;
        // Тут вычисляется верхняя граница времени, в которую относительно времени t влезает целое число step_t_.
        const double max_int_time = t + std::floor((max_time - t) / step_t_) * step_t_;

        // Тут вычисляется нижняя граница (учитывая все случаи) так, чтобы по итогу полное время моделирования было n_.
        return std::max(0., std::min(t - n_ / 2, max_int_time - n_));
    }

    //Функция моделирования облачности.
    double Dynamic_model::process(const double t, const double width, const double len) {
        //Создается генератор случайных чисел.
        std::random_device rd;
        std::mt19937 generator(rd());
        std::normal_distribution<double> e(0., sigma_);

        //Пересчитывается нижняя граница для промежутка моделирования.
        down_date_ = time_bound(t);
        constexpr std::size_t step_ind = 180 * 360;

        //Возвращает значение облачности на основе статических данных. Тут ind учитывает сразу долготу и широту.
        const auto static_value = [step_ind, *this](const double t, const std::size_t ind) -> double {
            const std::size_t t_ind = std::floor(t);
            return data_[step_ind * t_ind + ind];
        };

        //Вычисляем начальные значения облачности для всех широт и долгот как значения облачности в момент времени down_date_.
        for (std::size_t i = 0; i < step_ind; ++i) {
            data_loc_[i] = static_value(down_date_, i);
        }
        //Вычисляем остальные значения. Тут написано так: i / step_ind, потому что нужно целое число после деления.
        for (std::size_t i = step_ind; i < data_loc_.size(); ++i) {
            data_loc_[i] = std::clamp(delta_ * data_loc_[i - step_ind] +
                                      (1 - delta_) * static_value(down_date_ + i / step_ind * step_t_, i % step_ind) +
                                      e(generator), 0., 1.);
        }

        //Возвращаем нужное значение из вектора, в котором хранятся промоделированные данные.
        return get_value(t, width, len);
    }

    double Dynamic_model::cloud_fraction(const double t, const double width, const double len) {
        return down_date_ <= t && t <= down_date_ + n_ ? get_value(t, width, len) : process(t, width, len);
    }
}
