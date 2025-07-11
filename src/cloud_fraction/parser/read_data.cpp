#include "read_data.hpp"

#include <fstream>
#include <filesystem>
#include "string"

namespace cloud_fraction {
    [[nodiscard]] std::vector<double> read_data(const std::filesystem::path &folder_path) {
        std::vector<double> res(366 * 360 * 179);

        for (const auto &file_iter: std::filesystem::directory_iterator(folder_path)) {
            std::string line;
            double num;
            std::size_t index = 0;

            std::ifstream file(file_iter.path());

            if (!file.is_open()) { throw std::runtime_error("error"); }

            std::getline(file, line);
            std::istringstream(line) >> index;
            index *= 179 * 360;

            std::getline(file, line);
            std::istringstream iss(line);

            while (iss >> num) { res[index++] = num; }
        }

        return res;
    }
}
