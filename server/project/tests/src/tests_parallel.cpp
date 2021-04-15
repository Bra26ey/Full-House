#include "tools.h"

extern "C" {
    #include "merge_sort.h"
}

TEST(ParallelSort, Ordinary) {
    for (auto &entry : std::experimental::filesystem::directory_iterator(glob_test_dir / "ordinary")) {
        std::cout << entry.path() << std::endl;

        auto in = entry.path() / "in.txt";
        FILE *file = fopen(in.c_str(), "r");
        size_t size;
        fscanf(file, "%lu", &size);
        auto real = create_array(size);
        read_array(real, file);
        fclose(file);

        auto out = entry.path() / "out.txt";
        file = fopen(out.c_str(), "r");
        auto expected = create_array(size);
        read_array(expected, file);
        fclose(file);

        ASSERT_EQ(sort(real), 0);
        ASSERT_EQ(is_equal(real, expected), true);

        free_array(real);
        free_array(expected);
    }
}

TEST(ParallelSort, Stress) {
    for (auto &entry : std::experimental::filesystem::directory_iterator(glob_test_dir / "stress")) {
        std::cout << entry.path() << std::endl;

        auto in = entry.path() / "in.txt";
        FILE *file = fopen(in.c_str(), "r");
        size_t size;
        fscanf(file, "%lu", &size);
        auto real = create_array(size);
        read_array(real, file);
        fclose(file);

        auto out = entry.path() / "out.txt";
        file = fopen(out.c_str(), "r");
        auto expected = create_array(size);
        read_array(expected, file);
        fclose(file);

        ASSERT_EQ(sort(real), 0);
        ASSERT_EQ(is_equal(real, expected), true);

        free_array(real);
        free_array(expected);
    }
}

TEST(ParallelSort, Broken) {
    for (auto &entry : std::experimental::filesystem::directory_iterator(glob_test_dir / "broken")) {
        std::cout << entry.path() << std::endl;

        auto in = entry.path() / "in.txt";
        FILE *file = fopen(in.c_str(), "r");
        size_t size;
        fscanf(file, "%lu", &size);
        auto array = create_array(size);
        ASSERT_EQ((read_array(array, stdin) != 0), true);
        ASSERT_EQ((sort(array) != 0), true);

        fclose(file);
        free_array(array);
    }
    ASSERT_EQ((read_array(NULL, stdin) != 0), true);
    ASSERT_EQ((sort(NULL) != 0), true);
}
