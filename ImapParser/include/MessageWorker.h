#ifndef MESSAGE_WORKER_H
#define MESSAGE_WORKER_H
#include "Message.h"

/*!
	\brief Класс осуществляет работу с сообщениями и их хранение по мере работы программы
*/

class MessageWorker{
	private:
		std::vector<Message> messages;											///< Вектор, который сообщения
		Message tmpMessage;														///< Временное сообщение, в которое записываются токены, после заполнения добавляется в вектор messages
		int lastToken = IDLE;													///< Хранит тип последнего записанного токена
		/*!
			\brief Определяет по типу токена окончание записи заголовка
			\details Если тип токена переданного в параметре меньше последнего записанного токена
					 и тип токена > токена типа Content-Type
					 (обычно в токене данного типа и больших его заканчивается сообщение),
					 то заголовок закончился
			\param tokenType Тип токена
			\return true или false, в зависимости от того, закончился заголовок или нет
		*/
		bool EndOfHeader(int tokenType){
			if(tokenType < lastToken && lastToken >= R_CONTENT_TYPE) return true;
			return false;
		}
		/*!
			\brief Помещение временного сообщения tmlMessage в вектор сообщения messages для хранения
		*/
		void SaveMessage(){
			int idx = GetMessageById();
			if(idx < 0){
				//добавляем новое сообщение, только если присутствует id
				if(tmpMessage.GetId().length() > 0) messages.push_back(tmpMessage);
			}
			else messages[idx].CompleteMessage(tmpMessage); //если нашли сообщение с таким же id, то дополяем его
			//очищаем временное сообщение
			tmpMessage.Clear();
		}
		/*!
			\brief Возвращает индекс сообщения из вектора messages
			\details Сравнивает значение id каждого сообщения со значением id временного сообщения tmpMessages
			\return Индекс сообщения из вектора messages с id, либо -1, если id не было найдено
		*/
		int GetMessageById(){
			if(tmpMessage.GetId().length() == 0) return -1;
			for(unsigned int i = 0; i < messages.size(); i++){
				if(messages[i].GetId() == tmpMessage.GetId()){
					return i;
				}
			}
			return -1;
		}
	public:
		~MessageWorker(){
			ClearAll();
		}
		/*!
			\brief Добавляет токен во временное сообщение
			\details Сперва проверяет, закончен ли заголовок, если закончен, сохраняет сообщение, 
					 иначе добавляет токен в заголовок
			\param token Токен, который требуется добавить в заголовок
		*/
		void AddToken(Token token){
			//если закончился ввод заголовка и временное сообщение не пусто
			if(EndOfHeader(token.type) && !tmpMessage.Empty()){
				//сохраняем сообщение
				SaveMessage();
			}
			//добавляем новый токен в заголовок
			tmpMessage.AddInHeader(token);
			//запоминаем последний токен
			lastToken = token.type;
		}
		/*!
			\brief Дополнение значения токена в заголовке
			\param token Токен, значение которого требуется дополнить в заголовке
		*/
		void AddInPrevToken(Token token){
			//дополняем заголовок
			tmpMessage.CompleteHeader(token);
			//запоминаем последний токен
			lastToken = token.type;
		}
		/*!
			\brief Записывает значение сообщение
			\param str Ссылка на строку, в которой хранится сообщение
		*/
		void AddMessageValue(std::string &str){
			tmpMessage.AddMessage(str);
		}
		/*!
			\brief Функция, которая записывает временное сообщение, 
				   если больше нет токенов и чтение файла .pcap завершилось
		*/
		void ReadingIsOver(){
			if(!tmpMessage.Empty()) SaveMessage();
			tmpMessage.Clear();
		}
		///Возвращает количество символов в заголовке
		int GetLengthOfBody(){
			return tmpMessage.GetHeaderLength();
		}
		///Возвращает ссылку на вектор сообщений, обнаруженных в файле .pcap
		std::vector<Message>& GetAllMessages(){
			return messages;
		}
		///Очищает вектор messages и временное сообщение tmpMessages
		void ClearAll(){
			tmpMessage.Clear();
			messages.clear();
		}
};

#endif