#include "tools.h"

std::experimental::filesystem::path glob_test_dir;

int main(int argc, char** argv) {
    testing::InitGoogleTest(&argc, argv);

    if (argc < 1) {
        std::cerr << "Usage: " << argv[0] << "[gtest params] <path_to_test_dir>" << std::endl;
        return -1;
    }

    return RUN_ALL_TESTS();
}
