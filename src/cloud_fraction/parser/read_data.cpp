#include "read_data.hpp"

#include <fstream>
#include <filesystem>
#include "string"

namespace cloud_fraction {
    std::vector<double> read_data(const std::filesystem::path &folder_path) {
        //Создаем вектор, и выделяем память на 366 * 360 * 180 значений, так как год высокосный, значиний по долготе 360, по широте 180.
        std::vector<double> res(366 * 360 * 180);

        //Итерируемся по всем файлам в папке.
        for (const auto &file_iter: std::filesystem::directory_iterator(folder_path)) {
            std::string line;
            double num;
            std::size_t index = 0;

            std::ifstream file(file_iter.path());

            if (!file.is_open()) { throw std::runtime_error("Ошибка открытия файла!"); }

            //считываем номер дня в году.
            std::getline(file, line);
            std::istringstream(line) >> index;
            //получаем index в векторе начиная, с которого нужно заполнять его данными за один день.
            index *= 180 * 360;

            //считываем строку данных.
            std::getline(file, line);
            std::istringstream iss(line);

            //инициализируем вектор, нвчиная с индекса index.
            while (iss >> num) { res[index++] = num; }
        }

        return res;
    }
}
