#include "ThreadPool.h"

CThreadPool::CThreadPool(size_t threadCount) {
    m_bStop = false;
    for (size_t i = 0; i < threadCount; ++i)
        m_Threads.emplace_back([this]() { this->Worker(); });
}

CThreadPool::~CThreadPool() {
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_bStop = true;
    }
    m_Condition.notify_all();
    for (std::thread& t : m_Threads)
        t.join();
}

void CThreadPool::Enqueue(std::function<void()> job) {
    {
        std::unique_lock<std::mutex> lock(m_QueueMutex);
        m_Jobs.push(std::move(job));
    }
    m_Condition.notify_one();
}

void CThreadPool::WaitAll() {
    std::unique_lock<std::mutex> lock(m_QueueMutex);
    m_DoneCondition.wait(lock, [this]() {
        return m_Jobs.empty() && (m_ActiveThreads == 0);
        });
}

void CThreadPool::Worker() {
    while (true) {
        std::function<void()> job;
        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            m_Condition.wait(lock, [this]() {
                return m_bStop || !m_Jobs.empty();
                });

            if (m_bStop && m_Jobs.empty())
                return;

            job = std::move(m_Jobs.front());
            m_Jobs.pop();
            ++m_ActiveThreads;
        }

        job();

        {
            std::unique_lock<std::mutex> lock(m_QueueMutex);
            --m_ActiveThreads;
            if (m_Jobs.empty() && m_ActiveThreads == 0)
                m_DoneCondition.notify_all();
        }
    }
}
