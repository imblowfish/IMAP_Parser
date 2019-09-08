#include <iostream>
//#define LINUX
#ifndef LINUX
#include <conio.h>
#endif
#include <locale.h>
#include <ImapParser.h>

void MessagePageView(std::vector<Message> &messages){
	char sym;
	int messCounter = 0;
	do{
		#ifndef LINUX
		system("CLS");
		#endif
		std::cout << "Message num." << messCounter + 1 << std::endl;
		messages[messCounter].Print();
		std::cout << "l to next"<< std::endl;
		std::cout << "k to prev" << std::endl;
		std::cout << "q to exit" << std::endl;
		#ifdef LINUX
		sym = getchar();
		#else
		sym = _getch();
		#endif
		if(sym == 'k' && messCounter > 0) messCounter--;
		else if(sym == 'l' && messCounter < (int)messages.size() - 1) messCounter++;
	} while(sym != 'q');
}

int main(int argc, char *argv[]){
	setlocale(LC_ALL, "ru_RU.UTF-8");
	ImapParser parser;
	char sym;
	do{
		#ifndef LINUX
		system("CLS");
		#endif
		std::cout << "1. Bind file" << std::endl;
		std::cout << "2. Bind function" << std::endl;
		std::cout << "3. Parse" << std::endl;
		std::cout << "q to EXIT" << std::endl;
		#ifdef LINUX
		sym = getchar();
		#else
		sym = _getch();
		#endif
		std::string filename = "";
		char select = '0';
		switch(sym){
			case '1':
				std::cout << "input path to file:";
				std::cin >> filename;
				parser.BindFile(filename);
				getchar();
			break;
			case '2':
				std::cout << "1. Generate HTML" << std::endl;
				std::cout << "2. Print all messages" << std::endl;
				std::cout << "3. Show by page" << std::endl;
				do{
					#ifdef LINUX
					select = getchar();
					#else
					select = _getch();
					#endif
				} while(select < '1' || select > '3');
				switch(select){
					case '1': 
						parser.BindFunction(IMAP_GENERATE_HTML);
					break;
					case '2':
						parser.BindFunction(IMAP_PRINT_ALL);
					break;
					case '3':
						parser.BindFunction(MessagePageView);
					break;
				}
			break;
			case '3':
				if(!parser.Parse()) getchar();
			break;
		}
	} while(sym != 'q');
	return 0;
}

