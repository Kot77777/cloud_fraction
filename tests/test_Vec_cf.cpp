#include <gtest/gtest.h>

#include <numbers>
#include "cloud_fraction/parser/Vec_cf.hpp"

TEST(Vec_cf, test) {
    const double pi = std::numbers::pi_v<double>;

    const std::string folder_path = "/home/kostya/Documents/cloud_fraction/processed_data";
    const cloud_fraction::Vec_cf vec_cf(folder_path, 0.5);

    const auto check = [&](const double num_day, const double width, const double len, const double exp) {
        ASSERT_EQ(vec_cf.cloud_fraction(num_day, width, len), exp);
    };

    //Тестируется случай когда день: 0, значение проверяется с координатами
    {
        const double num_day = 0.;
        const double width = 89 * pi / 180;
        const double len = - 179 * pi / 180;
        const double exp = 1.00;

        check(num_day, width, len, exp);
    }

    {
        const double num_day = 0.;
        const double width = 79.3 * pi / 180;
        const double len = - 179.6 * pi / 180;
        const double exp = 0.53;

        check(num_day, width, len, exp);
    }

    {
        const double num_day = 0.;
        const double width = 79.3 * pi / 180;
        const double len = - 179.6 * pi / 180;
        const double exp = 0.53;

        check(num_day, width, len, exp);
    }

    {
        const double num_day = 2.5;
        const double width = 33.5 * pi / 180;
        const double len = - 176.5 * pi / 180;
        const double exp = 0.44;

        check(num_day, width, len, exp);
    }
}