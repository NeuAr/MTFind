#include <exception>
#include <iosfwd>
#include <file_reader.h>

namespace mtfind 
{
	using namespace std;
	using namespace std::experimental::filesystem;

	FileReader::FileReader(const path& filePath)
		: streamReader_{ filePath }
	{
		// Если файл по переданному пути не был открыт, выбрасываем исключение
		if (!streamReader_.is_open())
		{
			throw exception{ "Произошла ошибка при открытии указанного файла" };
		}
	}

	FileReader::~FileReader()
	{
		// Закрываем поток для чтения файла
		streamReader_.close();
	}

	streampos FileReader::position()
	{
		// Возвращаем текущую позицию указателя потока для чтения файла
		return streamReader_.tellg();
	}

	void FileReader::setPosition(streampos value)
	{
		// Устанавливаем указатель потока для чтения файла в переданную позицию
		streamReader_.seekg(value);
	}

	bool FileReader::seekNextLine()
	{
		// Символ файла
		char symbol;

		// Читаем символы файла, пока не будет достигнут конец файла
		while ((symbol = streamReader_.get()) != char_traits<char>::eof())
		{
			// Проверяем, является ли текущий символ файла, символом перевода строки
			if (symbol == '\n')
			{
				// Если текущий символ файла является символом перевода строки
				// Позиция следующей строки в файле найдена
				// Завершаем операцию и возвращаем логическое значение, что позиция следующей строки в файле была найдена
				return true;
			}
		}

		// Если был достигнут конец файла и символ перевода строки не был найден
		// Возвращаем логическое значение, что позиция следующей строки в файле не была найдена
		return false;
	}

	bool FileReader::readLine(string& fileLine)
	{
		// Читаем строку файла и сохраняем в переданную переменную
		// Возвращаем логическое значение, была ли прочитана строка файла
		return getline(streamReader_, fileLine) ? true : false;
	}
}