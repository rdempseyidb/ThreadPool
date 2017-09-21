/*
 *  Copyright (C) 2017  Bob Dempsey
 *
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */
#include <queue>
#include <vector>
#include <stdexcept>

#include <boost/thread.hpp>

namespace ThreadPool
{

class ThreadPoolJob
{
public:
    ThreadPoolJob() { }
    ~ThreadPoolJob() { }

    //virtual void run(void* data) { throw std::runtime_error("Not implemented."); }
    virtual void run()=0;

private:
};

class ThreadPool
{
public:
    ThreadPool(int workers=0);
    ~ThreadPool();

    void submit(ThreadPoolJob* job);
    void start();
    void stop();
    int jobsWaiting();

    ThreadPoolJob* next();

private:
    ThreadPool(const ThreadPool& rhs);
    ThreadPool& operator=(const ThreadPool& rhs);

    boost::mutex jobPtrQueue_lk;
    boost::condition_variable jobPtrQueue_cond;
    std::queue<ThreadPoolJob*> jobPtrQueue;
    boost::thread_group* runners;
    int num_workers;
    volatile bool shuttingDown;
};

}
