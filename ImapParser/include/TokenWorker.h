﻿#ifndef TOKEN_WORKER_H
#define TOKEN_WORKER_H
#include "Token.h"

/*!
	\brief Класс работы с токенами
	\details Содержит функции для работы со строками, определения типа токена в строке и преобразования строк в токены
*/

class TokenWorker{
private:
	/*!
		\brief Функция разделения строки на отдельные слова
		\param str Ссылка на строку, которую требуется разделить
		\param splittedStr Ссылка на вектор, куда будет помещены слова из разделенной строки
	*/
	void SplitString(std::string &str, std::vector<std::string> &splittedStr);
	/*!
		\brief Получение значение токена из вектора слов с предварительным поиском начала токена
		\param splittedStr Ссылка на вектор, где содержатся слова
		\param tokenType Тип токена, значение которого функция будет искать в splittedStr
		\return Значение токена типа std::string
	*/
	std::string GetTokenValue(std::vector<std::string> &splittedStr, int tokenType);
	/*!
		\brief Получение значение из вектора слов без предварительного поиска начала токена
		\param splittedStr Ссылка на вектор, где содержатся слова
		\return Значение токена типа std::string
	*/
	std::string GetTokenValue(std::vector<std::string> &splittedStr);
public:
	/*!
		\brief Опредление типа токена, содержащегося в строке
		\param str Ссылка на строку, в которой осуществляется поиск
		\return Тип токена из множества TokenType
	*/
	int GetTokenTypeFromString(std::string &str);
	/*!
		\brief Определяет токен из строки по типу
		\param str Ссылка на строку, в которой осуществляется поиск
		\param tokenType Тип токена, значение которого ищется в str
		\param writeInPrev false - если производится поиск нового токена, true - если записывается значение в предыдущий найденый токен(по умолчанию false)
		\return Token со значением и типом
	*/
	Token GetToken(std::string &str, int tokenType, bool writeInPrev = false);
	/*!
		\brief Осуществляет поиск в строке количества символов в последующем сообщении
		\details Число содержится между фигурными скобками:{число}. После работы записывает в переменные, переданные
				 по ссылке значения индексов левой и правой фигурной скобок для дальнейшего нахождения подстроки в str,
				 содержащей только число и преобразования строкового представления числа в целочисленное
		\param str Ссылка на строку, в которой осуществляется поиск
		\param beg Ссылка на переменную, в которую будет записан индекс левой фигурной скобки
		\param end Ссылка на переменную, в которую будет записан индекс правой фигурной скобки
		\return true или false, в зависимости от того, было найдено число или нет
	*/
	bool CheckSymbolsCount(std::string &str, int &beg, int &end);
	/*!
		\brief Преобразует подстроку с число по найденным ранее индексам фигурных скобок в целочисленное значение
		\param str Ссылка на строку, в которой содержится число
		\param beg Ссылка на переменную, в которую записан индекс левой фигурной скобки
		\param end Ссылка на переменную, в которую записан индекс правой фигурной скобки
		\return Целочисленное значение строкового представления длины сообщения
	*/
	int GetSymbolsNum(std::string &str, int beg, int end){
		return atoi(str.substr(beg + 1, end - beg - 1).c_str());
	}
};

#endif