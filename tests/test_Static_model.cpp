#include <gtest/gtest.h>

#include <numbers>
#include <vector>
#include "cloud_fraction/models//Static_model.hpp"
#include "cloud_fraction/parser/read_data.hpp"

TEST(Static_model, test_cloud_fraction_1) {
    const double pi = std::numbers::pi_v<double>;

    const std::filesystem::path folder_path = "/home/kostya/Documents/cloud_fraction/processed_data";
    const cloud_fraction::Static_model vec_cf(folder_path, 0.5);

    const auto check = [&](const double num_day, const double width, const double len, const double exp) {
        ASSERT_EQ(vec_cf.cloud_fraction(num_day, width, len), exp);
    };

    //Тестируется определенный день с определенной долготой и широтой.
    {
        const double num_day = 0.;
        const double width = 89 * pi / 180;
        const double len = -179 * pi / 180;
        const double exp = 1.00;

        check(num_day, width, len, exp);
    } {
        const double num_day = 0.;
        const double width = 79.3 * pi / 180;
        const double len = -179.6 * pi / 180;
        const double exp = 0.53;

        check(num_day, width, len, exp);
    } {
        const double num_day = 0.;
        const double width = 79.3 * pi / 180;
        const double len = -179.6 * pi / 180;
        const double exp = 0.53;

        check(num_day, width, len, exp);
    } {
        const double num_day = 2.5;
        const double width = 33.5 * pi / 180;
        const double len = -176.5 * pi / 180;
        const double exp = 0.44;

        check(num_day, width, len, exp);
    }
}

TEST(Static_model, test_cloud_fraction_2) {
    const double pi = std::numbers::pi_v<double>;

    const std::filesystem::path folder_path = "/home/kostya/Documents/cloud_fraction/processed_data";
    const std::vector<double> data = cloud_fraction::read_data(folder_path);
    const cloud_fraction::Static_model vec_cf(folder_path, 0.5);

    const auto check = [&](const double num_day, const double width, const double len, const double exp) {
        ASSERT_EQ(vec_cf.cloud_fraction(num_day, width, len), exp);
    };

    //Тестируется каждый день с какой-то определенной долготой и широтой.
    {
        const auto deg_to_rad = [pi](const double a) -> double { return pi * a / 180; };
        const auto index = [](const std::size_t i, const std::size_t n, const std::size_t m) -> std::size_t {
            return 360 * (i * 180 + n) + m;
        };

        //i = *.5, то есть в каждом дне проверяется полдень.
        for (double i = 0.5; i < 366; ++i) {
            check(i, deg_to_rad(90), deg_to_rad(-180), data[index(i, 0, 0)]);
            check(i, deg_to_rad(65.3), deg_to_rad(-167.9), data[index(i, 24, 12)]);
            check(i, deg_to_rad(11), deg_to_rad(-103.95), data[index(i, 79, 76)]);
            check(i, deg_to_rad(-90), deg_to_rad(180), data[index(i, 179, 359)]);
        }
    }
}
