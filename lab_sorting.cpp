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
void Insertion_Sort(int* Array, int len, unsigned _int64* answer); // Сортировка вставкой
void quickSort(int* Array, int begin, int end, unsigned _int64* answer); // Быстрая сортировка
void Partition(int Array[], int l, int r, int& Equals, int& Greater, unsigned _int64* answer); // Входит в быструю сортировку
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
void(*func_array[3])(int*, int) = { generate, linearAscendINT, linearDescendINT }; // МАССИВ ФУНКЦИЙ ФОРМИРОВАНИЯ МАССИВОВ
unsigned _int64 answer[3] = { 0, 0, 0 }; // Результат работы алгоритма: время, количество сравнений и перестановок
int* main_array; // массив
int allLengths[4] = { 15, 10000, 50000, 100000 };
int lenMain;

int test[15];
int len = 15;

/*********************************************************************
 *              Г Л А В Н А Я   П Р О Г Р А М М А                    *
 *********************************************************************/

int main()
{
    setlocale(LC_ALL, "Russian");
    // TODO: проблема с размером 100000 => ключевой элемент ставить либо в середину, либо как у Вадима (он еще интересную хрень про это скинул)
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

            auto begin = std::chrono::steady_clock::now();
            quickSort(main_array, 0, lenMain - 1, answer);
            auto end = std::chrono::steady_clock::now();
            auto elapsed_ms = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
            answer[0] = elapsed_ms.count();

            if (lenMain == 15) printArray(main_array, lenMain);
            cout << "Время работы QS: " << answer[0] << " Количество сравнений: " << answer[1] << " Количество перестановок: " << answer[2] << endl;

            // Сортировка вставкой
            memset(answer, 0, 3); // Обнуление массива answer
            func_array[j](main_array, lenMain);

            begin = std::chrono::steady_clock::now();
            Insertion_Sort(main_array, lenMain, answer);
            end = std::chrono::steady_clock::now();
            elapsed_ms = std::chrono::duration_cast<std::chrono::seconds>(end - begin);
            answer[0] = elapsed_ms.count();

            if (lenMain == 15) printArray(main_array, lenMain);
            cout << "Время работы IS: " << answer[0] << " Количество сравнений: " << answer[1] << " Количество перестановок: " << answer[2] << endl;


            delete[] main_array;
        }
    }

} // main


/***********************************************************************
 *                Р Е А Л И З А Ц И Я    Ф У Н К Ц И Й                 *
 ***********************************************************************/

void quickSort(int* Array, int begin, int end, unsigned _int64* answer)
{
    if (end - begin < 1) return;
    answer[1]++; // Счет сравнений

    int Equals = begin, Greater = begin;
    Partition(Array, begin, end, Equals, Greater, answer);
    // cout << begin << " " << qIndex << " " << end << endl; 
    quickSort(Array, begin, Equals - 1, answer);
    quickSort(Array, Greater, end, answer);
} // quickSort

//разбиение
void Partition(int Array[], int l, int r, int& Equals, int& Greater, unsigned _int64* answer) {
    //n - длина разбиваемой части массива, pivot - случайный опорный элемент из массива
    int n = r - l + 1, pivot = Array[rand() % n + l];
    int Now = l; //индекс текущего элемента
    Equals = l; //стартуем с l
    Greater = l;

    while (Now <= r) {
        answer[1] += 3; //цикл + 2 условия
        if (Array[Now] == pivot) { //добавляем к элементам, равным опорному
            swap(Array[Now], Array[Greater]);
            Greater++;
            answer[2]++;
        }
        if (Array[Now] < pivot) { //добавляем к элементам, строго меньшим опорного
            swap(Array[Now], Array[Greater]);
            swap(Array[Greater], Array[Equals]);
            Greater++;
            Equals++;
            answer[2] += 2;
        }
        Now++;
    }
    answer[1]++;//последнее сравнение, когда не входим в цикл
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

void noteRes(const char* filename, int lenArray, int time, int is_compare)
{
    ofstream fout(filename, ios::app);
    fout << lenArray << " " << time;

    if (is_compare) fout << " " << is_compare << "\n"; // Если BLS/SLS, то значение, возвращаемое функцией, и есть количество сравнений
    else fout << "\n";

    fout.close();
} // void noteRes

void Insertion_Sort(int* Array, int len, unsigned _int64* answer) {
    for (int i = 0; i < len; i++) {
        for (int j = i; j > 0 && Array[j - 1] > Array[j]; j--) {
            answer[1] += 2;
            answer[2]++;
            int tmp = Array[j - 1];
            Array[j - 1] = Array[j];
            Array[j] = tmp;
        }
        answer[1]++;
    }
}