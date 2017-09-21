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

// Your thread job will inherit from this base class
// Your inherited class must have good destructor
//   behavior because the ThreadPool class will call it.
class ThreadPoolJob
{
public:
    ThreadPoolJob() { }
    virtual ~ThreadPoolJob() { }

    //The next job will not run until/unless run() returns
    virtual void run()=0;

private:
};

class ThreadPool
{
public:
    //If you do not specify the number of workers it will
    //  default to the number of cores
    ThreadPool(int workers=0);
    ~ThreadPool();

    //Starts the workers (and starts any queued jobs)
    void start();
    //Queues a job. This class takes ownership of the pointer
    //  resources. In particular, this class will delete the
    //  job object when job->run() completes.
    void submit(ThreadPoolJob* job);
    //Waits for current running job(s) to complete, stops the worker
    //  threads, and drains the job queue.
    void stop();
    //The number of jobs waiting to run
    int jobsWaiting();

protected:
    friend class ThreadPoolWorker;
    //pulls the next job out of the job queue
    ThreadPoolJob* next();

private:
    //This class is non-copyable
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
