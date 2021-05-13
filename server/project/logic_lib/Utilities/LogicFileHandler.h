//
// Created by aleksandr on 15.04.2021.
//

#ifndef FULL_HOUSE_FILEHANDLER_H
#define FULL_HOUSE_FILEHANDLER_H

#include <fstream>
#include <string>
#include <sstream>
#include <iostream>





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

#endif //FULL_HOUSE_FILEHANDLER_H
