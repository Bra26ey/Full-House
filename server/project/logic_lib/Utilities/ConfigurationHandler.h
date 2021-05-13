//
// Created by aleksandr on 17.04.2021.
//

#ifndef FULL_HOUSE_CONFIGURATIONHANDLER_H
#define FULL_HOUSE_CONFIGURATIONHANDLER_H

#include "LogicFileHandler.h"
#include "HandConfiguration.h"

class ConfigurationHandler {
public:
    ConfigurationHandler(FileHandler& file_handler);
    void HandConfigurationInit(HandConfiguration& hand_config);

    void Read();
    void Write();
private:
    FileHandler& file_handler;
};

#endif //FULL_HOUSE_CONFIGURATIONHANDLER_H
