#pragma once


#include <vector>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>

//������ �ߴµ� �ƹ������� �Ⱦ��� ���� ȿ���� �̹��ؼ�
class CThreadPool {
public:
    CThreadPool(size_t threadCount = std::thread::hardware_concurrency());
    ~CThreadPool();

    void Enqueue(std::function<void()> job);
    void WaitAll();

private:
    void Worker();

private:
    std::vector<std::thread> m_Threads;
    std::queue<std::function<void()>> m_Jobs;
    std::mutex m_QueueMutex;
    std::condition_variable m_Condition;
    std::condition_variable m_DoneCondition;
    std::atomic<int> m_ActiveThreads{ 0 };
    bool m_bStop = false;
};
