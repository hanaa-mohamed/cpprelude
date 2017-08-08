#pragma once

#include <functional>
#include <cpprelude/threading.h>
#include <cpprelude/queue_array.h>

namespace cpprelude
{
	

	struct job_queue
	{
		queue_array <std::function<void()> > jq;
		void
			main()
		{
			while (true)
			{
				if (!jq.empty())
				{
					(jq.front())();
					jq.dequeue();
				}



			}

		}

		template<typename R,typename... ArgsT>
		isize
			schedule(job_queue& check, R& fn, ArgsT&... args)
		{
			jq.enqueue(std::bind(fn, forward<ArgsT>(args)...));
			return 0;

		};

		template<typename R, typename... ArgsT>
		isize
			schedule(job_queue& check, R&& fn, ArgsT&&... args)
		{
			jq.enqueue(std::bind(fn, forward<ArgsT>(args)...));
			return 0;

		};


	};

	

}
