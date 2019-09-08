#ifndef TOKEN_H
#define TOKEN_H
#include <iostream>
#include <string>
#include <vector>

///Множество возможных значений токена
enum TokenType{
	R_FROM,					///<Токен From(от кого)
	R_REPLY_TO,				///<Токен In-Reply-To(в ответ на что)
	R_TO,					///<Токен To(кому)
	R_RETURN_PATH,			///<Токен Return-Path(обратный адрес)
	R_SUBJECT,				///<Токен Subject(Тема)
	R_DATE,					///<Токен Date(Дата)
	R_ID,					///<Токен Message-ID(идентификатор сообщения)
	R_REFERENCE,			///<Токен References(сноски на другие сообщения)
	R_RECEIVED,				///<Токен Received(информация о том, откуда было получено сообщение)
	R_USER_AGENT,			///<Токен User-Agent(имя почтовой службы)
	R_MIME,					///<Токен MIME-Version(версия стандарта передачи данных)
	R_CONTENT_TYPE,			///<Токен Content-Type(наиболее полное описание данных, содержащихся в теле)
	R_CONT_TRANS_ENCODING,	///<Токен Content-Transfer-Encoding(тип кодирования информации в сообщении)
	R_CONTENT_LANGUAGE,		///<Токен Content-Language(язык сообщения)
	R_MESSAGE,				///<Токен Message(текст сообщения)
	IDLE					///<Токен Idle(ни один из вышеперечисленных, игнорируется при разборе)
};

/*!
	\details Вектор содержит текстовое наименование токенов. 
	Используется при выводе сообщений и поиске ключевых слов при разборе
*/
static const std::vector<std::string> tokenNames = {"From:",
													"In-Reply-To:",
													"To:",
													"Return-Path:",
													"Subject:",
													"Date:",
													"Message-ID:",
													"References:",
													"Received:",
													"User-Agent:",
													"MIME-Version:",
													"Content-Type:",
													"Content-Transfer-Encoding:",
													"Content-Language:"
													};

/*!
	\brief Класс токена, минимального сегмента при разборе файла
*/
class Token{
	public:
		int type;				///< Тип токена
		std::string value;		///< Значение токена
		/*!
			\brief Конструктор по умолчанию
			\details Присваивает значение type равное IDLE и value равное пустой строке
		*/
		Token(){
			type = IDLE;
			value = "";
		}
		/*!
			\brief Конструктор с  параметрами
			\param value Значение токена
			\param type Тип токена, принимает любое значение из множества TokenType
		*/
		Token(std::string value, int type){
			this->value = value;
			//проверка на значение типа токена
			if(type >= R_FROM && type <= IDLE) this->type = type;
			else this->type = IDLE;
		}
};

#endif