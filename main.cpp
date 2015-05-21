#include <boost/filesystem.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <map>


/*Описание работы программы:
Программа запрашивает у пользователя путь к директории для просмотра файлов.
Если директория не верна, выводится сообщение и прекращается работа программы. 
Если верна - производится попытка чтения файла, в котором хранятся последние 
сохраненные данные по файлам и  времени их редактирования в этой диретокрии,
если только директория не проверяется впервые.
Затем проверяются все объекты в директории, отсеиваются папки и файл с сохранениями,
информация об остальных файлах добавляется в словарь, откуда затем данные записываются
в файл сохранений (данные перезаписываются или файл сохранений создается впервые). В принципе,
сохранять данные можно в том же цикле, что и чтение данных в лямбда-функции, но для наглядности
вынесено в отдельный цикл.
После этого проверяется, были ли добавлены данные из файла в словарь старых данных. Если были,
то их можно проверить с новыми данными. Производится поиск элементов из словаря новых
данных в словаре старых данных. Если елемент был найден, проверяется время изменения. Если
оно не совпадает, значит после последней проверки файл был изменен. Если элемент вообще не был
найден, значит он был создан после последней проверки. Аналогичная операция производится в 
обратном порядке без проверки времени изменения: если элемент из словаря старых данных не был найден в 
словаре новых данных, значи он был удален из директории. Работа программы завершается.
Программа имеет большой недостаток - временную метку можно подменить. Эту проблему пытаюсь решить 
во второй программе.
*/




int main(int argc, char** argv)
{



	std::string fileTimeName = "NameTimeFile.txt"; // файл для сохранения имени файла и времени его изменения
	boost::filesystem::path filePath; // путь к директории
	std::cout << "Enter a path: \n";
	std::cin >> filePath;
	std::map<std::string, std::time_t> newNameTime; //словарь для хранения новых данных о времени изменения файла
	std::map<std::string, std::time_t> oldNameTime; //словарь для хранения сохраненных данных о времени изменения файла



	if (boost::filesystem::exists(filePath)) // проверка существования директории
	{


		std::ifstream infile(filePath.string() + "\\" + fileTimeName); // поток для чтения из файла
		if (infile.is_open()) // если удается открыть, значит эта директтория уже проверялась, нужно считать сохраненные данные
		{
			std::string name;
			std::time_t time;
			infile >> name;
			while (!infile.eof() && !name.empty())
			{
				infile >> time;
				oldNameTime.insert(std::make_pair(name, time)); // добавляем в словарь
				infile >> name;
			}
		}
		else
		{
			std::cout << "The directory " << filePath << " is viewed for the first time.\n";
		}
		infile.close();



		std::for_each(boost::filesystem::directory_iterator(filePath), // для каждого объекта в директории
					  boost::filesystem::directory_iterator(), 
					  [&newNameTime, &fileTimeName](const boost::filesystem::path& file)
					  {
					  std::string fileName = file.filename().string();
			          if (boost::filesystem::is_regular_file(file) && fileName != fileTimeName) // проверяем, файл ли это, и что он не является файлом для сохранения информации
					  {
							newNameTime.insert(std::make_pair(file.filename().string(), boost::filesystem::last_write_time(file))); // вставляем имя файла и время его редактирования
					  }
					  });


		
		std::ofstream outfile(filePath.string() + "\\" + fileTimeName); // поток для записи в файл обновленных данных
		if (outfile.is_open())
		{
			for (auto elem : newNameTime) outfile << elem.first << " " << elem.second << std::endl; // записываем в файл все данные из словаря
		}
		outfile.close();



		if (!oldNameTime.empty()) // если словарь старых данных пуст, значит директория проверялась впервые и сверять не с чем
		{
			for (auto elem : newNameTime)
			{
				auto findelem = oldNameTime.find(elem.first); // ищем в старых данных по имени файла элементы из нового словаря
				if (findelem != oldNameTime.end()) // если удалось найти файл
				{
					if (findelem->second != elem.second) // проверяем, изменилось ли время редактирования, если да - выводим сообщение
					{
						std::cout << elem.first << " was changed!\n";
					}
					
				}
				else // если новый файл не удалось найти в словаре старых данных, значит он был создан - выводим сообщение
				{
					std::cout << elem.first << " was created!\n";
				}
			}
			for (auto elem : oldNameTime)
			{
				auto findelem = newNameTime.find(elem.first); // ищем в словаре новых данных старые файлы
				if (findelem == newNameTime.end()) // если найти не удалось, значит файл был удален - оповещаем
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