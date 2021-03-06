// Lab1.cpp: Листинг программы для Задания 1, 1-й лабораторной работы 2-го семестра
// Вариант 8
//*********************************************************************************
// Имя файла: Lab1.cpp
// Резюме: Пример программы написанной на языке программирования С++
// Описание:
// Программа позволяет просматривать текстовые файлы,
//  получать справку и прокручивать экран
// - для корректного вызова программы требуется указать путь к файлу
// - при запуске программы без параметров отображается справочная страница
//  Программа поддерживает некоторые горячие клавиши:
// - PgUp, PgDown - прокрутка текста;
// - h - вызов справки;
// - r - возврат к текстовому файлу;
// - ESC - завершение программы;
// Дата создания: 2022 / 03 / 01
// Автор : Студент НГТУ ИРИТ, Халеев А.А., гр. 21-ИВТз
// Платформа : OS Ubuntu 20.04, Jet Brains Clion IDE (Trial)
// *********************************************************************************

/* Подключение модулей */
#include "func.h"
#include <cerrno>
#include <iostream>

/* Для корректной работы программы необходимо передать путь к текстовому файлу в качестве аргумента командной строки */
int main(int argc, char* argv[]) {
    /* Обработка параметров командной строки при запуске программы*/
    if (argc != 2){  // Если не получено аргументов командной строки, или их количество больше одного
        RaiseError(); // Вызов функции (печать справки и завершение программы с помощью "горячей" клавиши)
    }
    CustomizeTerminal(); // Очистка экрана и применение нужных параметров консоли
    errno = 0;  // Переменная модуля errno.h, хранящая целочисленный код последней ошибки. 0 - отсутствие ошибок
    std::set_new_handler(NewError); // Назначение функции, которая будет вызвана при получении ошибки выделения памяти
    try {
        /* Выделение динамической памяти */
        std::string str_buffer; // Буферная строка для построчного считывания исходного файла

        /* Определение количества строк, содержащихся в целевом текстовом файле */
        int file_len = StringCounter(argv[1], str_buffer);

        /* Выделение динамической памяти */
        auto screen_states = new std::string[file_len]; // Двумерный массив "Состояний экрана"

        /* Инициализация массива состояний экрана соответствующими строками (буферами) */
        InitScrStates(argv[1], str_buffer, screen_states);

        /* Определение индекса начального экрана */
        int state_index = 0;

        /* Формирование содержимого экрана */
        PrintLogo(argv[1]); // Печать логотипа программы
        clear_file(); // Очистка части экрана, необходимой для отображения текущей части текстового файла
        std::cout << screen_states[state_index]; // Вывод текущей части текстового файла
        PrintQuickHelp("text_screen"); // Вывод строки, содержащей минимальную справочную информацию

        /* Блок управления программой с помощью функциональных клавиш */
        int mark = true; // флаг-индикатор продолжения/завершения программы
        while(mark) { // бесконечный цикл считывания клавиш
            switch (UserKey()) { // считывание кода клавиши введенной пользователем в режиме "реального времени"

                /* Клавиша ESC завершит цикл считывания клавиш */
                case Escape: mark = false; break;
                /* клавиша PgUp осуществляет прокрутку текста "вверх" */
                case Up:
                    if (state_index > 0){ // если текущий экран не является стартовым
                        state_index--; // уменьшить на 1 индекс текущего состояния экрана (текста, прокрутки)
                        clear_file(); // Очистить часть экрана, необходимую для вывода текста
                        std::cout << screen_states[state_index]; // Вывод текущей (прокрученной) части текстового файла
                        PrintQuickHelp("text_screen");  // Вывод строки, содержащей минимальную справочную информацию
                    }
                    break;
                /* клавиша PgDown осуществляет прокрутку текста "вниз" */
                case Down:
                    if (state_index < file_len - (SCREEN_HEIGHT - LOGO_LEN)){ // если текущий экран не является последним
                        state_index++; // увеличить на 1 индекс текущего состояния экрана (текста, прокрутки)
                        clear_file(); // Очистить часть экрана, необходимую для вывода текста
                        std::cout << screen_states[state_index]; // Вывод текущей (прокрученной) части текстового файла
                        PrintQuickHelp("text_screen"); // Вывод строки, содержащей минимальную справочную информацию
                    }
                    break;
                /* клавиша h осуществляет вызов справочной страницы */
                case Help: {
                    PrintHelpScreen("help_screen"); // Вывод справочного экрана
                    int h_mark = true; // флаг-индикатор продолжения/завершения показа справочного экрана
                    while (h_mark){ // бесконечный цикл считывания клавиш
                        switch(UserKey()){ // считывание кода клавиши введенной пользователем в режиме "реального времени"

                            /* Клавиша ESC завершит цикл считывания клавиш как в режиме справки, так и в основном цикле
                             * и приведет в последствии к завершению программы*/
                            case Escape:{
                                h_mark = false;
                                mark = false;
                                break;
                            }
                            /* Клавиша r завершит цикл считывания клавиш в режиме справки, просмотр текста продолжится
                             * с экрана на котором была вызвана справка */
                            case Return:{
                                PrintLogo(argv[1]);
                                clear_file();
                                std::cout << screen_states[state_index];
                                PrintQuickHelp("text_screen");
                                h_mark = false;
                                break;
                            }
                            default: break;  // игнорирование любых клавиш, поведение которых неопределено
                        }
                    }
                    break;
                }
                default: break; // игнорирование любых клавиш, поведение которых неопределено
            }
        }
        reset_screen(); // Очистка экрана, буфера прокрутки и восстановление исходных графических параметров терминала

        /* Освобождение динамической памяти */
        delete[] screen_states;
    }
    catch (std::exception e) { // Обработка исключения, связанного с выделением динамической памяти
        perror("Memory allocation error");
    }
    return 0; // Завершение работы программы с кодом 0
}
