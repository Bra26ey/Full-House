#include "gtest/gtest.h"
#include "../Connection/Connection.h"
#include "../Connection/PoolConnections.h"


PoolConnections* PoolConnections::instance_{nullptr};
std::mutex PoolConnections::my_mutex_;


TEST(Singleton, get_instance) {
    PoolConnections* instance = nullptr;
    instance = PoolConnections::GetInstance();
    ASSERT_FALSE(instance == nullptr);
}

TEST(Singleton, get_connection__access_denied) {
    PoolConnections* instance = PoolConnections::GetInstance();
    ASSERT_EQ(instance->GetConnection(), nullptr);
}

TEST(Singleton, get_connection__ok) {
    PoolConnections* instance = PoolConnections::GetInstance();
    instance->SetParams("localhost", "****", "****", "poker_database");
    Connection* conn = instance->GetConnection();
    ASSERT_FALSE(conn == nullptr);
    conn->Close();
}

void Thread(Connection*& conn) {
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    PoolConnections* instance = PoolConnections::GetInstance();
    instance->SetParams("localhost", "****", "****", "poker_database");
    conn = instance->GetConnection();
}

TEST(Singleton, get_multiconnections) {
    Connection* conn1 = nullptr;
    Connection* conn2 = nullptr;

    std::thread t1(Thread, std::ref(conn1));
    std::thread t2(Thread, std::ref(conn2));

    t1.join();
    t2.join();

    ASSERT_FALSE(conn1 == nullptr);
    ASSERT_FALSE(conn2 == nullptr);
    ASSERT_FALSE(conn1 == conn2);

    conn1->Close();
    conn2->Close();
}
