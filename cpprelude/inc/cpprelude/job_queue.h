#pragma once

#include <functional>
#include <cpprelude/threading.h>
#include <cpprelude/queue_array.h>
#include <unordered_map>

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
	public:
		std::function <R()> precedure;
		R	result;
		

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
			finishid = true;
		}

	};

	struct _worker
	{
		std::thread thread_obj;
		u64 id,job_id;
		bool finished;
	};

	struct job_queue
	{
		u64 _job_cnt=0;
		queue_array <_job *> _undone_jobs;
		std::unordered_map <u64 , _job *> _done_jobs;

		void
		main()
		{
			while (true)
			{
				if (!_undone_jobs.empty())
				{
					_undone_jobs.front()->execute();
					_done_jobs[_undone_jobs.front()->id] =std::move(_undone_jobs.front());
					_undone_jobs.dequeue();
				}



			}

		}
		
		template<typename R,typename F, typename... ArgsT>
		u64
		schedule(F&& func, ArgsT&&... args)
		{
			_undone_jobs.enqueue(new _job_logic<R(ArgsT...)>(func, std::forward<ArgsT>(args)..., _job_cnt) );
		
			return cnt++;

		};

		bool
		is_done(u64 id)
		{
			std::unordered_map<u64, _job *>::const_iterator it = _done_jobs.find(id);
			return it != _done_jobs.end();
		}



	};

	

}
