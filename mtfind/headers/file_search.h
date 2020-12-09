#pragma once
#include <cstddef>
#include <deque>
#include <experimental\filesystem>
#include <mutex>
#include <stdint.h>
#include <string>
#include <vector>

namespace mtfind 
{
	// ����� ���������� ������
	class SearchResult 
	{
		public:
			// ������� � ������ �������� ���������
			size_t position;
			// ����� �������� ���������
			std::string text;

			SearchResult() = default;
			SearchResult(size_t position, std::string&& text) noexcept;
	};

	// ����� ���������� ������ � ������
	class LineSearchResult 
	{
		public:
			// ����� ������
			size_t lineNumber;
			// ���������� ������ � ������
			std::vector<SearchResult> results;

			LineSearchResult() = default;
			LineSearchResult(size_t lineNumber, std::vector<SearchResult>&& results);
	};

	// ����� ���������� ������ � �����
	class FileSearchResult 
	{
		public:
			// ���������� ��������� ����������� � �����
			size_t size = 0;
			// ���������� ������ � �����
			std::vector<std::deque<LineSearchResult>> results;
	};

	// ����� ���������� ��� ������ � �����
	class FileSearchProvider 
	{
		public:
			FileSearchProvider(std::experimental::filesystem::path&& filePath, uintmax_t maxFileSize);
			FileSearchProvider(const FileSearchProvider& fileSearchProvider) = delete;
			FileSearchProvider(FileSearchProvider&& fileSearchProvider) = delete;

			FileSearchProvider& operator=(const FileSearchProvider& fileSearchProvider) = delete;
			FileSearchProvider& operator=(FileSearchProvider&& fileSearchProvider) = delete;
			
			// ���������� ������ ���������� ������ � �����
			const std::vector<std::string>& lastSearchErrors() const noexcept;
			// ���������� ����������� ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����
			int minThreadsCount() const noexcept;
			// ���������� ������������ ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����
			int maxThreadsCount() const noexcept;
			// ���������� ������������ ����� ����� ��� ������ ����� � �����
			size_t maxMaskSize() const noexcept;
			// ����� ������������ ����� ����� ��� ������ ����� � �����
			void setMaxMaskSize(size_t value);
			// ��������� ����� �����, ��������������� ��������� �����, � ����� � ���������� ��������� ������ � �����
			// ����� ����������� ��� ������ ���������� ���������� �������
			FileSearchResult search(const std::string& mask, int threadsCount);
		private:
			// ����������� ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����
			const int MinThreadsCount = 1;
			// ������������ ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����
			const int MaxThreadsCount = 16;

			// ���� �� �����
			std::experimental::filesystem::path filePath_;
			// ������������ ����� ����� ��� ������ ����� � �����
			size_t maxMaskSize_ = 10;
			// ������ ���������� ������ � �����
			std::vector<std::string> lastSearchErrors_;
			// �������, ��� ���������� ������� � ���������� ������ � ����� ������� �������
			std::mutex lockerResultMutex_;
			// �������, ��� ���������� ������� � ������� ���������� ������ � ����� ������� �������
			std::mutex lockerErrorsMutex_;

			// ��������� ������ ������ � ����� � ���������� ���������� ��������, ���� �� ��������� ������
			bool addSearchError(const std::string& errorText) noexcept;
			// ������� ������ ���������� ������ � �����
			void clearLastSearchErrors();
			// ��������� ����� �����, ��������������� ��������� �����, � ��������� ���������� ����� �����, ������� �� ������ ����� � ��������� �������� � �����
			// ��������� ������ ����� ������� � ��������� ���������� ���������� ������ � �����
			void searchStrings(const std::string& mask, FileSearchResult& fileSearchResult, int threadNumber, intmax_t startPosition, size_t linesCount) noexcept;
	};
}