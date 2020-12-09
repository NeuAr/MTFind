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
		// ���� ���� �� ����������� ���� �� ��� ������, ����������� ����������
		if (!streamReader_.is_open())
		{
			throw exception{ "��������� ������ ��� �������� ���������� �����" };
		}
	}

	FileReader::~FileReader()
	{
		// ��������� ����� ��� ������ �����
		streamReader_.close();
	}

	streampos FileReader::position()
	{
		// ���������� ������� ������� ��������� ������ ��� ������ �����
		return streamReader_.tellg();
	}

	void FileReader::setPosition(streampos value)
	{
		// ������������� ��������� ������ ��� ������ ����� � ���������� �������
		streamReader_.seekg(value);
	}

	bool FileReader::seekNextLine()
	{
		// ������ �����
		char symbol;

		// ������ ������� �����, ���� �� ����� ��������� ����� �����
		while ((symbol = streamReader_.get()) != char_traits<char>::eof())
		{
			// ���������, �������� �� ������� ������ �����, �������� �������� ������
			if (symbol == '\n')
			{
				// ���� ������� ������ ����� �������� �������� �������� ������
				// ������� ��������� ������ � ����� �������
				// ��������� �������� � ���������� ���������� ��������, ��� ������� ��������� ������ � ����� ���� �������
				return true;
			}
		}

		// ���� ��� ��������� ����� ����� � ������ �������� ������ �� ��� ������
		// ���������� ���������� ��������, ��� ������� ��������� ������ � ����� �� ���� �������
		return false;
	}

	bool FileReader::readLine(string& fileLine)
	{
		// ������ ������ ����� � ��������� � ���������� ����������
		// ���������� ���������� ��������, ���� �� ��������� ������ �����
		return getline(streamReader_, fileLine) ? true : false;
	}
}