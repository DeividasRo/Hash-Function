#include "myhash.h"
#include "tests.h"
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <cmath>
#include <chrono>

std::string ReadFileToString(std::string file_name, int lines_to_read = 0)
{
    std::stringstream buffer;
    std::ifstream inf;
    inf.open("text-files/" + file_name);
    if (inf)
    {
        if (lines_to_read == 0)
            buffer << inf.rdbuf();
        else
        {
            std::string line;
            while (std::getline(inf, line) && lines_to_read != 0)
            {
                buffer << line;
                lines_to_read--;
            }
        }
    }
    else
    {
        throw std::runtime_error(std::string("Failed to load file!"));
    }
    buffer.clear();
    inf.close();
    return buffer.str();
}

int main(int argc, char *argv[])
{
    std::string input, output;

    if (argc == 2)
    {
        if (argv[1] == std::string("colltest")) // Hash function collision test
        {
            int tests[4] = {10, 100, 500, 1000};
            std::cout << "Testing hash function collisions: " << std::endl;
            for (int i = 0; i < sizeof(tests) / sizeof(int); i++)
            {
                std::cout << i + 1 << "# 25000 pairs of strings, " << tests[i] << " random symbols per string..." << std::endl;
                std::cout << "Collisions detected: " << CollisionTest(&Hash, tests[i]) << std::endl;
                std::cout << std::endl;
            }
            return 0;
        }
        else if (argv[1] == std::string("avaltest")) // Hash function avalanche test
        {
            int tests[4] = {10, 100, 500, 1000};
            std::cout << "Testing hash function avalanche effect: " << std::endl;
            for (int i = 0; i < sizeof(tests) / sizeof(int); i++)
            {
                int diffs[6] = {64, 256, 0, 0, 0, 0}; // 0-1 - min diff, 2-3 - max diff, 4-5 - total diff (hex first, bit second)
                std::cout << i + 1 << "# 25000 pairs of strings, " << tests[i] << " random symbols per string, only 1 differing symbol per pair..." << std::endl;
                AvalancheTest(&Hash, tests[i], diffs);
                std::cout << "Min hex difference: " << diffs[0] * 100 / 64 << "%" << std::endl;
                std::cout << "Max hex difference: " << diffs[2] * 100 / 64 << "%" << std::endl;
                std::cout << "Average hex difference: " << diffs[4] * 100 / 25000 / 64 << "%" << std::endl;
                std::cout << "Min bit difference: " << diffs[1] * 100 / 256 << "%" << std::endl;
                std::cout << "Max bit difference: " << diffs[3] * 100 / 256 << "%" << std::endl;
                std::cout << "Average bit difference: " << diffs[5] * 100 / 25000 / 256 << "%" << std::endl;
                std::cout << std::endl;
            }
            return 0;
        }
        else // Text file name
        {
            input = ReadFileToString(argv[1]);
        }
    }
    else if (argc == 3) // Text file name and amount of lines to read
    {
        input = ReadFileToString(argv[1], std::stoi(argv[2]));
    }
    else // No command line arguments
    {
        std::cout << "Enter your input:" << std::endl;
        std::getline(std::cin, input);
    }

    auto start = std::chrono::high_resolution_clock::now();

    output = Hash(input);

    auto stop = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start);

    std::cout << output << std::endl;
    std::cout << "Duration: " << duration.count() << " microseconds" << std::endl;

    return 0;
}