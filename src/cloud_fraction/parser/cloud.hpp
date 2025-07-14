#pragma once

#include <vector>
#include <filesystem>

namespace cloud_fraction {
    class cloud {
        /**
         * @param data_ Вектор из годовых значений облачности.
         * @param bound_ Пороговое значение облачности.
         */
        std::vector<double> data_;
        double bound_;

    public:
        cloud(const std::filesystem::path &path, double bound);

        /**
         * @brief Функция возвращает значение облачности по номеру дня, широте и долготе.
         *
         * @param num_day Номер дня в году (может быть и дробным) \f$ \in [0; 366) \f$.
         * @param width Координата по долготе, [рад].
         * @param len Координата по широте, [рад].
         *
         * @return Возвращает значение облачности \f$ \in [0; 1] \f$.
         *
         * @author Танов К. Л.
         */
        [[nodiscard]] double cloud_fraction(double num_day, double width, double len) const;

        /**
         * @brief Функция по пороговому значению определяет облачно или нет.
         *
         * @param num_day Номер дня в году (может быть и дробным) \f$ \in [0; 366) \f$.
         * @param width Координата по долготе, [рад].
         * @param len Координата по широте, [рад].
         *
         * @return Возвращает true, если значение облачности меньше порогового, иначе false.
         *
         * @author Танов К. Л.
         */
        [[nodiscard]] bool operator()(double num_day, double width, double len) const;
    };
}
