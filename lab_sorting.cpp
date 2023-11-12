/***********************************************************************************
*                         Программирование                                         *
************************************************************************************
*Project type :Win64 Console Application                                           *
*Project name :lab_sorting.sln                                                     *
*File name    :lab_sorting.cpp                                                     *
*Language     :CPP, MSVS 2022                                                      *
*Programmers  :Кожевников Артем Вадимович, Томчук Дмитрий Максимович,  М3О-209Б-22 *
*Modified By  :                                                                    *
*Created      :08.11.2023                                                          *
*Last revision:29.10.2023                                                          *
*Comment      :                                                                    *
***********************************************************************************/

#include <chrono>
#include <iostream>
#include <fstream>
using namespace std;

/*********************************************************************
 *              П Р О Т О Т И П Ы    Ф У Н К Ц И Й                   *
 *********************************************************************/

int quickSort(int* Array, int begin, int end, int* answer); // Быстрая сортировка
int Partition(int* Array, int begin, int end, int* answer); // Входит в быструю сортировку
void generate(int* array, int len); // Генерация нового массива
void linearAscendINT(int* Array, int len); //Генерация возрастающей последовательности
void linearDescendINT(int* Array, int len); //Генерация убывающей последовательности
void printArray(int* Array, int len); // Печать массива 
void noteRes(const char* filename, int lenArray, int time, int is_compare); // Запись в файл

/*******************************************************************/
/*           Г Л О Б А Л Ь Н Ы Е   П Е Р Е М Е Н Н Ы Е             */
/*******************************************************************/

// К О Н С Т А Н Т Ы



// П Е Р Е М Е Н Н Ы Е
void(*func_array[3])(int*, int) = {generate, linearAscendINT, linearDescendINT}; // МАССИВ ФУНКЦИЙ ФОРМИРОВАНИЯ МАССИВОВ
int answer[3] = { 0, 0, 0 }; // Результат работы алгоритма: время, количество сравнений и перестановок
int* main_array; // массив
int allLengths[4] = { 15, 10000, 50000, 100000 };
int lenMain;

/*********************************************************************
 *              Г Л А В Н А Я   П Р О Г Р А М М А                    *
 *********************************************************************/

int main()
{
    setlocale(LC_ALL, "Russian");
    // TODO: проблема с размером 10000 Я ХЗ
    // TODO: промежуточные вычисления
    // TODO: запись в файл так, чтобы можно было получить табличку
    
    for (int i = 0; i < 4; i++) {
        lenMain = allLengths[i];
        cout << "Длина массива = " << lenMain << endl;
        for (int j = 0; j < 3; j++)
        {
            // Быстрая сортировка
            main_array = new int[lenMain];
            memset(answer, 0, 3); // Обнуление массива answer
            func_array[j](main_array, lenMain);
            //if (lenMain == 15) printArray(main_array, lenMain);
            cout << "Случай массива (j): " << j << endl;

            auto begin = std::chrono::steady_clock::now();
            quickSort(main_array, 0, lenMain - 1, answer);
            auto end = std::chrono::steady_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            answer[0] = elapsed_ms.count();
            cout << endl;
            //if (lenMain == 15) printArray(main_array, lenMain);
            //cout << "Время работы QS: " << answer[0] << " Количество сравнений: " << answer[1] << " Количество перестановок: " << answer[2] << endl;

            // Сортировка вставкой

        }
    }
    

} // main


/***********************************************************************
 *                Р Е А Л И З А Ц И Я    Ф У Н К Ц И Й                 *
 ***********************************************************************/

int quickSort(int* Array, int begin, int end, int* answer)
{
    //cout << "begin = " << begin << " end = " << end << endl;
    if (begin >= end || end < 0 || begin < 0) return 0;
    answer[1]++; // Счет сравнений
    int qIndex = begin; int greaterIndex = begin;
    Partition(Array, begin, end, answer);
    quickSort(Array, begin, qIndex - 1, answer);
    quickSort(Array, qIndex+1, end, answer);
} // quickSort

 
int Partition(int* Array, int begin, int end, int&Q, int&greaterQ, int* answer)
{ // Проблема на размере 10000
    int n = (end - begin) + 1;
    //int qIndex = begin; 
    int pivot = rand() % n + begin; // Работает до 50000, БЛЯТЬ! Наверное, идея с Greater тоже влияет -- использовать ее
    int t = 0;
    for (int u = begin; u <= end; u++)
    {
        if (Array[u] == Array[pivot])
        {
            swap(Array[u], Array[greaterQ]);
            greaterQ++;
            answer[2]++; // Счет перестановок
        } // if
        if (Array[u] < Array[pivot])
        {
            swap(Array[u], Array[greaterQ]);
            swap(Array[greaterQ], Array[Q]);
            greaterQ++;
            Q++;
            answer[2]+=2; // Счет перестановок
        } // if
        answer[1] += 3; // Счет сравнений
    } // for u
    t = Array[pivot];
    Array[pivot] = Array[qIndex];
    Array[qIndex] = t;
    answer[2]++; // Счет перестановок
    return qIndex;
} // Partition

void generate(int* array, int len)
{
    for (int i = 0; i < len; i++)
    {
        array[i] = rand();
    } // for i
} // generate


void linearAscendINT(int* Array, int len)
{
    int inf = 0;
    int sup = INT_MAX;
    double coeff = (abs(sup - inf) * 1.0) / len; // Угловой коэффициент 
    int Const = inf; // На сколько поднять/опустить
    for (int i = 0; i < len; i++)
    {
        Array[i] = coeff * i + Const; // Отбрасываем дробную часть
    } // for i
} // void linearAscendINT

void linearDescendINT(int* Array, int len)
{
    int inf = 0;
    int sup = INT_MAX;
    double coeff = (-1)*(abs(sup - inf) * 1.0) / len; // Угловой коэффициент 
    int Const = sup; // На сколько поднять/опустить
    for (int i = 0; i < len; i++)
    {
        Array[i] = coeff * i + Const; // Отбрасываем дробную часть
    } // for i
} // void linearDescendINT

void printArray(int* Array, int len)
{
    for (int i = 0; i < len; i++)
    {
        cout << "\t" << Array[i];
    }
    cout << endl;
} // printArray

void noteRes(const char* filename, int lenArray, int time, int is_compare)
{
    ofstream fout(filename, ios::app);
    fout << lenArray << " " << time;

    if (is_compare) fout << " " << is_compare << "\n"; // Если BLS/SLS, то значение, возвращаемое функцией, и есть количество сравнений
    else fout << "\n";

    fout.close();
} // void noteRes
