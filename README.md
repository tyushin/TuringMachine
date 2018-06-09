Техническое задание по теме: 
«Симулятор машины Тьюринга с односторонней лентой»

Входные и выходные файлы в формате .txt:
tape.txt – имя файла содержащего значения входной ленты
program.txt – имя файла содержащего описание программы
result.txt – имя выходного файла

Формат входных данных:
Файл «tape.txt» содержит последовательно записанные символы входной ленты.
Пример tape.txt: 
10001

В файле «program.txt» записывается описание состояний (каждая команда описывается с новой строки)
Пример program.txt: 
1q1-1q1R
0q1-1q2L
1q2-1q0R

Название состояния начинается с q, после чего указывается номер, начиная с 1.
Пример: 
q2

Начальным состоянием является q1 состояние, с него будет начинаться исполнение программы.
q0 - является конечным состоянием. При переходе в него программа завершает своё выполнение.

Пустой символ обозначается пробелом.
Пример:
 q4- q7R
 
Состояния во входном файле описываются следующим образом:
Значение в текущей ячейки ленты, после чего номер состояния, после разграничивающий символ - ,сразу за ним значение, которое будет записано в ячейку, затем следующее состояние и направление движения каретки.
Пример: 
1q1-0q2R

Направление движения каретки описывается одним из символов:
L – сдвиг влево.
R – сдвиг вправо.
S – сдвиг не выполнять.

Формат выходных данных:
Значение ленты после выполнения программы будет записано в файл result.txt.
Также при выполнении программы, с фиксированным числом шагов, значение ленты будет записано в выходной файл result.txt.
Пример result.txt: 
11001

Алгоритм работы:
Прочитать значение ленты в файле tape.txt и программу работы в файле program.txt, после чего по данным, полученным из файла program.txt составить таблицу состояний, по которой будет анализироваться дальнейшее поведение прохождения по ленте. Начинать выполнение программы с начального состояния (q1) и выполнять до перехода в конечное состояние(q0). В конце выполнения вывести полученную ленту в файл result.txt.
Если по ходу выполнения программы каретка попытается выйти за границу ленты (т.к. она односторонняя, следовательно, она ограничена слева), то программа завершит свою работу и напишет об этой ошибке.

Метод испытания: 
Набор тестовых файлов (tape.txt и program.txt) и ожидаемых результатов, которые будут подтверждать работоспособность симулятора машины Тьюринга с односторонней лентой.
