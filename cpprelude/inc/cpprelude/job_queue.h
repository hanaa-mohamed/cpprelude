#pragma once

#include <functional>
#include <cpprelude/threading.h>
#include <cpprelude/queue_array.h>

namespace cpprelude
{
	

	struct job_queue
	{
		isize cnt=0;
		queue_array <std::pair<int,std::function<void()> > > jq;

		void
			main()
		{
			while (true)
			{
				if (!jq.empty())
				{
					jq.front().second();
					jq.dequeue();
				}



			}

		}

		template<typename R,typename... ArgsT>
		isize
			schedule(job_queue& check, R& fn, ArgsT&... args)
		{
			jq.enqueue(std::make_pair(cnt,std::bind(fn, tmp::forward<ArgsT>(args)...) ));
			return cnt++;

		};

		template<typename R, typename... ArgsT>
		isize
			schedule(job_queue& check, R&& fn, ArgsT&&... args)
		{
			jq.enqueue(std::make_pair(cnt,std::bind(fn, tmp::forward<ArgsT>(args)...)));
			return cnt++;

		};

		bool
		is_done(isize id)
		{
			if (jq.front().first > id)
				return true;
			return false;
		}



	};

	

}
