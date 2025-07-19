#include "gtest/gtest.h"

#include <fstream>
#include <format>
#include "cloud_fraction/models/Dynamic_model.hpp"

TEST(Dynamic_model, test_time_bound) {
    const std::filesystem::path folder_path = "/home/kostya/Documents/cloud_fraction/processed_data";
    const double delta = 0.5;
    const double sigma = 0.1;

    const auto check = [&](const double t, const double n, const double step_t) -> double {
        cloud_fraction::Dynamic_model d(step_t, delta, sigma, n, folder_path);
        return d.time_bound(t);
    };

    //Тестируется случай когда t = 245; n = 8; step_t = 1 / 8. Тогда промежуток моделирования: [241, 249]
    {
        const double t = 245;
        const double n = 8;
        const double step_t = 1. / 8;

        ASSERT_EQ(check(t, n, step_t), 241.);
    }

    //Тестируется случай когда t = 3; n = 8; step_t = 1 / 8. Тогда промежуток моделирования: [0, 8].
    {
        const double t = 3;
        const double n = 8;
        const double step_t = 1. / 8;

        ASSERT_EQ(check(t, n, step_t), 0.);
    }

    //Тестируется случай когда t = 366; n = 8; step_t = 1 / 8. Тогда промежуток моделирования: [358.875, 366.875].
    {
        const double t = 366;
        const double n = 8;
        const double step_t = 1. / 8;

        ASSERT_EQ(check(t, n, step_t), 358.875);
    }

    //Тестируется случай когда t = 366; n = 8; step_t = 5 / 24. Тогда промежуток моделирования: [358.625, 366.625].
    {
        const double t = 366;
        const double n = 8;
        const double step_t = 5. / 24;

        ASSERT_EQ(check(t, n, step_t), 358. + 5./8);
    }
}

//Записываются смоделированные данные в окрестности 200-го дня в течение 7 дней с шагом 3ч.
TEST(Dynamic_model, test) {
    const std::filesystem::path folder_path = "/home/kostya/Documents/cloud_fraction/processed_data";
    const double delta = 0.5;
    const double sigma = 0.1;
    const double t = 200.;
    const double n = 7.;
    const double step_t = 1. / 8;
    constexpr std::size_t step_ind = 180 * 360;

    cloud_fraction::Dynamic_model d(step_t, delta, sigma, n, folder_path);
    const double res = d.cloud_fraction(t, 0., 0.);
    for (std::size_t i = 0; i <= 56; ++i) {
        std::ofstream file(std::format("/home/kostya/Documents/cloud_fraction/dynamic_data/{}.txt", i));
        for (std::size_t j = 0; j < step_ind; ++j) {
            file << d.get_data(i * step_ind + j) << " ";
        }
    }
}
