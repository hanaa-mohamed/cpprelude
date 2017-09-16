// #include <iostream>
// #include <thread>
// #include <cpprelude/queue_array.h>
// #include <cpprelude/threading.h>
// using namespace cpprelude;
//
// void producer(thread_unique<queue_array<i32>>* production_queue, i32 limit)
// {
// 	thread_context context = production_queue->new_context();
// 	while(limit--)
// 	{
// 		if(production_queue->write(context))
// 		{
// 			production_queue->value.enqueue(limit);
// 			production_queue->write_release(context);
// 		}
// 	}
// }
//
// void consumer(thread_unique<queue_array<i32>>* production_queue, i32 limit)
// {
// 	thread_context context = production_queue->new_context();
// 	while(limit)
// 	{
// 		if(production_queue->write(context))
// 		{
// 			if(!production_queue->value.empty())
// 			{
// 				std::cout << "consumed: " << production_queue->value.front() << std::endl;
// 				production_queue->value.dequeue();
// 				limit--;
// 			}
// 			else
// 			{
// 				std::cout << "nothing!" << std::endl;
// 			}
//
// 			production_queue->write_release(context);
// 		}
// 	}
// }
//
// int
// main(int argc, char** argv)
// {
// 	thread_unique<queue_array<i32>> production_queue;
//
// 	std::thread p1(producer, &production_queue, 1000);
// 	std::thread p2(producer, &production_queue, 1000);
//
// 	std::thread c1(consumer, &production_queue, 500);
// 	std::thread c2(consumer, &production_queue, 1500);
//
// 	p1.join();
// 	p2.join();
// 	c1.join();
// 	c2.join();
//
// 	std::cout << (production_queue.value.empty() ? "queue is empty" : "oops! not empty queue") << std::endl;
// 	return 0;
// }
