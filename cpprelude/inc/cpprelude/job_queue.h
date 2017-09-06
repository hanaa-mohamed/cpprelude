#pragma once

#include <cpprelude/threading.h>
#include <cpprelude/queue_array.h>
#include <cpprelude/dynamic_array.h>
#include <functional>
#include <unordered_map>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>

namespace cpprelude
{
	struct _job
	{
		
		bool finishid;
		u64 id;

		virtual void execute()=0 ;

		_job() 
		{
			id = -1;
			finishid = false;
		}

		_job(u64 id)
		{
			this->id = id;
			finishid = false;
		}

		virtual ~_job() {}

	};

	template <typename F>
	struct _job_logic;

	template <typename R, typename ... Args>
	struct _job_logic<R(Args...)>:_job
	{
	
		std::function <R()> precedure;
		R	result;
		std::promise <R> promise_obj;

		_job_logic ()
		{
		
		}

		template <class F>
		_job_logic(F&& func, Args&&... args,u64 id):_job(id)
		{
			precedure = std::bind(tmp::forward<F>(func), tmp::forward<Args>(args)...);
		}

		virtual void 
		execute()
		{
			result = precedure();
			promise_obj.set_value(result);
			finishid = true;
		}

	};

	struct _worker
	{
		std::thread thread_obj;
		u64 id, job_id;
		bool finished;

		_worker()
		{
			finished = false;

		}
		template <typename... ArgsT>
		_worker(u64 id, u64 job_id, ArgsT&&... args) :thread_obj([&]()->
		void
		{
		auto tmp_bind =	std::bind(args...);
		tmp_bind();
		finished = true;
		}
		)
		{
			finished = false;
			this->id = id;
			this->job_id = job_id;
		}

		bool
		join_worker()
		{
			if (thread_obj.joinable()) 
			{
				thread_obj.join();
				return true;
			}
			return false;
		}
		
	};

	struct job_queue
	{
		u64 _job_cnt;
		u64 _workers_cnt;
		i16 _cores_number;
		std::mutex stls_mutex;
		std::condition_variable	busy_waiting_solver;
		queue_array <_job *> _undone_jobs;
		std::unordered_map <u64 , _job *> _done_jobs;
		dynamic_array <_worker> workers;

		job_queue()
		{
			_job_cnt = 0;
			_workers_cnt=0;
			_cores_number = std::thread::hardware_concurrency();

		}

		void
		main()
		{
			std::thread _main_thread(&job_queue::_executor, this);
			_main_thread.detach();
			
		}

		void _executor()
		{
			while (true)
			{
				std::unique_lock <std::mutex> locker(stls_mutex);
				busy_waiting_solver.notify_one();
				busy_waiting_solver.wait(locker, [&]() {return !_undone_jobs.empty(); });
				workers.insert_back(_worker(_workers_cnt++, _undone_jobs.front()->id,&_job::execute,_undone_jobs.front() ));
				workers.back()->join_worker();
				_done_jobs[_undone_jobs.front()->id] = std::move(_undone_jobs.front());
				_undone_jobs.dequeue();
				locker.unlock();
			}
		}

		template<typename R,typename F, typename... ArgsT>
		u64
		schedule(F&& func, ArgsT&&... args)
		{
			std::lock_guard<std::mutex> locker(stls_mutex);
			_undone_jobs.enqueue(new _job_logic<R(ArgsT...)>(func, std::forward<ArgsT>(args)..., _job_cnt) );
		
			return cnt++;

		};

		bool
		is_done(u64 id)
		{
			std::unique_lock<std::mutex> locker(stls_mutex);
			std::unordered_map<u64, _job *>::const_iterator it = _done_jobs.find(id);
			locker.unlock();
			if(it != _done_jobs.end())
			{
				if (it->second->finishid == true)
					return true;
			}

			return false;
		}

		template <typename R>
		R
		wait_result(u64 id)
		{
			std::future<R> return_getter = static_cast<_job_logic<R> *>(_done_jobs[id])->promise_obj.get_future();
			//if (is_done(id)) 
			//{
				

			//}
			/*else
			{
				queue_array<_job *>::const_iterator it;
				return_getter=
			}*/
			return return_getter.get();
		}

		~job_queue()
		{
			std::lock_guard<std::mutex> locker(stls_mutex);
			for (std::unordered_map<u64, _job *>::iterator it = _done_jobs.begin(); it != _done_jobs.end(); ++it)
			{
				delete it->second;
			}

			while (!_undone_jobs.empty())
			{
				_job* remaining_job = _undone_jobs.front();
				_undone_jobs.dequeue();
				delete remaining_job;
			}

		}

	};

}
