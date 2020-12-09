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
	// Класс результата поиска
	class SearchResult 
	{
		public:
			// Позиция в строке искомого фрагмента
			size_t position;
			// Текст искомого фрагмента
			std::string text;

			SearchResult() = default;
			SearchResult(size_t position, std::string&& text) noexcept;
	};

	// Класс результата поиска в строке
	class LineSearchResult 
	{
		public:
			// Номер строки
			size_t lineNumber;
			// Результаты поиска в строке
			std::vector<SearchResult> results;

			LineSearchResult() = default;
			LineSearchResult(size_t lineNumber, std::vector<SearchResult>&& results);
	};

	// Класс результата поиска в файле
	class FileSearchResult 
	{
		public:
			// Количество найденных результатов в файле
			size_t size = 0;
			// Результаты поиска в файле
			std::vector<std::deque<LineSearchResult>> results;
	};

	// Класс провайдера для поиска в файле
	class FileSearchProvider 
	{
		public:
			FileSearchProvider(std::experimental::filesystem::path&& filePath, uintmax_t maxFileSize);
			FileSearchProvider(const FileSearchProvider& fileSearchProvider) = delete;
			FileSearchProvider(FileSearchProvider&& fileSearchProvider) = delete;

			FileSearchProvider& operator=(const FileSearchProvider& fileSearchProvider) = delete;
			FileSearchProvider& operator=(FileSearchProvider&& fileSearchProvider) = delete;
			
			// Возвращает ошибки последнего поиска в файле
			const std::vector<std::string>& lastSearchErrors() const noexcept;
			// Возвращает минимальное количество потоков для поиска в файле строк, соответствующих указанной маске
			int minThreadsCount() const noexcept;
			// Возвращает максимальное количество потоков для поиска в файле строк, соответствующих указанной маске
			int maxThreadsCount() const noexcept;
			// Возвращает максимальную длину маски для поиска строк в файле
			size_t maxMaskSize() const noexcept;
			// Задаёт максимальную длину маски для поиска строк в файле
			void setMaxMaskSize(size_t value);
			// Выполняет поиск строк, соответствующих указанной маске, в файле и возвращает результат поиска в файле
			// Поиск выполняется при помощи указанного количества потоков
			FileSearchResult search(const std::string& mask, int threadsCount);
		private:
			// Минимальное количество потоков для поиска в файле строк, соответствующих указанной маске
			const int MinThreadsCount = 1;
			// Максимальное количество потоков для поиска в файле строк, соответствующих указанной маске
			const int MaxThreadsCount = 16;

			// Путь до файла
			std::experimental::filesystem::path filePath_;
			// Максимальная длина маски для поиска строк в файле
			size_t maxMaskSize_ = 10;
			// Ошибки последнего поиска в файле
			std::vector<std::string> lastSearchErrors_;
			// Мьютекс, для блокировки доступа к результату поиска в файле текущим потоком
			std::mutex lockerResultMutex_;
			// Мьютекс, для блокировки доступа к ошибкам последнего поиска в файле текущим потоком
			std::mutex lockerErrorsMutex_;

			// Добавляет ошибку поиска в файле и возвращает логическое значение, была ли добавлена ошибка
			bool addSearchError(const std::string& errorText) noexcept;
			// Очищает ошибки последнего поиска в файле
			void clearLastSearchErrors();
			// Выполняет поиск строк, соответствующих указанной маске, в указанном количестве строк файла, начиная со строки файла с указанной позицией в файле
			// Результат поиска будет сохранён в указанную переменную результата поиска в файле
			void searchStrings(const std::string& mask, FileSearchResult& fileSearchResult, int threadNumber, intmax_t startPosition, size_t linesCount) noexcept;
	};
}