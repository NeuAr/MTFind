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

// Выводит список ошибок поиска в файле в стандартный поток вывода сообщений об ошибках
void printSearchErrors(const vector<string>& searchErrors)
{
	// Проверяем, есть ли ошибки поиска в файле
	if (!searchErrors.empty())
	{
		// Если ошибки поиска в файле есть
		// Выводим заголовок раздела ошибок в стандартный поток вывода сообщений об ошибках
		cerr << "Ошибки:" << endl;

		// Проходимся по переданному списку ошибок поиска в файле
		for (const string& errorText : searchErrors)
		{
			// Выводим текст ошибки в стандартный поток вывода сообщений об ошибках
			cerr << errorText << endl;
		}
	}
}

// Выводит результат поиска в файле в стандартный выходной поток
void printSearchResult(const FileSearchResult& searchResult)
{
	// Выводим заголовок раздела результата поиска в файле в стандартный выходной поток
	cout << "Результат:" << endl;
	// Выводим количество найденных результатов в файле в стандартный выходной поток
	cout << searchResult.size << endl;

	// Проходимся по списку результатов поиска в файле
	for (const deque<LineSearchResult>& threadSearchResults : searchResult.results)
	{
		// Проходимся по списку результатов поиска в строках файла потоком
		for (const LineSearchResult lineSearchResult : threadSearchResults)
		{
			// Проходимся по списку результатов поиска в строке файла
			for (const SearchResult& searchResult : lineSearchResult.results)
			{
				// Выводим номер строки файла, позицию в строке и текст искомого фрагмента в стандартный выходной поток
				cout << lineSearchResult.lineNumber << " " << searchResult.position << " " << searchResult.text << endl;
			}
		}		
	}
}

// Точка входа в программу
int main(int argc, char* argv[])
{
	// Задаём локаль, которая будет использоваться текущей программой
	setlocale(LC_ALL, "Russian");

	// Максимальная длина маски для поиска строк файле
	constexpr size_t MaxMaskSize = 100;
	// Максимальный размер файла для поиска в нём строк, соответствующих указанной маске
	constexpr uintmax_t MaxFileSize = 1024 * 1024 * 1024;
	
	try 
	{
		// Количество потоков для поиска в файле строк, соответствующих указанной маске, по умолчанию, равно 10
		int threadsCount = 10;

		// Проверяем количество аргументов, переданных в программу
		if (argc < 3 || argc > 4)
		{
			// Аргументы программы переданы некорректно, выбрасываем исключение
			throw invalid_argument{ "Аргументы программы переданы некорректно (mtfind.exe {<Путь к файлу>} {<Маска для поиска>} [<Количество потоков>])" };
		}

		// Сохраняем путь до файла, в котором нужно выполнить поиск строк, соответствующих указанной маске
		string filePath = argv[1];
		// Сохраняем маску для поиска строк в файле
		string mask = argv[2];

		// Проверяем, было ли передано количество потоков для поиска в файле строк, соответствующих указанной маске
		if (argc == 4)
		{
			// Количество потоков для поиска в файле строк, соответствующих указанной маске, было передано
			// Пытаемся выполнить конвертацию типа переданного количества потоков для поиска в файле строк, соответствующих указанной маске, 
			// из string в int
			if (!(stringstream{ argv[3] } >> threadsCount))
			{
				// Конвертация типа переданного количества потоков для поиска в файле строк, соответствующих указанной маске, из string в int не была выполнена
				// Выбрасываем исключение
				throw invalid_argument{ "Количество потоков для поиска в файле строк, соответствующих указанной маске, не является целым числом" };
			}
		}

		// Создаём класс провайдера для поиска в файле по указанному пути
		FileSearchProvider fileSearchProvider{ path{ move(filePath) }, MaxFileSize };
		// Задаём максимальную длину маски для поиска строк в указанном файле
		fileSearchProvider.setMaxMaskSize(MaxMaskSize);
		// Выполняем поиск строк, соответствующих указанной маске, в указанном файле и получаем результат поиска в файле
		// Поиск строк выполняем с помощью указанного количества потоков
		FileSearchResult fileSearchResult = fileSearchProvider.search(mask, threadsCount);

		// Выводим список ошибок поиска в указанном файле в стандартный поток вывода сообщений об ошибках
		printSearchErrors(fileSearchProvider.lastSearchErrors());
		// Выводим результат поиска в указанном файле в стандартный выходной поток
		printSearchResult(fileSearchResult);
	}
	catch (const exception& e) 
	{
		// В случае, если возникло исключение
		// Выводим текст ошибки в стандартный поток вывода сообщений об ошибках
		cerr << "Ошибка: " << e.what() << endl;
	}
	catch (...) 
	{
		// В случае, если возникло исключение
		// Выводим текст ошибки в стандартный поток вывода сообщений об ошибках
		cerr << "Произошла неизвестная ошибка" << endl;
	}

	// Возвращаем код возврата из программы
	// Программа была выполнена успешно, код 0
	return 0;
}