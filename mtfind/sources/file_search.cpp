#include <cstddef>
#include <deque>
#include <exception>
#include <regex>
#include <stdint.h>
#include <string>
#include <thread>
#include <vector>
#include <file_reader.h>
#include <file_search.h>
#include <join_thread.h>

namespace mtfind 
{
	using namespace std;
	using namespace std::experimental::filesystem;

	// ���� ������� ����� �����, ��������������� ��������� �����, � ��������� ������ � ���������� ������ ��������� �������
	vector<size_t> findPositions(const char* line, const char* mask)
	{
		// ������ ��������� ������� ����� �����, ��������������� ���������� �����, � ���������� ������
		vector<size_t> results;
		// ������� ������ ���������� �������� � ���������� ������ � ��������� � ���������� �����
		size_t matchStartPosition = 0;
		// ���������� ���������� �������� � ���������� ������ � ��������� � ���������� �����
		size_t matchCount = 0;
		
		// ���������, �������� �� ���������� ��������� �� ������ � �� ����� �������� �����������
		if (line != nullptr && mask != nullptr)
		{
			// ���� ���������� ��������� �� ������ � �� ����� �� �������� �������� �����������
			// ��������� �� �������� ���������� ������, ���� �� ����� ��������� ����� ������
			while (*line != '\0')
			{
				// ���������, ��������� �� ������� ������ � ���������� ������ � ������� �������� � ���������� �����
				// ���� ������� ������ � ���������� ����� (?), �� ���������� ����� ������, ������� ���������� ������������� ��� ��������� ��������
				if (*line == *mask || *mask == '?')
				{
					// ���� ������� ������ � ���������� ������ ��������� � ������� �������� � ���������� �����
					// ��������� � ���������� ������� ���������� ����� 
					// � ����������� ���������� ���������� �������� � ���������� ������ � ��������� � ���������� �����
					++mask;
					++matchCount;
				}
				else
				{
					// ���� ������� ������ � ���������� ������ �� ��������� � ������� �������� � ���������� �����
					// ����������� ������� ������ ���������� �������� � ���������� ������ � ��������� � ���������� �����
					++matchStartPosition;

					// ���� �� ����� ���� ���������� �������� � ���������� ������ � ��������� � ���������� �����
					// ������������ �� ���������� �������� ���������� ����� � � ���������� ������, � � ���������� �����, ���-�� ������ ����� �� �����
					// ���������� ���������� ���������� �������� � ���������� ������ � ��������� � ���������� �����
					if (matchCount > 0)
					{
						line -= matchCount;
						mask -= matchCount;
						matchCount = 0;
					}
				}

				// ���������, ��� �� ��������� ����� ������ ���������� �����
				if (*mask == '\0')
				{
					// ���� ����� ������ ���������� ����� ��� ���������
					// �������, ��� ���� ������� ���������� ����� ���������� ������ � ���������� ������
					// ��������� ������� ������ ���������� �������� � ���������� ������ � ��������� � ���������� ����� � ������ ��������� �������
					// ��������� ������� ������ ���������� �������� � ���������� ������ � ��������� � ���������� ����� �� ���������� ���������� �������� �����, 
					// ����� ��� ��������������� �������� ������� ������
					// ��������� � ������� ������� ���������� �����, ����� ������ ����� ������
					// ���������� �������� � ���������� �����, � ������ ������, ����� ���������� ���������� �������� � ���������� ������ � ��������� � ���������� �����
					// ���������� ���������� ���������� �������� � ���������� ������ � ��������� � ���������� �����
					results.push_back(matchStartPosition);
					matchStartPosition += matchCount;
					mask -= matchCount;
					matchCount = 0;
				}

				// ��������� � ���������� ������� ���������� ������
				++line;
			}
		}
		
		// ���������� c����� ��������� ������� ����� �����, ��������������� ���������� �����, � ���������� ������
		return results;
	}

	// ���� ������, ��������������� ��������� �����, � ��������� ������ � ���������� ������ ��������� ����� � �� ������� � ��������� ������ 
	vector<SearchResult> findStrings(const string& line, const string& mask)
	{
		// ������ ��������� �����, ��������������� ���������� �����, � �� ������� � ���������� ������
		vector<SearchResult> results;

		// ���������, �������� �� ���������� ������ � ����� �������
		if (!line.empty() && !mask.empty())
		{
			// ���� ���������� ������ � ����� �� �������� �������
			// �������� ������ ������� ����� �����, ��������������� ���������� �����, � ���������� ������
			vector<size_t> stringsPositions = findPositions(line.c_str(), mask.c_str());

			// ���������, �������� �� ���������� ������ ������� ����� �����, ��������������� ���������� �����, � ���������� ������, ������
			if (!stringsPositions.empty())
			{
				// ���� ���������� ������ ������� ����� �����, ��������������� ���������� �����, � ���������� ������, �� �������� ������
				// �������� ����� ������ ���������� �����
				size_t maskSize = mask.size();

				// ���������� �� ����������� ������ ������� ����� �����, ��������������� ���������� �����, � ���������� ������
				for (size_t stringPosition : stringsPositions)
				{
					// �������� ����� ���������� ������ �� ������� ��������� ������� ������ ������ � ������, ��� ����� ������ ���������� �����
					// ��������� ���������� ����� ���������� ������ � � ������� � c����� ��������� �����
					results.emplace_back((stringPosition + 1), line.substr(stringPosition, maskSize));
				}
			}
		}
		
		// ���������� c����� ��������� �����, ��������������� ���������� �����, � �� ������� � ���������� ������
		return results;
	}

	/*
		- SearchResult
	*/

	SearchResult::SearchResult(size_t position, string&& text) noexcept
		: position{ position }, text{ move(text) }
	{

	}

	/*
		- LineSearchResult
	*/

	LineSearchResult::LineSearchResult(size_t lineNumber, vector<SearchResult>&& results)
		: lineNumber{ lineNumber }, results{ move(results) }
	{

	}

	/*
		- FileSearchProvider
	*/

	FileSearchProvider::FileSearchProvider(path&& filePath, uintmax_t maxFileSize)
		: filePath_{ move(filePath) }
	{
		// ���� ����� �� ����������� ���� �� ����������, ����������� ����������
		if (!exists(filePath_))
		{
			throw exception{ "����� �� ���������� ���� �� ����������" };
		}

		// ���� ������ ����� �� ����������� ����, ������ ����������� ����������� ����������� ������� �����, ����������� ����������
		if (file_size(filePath_) > maxFileSize)
		{
			throw exception{ "������ ���������� �����, ������ ����������� ����������� �������" };
		}
	}

	bool FileSearchProvider::addSearchError(const string& errorText) noexcept
	{
		try 
		{
			// ������������� ���������� ������� � ������� ���������� ������ � ����� ������� �������
			unique_lock<mutex> lockerErrors{ lockerErrorsMutex_ };
			// ��������� ������ � ���������� ������� � ����� ������ ������ ���������� ������ � �����
			lastSearchErrors_.push_back(errorText);
			// ���������� ���������� ��������, ��� ������ ������ � ����� ���� ���������
			return true;
		}
		catch (...) 
		{
			// � ������, ���� �������� ����������
			// ���������� ���������� ��������, ��� ������ ������ � ����� �� ���� ���������
			return false;
		}
	}

	void FileSearchProvider::clearLastSearchErrors()
	{
		// ������������� ���������� ������� � ������� ���������� ������ � ����� ������� �������
		unique_lock<mutex> lockerErrors{ lockerErrorsMutex_ };

		// ���������, ���� �� ������ ���������� ������ � �����
		// ���� ������ ���������� ������ � ����� ����, ������� ��
		if (!lastSearchErrors_.empty())
		{
			lastSearchErrors_.clear();
		}
	}
	
	void FileSearchProvider::searchStrings(const string& mask, FileSearchResult& fileSearchResult, int threadNumber, intmax_t startPosition, size_t linesCount) noexcept
	{
		try 
		{
			// ������ ������ ������ ��� ������ ���������� �����
			FileReader fileReader{ filePath_ };
			// ������������� ��������� ������� ��� ������ ����� � ���������� �������
			fileReader.setPosition(startPosition);
			// ���������� ������ �����, ��������������� ���������� �����, � ������� �����
			deque<LineSearchResult> linesSearchResults;
			// ���������� ��������� �����, ��������������� ���������� �����, � ������� �����
			size_t linesFoundCount = 0;
			// ����� ������, � ������� ����� ������ ������ ����� �� ���������� �����
			// ��� ��� ��� ������ ������ ���������� ���������� ����� �� ���������� ����� �� �������, 
			// � ������� �������� ����� �������������� � ����������� �� ������ ������, 
			// �� ����� ������, � ������� ����� ������ ������ ����� �� ���������� ����� �����, 
			// ���������� �����, �������� ������� ���������� �� ����� ������ � ���� 1
			size_t startLineNumber = (threadNumber * linesCount) + 1;

			// ������ ���������� ���������� ����� �� ���������� �����
			for (size_t readLines = 0; readLines < linesCount; ++readLines)
			{
				// ������� ������ �����
				string fileLine;

				// ������ ������ �� ���������� �����
				// ���� ������ �� ���������� ����� �� ���� ���������, �� ��� ��������� ����� ���������� �����
				// ��������� ������ ����� �� ���������� �����
				if (!fileReader.readLine(fileLine))
				{
					break;
				}

				// ���������, �������� �� ������� ������ ����� ������
				// ���� ������� ������ ����� �������� ������, ��������� � ��������� ������ �����
				if (fileLine.empty())
				{
					continue;
				}

				// �������� ������ ��������� �����, ��������������� ���������� �����, � �� ������� � ������� ������ �����
				vector<SearchResult> lineSearchResults = findStrings(fileLine, mask);

				// ���������, �������� �� ������ ��������� �����, ��������������� ���������� �����, � �� ������� � ������� ������ �����, ������
				// ���� ������ ��������� �����, ��������������� ���������� �����, � �� ������� � ������� ������ ����� �� �������� ������
				// ��������� ���������� ������ � ������� ������ ����� � ������ ����������� 
				// � ����������� ����� ���������� ��������� �����, ��������������� ���������� �����, � ������� �����
				if (!lineSearchResults.empty())
				{
					linesFoundCount += lineSearchResults.size();
					// ����� ������� ������ �������� ��� ����� ������ ������, � ������� ����� ����� ������ ����� �� ���������� ����� 
					// � ���������� ��� ����������� ����� �� ���������� �����
					linesSearchResults.emplace_back((startLineNumber + readLines), move(lineSearchResults));
				}
			}

			// ���������, ���� �� ������� ������, ��������������� ���������� �����, � ����������� ������� �����
			if (linesFoundCount > 0)
			{
				// ���� ������, ��������������� ���������� �����, ���� ������� � ����������� ������� �����
				// ������������� ���������� ������� � ���������� ������ � ����� ������� �������
				unique_lock<mutex> lockerResult{ lockerResultMutex_ };
				// ��������� ���������� ������ �����, ��������������� ���������� �����, � ����������� ������� ����� 
				// � ������� ����������� ���������� ������ � ����� � ������� �������� ������
				fileSearchResult.results.at(threadNumber) = move(linesSearchResults);
				// ����������� ���������� ��������� ����������� � ����� �� ���������� ��������� �����, ��������������� ���������� �����, � ����������� ������� �����
				fileSearchResult.size += linesFoundCount;
			}
		}
		catch (const exception& e)
		{
			// � ������, ���� �������� ����������
			// ��������� ������ ������ � �����
			addSearchError(e.what());
		}
		catch (...) 
		{
			// � ������, ���� �������� ����������
			// ��������� ������ ������ � �����
			addSearchError("����������� ������ ��� ������ �����, ��������������� ��������� �����, � ������� ���������� �����");
		}
	}

	const vector<string>& FileSearchProvider::lastSearchErrors() const noexcept
	{
		// ���������� ������ ���������� ������ � �����
		return lastSearchErrors_;
	}

	int FileSearchProvider::minThreadsCount() const noexcept
	{
		// ���������� ����������� ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����
		return MinThreadsCount;
	}

	int FileSearchProvider::maxThreadsCount() const noexcept
	{
		// ���������� ������������ ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����
		return MaxThreadsCount;
	}

	size_t FileSearchProvider::maxMaskSize() const noexcept
	{
		// ���������� ������������ ����� ����� ��� ������ ����� � �����
		return maxMaskSize_;
	}

	void FileSearchProvider::setMaxMaskSize(size_t value)
	{
		// ���� ���������� ������������ ����� ����� ��� ������ ����� � ����� ����� 0, ����������� ����������
		if (value == 0)
		{
			throw invalid_argument{ "������������ ����� ����� ��� ������ ����� � ����� ����� 0" };
		}

		// ��������� ������������ ����� ����� ��� ������ ����� � �����
		maxMaskSize_ = value;
	}

	FileSearchResult FileSearchProvider::search(const string& mask, int threadsCount)
	{
		// ���� ���������� ����� ��� ������ ����� � ����� ������, ����������� ����������
		if (mask.empty())
		{
			throw invalid_argument{ "����� ��� ������ ����� � ����� ������" };
		}

		// ���� ����� ���������� ����� ��� ������ ����� � ����� ������ ����������, ����������� ����������
		if (mask.size() > maxMaskSize_)
		{
			throw invalid_argument{ "����� ����� ��� ������ ����� � ����� ������ ����������� �������� - " + to_string(maxMaskSize_) };
		}

		// ���� ���������� ����� ��� ������ ����� � ����� ������� ������ �� �������� (?), ����������� ����������
		if (regex_search(mask, regex{ "^[\\?]+$" }))
		{
			throw invalid_argument{ "����� ��� ������ ����� � ����� ������� ������ �� �������� (?)" };
		}

		// ���� ���������� ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����, ���������� �� � ��������� ���������, ����������� ����������
		if (threadsCount < MinThreadsCount || threadsCount > MaxThreadsCount)
		{
			throw invalid_argument{ "���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����, �� ��������� � ��������� �� " +
				to_string(MinThreadsCount) + " �� " + to_string(MaxThreadsCount) };
		}

		// ������� ������ ���������� ������ � �����
		lastSearchErrors_.clear();

		// ��������� ������ � �����
		FileSearchResult fileSearchResult;
		
		{
			// ������ ������� ��� ������ � ����� �����, ��������������� ���������� �����
			vector<JoinThread> searchThreads;

			{
				// ������ ������ ������ ��� ������ ���������� �����
				FileReader fileReader{ filePath_ };
				// ������ ������� ����� ����� � ��������� �����
				deque<intmax_t> linesPositions;

				// ���� ������� ����� ����� � ��������� �����, ���� �� ����� ��������� ����� �����
				do
				{
					// ��������� ������� ������� ��������� ������� ��� ������ ���������� ����� � ������ ������� ����� ����� � ��������� �����
					linesPositions.push_back(fileReader.position());
				} while (fileReader.seekNextLine());

				// �������� ���������� ����� � ��������� �����
				size_t linesCount = linesPositions.size();

				// ���������, ������ �� ���������� ����� � ��������� �����, ����������� ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����
				if (threadsCount > linesCount)
				{
					// ���� ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����, ������ ���������� ����� � ��������� �����
					// ������ ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����, ������ ���������� ����� � ��������� �����, 
					// ���-�� �� ��������� ������ ������ �������
					threadsCount = linesCount;
				}

				// �������� ���������� ����� ���������� ����� ��� ������ �����, ��������������� ���������� �����, ����� �������
				// ��� ����� ����� ����� ���������� ����� � ��������� ����� �� ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����, 
				// � �������� � ������ �����
				size_t threadLinesCount = static_cast<size_t>((linesCount / threadsCount));

				// ���������, ���� �� ������� ��� ������� ���������� ����� � ��������� ����� �� ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����
				if ((linesCount % threadsCount) != 0)
				{
					// ���� ���� ������� ��� ������� ���������� ����� � ��������� ����� �� ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����, 
					// �� ��������� ���� � ���������� ����� ���������� ����� ��� ������ �����, ��������������� ���������� �����, ����� �������, 
					// ���-�� ��������� ������ ���������� ����� �� ������� ����� ��������
					++threadLinesCount;
				}

				// ����������� ����� � ������ ������� ��� ������ � ����� �����, ��������������� ���������� �����, ��� ������� ���������� �������
				searchThreads.reserve(threadsCount);
				// ����������� ����� � ������ ����������� ������ � ����� �����, ��������������� ���������� �����, ��� ������� ���������� �������
				fileSearchResult.results.reserve(threadsCount);

				// ������ ������ ���������� ������� ��� ������ � ����� �����, ��������������� ���������� �����, � ��������� �� � ������
				for (int threadNumber = 0; threadNumber < threadsCount; ++threadNumber)
				{
					// �������� ����� ������ � ������ ������� ����� ����� � ��������� �����, 
					// � ������� ������� ����� ����� �������� ����� �����, ��������������� ���������� �����
					// �������� ����� �������� ������ �� ���������� ����� ���������� ����� ��� ������ �����, ��������������� ���������� �����, ����� ������� 
					size_t threadStartLineNumber = threadNumber * threadLinesCount;

					// ���������, ���� �� � ������ ������� ����� ����� � ��������� �����, ������ � ���������� �������, 
					// � ������� ������� ����� ����� �������� ����� �����, ��������������� ���������� �����
					if (threadStartLineNumber >= linesCount)
					{
						// � ������ ������� ����� ����� ��� ������ � ���������� �������, 
						// � ������� ������� ����� ����� �������� ����� �����, ��������������� ���������� �����
						// ������, ������ ������� ��� ������ � ����� �����, ��������������� ���������� �����, �� ���������
						// ��������� �������� ������� ��� ������ � ����� �����, ��������������� ���������� �����, � ���������� �� � ������
						break;
					}

					{
						// ������������� ���������� ������� � ���������� ������ � ����� ������� �������
						unique_lock<mutex> lockerResult{ lockerResultMutex_ };
						// ��������� � ��������� ������ � ����� ������� ��� ���������� ����������� ������ �����, ��������������� ���������� �����, ������� �������
						fileSearchResult.results.emplace_back();
					}
				
					// ������ ������� ����� ��� ������ � ����� �����, ��������������� ���������� �����, � ��������� ��� � ������
					searchThreads.emplace_back(thread{ &FileSearchProvider::searchStrings, this, ref(mask), ref(fileSearchResult),
						threadNumber, linesPositions.at(threadStartLineNumber), threadLinesCount });
				}
			}
		}	

		// ���������� ��������� ������ � �����
		return fileSearchResult;
	}
}