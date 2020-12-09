#include <join_thread.h>

namespace mtfind 
{
	using namespace std;

	JoinThread::JoinThread(thread&& thread) noexcept
		: thread_{ move(thread) }
	{

	}

	JoinThread::~JoinThread() 
	{
		// Проверяем, является ли поток подключаемым
		// Если поток является подключаемым, делаем его неподключаемым, вызывая метод join, 
		// то есть дожидаемся завершения потока
		if (thread_.joinable()) 
		{
			thread_.join();
		}
	}

	thread& JoinThread::get() noexcept
	{
		// Возвращаем ссылку на объект потока
		return thread_;
	}
}