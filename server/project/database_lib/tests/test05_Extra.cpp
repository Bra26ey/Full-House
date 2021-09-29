#include <gtest/gtest.h>
#include <../Connection/SafetyConnection.h>
#include <../Models/Board.h>
#include <../Models/User.h>


std::vector<std::size_t> USER_IDS;


// simple function for testing asynchronous task
void AsyncUserTask(User& usr, const std::string& task, std::size_t user_id, const std::string& login = "") {
    if (task == "insert") {
        auto out = usr.RegUser(login, login + "_password");
        ASSERT_EQ(out.second, OK);
        if (out.second == OK) {
            USER_IDS[user_id] = out.first;
        }
    }
    else if (task == "delete") {
        ASSERT_EQ(usr.DeleteUser(user_id), OK);
    }
    else if (task == "update") {
        ASSERT_EQ(usr.UpdateMoney(user_id, 10.0), OK);
    }
}

TEST(UserAsync, async__ok) {
    USER_IDS.resize(10);
    User user;
    std::vector<std::future<void>> tasks;
    std::string login = "login";
    // insert 10 users
    for (std::size_t i = 0; i < 10; ++i) {
        tasks.push_back(std::async(AsyncUserTask, std::ref(user), "insert",
                                   i, login + std::to_string(i)));
    }
    // wait all tasks
    for (auto & task : tasks) {
        task.wait();
    }
}

TEST(UserAsync, update__ok) {
    User user;
    std::vector<std::future<void>> tasks;
    std::string login = "login";
    ASSERT_EQ(user.GetUser(USER_IDS[0], false).money, 0);
    for (std::size_t i = 0; i < 10; ++i) {
        tasks.push_back(std::async(AsyncUserTask, std::ref(user), "update",
                                   USER_IDS[i], login + std::to_string(i)));
    }
    // wait all tasks
    for (auto & task : tasks) {
        task.wait();
    }
}

TEST(UserAsync, delete__ok) {
    User user;
    std::vector<std::future<void>> tasks;
    std::string login = "login";
    ASSERT_EQ(user.GetUser(USER_IDS[0], false).money, 10);
    // delete users
    for (std::size_t i = 0; i < 10; ++i) {
        tasks.push_back(std::async(AsyncUserTask, std::ref(user), "delete",
                                   USER_IDS[i], login + std::to_string(i)));
    }
    // wait all tasks
    for (auto & task : tasks) {
        task.wait();
    }
}
