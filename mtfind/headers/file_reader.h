#pragma once
#include <experimental\filesystem>
#include <fstream>
#include <iosfwd>
#include <string>

namespace mtfind 
{
	// Класс для чтения файла
	class FileReader
	{
		public:
			explicit FileReader(const std::experimental::filesystem::path& filePath);
			FileReader(FileReader&& fileReader) = default;
			
			~FileReader();
			
			FileReader& operator=(FileReader&& fileReader) = default;
		
			// Возвращает текущую позицию указателя объекта для чтения файла
			std::streampos position();
			// Устанавливает указатель объекта для чтения файла в указанную позицию
			void setPosition(std::streampos value);
			// Ищет позицию следующей строки в файле и возвращает логическое значение, была ли найдена позиция следующей строки в файле
			bool seekNextLine();
			// Читает строку файла, сохраняет её в указанную переменную и возвращает логическое значение, была ли прочитана строка файла
			bool readLine(std::string& fileLine);

		private:
			// Поток для чтения файла
			std::ifstream streamReader_;
	};
}