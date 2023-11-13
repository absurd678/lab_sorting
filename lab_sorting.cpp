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
*Last revision:12.11.2023                                                          *
*Comment      :                                                                    *
***********************************************************************************/

#include <chrono>
#include <iostream>
#include <fstream>
using namespace std;

/*********************************************************************
 *              П Р О Т О Т И П Ы    Ф У Н К Ц И Й                   *
 *********************************************************************/
void Insertion_Sort(int* Array, int len, unsigned __int64* answer); // Сортировка вставкой
void quickSort(int* Array, int begin, int end, unsigned __int64* answer, int len); // Быстрая сортировка
void Partition(int* Array, int begin, int end, int& Q, int& greaterQ, unsigned __int64* answer, int len); // Входит в быструю сортировку
int findOptimalQ(int* Array, int begin, int end, unsigned __int64* answer); // ~*NEW*~ поиск к-й порядковой статистики
void generate(int* array, int len); // Генерация нового массива
void linearAscendINT(int* Array, int len); //Генерация возрастающей последовательности
void linearDescendINT(int* Array, int len); //Генерация убывающей последовательности
void printArray(int* Array, int len); // Печать массива 
void noteRes(const char* filename, int lenArray, int time, int is_compare); // Запись в файл

/*******************************************************************/
/*           Г Л О Б А Л Ь Н Ы Е   П Е Р Е М Е Н Н Ы Е             */
/*******************************************************************/

// К О Н С Т А Н Т Ы
const int arrLenQuant = 4; // Кол-во размеров
const int arrQuant = 3; // Кол-во случаев массивов
const int answerLen = 3; 


// П Е Р Е М Е Н Н Ы Е
void(*func_array[arrQuant])(int*, int) = { generate, linearAscendINT, linearDescendINT }; // МАССИВ ФУНКЦИЙ ФОРМИРОВАНИЯ МАССИВОВ
unsigned _int64 answer[answerLen] = { 0, 0, 0 }; // Результат работы алгоритма: время, количество сравнений и перестановок
int* main_array; // массив
int allLengths[arrLenQuant] = { 15, 10000, 50000, 100000 }; // Массив размерностей
int lenMain;

/*********************************************************************
 *              Г Л А В Н А Я   П Р О Г Р А М М А                    *
 *********************************************************************/

int main()
{
    setlocale(LC_ALL, "Russian");
  
    for (int i = 0; i < arrLenQuant; i++) {
        lenMain = allLengths[i]; // Длина массива
        cout << "Длина массива = " << lenMain << endl;
        for (int j = 0; j < arrQuant; j++)
        {
            cout << endl << "Последовательность №" << j << endl;
            // Быстрая сортировка
            main_array = new int[lenMain];
            answer[0] = 0;
            answer[1] = 0;
            answer[2] = 0;
            func_array[j](main_array, lenMain);

            cout << "БЫСТРАЯ СОРТИРОВКА" << endl;
            if (lenMain == 15) printArray(main_array, lenMain);; // ПРОМЕЖУТОЧНЫЕ ВЫЧИСЛЕНИЯ
            auto begin = std::chrono::steady_clock::now();
            quickSort(main_array, 0, lenMain - 1, answer, lenMain); // СОРТИРОВКА QS
            auto end = std::chrono::steady_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            answer[0] = elapsed_ms.count();
            if (lenMain == 15) { cout << "РЕЗУЛЬТАТ: " << endl; printArray(main_array, lenMain); } // ПРОМЕЖУТОЧНЫЕ ВЫЧИСЛЕНИЯ
            cout << "Время работы QS: " << answer[0] << " Количество сравнений: " << answer[1] << " Количество перестановок: " << answer[2] << endl;

            // Сортировка вставкой ЧЕМОДАН ВОКЗАЛ НАХУЙ
            //answer[0] = 0;
            //answer[1] = 0;
            //answer[2] = 0;
            //func_array[j](main_array, lenMain);

            //cout << "СОРТИРОВКА ВСТАВКОЙ" << endl;
            //if (lenMain == 15) printArray(main_array, lenMain); // ПРОМЕЖУТОЧНЫЕ ВЫЧИСЛЕНИЯ
            //begin = std::chrono::steady_clock::now();
            //Insertion_Sort(main_array, lenMain, answer); // СОРТИРОВКА IS
            //end = std::chrono::steady_clock::now();
            //elapsed_ms = std::chrono::duration_cast<std::chrono::microseconds>(end - begin);
            //answer[0] = elapsed_ms.count();
            //if (lenMain == 15) { cout << "РЕЗУЛЬТАТ: " << endl; printArray(main_array, lenMain); } // ПРОМЕЖУТОЧНЫЕ ВЫЧИСЛЕНИЯ
            //cout << "Время работы IS: " << answer[0] << " Количество сравнений: " << answer[1] << " Количество перестановок: " << answer[2] << endl;


            delete[] main_array;
        }
    }

} // main


/***********************************************************************
 *                Р Е А Л И З А Ц И Я    Ф У Н К Ц И Й                 *
 ***********************************************************************/

void quickSort(int* Array, int begin, int end, unsigned __int64* answer, int len)
{
    if (begin >= end || end < 0 || begin < 0) return;
    answer[1]+=3; // Счет сравнений
    int qIndex = begin; // Индекс с ключевым элементом
    //int greaterIndex = begin; // Индекс ближайшего к ключевому элементу числа, большего ключевого
    Partition(Array, begin, end, qIndex, answer, len);
    // Т.о., Между qIndex и greaterIndex остаются элементы, равные ключевому, их не рассматриваем.
    quickSort(Array, begin, qIndex - 1, answer, len);
    quickSort(Array, qIndex, end, answer, len);
} // quickSort

 
void Partition(int* Array, int begin, int end, int&Q, unsigned __int64* answer, int len)
{
    int n = (end - begin) + 1; // Длина подмассива
    int pivot = 0;

    if (n <= 5) { pivot = Array[rand() % n + begin]; cout << "Подмассив длиной <= 5" << endl; } // Случайное ключевое значение
    else pivot = findOptimalQ(Array, begin, end, answer); // ~* Ищем оптимальную k-ю статистику *~ 

    if (len == 15 && n > 5) cout << "pivot = " << pivot << endl; // ПРОМЕЖУТОЧНЫЕ ВЫЧИСЛЕНИЯ

    for (int u = begin; u <= end; u++) // Цикл по области U
    {
        if (Array[u] <= pivot)  // Перемещение в область L
        {
            swap(Array[u], Array[Q]);
            Q++;
            answer[2]+=1; // Счет перестановок
            if (len == 15) printArray(Array, len); // ПРОМЕЖУТОЧНЫЕ ВЫЧИСЛЕНИЯ
        } // if
        answer[1] += 2; // Счет сравнений
    } // for u

} // Partition

// Ошибка...
int findOptimalQ(int* Array, int begin, int end, unsigned __int64* answer)  // Ищем оптимальный промежуточный элемент, который
// разобьет подмассив на две части
{
    int lenArr = (end - begin + 1); // Длина подмассива
    // Разбиваем массив на 5 частей, находим медианы этих подподмассивов, записываем их в массив медиан
    int medianAmout = lenArr / 5 + 1; 
    if (lenArr % 5 == 0) medianAmout--;
    int* medians = new int[medianAmout];
    for (int i = 1; i < medianAmout+1; i++)
    {
        int a = begin + 5 * i;
        int b = a + 4;
        if (b > end) b = end;
        quickSort(Array, a, b, answer, lenArr);
        medians[i - 1] = Array[a + (b - a) / a];
    }
    // Находим медиану массива медиан, это и есть то, что нужно
    quickSort(medians, 0, medianAmout - 1, answer, medianAmout);
    int pivotIdx = medianAmout / 2;
    return medians[pivotIdx];
}

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
    int sup = 100000;
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
    int sup = 100000;
    double coeff = (-1) * (abs(sup - inf) * 1.0) / len; // Угловой коэффициент 
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

void Insertion_Sort(int* Array, int len, unsigned __int64* answer) {
    for (int i = 0; i < len; i++) {
        for (int j = i; j > 0 && Array[j - 1] > Array[j]; j--) {
            answer[1] += 2;
            answer[2]++;
            int tmp = Array[j - 1];
            Array[j - 1] = Array[j];
            Array[j] = tmp;
            if (len == 15) printArray(Array, len); // ПРОМЕЖУТОЧНЫЕ ВЫЧИСЛЕНИЯ
        }
        answer[1]++;
    }
}