#pragma once
#include <experimental\filesystem>
#include <fstream>
#include <iosfwd>
#include <string>

namespace mtfind 
{
	// ����� ��� ������ �����
	class FileReader
	{
		public:
			explicit FileReader(const std::experimental::filesystem::path& filePath);
			FileReader(FileReader&& fileReader) = default;
			
			~FileReader();
			
			FileReader& operator=(FileReader&& fileReader) = default;
		
			// ���������� ������� ������� ��������� ������� ��� ������ �����
			std::streampos position();
			// ������������� ��������� ������� ��� ������ ����� � ��������� �������
			void setPosition(std::streampos value);
			// ���� ������� ��������� ������ � ����� � ���������� ���������� ��������, ���� �� ������� ������� ��������� ������ � �����
			bool seekNextLine();
			// ������ ������ �����, ��������� � � ��������� ���������� � ���������� ���������� ��������, ���� �� ��������� ������ �����
			bool readLine(std::string& fileLine);

		private:
			// ����� ��� ������ �����
			std::ifstream streamReader_;
	};
}