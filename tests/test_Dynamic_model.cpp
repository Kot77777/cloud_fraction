#include "gtest/gtest.h"

#include "cloud_fraction/models/Dynamic_model.hpp"

TEST(Dynamic_model, test1) {
    const std::filesystem::path folder_path = "/home/kostya/Documents/cloud_fraction/processed_data";
    const double step_t = 1./8;
    const double delta = 0.5;
    const double sigma = 0.1;
    const double n = 7;

    cloud_fraction::Dynamic_model d(step_t, delta, sigma, n, folder_path);
    const double cf1 = d.cloud_fraction(129, 0, 0);
    const double cf2 = d.cloud_fraction(120, 0, 0);

}