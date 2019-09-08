#include "ImapParser.h"

bool ImapParser::BindFile(std::string filepath){
	if(pcapFile.is_open()) pcapFile.close();
	pcapFile.open(filepath, std::ios::binary);
	if(!pcapFile.is_open()){
		std::cout << "BIND FILE ERROR: Can't open file" << std::endl;
		getchar();
		return false;
	}
	std::cout << "Bind file " << filepath << " success" << std::endl;
	getchar();
	return true;
}

bool ImapParser::BindFunction(void(&function)(std::vector<Message>&)){
	//записываем адрес функции в указатель
	funcPointer = function;
	if(!funcPointer){
		std::cout << "ImapParser BindFunction Error" << std::endl;
		getchar();
		return false;
	}
	//обнуляем указатель на функцию-член
	classFuncPointer = nullptr;
	std::cout << "Bind function success" << std::endl;
	getchar();
	return true;
}

bool ImapParser::BindFunction(int functionType){
	if(functionType < IMAP_GENERATE_HTML || functionType > IMAP_PRINT_ALL) {
		std::cout << "ImapParser BindFunction Error: Wrong function type" << std::endl;
		getchar();
		return false;
	}
	//в зависимости от выбора функции запоминаем адрес в указатель на функцию-член класса
	switch(functionType){
		case IMAP_GENERATE_HTML:
			classFuncPointer = &ImapParser::GenerateHTML;
			break;
		case IMAP_PRINT_ALL:
			classFuncPointer = &ImapParser::PrintAll;
			break;
	}
	if(!classFuncPointer){
		std::cout << "ImapParser BindFunction Error" << std::endl;
		return false;
	}
	//обнуляем указатель на функцию
	funcPointer = nullptr;
	std::cout << "Bind function success" << std::endl;
	getchar();
	return true;
}

void ImapParser::GenerateHTML(std::vector<Message> &messages){
	//если сообщений нет, нечего выводить
	if(messages.size() == 0){
		std::cout << "ImapParser GenerateHTML Error: messages size = 0" << std::endl;
		getchar();
		return;
	}
	//генерация html страницы для просмотра сообщений
	#ifdef LINUX
	std::ofstream htmlFile("./bin/html/message.html", std::ios::out);
	#else
	std::ofstream htmlFile("html/messages.html", std::ios::out);
	#endif
	std::string pathToStyle = "style.css";
	if(!htmlFile.is_open()){
		std::cout << "GenerateHTML Error:Can't open file" << std::endl;
		return;
	}
	htmlFile << "<!DOCTYPE><html><head>" << std::endl;
	htmlFile << "<meta charset = \"utf-8\">" << std::endl;
	htmlFile << "<title>Messages</title>" << std::endl;
	htmlFile << "<link href = \"" << pathToStyle << "\" rel = \"stylesheet\"" << std::endl;
	htmlFile << "</head><body>" << std::endl;
	for(unsigned int i = 0; i < messages.size(); i++){
		htmlFile << "<table border = \"1\">" << std::endl;
		//выводим номер сообщения
		htmlFile << "<caption>Message num." << i + 1 << "</caption>" << std::endl;
		//получаем заголовок сообщения по индексу
		std::vector<Token> headerTokens = messages[i].GetHeaderTokens();
		//выводим значения токенов заголовка
		for(unsigned int j = 0; j < headerTokens.size(); j++){
			htmlFile << "<tr>" << std::endl;
			htmlFile << "<td>" << tokenNames[headerTokens[j].type] << "</td>" << std::endl;
			htmlFile << "<td>";
			//если в строке есть символы, которые создадут проблемы при просмотре в html, заменяем их аналогами из html
			if(headerTokens[j].value.find("<") != std::string::npos || headerTokens[j].value.find(">") != std::string::npos || headerTokens[j].value.find('\n') != std::string::npos){
				for(unsigned int k = 0; k < headerTokens[j].value.length(); k++){
					if(headerTokens[j].value[k] == '<') htmlFile << "&lt;";
					else if(headerTokens[j].value[k] == '>') htmlFile << "&gt;";
					else if(headerTokens[j].value[k] == '\n') htmlFile << "<br>";
					else htmlFile << headerTokens[j].value[k];
				}
			} else htmlFile << headerTokens[j].value << std::endl;	//иначе просто выводим
			htmlFile << "</td>" << std::endl;
			htmlFile << "</tr>" << std::endl;
		}
		htmlFile << "<td colspan = 2>";
		std::string messageValue = messages[i].GetMessage();
		//выводим сообщение
		for(unsigned int i = 0; i < messageValue.length(); i++){
			//заменяем символы, которые могут создать проблемы при просмотре в html аналогами
			if(messageValue[i] == '\n') htmlFile << "<br>" << std::endl;
			else htmlFile << messageValue[i];
		}
		htmlFile << "</td>" << std::endl;
		htmlFile << "</table>" << std::endl;
	}
	htmlFile << "</body></html>" << std::endl;
	std::cout << "ImapParser GenerateHTML success" << std::endl;
	getchar();
	htmlFile.close();
}

bool ImapParser::Parse(){
	if(!pcapFile.is_open()){
		std::cout << "PARSE FILE ERROR: File is not bound" << std::endl;
		return false;
	}
	//переходим в начало файла
	pcapFile.clear();
	pcapFile.seekg(0);
	//очищаем список сообщений, если они там до этого были
	mWorker.ClearAll();
	//вводим переменные для запоминания токенов
	int nextState = IDLE, prevState = IDLE;
	//переменные для хранения индексов фигурных скобок в строке, между которыми находится число символов в сообщении
	int beg = -1, end = -1;
	//переменные для хранения количества символов в заголовке и общего количества символов в сообщении
	int headerLength = 0, fullMessageLength = 0;
	//считываем построчно
	while(getline(pcapFile, parseStr)){
		//если считываем сообщение
		if(nextState == R_MESSAGE){
			//рассчитываем, сколько символов уже занимает сообщение
			int messageLength = headerLength + messageStr.length() + parseStr.length();
			//если оно меньше общего количества символов на сообщение, то записываем
			if(messageLength <= fullMessageLength){
				messageStr += parseStr + '\n';
				continue;
			} else{
				//иначе добавляем значение сообщения в токен
				mWorker.AddMessageValue(messageStr);
				//очищаем строку
				messageStr.clear();
				//сбрасываем переменные для запоминания токенов
				nextState = prevState = IDLE;
			}
		}
		/*
			проверяем, есть ли в строке число между фигурными скобками.
			попутно отправляем в качестве параметров переменные, в которые
			будет записываться индекс левой и правой фигурных скобок
		*/
		if(tWorker.CheckSymbolsCount(parseStr, beg, end)){
			//если есть, узнаем длину сообщения в символах
			fullMessageLength = tWorker.GetSymbolsNum(parseStr, beg, end);
		}
		//узнаем, какой токен есть в строке
		nextState = tWorker.GetTokenTypeFromString(parseStr);
		if(nextState != IDLE){		//если токен есть
			//добавляем токен в заголовок
			mWorker.AddToken(tWorker.GetToken(parseStr, nextState));
			//запоминаем последний прочитанный токен
			prevState = nextState;
		} else if(prevState != IDLE){ //если следующий токен не обнаружен, но предыдущий не IDLE
			//записываем строку в токен, отправляя true в качестве значения параметра writeInPrev
			Token token = tWorker.GetToken(parseStr, prevState, true);
			//если длина значения токена = 0, сбрасываем последний токен в IDLE
			if(token.value.length() == 0) prevState = IDLE;
			else mWorker.AddInPrevToken(token);					//иначе, дополняем предыдущий токен
		}
		//если предыдущее состояние = Content-Language(после него обычно располагается сообщение)
		if(prevState == R_CONTENT_LANGUAGE){
			//узнаем длину заголовка в символах
			headerLength = mWorker.GetLengthOfBody();
			//обозначаем, что дальше будет токен сообщения
			nextState = R_MESSAGE;
		}
	}
	//вызываем функцию для того, чтобы сохранить последнее временное сообщение
	mWorker.ReadingIsOver();
	std::cout << "Parse success" << std::endl;
	getchar();
	//вызываем функцию, которая была привязана, либо проскакиваем данный пункт
	if(classFuncPointer) (this->*classFuncPointer)(mWorker.GetAllMessages());
	if(funcPointer) funcPointer(mWorker.GetAllMessages());
	return true;
}

void ImapParser::PrintAll(std::vector<Message> &messages){
	std::cout << "Count of messages is " << messages.size() << std::endl;
	for(unsigned int i = 0; i < messages.size(); i++){
		std::cout << "Message num." << i + 1 << std::endl;
		messages[i].Print();
	}
	getchar();
}