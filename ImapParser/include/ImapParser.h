#ifndef IMAP_PARSER_H
#define IMAP_PARSER_H
#include <fstream>
#include "MessageWorker.h"
#include "TokenWorker.h"
//#define LINUX

///Множество наименований функций-членов класса, которые могут быть вызваны после окончания обработки файла .pcap
enum FunctionTypes{
	IMAP_GENERATE_HTML,			///< Сгенерировать html-документ для просмотра всех сообщений с заголовками(генерируется в папку html)
	IMAP_PRINT_ALL				///< Вывод всех сообщений с загловками в консоль
};

/*!
	\brief Класс обработки файла .pcap
	\details Определяет заголовки и сообщения в файле .pcap
*/
class ImapParser{
	private:
		std::ifstream pcapFile;											///< Файл, в котором содержится переписка по протоколу IMAP
		void (ImapParser::*classFuncPointer)(std::vector<Message>&);	///< Указатель на функцию-член класса
		void (*funcPointer)(std::vector<Message>&);						///< Указатель на функцию
		TokenWorker tWorker;											///< Экземпляр класса TokenWorker для работы с токенами
		MessageWorker mWorker;											///< Экземпляр класса MessageWorker для работы с сообщениями
		std::string parseStr;											///< В строку записываются строки из файла
		std::string messageStr;											///< В строку накапливается сообщение
		/*!
			\brief Генерирует HTML-документ на основе выделенных сообщений и заголовков
			\param messages ссылка на вектор сообщений
		*/
		void GenerateHTML(std::vector<Message> &messages);	
		/*!
			\brief Выводит все сообщения и заголовки в консоль
			\param messages ссылка на вектор сообщений
		*/
		void PrintAll(std::vector<Message> &messages);
	public:
		///Обнуляет все указатели и инициализирует строки пустыми значениями
		ImapParser(){
			classFuncPointer = nullptr;
			funcPointer = nullptr;
			parseStr = "";
			messageStr = "";
		}
		///Закрывает .pcap файл, если тот открыт
		~ImapParser(){
			if(pcapFile.is_open()) pcapFile.close();
		}
		/*!
			\brief Привязка файла, в котором будут выделяться сообщения и заголовки
			\param filepath Путь до .pcap файла
			\return true или false в зависимости от успеха привязки файла
		*/
		bool BindFile(std::string filepath);
		/*!
			\brief Привязка функции, которая будет вызвана после обработки файла и выделения заголовком и сообщений
			\param function указатель на функцию, которая будет вызвана после обработки
			\code
				Пример функции, которая может быть привязана
				void PrintMessages(std::vector<Message>& message){
					for(unsigned int i=0; i < message.size(); i++) message.Print();
				}
			\endcode
			\return true или false в зависимости от успеха привязки функции
		*/
		bool BindFunction(void(&function)(std::vector<Message>&));
		/*!
			\brief Привязка функции-члена класса, которая будет вызвана после обработки файла и
				   выделения заголовком и сообщений
			\param functionType тип функции из множества FunctionTypes
			\return true или false в зависимости от успеха привязки функции
		*/
		bool BindFunction(int functionType);
		/*!
			\brief Разбор привязанного файла
			\details Проверяет, привязан ли файл, если привязан, то начинает разбор.
					 После разбора проверяет, указывает ли на что-нибудь classFuncPointer или funcPointer
					 Если указывают, то вызывается соответствующая функция
			\warning Привязана может быть либо функция, либо функция-член класса. 
					 Нельзя вызывать 2 функции после разбора файла
			\return true или false в зависимости от успеха разбора
		*/
		bool Parse();
		/*!
			\brief Возвращает ссылку на вектор сообщений, которые были прочитаны в файле .pcap
			\return Ссылку на вектор типа Message
		*/
		std::vector<Message>& GetMessages(){
			return mWorker.GetAllMessages();
		}
};
#endif