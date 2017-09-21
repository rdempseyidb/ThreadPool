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

class ThreadPool;

class ThreadPoolWorker
{
public:
    ThreadPoolWorker(ThreadPool* p) : pool(p) { }
    ~ThreadPoolWorker() { }

    void operator()();

private:
    ThreadPool* pool;
};

}
