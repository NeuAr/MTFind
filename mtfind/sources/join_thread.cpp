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
		// ���������, �������� �� ����� ������������
		// ���� ����� �������� ������������, ������ ��� ��������������, ������� ����� join, 
		// �� ���� ���������� ���������� ������
		if (thread_.joinable()) 
		{
			thread_.join();
		}
	}

	thread& JoinThread::get() noexcept
	{
		// ���������� ������ �� ������ ������
		return thread_;
	}
}