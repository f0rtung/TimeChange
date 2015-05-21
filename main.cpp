#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <map>


/*�������� ������ ���������:
��������� ����������� � ������������ ���� � ���������� ��� ��������� ������.
���� ���������� �� �����, ��������� ��������� � ������������ ������ ���������. 
���� ����� - ������������ ������� ������ �����, � ������� �������� ��������� 
����������� ������ �� ������ �  ������� �� �������������� � ���� ����������,
���� ������ ���������� �� ����������� �������.
����� ����������� ��� ������� � ����������, ����������� ����� � ���� � ������������,
���������� �� ��������� ������ ����������� � �������, ������ ����� ������ ������������
� ���� ���������� (������ ���������������� ��� ���� ���������� ��������� �������). � ��������,
��������� ������ ����� � ��� �� �����, ��� � ������ ������ � ������-�������, �� ��� �����������
�������� � ��������� ����.
����� ����� �����������, ���� �� ��������� ������ �� ����� � ������� ������ ������. ���� ����,
�� �� ����� ��������� � ������ �������. ������������ ����� ��������� �� ������� �����
������ � ������� ������ ������. ���� ������� ��� ������, ����������� ����� ���������. ����
��� �� ���������, ������ ����� ��������� �������� ���� ��� �������. ���� ������� ������ �� ���
������, ������ �� ��� ������ ����� ��������� ��������. ����������� �������� ������������ � 
�������� ������� ��� �������� ������� ���������: ���� ������� �� ������� ������ ������ �� ��� ������ � 
������� ����� ������, ����� �� ��� ������ �� ����������. ������ ��������� �����������.
��������� ����� ������� ���������� - ��������� ����� ����� ���������. ��� �������� ������� ������ 
�� ������ ���������.
*/




int main(int argc, char** argv)
{



	std::string fileTimeName = "NameTimeFile.txt"; // ���� ��� ���������� ����� ����� � ������� ��� ���������
	boost::filesystem::path filePath; // ���� � ����������
	std::cout << "Enter a path: \n";
	std::cin >> filePath;
	std::map<std::string, std::time_t> newNameTime; //������� ��� �������� ����� ������ � ������� ��������� �����
	std::map<std::string, std::time_t> oldNameTime; //������� ��� �������� ����������� ������ � ������� ��������� �����



	if (boost::filesystem::exists(filePath)) // �������� ������������� ����������
	{


		std::ifstream infile(filePath.string() + "\\" + fileTimeName); // ����� ��� ������ �� �����
		if (infile.is_open()) // ���� ������� �������, ������ ��� ����������� ��� �����������, ����� ������� ����������� ������
		{
			std::string name;
			std::time_t time;
			infile >> name;
			while (!infile.eof() && !name.empty())
			{
				infile >> time;
				oldNameTime.insert(std::make_pair(name, time)); // ��������� � �������
				infile >> name;
			}
		}
		else
		{
			std::cout << "The directory " << filePath << " is viewed for the first time.\n";
		}
		infile.close();



		std::for_each(boost::filesystem::directory_iterator(filePath), // ��� ������� ������� � ����������
					  boost::filesystem::directory_iterator(), 
					  [&newNameTime, &fileTimeName](const boost::filesystem::path& file)
					  {
					  std::string fileName = file.filename().string();
			          if (boost::filesystem::is_regular_file(file) && fileName != fileTimeName) // ���������, ���� �� ���, � ��� �� �� �������� ������ ��� ���������� ����������
					  {
							newNameTime.insert(std::make_pair(file.filename().string(), boost::filesystem::last_write_time(file))); // ��������� ��� ����� � ����� ��� ��������������
					  }
					  });


		
		std::ofstream outfile(filePath.string() + "\\" + fileTimeName); // ����� ��� ������ � ���� ����������� ������
		if (outfile.is_open())
		{
			for (auto elem : newNameTime) outfile << elem.first << " " << elem.second << std::endl; // ���������� � ���� ��� ������ �� �������
		}
		outfile.close();



		if (!oldNameTime.empty()) // ���� ������� ������ ������ ����, ������ ���������� ����������� ������� � ������� �� � ���
		{
			for (auto elem : newNameTime)
			{
				auto findelem = oldNameTime.find(elem.first); // ���� � ������ ������ �� ����� ����� �������� �� ������ �������
				if (findelem != oldNameTime.end()) // ���� ������� ����� ����
				{
					if (findelem->second != elem.second) // ���������, ���������� �� ����� ��������������, ���� �� - ������� ���������
					{
						std::cout << elem.first << " was changed!\n";
					}
					
				}
				else // ���� ����� ���� �� ������� ����� � ������� ������ ������, ������ �� ��� ������ - ������� ���������
				{
					std::cout << elem.first << " was created!\n";
				}
			}
			for (auto elem : oldNameTime)
			{
				auto findelem = newNameTime.find(elem.first); // ���� � ������� ����� ������ ������ �����
				if (findelem == newNameTime.end()) // ���� ����� �� �������, ������ ���� ��� ������ - ���������
				{
					std::cout << elem.first << " was deleted!\n";
				}
			}
		}
		
	}
	else
	{
		std::cout << "Incorrect dir!\n";
	}
	std::cout << "\nDONE!\n";
	system("pause");
	return 0;
}