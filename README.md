#ImapParser

Обработчик *.pcap файлов с обменом сообщениями по протоколу IMAP,
выделяющий заголовки и сообщения.

###Содержание
1. Об API
2. Предварительная подготовка
3. Сборка API
4. Подключение API к проекту
5. Разбор тестовой программы
    1. Сборка примера
    2. Что можно делать в примере
6. Документация по исходному коду
7. Инструкция по использованию


--------------------------------
### 1 Об API
Библиотека ImapParser предназначена для разбора файлов .pcap, 
в которых содержится переписка по протоколу IMAP без шифрования. 
Для работы необходимо привязать файл, после чего начать разбор 
файла. Вы также можете привязать какую-либо функцию, которая 
будет вызываться после завершения разбора .pcap файла.
Подробное устройство библиотеки и особенности ее использования 
описаны ниже.
   

--------------------------------
### 2 Предварительная подготовка
Для сборки API и примеров, приведенных ниже Вам потребуется
скачать несколько сторонних программ:

* [CMake](https://cmake.org) используется для сборки API и присоединения 
    API к проекту.
* [Doxygen](http://www.doxygen.nl/) для генерации документации по проекту. 
    Данное руководство постарается полностью охватить основные моменты работы
    с библиотекой, но Вам может потребоваться переписать ее или лучше понять 
    внутреннее устройство.
* [MinGW](https://sourceforge.net/projects/mingw-w64/) для Windows, либо [g++](https://losst.ru/ustanovka-gcc-v-ubuntu-16-04) для Linux.
    Требуется для сборки проектов на основе сгенерированных CMake makefile. 
    

--------------------------------
### 3 Сборка API
После того, как Вы скачали все вышеперечисленные программы нам потребуется
запустить командную строку(либо терминал для пользователей Linux). Далее
следует зайти в папку через консоль(терминал), в которой находится файл 
CMakeLists.txt. После того как вы зашли в данную папку, можно начинать
сборку библиотеки.
По умолчанию в файле CMakeLists.txt находятся команды для сборки библиотеки
и тестового проекта, однако вы можете поменять содержимое файла CMakeLists.txt
и вписать туда следующие команды:
<pre><code>
    #Устанавливаем минимальную версию CMake
    cmake_minimum_required(VERSION 3.0)
    #Название проекта
    set(PROJECT ImapLib)
    project(${PROJECT})
    #настройка путей для сборки
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    #сюда записываем путь до исходников библиотеки в папке include
    set(LIB_PATH "include/")
    #здесь прописаны все заголовочные файлы, которые требуются сборки API
    set(HEADERS ${LIB_PATH}ImapParser.h ${LIB_PATH}Message.h ${LIB_PATH}MessageHeader.h 
    ${LIB_PATH}MessageWorker.h ${LIB_PATH}Token.h ${LIB_PATH}TokenWorker.h)
    #здесь прописаны все .cpp файлы, необходимые для сборки API
    set(SOURCES ${LIB_PATH}ImapParser.cpp ${LIB_PATH}TokenWorker.cpp)
    #сборка динамической библиотеки
    add_library(ImapParser SHARED ${HEADERS} ${SOURCES})
</code></pre>

**Если проект собирается для компьютеров под управлением ОС на базе LINUX, то 
перед началом сборки следует зайти в файл ImapParser.h и раскомментировать 
там строку #define LINUX**

После этого требуется выполнить команду сборки Makefile CMake:

####Для *Windows*
>cmake . -G "MinGW Makefiles"

####Для *Linux*
>cmake . -G "Unix Makefiles"

После этого в папке должен появиться Makefile, и Вам достаточно
вызвать либо mingw32-make для Windows, либо make для Linux.
Если не было ошибок, то собранная библиотека появится в папке /bin/
в виде файла libImapParser.dll

Если же у Вас появились ошибки, попробуйте следующие варинты их решения:

* Проверьте, чтобы в пути до папки с проектом не было русских символов.
* Введите в командную строку locate g++(для Linux) или where g++(для Windows)
Если в результате ничего не будет найдено, то требуется установить соответствующий 
компилятор. Если же Вы установили его, но не выдается результат на where g++ Вам 
следует добавить MinGW в PATH вашей системы по [инструкции](https://nicothin.pro/page/windows-path)


--------------------------------
### 4 Подключение API к проекту
Если Вы собрали библиотеку и написали проект, в котором она используется, то для сборки 
проекта на CMake Вам понадобятся следующие команды в файле CMakeLists.txt:

<pre>
<code>
    cmake_minimum_required(VERSION 3.0)
    set(PROJECT YourProjectName)
    project(${PROJECT})
    
    #пути до файлов Вашего проекта
    set(YOUR_PROJECT_SOURCES main.cpp)
    #можете поменять путь для исполяемого файла .exe на свой
    #изменив "/bin" на свое название папки в CMAKE_RUNTIME_OUTPUT_DIRECTORY
    set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
    
    #Исполняемый файл
    add_executable(${PROJECT} ${YOUR_PROJECT_SOURCES})
    
    #Сюда пишем путь до папки include проекта
    set(IMAP_HEADERS "include/")
    #Сюда пишем путь до файла .dll
    set(IMAP_LIB "/bin/")
    
    #Добавляем директории с библиотеками
    include_directories(${IMAP_HEADERS})
    include_directories(${IMAP_LIB})
    #Вводим переменную в которой хранится путь до .dll файла
    set(LIB ${IMAP_LIB}libImapParser.dll)
    #привязываем библиотеку к проекту
    target_link_libraries(${PROJECT} ${LIB})
</code>
</pre>

Создаем Makefile как и в прошлый раз воспользовавшись командой:

>cmake . -G "MinGW Makefiles"(**для *Windows***)

>cmake . -G "Unix Makefiles"(**для *Linux***)

Далее компилируем исполняемый файл командой:

>mingw32-make(***Windows***), 

>make(***Linux***)

После этого исполняемый файл появится в папке bin. Куда до этого мы
собрали библиотеку. Если же файла библиотеки там нет, то следует 
поместить libImapParser.dll в папку с исполняемым файлов .exe 
Вашего проекта.

Если у Вас появились ошибки:

* CMake не может найти путь до исходных файлов библиотеки.

  >Попробуйте прописать абсолютный путь(между двойными кавычками)
  в следующих строках   файла CMakeLists.txt:

  >>set(IMAP_HEADERS "include/")

  >>set(IMAP_LIB "/bin/")

* Проект собрался, но при запуске .exe файла выдается ошибка об
  отсутствии файла libImapParser.dll.

  >Проверьте находится ли файл libImapParser.dll в папке с вашим
  .exe файлом, если нет, то скопируйте его туда

* Проект собрался, но выдает ошибку dll библиотеки о том, что невозможно
  войти в процедуру __gxx_personality_v0(**для пользователей *Windows***)

  > Откройте командную строку и попробуйте ввести туда следующие команды:

  >>where libstdc++-6.dll

  Если вы обнаружите более одной папки с данным файлом, удалите тот файл,
  который не относится к MinGW после чего пересоберит проект заново.


--------------------------------
###5 Сборка примера

####1 Сборка примера

Данный раздел покажет как собрать тестовый пример, который располагается в папке
ImapParser. Пример позволит продемонстрировать возможности API.

**Если проект собирается для компьютеров под управлением ОС на базе LINUX, то 
перед началом сборки следует зайти в файл ImapParser.h и раскомментировать 
там строку #define LINUX**

Скачайте архив с API и распакуйте. После чего зайдите через командную строку, 
либо терминал в папку с проектом, выполните команду:

>cmake . -G "MinGW Makefiles"(**для *Windows***)

>cmake . -G "Unix Makefiles"(**для *Linux***)

Далее скомпилируйте исполняемый файл командой:

>mingw32-make(***Windows***), 

>make(***Linux***)

Все возможные ошибки при сборке проекта описаны в разделах выше и особо не отличаются
для тестового проекта.
После сборки проекта исполняемый .exe файл появится в папке '/bin'. Также вы заметите,
что в папке '/bin' появились папки 'html', внутри которой находится файл 'style.css'
и также еще одну папку 'files', в которой находится 2 .pcap файла.

####2 Что можно делать в примере

Папка 'html' требуется для генерации html-документа на основе прочитанных сообщений,
файл style.css позволяет отобразить html-документ в более цивильном виде. 
Папка 'files' в свою очередь удобна для файлов, которые затем понадобится разобрать
средствами API ImapParser.
После того, как вы собрали тестовый пример, можете попробовать в деле некоторые функции,
которые в нем прописаны, а именно:

* Привязка файла. Причем можно прописывать как относительный путь до папки files, так и 
абсолютный до любого .pcap файла

* Привязка функции, которая будет вызвана после разбора файла. На данный момент их 3:
    * Функция генерации html-документа на основе прочитанных сообщений.
    html-документ сгенерируется в папку 'bin/html' и будет называться messages.html.
    Вы можете запустить его в любом браузере и просмотреть все сообщения
    * Функция вывода всех сообщений на экран
    * Функция вывода сообщений постранично(каждый на отдельной странице)

* Запуск разбора привязанного файла. Отметим, что разбор невозможен, если вы не привязали
файл. На привязку функций это не распространяется, если вы не привязали функцию, то разбор
начнется и завершится без каких-либо ошибок и предупреждений.

**На Windows при сборке проекта средствами MinGW вместо русских букв могут выводиться
непонятные символы, в этой ситуации для просмотра содержимого сообщений рекомендуется
генерировать html-страницу и смотреть сообщения в браузере.**


--------------------------------
###6 Документация по исходному коду

Возможно вы захотите узнать подробности о функциях в составе API или понять, как они 
взаимосвязаны.Тогда Вам потребуется документация, в которой расписано внутреннее 
устройство API ImapParser.

В папке с проектом Вы можете найти файл doxygen_config, если до этого Вы уже установили 
Doxygen, то от Вас потребуется только зайти в папку через командную строку или терминал, 
и ввести команду:

>doxygen doxygen_config

После чего документация по проекту будет сгенерирована в папку documentation. Зайдя в 
нее и открыв файл index.html у Вас откроется полная документация по проекту. 


###7 Инструкция по использованию

Ранее уже было описано подключение API к проекту, сборка самого API и возможности тестового
примера.
Здесь же все это будет описано более подробно.

От Вас требуется вписать:

>```#include <ImapParser.h>```

в вашем проекте. 

Объявить экземпляр класса ImapParser, назовем его parser:

>ImapParser parser;

Вся работа с API проходит через данный экземпляр. Но чтобы ему было с чем работать, 
следует привязать файл для разбора в формате .pcap, в котором происходит обмен 
сообщеняими по протоколу IMAP.

Привязка файла происходит при помощи функции 'BindFile(std::string filepath)'. Давайте 
привяжем для разбора файл под названием 'belovedGrandmotherMessage.pcap' который находится 
в папке 'files' проекта, для этого напишем следующее:

>parser.BindFile("files/belovedGrandmotherMessage.pcap");

После этого, вы должны увидеть сообщение о том, что файл успешно привязан:

>Bind file with name:files/belovedGrandmotherMessage.pcap<br>
 Bind success

Теперь мы можем начать разбор, после которого, вызвав функцию 'GetMessages()' вы сможем
получить ссылку на вектор всех сообщений, которые содержались в .pcap файле. Или же 
мы можем сделать еще одну интересную вещь: привязать функцию, которая вызовется автоматически
после разбора сообщения.

Есть два варианта сделать это:

* Привязать одну из функций-членов класса ImapParser, список которых вы можете получить, если
введете 'IMAP_' в своей среде разработке(если та поддерживает автозаполнение), либо посмотрев
содержимое множества FunctionTypes в документации. Чтобы сделать это следует написать следующее:
>parser.BindFunction(IMAP_NAME_OF_FUNCTION);

* Привязать написанную собственноручно функцию. Для таких функций есть ограничение:
    * Они не должны ничего возвращать
    * Функция имеет единственный параметр, который является ссылкой на вектор типа Message
Пример подобной функции:
<pre>
<code>
		void example_function(std::vector&ltMessage&gt messages){
			//do something
		}
</code>
</pre>
Для привязки своей функции(для примера возьмем описанную выше example_fuction)
следует наипсать следующее:
>parser.BindFunction(example_function);

**На привязку функций, которые будут вызваны после разбора .pcap файла действует еще одно 
ограничение. *Вы можете привязать либо функцию-член класса, либо собственноручно написанную
функцию*. Если вы попробуете привязать обе функции, то вызовется та, которая была привязана
последней.**
    