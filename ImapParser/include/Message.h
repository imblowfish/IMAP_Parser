#ifndef MESSAGE_H
#define MESSAGE_H
#include <vector>
#include "MessageHeader.h"

/*!
	\brief Класс сообщения
	\details Хранит значения заголовка и сообщения, а также содержит функции для добавления и получения токенов
*/

class Message{
	private:
		MessageHeader header;					///< Заголовок сообщения
		std::string value;						///< Значение сообщения
		std::string id;							///< Идентификатор сообщения
	public:
		Message(){
			value = "";
		}
		~Message(){
			Clear();
		}
		/*!
			\brief Добавление токена в заголовок
			\param token Токен, который требуется добавить в заголовок
		*/
		void AddInHeader(Token token){
			//если тип токена - id, то записываем, чтобы потом не искать его значения перебором в header
			if(token.type == R_ID) this->id = token.value;
			header.AddToken(token);
		}
		/*!
			\brief Функция дополнения токена заголовка
			\param token Токен, со значением, которое требуется добавить к токену в заголовке
		*/
		void CompleteHeader(Token token){
			header.CompleteToken(token);
		}
		/*!
			\brief Функция добавления в сообщение, собственно, самого сообщения(переписки)
			\param value Сообщение
		*/
		void AddMessage(std::string value){
			this->value = value;
		}
		///Очистка сообщения
		void Clear(){
			header.ClearHeader();
			value = "";
		}
		/*!
			\brief Проверка сообщения на пустоту
			\return false, если в заголовке нет токенов и в сообщении нет значения, и true в обратном случае
		*/
		bool Empty(){
			return value.length() == 0 && header.Empty();
		}
		/*!
			\brief Дополнение сообщения и его заголовка значениями и заголовком из другого сообщения
			\param message Ссылка на сообщение, из которого будут браться значения для дополнения
		*/
		void CompleteMessage(Message &message){
			if(message.value > this->value) value = message.value;
			message.GetHeader().CopyValuesIn(this->header);
		}
		///Возвращает заголовок сообщения
		MessageHeader& GetHeader(){
			return header;
		}
		///Возвращает id сообщения
		std::string GetId(){
			//если id записано в классе сообщения, возаращаем
			if(id.length() > 0) return this->id;
			//иначе ищем токен типа id в заголовке
			return header.GetId();
		}
		///Вывод заголовка и значения сообщения
		void Print(){
			header.Print();
			std::cout << "---MESSAGE---" << std::endl;
			std::cout << value << std::endl;
		}
		///Возвращает количество символов, которые занимает заголовок
		int GetHeaderLength(){
			return header.GetLength();
		}
		/*!
			\brief Возвращает значение токена по типу
			\param tokenType Тип токена, значение которого будем искать в заголовке
			\return строка std::string значения токена
		*/
		std::string GetHeaderValue(int tokenType){
			return header.GetToken(tokenType);
		}
		///Возвращает вектор токенов из заголовка
		std::vector<Token>& GetHeaderTokens(){
			return header.GetAllTokens();
		}
		/*!
			\brief Возвращает значение сообщения без заголовка в виде строки std::string
			\return Строка std::string, в которой содержится сообщение
		*/
		std::string GetMessage(){
			return this->value;
		}
};

#endif