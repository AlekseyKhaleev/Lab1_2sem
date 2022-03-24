/* Данный модуль содержит определения функций, необходимых для корректной работы программы Lab1 */

#pragma once

/* Подключение модулей */
#include "definitions.h"
#include <string>
#include <termios.h>

/* Изменение параметров терминала в для корректной работы программы в соответствии с заданием */
void CustomizeTerminal();

/* Формирование и вывод логотипа программы в окно терминала */
void PrintLogo(char file_name[]);

/* Вывод строки минимальной справки в зависимости от причины вызова */
void PrintQuickHelp(const std::string& reason);

/* Формирование и вывод справочного экрана в окно терминала */
void PrintHelpScreen(const std::string& reason);

/* Вычисление количества строк в файле */
int StringCounter(char* path, std::string str_buffer);

/* Инициализация массива строк (состояний экрана) строками, считанными из файла */
void InitScrStates(char* path, std::string str_buffer, std::string* screen_states);

/* Подсветка синтаксиса (ключевых слов из соответствующего массива) в строке посредством дополнения */
void SyntaxHighLighting(std::string& target_string);

/* Считывание кода нажатой клавиши в терминале без промежуточной буферизации */
int UserKey();

/* Аварийное завершение программы с предварительным выводом справочной информации */
void RaiseError();

/* Функция вызова исключения связанного с выделением памяти для передачи в set_new_handler() */
void NewError();

