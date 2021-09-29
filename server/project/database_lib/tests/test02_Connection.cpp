#include "gtest/gtest.h"
#include "../Connection/Connection.h"
#include "../Connection/PoolConnections.h"
#include "../Connection/SafetyConnection.h"


std::unique_ptr<PoolConnections> PoolConnections::instance_{nullptr};
std::mutex PoolConnections::my_mutex_;


TEST(Singleton, get_instance) {
    PoolConnections* instance = nullptr;
    instance = PoolConnections::GetInstance();
    ASSERT_FALSE(instance == nullptr);
}

TEST(Singleton, get_connection__ok) {
    SafetyConnection conn;
    ASSERT_EQ(conn.IsConnected(), true);
}

TEST(SafetyConnection, many_conn) {
    PoolConnections* instance = PoolConnections::GetInstance();
    instance->SetParams("config.txt");
    SafetyConnection conn1;
    SafetyConnection conn2;
    SafetyConnection conn3;

    ASSERT_EQ(conn1.IsConnected(), true);
    ASSERT_EQ(conn2.IsConnected(), true);
    ASSERT_EQ(conn3.IsConnected(), true);

    conn1.ReturnConnection();
    conn3.ReturnConnection();

    ASSERT_EQ(conn1.IsConnected(), false);
    ASSERT_EQ(conn2.IsConnected(), true);
    ASSERT_EQ(conn3.IsConnected(), false);
}

void Thread(SafetyConnection& sf_conn) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    std::string query_string = "SELECT 1;";
    sql::Statement *stmt = sf_conn.GetConnection().SetQuery(query_string);
    sql::ResultSet *res = stmt->executeQuery(query_string);
    delete stmt;
    delete res;
}

TEST(SafetyConnection, get_multiconnections) {
    SafetyConnection conn1;
    SafetyConnection conn2;

    std::thread t1(Thread, std::ref(conn1));
    std::thread t2(Thread, std::ref(conn2));

    t1.join();
    t2.join();

    ASSERT_FALSE(!conn1.IsConnected());
    ASSERT_FALSE(!conn1.IsConnected());
}

void Task(const int& num_task, SafetyConnection& sf_conn) {
    sf_conn.Lock();

    sleep(num_task);
    std::string query_string = "SELECT 1;";
    sql::Statement *stmt = sf_conn.GetConnection().SetQuery(query_string);
    sql::ResultSet *res = stmt->executeQuery(query_string);
    delete stmt;
    delete res;

    sf_conn.Unlock();
}

TEST(Connection, get_async) {
    PoolConnections* instance = PoolConnections::GetInstance();
    instance->SetParams("config.txt");
    SafetyConnection conn1;
    SafetyConnection conn2;
    SafetyConnection conn3;

    auto a1 = std::async(Task, 1, std::ref(conn1));
    auto a2 = std::async(Task, 2, std::ref(conn2));
    auto a3 = std::async(Task, 1, std::ref(conn3));
}
