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
#include <iostream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/syscall.h>
#include <ctime>
//#define NDEBUG
#include <cassert>

#include "ThreadPool.h"

using namespace std;

namespace
{

class MyJob : public ThreadPool::ThreadPoolJob
{
public:
    MyJob(int d) : data(d) { }
    ~MyJob() { }

    void run()
    {
        for (int i = 0; i < 5; i++)
        {
            cout << "Thread " << data << " running..." << endl;
            sleep(1);
        }
        cout << "Thread " << data << " ending..." << endl;
    }

private:
    int data;
};

class MyJob2 : public ThreadPool::ThreadPoolJob
{
public:
    MyJob2(int d) : data(d) { }
    ~MyJob2() { }

    void run()
    {
        struct timespec ts = { 0, 500000000 };
        for (int i = 0; i < 10; i++)
        {
            cout << "Thread " << data << " running..." << endl;
            nanosleep(&ts, 0);
        }
        cout << "Thread " << data << " ending..." << endl;
    }

private:
    int data;
};

}

int main(int argc, char** argv)
{
    ThreadPool::ThreadPool tp(4);
    tp.start();

    ThreadPool::ThreadPoolJob* jobp = 0;

    jobp = new MyJob(1);
    tp.submit(jobp);
    jobp = new MyJob(2);
    tp.submit(jobp);
    jobp = new MyJob(3);
    tp.submit(jobp);
    jobp = new MyJob2(4);
    tp.submit(jobp);
    jobp = new MyJob2(5);
    tp.submit(jobp);
    assert(tp.jobsWaiting() >= 1);

    sleep(20);

    tp.stop();
    cout << "All threads stopped" << endl;

    return 0;
}

