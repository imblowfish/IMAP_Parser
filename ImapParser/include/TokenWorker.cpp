#include "TokenWorker.h"

bool TokenWorker::CheckSymbolsCount(std::string &str, int &beg, int &end){
	beg = end = -1;
	bool found = false;
	for(unsigned int i = str.length(); i > 0; --i){
		if(str[i] == '}' && end < 0) end = i;
		if(str[i] == '{' && beg < 0) beg = i;
		if(end >= 0 && beg >= 0){
			found = true;
			break;
		}
	}
	//возвращает true, только если левая фигурная скобка находится левее правой фигурной скобки(там может быть число)
	if(found && beg < end) return true;
	return false;
}

Token TokenWorker::GetToken(std::string &str, int tokenType, bool writeInPrev){
	std::vector<std::string> splitStr;
	//разделяем строку
	SplitString(str, splitStr);
	/*
	  если производится запись в предыдущий токен(начало которого уже было найдено), 
	  то записываем все значение, иначе ищем начало токена и также записываем
	*/
	if(writeInPrev) return Token(GetTokenValue(splitStr), tokenType);
	else return Token(GetTokenValue(splitStr, tokenType), tokenType);
}

std::string TokenWorker::GetTokenValue(std::vector<std::string> &splittedStr, int tokenType){
	std::string value = "";
	bool write = false;
	for(unsigned int i = 0; i < splittedStr.size(); i++){
		//запись значения токена
		if(write) value += splittedStr[i] + " ";
		//если находим начало токена, то отмечаем начало записи значения
		if(splittedStr[i].find(tokenNames[tokenType]) != std::string::npos) write = true;
	}
	return value;
}


std::string TokenWorker::GetTokenValue(std::vector<std::string> &splittedStr){
	//т.к. нашли начало токена, то можем записывать всю строку(не требуется поиск начала токена)
	std::string value = "";
	for(unsigned int i = 0; i < splittedStr.size(); i++){
		value += splittedStr[i] + " ";
	}
	return value;
}

int TokenWorker::GetTokenTypeFromString(std::string &str){
	//осущесвляет поиск по ключевым словам, после которых находится значение токена
	for(unsigned int i = 0; i < tokenNames.size(); i++){
		if(str.find(tokenNames[i]) != std::string::npos) return i;
	}
	return IDLE;
}

void TokenWorker::SplitString(std::string &str, std::vector<std::string> &splittedStr){
	int beg = 0;
	for(unsigned int i = 0; i < str.length(); i++){
		//если обнаружили пробел(пустой символ)
		if(str[i] >= 0 && str[i] <= 32){
			//добавляем подстроку без пробелов в вектор
			if(i - beg > 0) splittedStr.push_back(str.substr(beg, i - beg));
			beg = i + 1;
		}
	}
}