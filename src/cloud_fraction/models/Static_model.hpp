/**
 * @file
 * @brief Класс для статической модели облачности.
 */
#pragma once

#include <vector>
#include <filesystem>

namespace cloud_fraction {
    /**
     * Класс для статической модели облачности.
     */
    class Static_model {
        /**
         * @param data_ Вектор из годовых значений облачности.
         */
        std::vector<double> data_;

        /**
         * @param bound_ Пороговое значение облачности.
         */
        double bound_;

    public:
        Static_model(const std::filesystem::path &path, double bound);

        /**
         * @brief Функция возвращает значение облачности по номеру дня, широте и долготе.
         *
         * @param num_day Номер дня в году (может быть и дробным) \f$ \in [0; 367) \f$.
         * @param width Координата по широте, [рад].
         * @param len Координата по долготе, [рад].
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
         * @param width Координата по широте, [рад].
         * @param len Координата по долготе, [рад].
         *
         * @return Возвращает true, если значение облачности меньше порогового, иначе false.
         *
         * @author Танов К. Л.
         */
        [[nodiscard]] bool operator()(double num_day, double width, double len) const;
    };
}
