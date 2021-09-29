//
// Created by aleksandr on 15.04.2021.
//
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>

#include "LogicFileHandler.h"

namespace logic {

FileHandler::FileHandler(const std::string& input, const std::string& output): input_file(input), output_file(output) {

}

void FileHandler::Read() {
    std::string line;
    if (input_file.is_open()) {
        while (getline(input_file, line)) {
            buffer << line << '\n';
        }
    }
}


void FileHandler::Write() {
    if (output_file.is_open()) {
        output_file << buffer.str();
    }
}

FileHandler::~FileHandler() {
    if (input_file.is_open()) {
        input_file.close();
    }
    if (output_file.is_open()) {
        output_file.close();
    }
}

}  // namespace logic