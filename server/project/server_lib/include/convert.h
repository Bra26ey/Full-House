#pragma once

#include "HandConfiguration.h"
#include "Board.h"

logic::HandConfiguration convert(database::active_board const &board);
database::hand_configuration convert(logic::HandConfiguration const &hand_config);