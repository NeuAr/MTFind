#pragma once
#include <thread>

namespace mtfind
{
	// ����� ������, ����������� join ��� ���������� ������� ������
	class JoinThread
	{
		public:
			explicit JoinThread(std::thread&& thread) noexcept;
			JoinThread(JoinThread&& joinThread) noexcept = default;

			~JoinThread();

			JoinThread& operator=(JoinThread&& joinThread) noexcept = default;

			// ���������� ������ �� ������ ������
			std::thread& get() noexcept;
		private:
			// �����
			std::thread thread_;
	};
}