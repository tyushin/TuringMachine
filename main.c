#include <stdio.h>
#include <stdlib.h>

typedef struct _progr {
    char inputsymb; //Входной символ
    char outputsymb; //Символ на выходе
    int newstate; //Новое состояние
    char action; //Действие
} progr;
/*Список*/
/*Структура узел*/
typedef struct _Item {
    char value; //Хранит значение
    struct _Item *next; //Следующий элемент
    struct _Item *prev; //Предыдущий элемент
} Item;
typedef struct _LinkedList {
    size_t length; //Хранит размер списка
    Item *head; //Начало списка
    Item *tail; //Конец списка
} LinkedList;
/*Инициализация списка*/
LinkedList* createLinkedList() {
    LinkedList *tmp;
    tmp = (LinkedList*)malloc(sizeof(LinkedList)); //Динамическое выделение памяти
    tmp->length = 0; //Обнуление размера списка
    tmp->head = tmp->tail = NULL;
    return tmp;
}
/*Функция удаления списка*/
void deleteLinkedList(LinkedList *list) {
    Item *tmp = list->head;
    Item *next = NULL;
    while (tmp) {
        next = tmp->next;
        free(tmp);
        tmp = next;
    }
    free(list);
}

void addToList(LinkedList *list, char value) {
    Item *tmp = (Item*)malloc(sizeof(Item));
        if (tmp == NULL) {
            printf("No more free memory for tape! \n");
            printf("End simulation");
            exit(0);
        }
    tmp->value = value;
    tmp->next = NULL;
    tmp->prev = list->tail;
    if (list->tail) {
        list->tail->next = tmp;
    }
    list->tail = tmp;

    if (list->head == NULL) {
        list->head = tmp;
    }
    list->length++;
}

/*Функция распечатывания списка*/
void printLinkedList(LinkedList *list, char separator, FILE *outputfile) {
    Item *tmp;
    tmp = list->head;
    while (tmp != NULL) {
        if (outputfile != NULL) {
            fprintf(outputfile, "%c", tmp->value);
        }
        else {
            printf("%c%c", tmp->value, separator);
        }
        tmp = tmp->next;
    }
    printf("\n");
}

/*Операция над лентой(алфавитом)*/
void turing(LinkedList *list, struct _progr arr[], int maxRepeat, int numberSteps, int startPosition, int debug) {
    int stop = 0; //Переменная останова
    int headstate = 1; //Состояние головки
    int commandfound; //Была ли найдена комманда
    char replace_symb; //Переменная для внесения нового значения
    char temp; //Переменная для записи значения из head->value

    int i = 0; //Переменные для счетчика
    Item *tmp; //Переменная ссылающаяся на структуру _Raw
    tmp = list->head; //Присвоение переменной ссылки на Head в списке list
    while (startPosition > 0){
        if (tmp->next == NULL) { // лента - бесконечная значит надо увеличить
            addToList(list, ' ');
        }
        tmp = tmp->next;
        startPosition--;
    }
    if (numberSteps == 0){
        numberSteps = -1;
    }
    int position;
    while (tmp != NULL && stop != 1 && numberSteps!=0) {
        commandfound = 0;
        numberSteps--;
        for (i = 0; i < maxRepeat; i++) {
            temp = tmp->value; //Присвоение переменной - значения лежащей в Head списка list
            position = (headstate-1)*maxRepeat + i;
            if (temp == arr[position].inputsymb) {
                commandfound = 1;
                if (debug) {
                    printf("Command: %cq%d-%cq%d%c\n", arr[position].inputsymb, headstate, arr[position].outputsymb, arr[position].newstate, arr[position].action);
                }
                replace_symb = arr[position].outputsymb;
                headstate = arr[position].newstate;

                if (headstate == 0) {
                    stop = 1;
                }

                tmp->value = replace_symb;
                if (debug) {
                    printf("TapeState: ");
                    printLinkedList(list, ' ', NULL);
                }
                switch (arr[position].action) {
                    case 'R': {
                        if (tmp->next == NULL) { // лента - бесконечная значит надо увеличить
                            addToList(list, ' ');
                        }
                        tmp = tmp->next;
                        break;
                    } //При команде R сдвигается на следующий элемент в списке
                    case 'L': {
                        if (tmp->prev == NULL) { // выход за границу ленты
                            printf("Out-of-tape ERROR \n");
                            printf("End simulation");
                            exit(0);
                        }
                        tmp = tmp->prev;
                        break;
                    } //При команде L сдвигается на предыдущий элемент в списке
                    case 'S': { break;} //При команде S остается на месте
                    default: {
                        printf("unknown command: %c \n", arr[position].action);
                        printf("End simulation");
                        exit(0);
                    }
                }
                break;
            }
        }
        if(commandfound!=1) {
            printf("No command found for charter %c \n", tmp->value);
            printf("End simulation");
            exit(0);
        }
    }
    printf("\n");
}

/*main*/
int main(int argc, char * argv[]) {
    LinkedList *tape = createLinkedList(); //Инициализация списка
    int numberSteps;
    int startPosition;
    int headstate = 1, len = 0, c = 0, tapelength = 0, i = 0, b = 0;
    long size = 0;
    char line[100];
    char bufferLine[255];
    char *firstpc;
    char *secpc;
    char *token;
    progr *arr; //Массив стуктуры для записи программы

    printf("Enter the number of steps (full = 0):");
    scanf("%d", &numberSteps);
    printf("Еnter the starting position (first = 0):");
    scanf("%d", &startPosition);
    /*Заполнение ленты из файла*/
    FILE *fp = fopen(argv[1], "r"); //Открытие файла
    if (fp == NULL){
        printf("Cant open file with tape \n");
        printf("End simulation");
        exit(0);
    }
    fseek(fp, 0, SEEK_END); //Проход до конца файла
    rewind(fp); //Возвращение к началу файла
    while (!feof(fp)) { //До тех пор пока не будет достигнут конец файла
        char tapeChar;
        fscanf(fp, "%c", &tapeChar); //По одному символу будет записыватся в массив
        addToList(tape, tapeChar);
        tapelength++; //Переход на следующий элемент массива
    }
    fclose(fp);

    /*Извлечение программы из файла*/
    FILE *prog = fopen(argv[2], "r");
    if (prog == NULL){
        printf("Cant open file with program \n");
        printf("End simulation");
        exit(0);
    }
    int maxState = 0;
    int nowState;
    int equalState = 0;
    int repeat = 0;
    int maxRepeat = 0;
    while (fgets(line, 100, prog) != NULL) {
        if (sscanf(line,"%*cq%d-%*cq%*d%*c\n",&(nowState)) >= 0) {
            if (nowState != equalState){
                equalState = nowState;
                if (repeat > maxRepeat){
                    maxRepeat = repeat;
                }
                repeat = 0;
            }
            if (maxState < nowState){
                maxState = nowState;
            }
        }
        else {
            printf("Error reading command. Line: %s has incorrect format \n", line);
            printf("End simulation");
            exit(0);
        }
        repeat++;
    }
    arr = (progr*)malloc((maxState*maxRepeat)* sizeof(progr)); //инициализируем массив комманд
    if (arr == NULL) {
        printf("No more free memory for program! \n");
        printf("End simulation");
        exit(0);
    }
    fseek(prog,0,SEEK_SET);
    nowState = 0;
    equalState = 0;
    i = 0;
    int position;
    while (fgets(line, 100, prog) != NULL) {
        if (sscanf(line,"%*cq%d-%*cq%*d%*c\n", &nowState) >= 0) {
            if (nowState!= equalState){
                equalState = nowState;
                i = 0;
            }
        }
        position = (nowState - 1)*maxRepeat + i;
        if (sscanf(line,"%cq%*d-%cq%d%c\n", &(arr[position].inputsymb), &(arr[position].outputsymb), &(arr[position].newstate), &(arr[position].action)) >= 0) {
            printf("%cq%d-%cq%d%c\n", arr[position].inputsymb, nowState, arr[position].outputsymb, arr[position].newstate, arr[position].action);
        }
        i++;
    }
    fclose(prog);


    /*Окончание считывания*/
    printLinkedList(tape, ' ', NULL);
    printf("Tape length %d\n", tape->length);

    turing(tape, arr, maxRepeat, numberSteps, startPosition, 1);
    printf("Result: ");
    printLinkedList(tape, ' ', NULL);
    FILE *result = fopen("result.txt", "wb");
    if (result == NULL){
        printf("Cant open file with result \n");
        printf("End simulation");
        exit(0);
    }
    printLinkedList(tape, ' ', result);
    fclose(result);
    free(arr);
    deleteLinkedList(tape);
    system("pause");
}