#include <cstddef>
#include <deque>
#include <exception>
#include <experimental\filesystem>
#include <iostream>
#include <sstream>
#include <stdint.h>
#include <string>
#include <vector>
#include <file_search.h>

using namespace mtfind;
using namespace std;
using namespace std::experimental::filesystem;

// ������� ������ ������ ������ � ����� � ����������� ����� ������ ��������� �� �������
void printSearchErrors(const vector<string>& searchErrors)
{
	// ���������, ���� �� ������ ������ � �����
	if (!searchErrors.empty())
	{
		// ���� ������ ������ � ����� ����
		// ������� ��������� ������� ������ � ����������� ����� ������ ��������� �� �������
		cerr << "������:" << endl;

		// ���������� �� ����������� ������ ������ ������ � �����
		for (const string& errorText : searchErrors)
		{
			// ������� ����� ������ � ����������� ����� ������ ��������� �� �������
			cerr << errorText << endl;
		}
	}
}

// ������� ��������� ������ � ����� � ����������� �������� �����
void printSearchResult(const FileSearchResult& searchResult)
{
	// ������� ��������� ������� ���������� ������ � ����� � ����������� �������� �����
	cout << "���������:" << endl;
	// ������� ���������� ��������� ����������� � ����� � ����������� �������� �����
	cout << searchResult.size << endl;

	// ���������� �� ������ ����������� ������ � �����
	for (const deque<LineSearchResult>& threadSearchResults : searchResult.results)
	{
		// ���������� �� ������ ����������� ������ � ������� ����� �������
		for (const LineSearchResult lineSearchResult : threadSearchResults)
		{
			// ���������� �� ������ ����������� ������ � ������ �����
			for (const SearchResult& searchResult : lineSearchResult.results)
			{
				// ������� ����� ������ �����, ������� � ������ � ����� �������� ��������� � ����������� �������� �����
				cout << lineSearchResult.lineNumber << " " << searchResult.position << " " << searchResult.text << endl;
			}
		}		
	}
}

// ����� ����� � ���������
int main(int argc, char* argv[])
{
	// ����� ������, ������� ����� �������������� ������� ����������
	setlocale(LC_ALL, "Russian");

	// ������������ ����� ����� ��� ������ ����� �����
	constexpr size_t MaxMaskSize = 100;
	// ������������ ������ ����� ��� ������ � �� �����, ��������������� ��������� �����
	constexpr uintmax_t MaxFileSize = 1024 * 1024 * 1024;
	
	try 
	{
		// ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����, �� ���������, ����� 10
		int threadsCount = 10;

		// ��������� ���������� ����������, ���������� � ���������
		if (argc < 3 || argc > 4)
		{
			// ��������� ��������� �������� �����������, ����������� ����������
			throw invalid_argument{ "��������� ��������� �������� ����������� (mtfind.exe {<���� � �����>} {<����� ��� ������>} [<���������� �������>])" };
		}

		// ��������� ���� �� �����, � ������� ����� ��������� ����� �����, ��������������� ��������� �����
		string filePath = argv[1];
		// ��������� ����� ��� ������ ����� � �����
		string mask = argv[2];

		// ���������, ���� �� �������� ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����
		if (argc == 4)
		{
			// ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����, ���� ��������
			// �������� ��������� ����������� ���� ����������� ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����, 
			// �� string � int
			if (!(stringstream{ argv[3] } >> threadsCount))
			{
				// ����������� ���� ����������� ���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����, �� string � int �� ���� ���������
				// ����������� ����������
				throw invalid_argument{ "���������� ������� ��� ������ � ����� �����, ��������������� ��������� �����, �� �������� ����� ������" };
			}
		}

		// ������ ����� ���������� ��� ������ � ����� �� ���������� ����
		FileSearchProvider fileSearchProvider{ path{ move(filePath) }, MaxFileSize };
		// ����� ������������ ����� ����� ��� ������ ����� � ��������� �����
		fileSearchProvider.setMaxMaskSize(MaxMaskSize);
		// ��������� ����� �����, ��������������� ��������� �����, � ��������� ����� � �������� ��������� ������ � �����
		// ����� ����� ��������� � ������� ���������� ���������� �������
		FileSearchResult fileSearchResult = fileSearchProvider.search(mask, threadsCount);

		// ������� ������ ������ ������ � ��������� ����� � ����������� ����� ������ ��������� �� �������
		printSearchErrors(fileSearchProvider.lastSearchErrors());
		// ������� ��������� ������ � ��������� ����� � ����������� �������� �����
		printSearchResult(fileSearchResult);
	}
	catch (const exception& e) 
	{
		// � ������, ���� �������� ����������
		// ������� ����� ������ � ����������� ����� ������ ��������� �� �������
		cerr << "������: " << e.what() << endl;
	}
	catch (...) 
	{
		// � ������, ���� �������� ����������
		// ������� ����� ������ � ����������� ����� ������ ��������� �� �������
		cerr << "��������� ����������� ������" << endl;
	}

	// ���������� ��� �������� �� ���������
	// ��������� ���� ��������� �������, ��� 0
	return 0;
}