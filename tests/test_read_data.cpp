#include <gtest/gtest.h>
#include <string>

#include "cloud_fraction/parser/read_data.hpp"

TEST(ReadDate, test) {
    const std::string folder_path = "/home/kostya/Documents/cloud_fraction/processed_data";

    const std::vector<double> res = cloud_fraction::read_data(folder_path);
}
