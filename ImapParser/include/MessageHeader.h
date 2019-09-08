#ifndef MESSAGE_HEADER_H
#define MESSAGE_HEADER_H
#include <iostream>
#include <string>
#include "Token.h"

/*!
	\brief Класс заголовка сообщения
	\details Содержит значения всех найденных токенов, относящихся к одному сообщению
*/

class MessageHeader{
private:
	std::vector<Token> tokens;									///< Вектор токенов заголовка типа Token
	/*!
		\brief Возвращает индекс токена  из вектора tokens по требуемому типу
		\param tokenType Тип токена, поиск которого будет осуществляться в заголовке
		\return Значение индекса из вектора tokens, либо -1, если токен заданного типа не был найден
	*/
	int GetTokenByType(int tokenType){
		for(unsigned int i = 0; i < tokens.size(); i++)
			if(tokens[i].type == tokenType) return i;
		return -1;
	}
public:
	~MessageHeader(){
		ClearHeader();
	}
	//добавление токена в заголовок
	/*!
		\brief Добавление нового токена в заголовок
		\details Добавляет только те токены, которых еще нет в векторе токенов
		\param token, содержит значение и тип токена, который требуется поместить в заголовок
	*/
	void AddToken(Token token){
		//ищет токен такого же типа
		int idx = GetTokenByType(token.type);
		//если не нашел, добавляет токен в заголовок
		if(idx < 0) tokens.push_back(token);
	}
	/*!
		\brief Дополнение значения токена из заголовка
		\param token, содержит значение и тип токена, значение которого требуется дополнить
	*/
	void CompleteToken(Token token){
		int idx = GetTokenByType(token.type);
		if(idx >= 0) tokens[idx].value += " " + token.value;
	}
	///Удаление всех токенов из заголовка
	void ClearHeader(){
		tokens.clear();
	}
	/*!
		\brief Проверка заголовка на пустоту
		\return true или false
	*/
	bool Empty(){
		return tokens.size() == 0;
	}
	/*!
		\brief Возвращает id сообщения
		\return Строку std::string содержащую id сообщения, либо пустую строку, если id нет
	*/
	std::string GetId(){
		int idx = GetTokenByType(R_ID);
		if(idx >= 0) return tokens[idx].value;
		else return "";
	}
	/*!
		\brief Копирует свои значения токенов в другой заголовок
		\param header заголовок, в который будут скопированы значения
	*/
	void CopyValuesIn(MessageHeader &header){
		for(unsigned int i = 0; i < tokens.size(); i++) header.AddToken(tokens[i]);
	}
	///Вывод токенов заголовка
	void Print(){
		std::cout << "HEADER" << std::endl;
		for(unsigned int i = 0; i < tokens.size(); i++){
			std::cout << tokenNames[tokens[i].type] << tokens[i].value << std::endl;
		}
	}
	/*!
		\brief Возвращает количество токенов в заголовке
		\return Количество токенов в заголовке
	*/
	int GetTokensNum(){
		return tokens.size();
	}
	/*!
		\brief Возвращает количество символов в заголовке, включая названия заголовков(From, To и т.д.)
		\details Данная функция используется для расчета количества символов в сообщении, которое следует за заголовком
		\return Количество символов в заголовке
	*/
	int GetLength(){
		int headerNamesLength = 0;													//количество символов для названий токенов
		int headerValuesLength = 0;													//количество символов для значений токенов
		for(unsigned int i = 0; i < tokenNames.size(); i++){
			headerNamesLength += tokenNames[i].length();						//добавляет количество символов названий токенов
			if(i < tokens.size()) headerValuesLength += tokens[i].value.length();	//и значений токенов
		}
		return headerValuesLength + headerNamesLength;
	}
	/*!
		\brief Возвращает значение токена по типу
		\param tokenType Тип токена, значение которого требуется получить из заголовка
		\return Строку std::string со значением токена
	*/
	std::string GetToken(int tokenType){
		for(unsigned int i = 0; i < tokens.size(); i++)
			if(tokens[i].type == tokenType) return tokens[i].value;
	}
	/*!
		\brief Возвращает вектор токенов заголовка
		\return Вектор токенов заголовка
	*/
	std::vector<Token>& GetAllTokens(){
		return tokens;
	}
};
#endif