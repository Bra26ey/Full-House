//
// Created by aleksandr on 17.04.2021.
//

#pragma once

#include "LogicFileHandler.h"
#include "HandConfiguration.h"

namespace logic {

class ConfigurationHandler {
public:
    ConfigurationHandler(FileHandler& file_handler);
    void HandConfigurationInit(HandConfiguration& hand_config);

    void Read();
    void Write();
private:
    FileHandler& file_handler;
};

}  // namespace logic
