//
// Created by aleksandr on 15.04.2021.
//

#pragma once

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

namespace logic {

class FileHandler {
public:
    explicit FileHandler(const std::string& input, const std::string& output = "");

    void Read();
    void Write();


    ~FileHandler();

    std::stringstream buffer;

private:
    std::ifstream input_file;
    std::ofstream output_file;
};

}  // namespace logic
