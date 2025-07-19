#pragma once
#include <vector>

#include "Static_model.hpp"

namespace cloud_fraction {
    class Dynamic_model {
        /**
         * @param data_loc_ Вектор, в котором хранятся данные облачности в окрестноти времени t.
         */
        std::vector<double> data_loc_;
        /**
         * @param data_ Вектор из годовых значений облачности.
         */
        std::vector<double> data_;
        /**
         * @param step_t_ Шаг по времени, с которым идет моделирование, [дн].
         */
        double step_t_;
        /**
         * @param delta_ Параметр модуляци.
         */
        double delta_;
        /**
         * @param sigma_ Среднеквадратичное отклонение случайной величины.
         */
        double sigma_;
        /**
         * @param down_date_ Нижняя граница временного промежутка, в течение которого идет моделирования, [дн].
         */
        double down_date_;
        /**
         * @param n_ Время моделирования, [дн].
         */
        double n_;

    public:
        Dynamic_model(double step_t, double delta, double sigma, double n, const std::filesystem::path &path);

        [[nodiscard]] double get_data(std::size_t i) const;

        [[nodiscard]] double get_value(double t, double width, double len) const;

        [[nodiscard]] double time_bound(double t) const;

        [[nodiscard]] double process(double t, double width, double len);

        /**
         * @brief Функция возвращает значение облачности по врмени, широте и долготе.
         *
         * @param t Время относительно начала года, [дн].
         * @param width Координата по широте, [рад].
         * @param len Координата по долготе, [рад].
         *
         * @return Возвращает значение облачности \f$ \in [0; 1] \f$.
         */
        [[nodiscard]] double cloud_fraction(double t, double width, double len);
    };
}
