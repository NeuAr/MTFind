#pragma once
#include <thread>

namespace mtfind
{
	// Класс потока, вызывающего join при разрушении объекта класса
	class JoinThread
	{
		public:
			explicit JoinThread(std::thread&& thread) noexcept;
			JoinThread(JoinThread&& joinThread) noexcept = default;

			~JoinThread();

			JoinThread& operator=(JoinThread&& joinThread) noexcept = default;

			// Возвращает ссылку на объект потока
			std::thread& get() noexcept;
		private:
			// Поток
			std::thread thread_;
	};
}