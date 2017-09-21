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
#include <unistd.h>
#include <stdexcept>

#include <boost/thread.hpp>

#include "ThreadPool.h"
#include "ThreadPoolWorker.h"

using namespace std;
using namespace boost;

namespace ThreadPool
{

ThreadPool::ThreadPool(int workers) :
    num_workers(workers),
    shuttingDown(false),
    runners(0)
{
    if (0 == num_workers) num_workers = sysconf(_SC_NPROCESSORS_ONLN);
}

ThreadPool::~ThreadPool()
{
}

void ThreadPool::submit(ThreadPoolJob* job)
{
    unique_lock<mutex> lk(jobPtrQueue_lk);

    jobPtrQueue.push(job);
    jobPtrQueue_cond.notify_one();
}

int ThreadPool::jobsWaiting()
{
    unique_lock<mutex> lk(jobPtrQueue_lk);

    return jobPtrQueue.size();
}

void ThreadPool::start()
{
    if (runners != 0)
    {
        throw runtime_error("Trying to start a pool without stopping first.");
    }

    runners = new thread_group();

    for (int i = 0; i < num_workers; i++)
    {
        thread* thd = new thread(ThreadPoolWorker(this));
        runners->add_thread(thd);
    }
}

void ThreadPool::stop()
{
    shuttingDown = true;
    jobPtrQueue_cond.notify_all();

    if (runners)
    {
        runners->join_all();
        delete runners;
        runners = 0;
    }

    while (!jobPtrQueue.empty())
    {
        ThreadPoolJob* p = jobPtrQueue.front();
        jobPtrQueue.pop();
        delete p;
    }
}

ThreadPoolJob* ThreadPool::next()
{
    if (shuttingDown) return 0;

    unique_lock<mutex> lk(jobPtrQueue_lk);

    while (jobPtrQueue.empty())
    {
        jobPtrQueue_cond.wait(lk);
        if (shuttingDown) return 0;
    }

    ThreadPoolJob* p = jobPtrQueue.front();
    jobPtrQueue.pop();

    return (shuttingDown ? 0 : p);
}

}

