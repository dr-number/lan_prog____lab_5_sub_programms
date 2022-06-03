#include <iostream>
#include <Windows.h>
#include <random>
#include <iomanip>
#include <vector>
#include <random>
#include <regex>

#define PI 3.14159265

using namespace std;
HANDLE handleConsole;

enum ConsoleColor
{
    Blue = 1,
    Green = 2,
    Red = 4,
    Yellow = 14,
    White = 15,
    Black = 0
};

string RoundStr(float var, int after = 2) {

    string s = to_string(var);
    int size = s.size();

    string result = "";
    bool isAfter = false;
    int afterCount = 0;

    for (int i = 0; i < size; ++i) {

        if (afterCount == after)
            break;
        
        if (isAfter)
            ++afterCount;

        if (!isAfter && s[i] == ',')
            isAfter = true;

        result += s[i];

    }

    return result;
}

float Round(float var)
{
    // 37.66666 * 100 = 3766.66
    // 3766.66 + .5 = 3767.16 для значения округления

    // затем вводим тип int в значение 3767
    // затем делим на 100, поэтому значение преобразуется в 37,67

    float value = (int)(var * 100 + .5);
    return stod(RoundStr((float)value / 100));

}

string GetLine() {
    string line;
    getline(cin, line);
    cin.clear();
    return line;
}

class MyInput {

public:
    bool isDouble(string str) {
        return regex_match(str, regex("^[-]?[0-9]*?,?[0-9]*"));
    }

    bool isNum(string str) {
        return regex_match(str, regex("^[-]?[0-9]*"));
    }

    int InputIntData(string text, int min, int max, int defaultValue = -1) {

        string xStr = "";
        double result = 0;
        bool isNumber = true;

        while (true) {

            SetConsoleTextAttribute(handleConsole, White);
            cout << text;

            xStr = GetLine();

            if (xStr == "" && defaultValue != -1)
                return defaultValue;


            try {
                result = stod(xStr.c_str());
                isNumber = true;
            }
            catch (...) {
                isNumber = false;
            }

            if (!(isNumber && isNum(xStr))) {
                SetConsoleTextAttribute(handleConsole, Red);
                cout << endl << xStr + " - не число!" << endl << endl;
            }
            else if (result > max || result < min) {
                SetConsoleTextAttribute(handleConsole, Red);
                cout << endl << "Число должно лежать в промежутке [" << min << "; " << max << "]!" << endl << endl;
            }
            else
                break;
        }

        return result;
    }

    double InputDoubleData(string text, int min, int max, int defaultValue = -1) {

        string xStr = "";
        double result = 0;
        bool isNumber = true;

        while (true) {

            SetConsoleTextAttribute(handleConsole, White);
            cout << text;

            xStr = GetLine();

            if (xStr == "" && defaultValue != -1)
                return defaultValue;


            try {
                result = stod(xStr.c_str());
                isNumber = true;
            }
            catch (...) {
                isNumber = false;
            }

            if (!(isNumber && isDouble(xStr))) {
                SetConsoleTextAttribute(handleConsole, Red);
                cout << endl << xStr + " - не число!" << endl << endl;
            }
            else if (result > max || result < min) {
                SetConsoleTextAttribute(handleConsole, Red);
                cout << endl << "Число должно лежать в промежутке [" << min << "; " << max << "]!" << endl << endl;
            }
            else
                break;
        }

        return result;
    }
};

class MyQuestion {

public:
    const string QUESTION_RANDOM_DATA = "Сгенерировать данные случайным образом [y/n]?";
    const string QUESTION_IN_ORDER_DATA = "Взять числа по порядку [y/n]?";
    const string QUESTION_SHOW_CALC = "Показывать ход вычислений [y/n]?";

    bool isQuestion(string textQuestion) {

        cout << textQuestion << endl;
        string answer = GetLine();

        transform(answer.begin(), answer.end(), answer.begin(), ::tolower);
        return answer == "y" || answer == "";
    }
};

class MyRandom {

public:
    const double MIN_RANDOM = 10, MAX_RANDOM = 1000;

    double GetRandom(int min, int max) {

        random_device random_device; // Источник энтропии.
        mt19937 generator(random_device()); // Генератор случайных чисел.

        // (Здесь берется одно инициализирующее значение, можно брать больше)
        uniform_int_distribution<> distribution(min, max); // Равномерное распределение [min, max]

        return distribution(generator);

        //return rand() % (max - min) - min;
        //return rand() % max + min;
    }

};

class MyArray {
public:
    int const MIN_VALUE = -10000;
    int const MAX_VALUE = 10000;

    int const DEFAULT_COUNT_VALUE = 20;

    int const MIN_COUNT = 5;
    int const MAX_COUNT = 10000;

    int const DEFAULT_ROW = 10;
    int const DEFAULT_COL = 15;

    vector <int> Swap(vector <int> arr, int index1, int index2) {
        int tmp = arr[index1];
        arr[index1] = arr[index2];
        arr[index2] = tmp;
        return arr;
    }

    vector <int> CreateRandomArray(int n, int minValue, int maxValue) {

        vector <int> arr;
        MyRandom myRandom = *new MyRandom();

        for (int i = 0; i < n; ++i)
            arr.push_back(myRandom.GetRandom(minValue, maxValue));

        return arr;
    }

    vector <int> CreateInputArray(int n, int minValue, int maxValue) {

        vector <int> arr;
        MyInput myInput = *new MyInput();

        for (int i = 0; i < n; ++i)
            arr.push_back(myInput.InputIntData("Введите целое число (" + to_string(i + 1) + " из " + to_string(n) + "): ", minValue, maxValue));

        return arr;
    }

    vector <int> CreateArray(int n, int minValue, int maxValue) {

        MyQuestion myQuestion = *new MyQuestion();

        bool isRandomData = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);

        if (isRandomData)
            return CreateRandomArray(n, minValue, maxValue);

        return CreateInputArray(n, minValue, maxValue);
    }

    vector <int> CreateArray(int n) {
        return CreateArray(n, MIN_VALUE, MAX_VALUE);
    }

    vector <int> CreateArray() {
        MyInput myInput = *new MyInput();
        int size = myInput.InputIntData("Сколько элементов должно быть в массиве? [по умолчанию " + to_string(DEFAULT_COUNT_VALUE) + "]: ", MIN_COUNT, MAX_COUNT, DEFAULT_COUNT_VALUE);
        return CreateArray(size);
    }

    void PrintArray(vector <int> arr) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        int size = arr.size();

        for (int i = 0; i < size; ++i) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "[" << i << "] ";

            SetConsoleTextAttribute(handleConsole, White);
            cout << arr[i] << endl;
        }
    }

};

class Task6 {
private:
    //выводит на экран вектор выделяя его различными цветами.
    void PrintVector(string nameVector, vector <int> array) {

        int size = array.size();

        SetConsoleTextAttribute(handleConsole, Green);
        cout << nameVector;
        
        SetConsoleTextAttribute(handleConsole, Blue);
        cout << " =";

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << " { ";

        for (int i = 0; i < size; ++i) {

            if (i != 0)
                cout << "; ";

            cout << array[i];
        }

        cout << " } " << endl;
    }

    //перемножает вектора и выводит промежуточный результат на экран (в зависимости от параметра isPrint).
    int getMultiplication(vector<int> vector1, vector<int> vector2, bool isPrint) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        int size = vector1.size();

        int result = 0;

        if (!isPrint) {

            for (int i = 0; i < size; ++i) 
                result += vector1[i] * vector2[i];
            
            return result;
        }

        for (int i = 0; i < size; ++i) {

            SetConsoleTextAttribute(handleConsole, White);
            cout << vector1[i] << " * " << vector2[i];

            if (i != size - 1) {
                SetConsoleTextAttribute(handleConsole, Green);
                cout << " + ";
            }
            else {
                SetConsoleTextAttribute(handleConsole, Blue);
                cout << " = ";

                for (int i = 0; i < size; ++i) {
                    SetConsoleTextAttribute(handleConsole, Yellow);
                    cout << vector1[i] * vector2[i];

                    result += vector1[i] * vector2[i];

                    if (i != size - 1) {
                        SetConsoleTextAttribute(handleConsole, Green);
                        cout << " + ";
                    }
                    else {
                        SetConsoleTextAttribute(handleConsole, Blue);
                        cout << " = ";

                        SetConsoleTextAttribute(handleConsole, Green);
                        cout << result << endl;
                    }
                }
            }
        }

        return result;
    }

public:
    void Init() {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        cout << "Вычислить скалярное произведение векторов произвольной размерности" << endl << endl;

        vector<int> vector1, vector2;

        MyArray myArray = *new MyArray();
        MyQuestion myQuestion = *new MyQuestion();
        MyInput myInput = *new MyInput();

        bool isRandom = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);
        int dimension = myInput.InputIntData("Введите размерность векторов [по умолчанию " + to_string(myArray.DEFAULT_COL) + "]: ", myArray.MIN_COUNT, myArray.MAX_COUNT, myArray.DEFAULT_COL);
        bool isShowCalc = myQuestion.isQuestion(myQuestion.QUESTION_SHOW_CALC);

        if (isRandom) {
            vector1 = myArray.CreateRandomArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);
            vector2 = myArray.CreateRandomArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);
        }
        else {

            SetConsoleTextAttribute(handleConsole, Green);
            cout << "Введение вектора \"a\"" << endl;
            SetConsoleTextAttribute(handleConsole, White);
            vector1 = myArray.CreateInputArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);

            SetConsoleTextAttribute(handleConsole, Green);
            cout << "\nВведение вектора \"b\"" << endl;
            SetConsoleTextAttribute(handleConsole, White);
            vector2 = myArray.CreateInputArray(dimension, myArray.MIN_VALUE, myArray.MAX_VALUE);
        }

        SetConsoleTextAttribute(handleConsole, Blue);
        cout << "\nРазмерность: " << dimension << endl;

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "Вектора: " << endl;
        PrintVector("a", vector1);
        PrintVector("b", vector2);
       

        SetConsoleTextAttribute(handleConsole, Green);
        
        cout << "\nРезультат умножения векторов:" << endl;

        int result = getMultiplication(vector1, vector2, isShowCalc);

        if(!isShowCalc)
            cout << result << endl;
    }

};


class Task16 {
private:
    const int DEFAULT_RAND_START_SUM = 60;
    const int START_RAND_SUM_MIN = 1;
    const int START_RAND_SUM_MAX = 1000;

    const int DEFAULT_START_SUM = DEFAULT_RAND_START_SUM * 1000;
    const int START_SUM_MIN = START_RAND_SUM_MIN * 1000;
    const int START_SUM_MAX = START_RAND_SUM_MAX * 1000;

    const int DEFAULT_EASY_N_YEAR = 8;
    const int DEFAULT_HARD_N_YEAR = 15;
    const int N_YEAR_MIN = 1;
    const int N_YEAR_MAX = 15;

    const int DEFAULT_PER = 4;
    const int PER_MIN = 1;
    const int PER_MAX = 25;

    void PrintInfo(string title, string value, string postTitle = "") {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << title << " ";

        SetConsoleTextAttribute(handleConsole, Green);
        cout << value;

        if (postTitle != "") {
            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << " " << postTitle;
        }

        cout << endl;
    }

    string GetWordYear(int n) {

        if (n == 1) return "год";
        if (n >= 2 && n <= 4) return "года";
        if (n >= 5) return "лет";

        return "лет";
    }

    //Вывод суммы с разделителем. Например 10 000
    string GetMoneySeparator(string str, string separator = " ") {

        int size = str.size() - 4;

        string result = "";
        int n = 0;

        for (int i = size; i >= 0; --i, ++n) {

            if (n != 0 && n % 3 == 0)
                result = separator + result;

            result = str[i] + result;

        }

        return result + str[size + 1] + str[size + 2] + str[size + 3];
    }

    //вычисляет «простые» проценты за год.
    double GetEasyProfitYear(int startSum, double per, bool isLongYear, bool isPrint) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        int daysInYear = isLongYear ? 366 : 365;

        double result = ((startSum / 100 * per) * 365) / daysInYear;

        if (isPrint) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "(" << GetMoneySeparator(RoundStr(Round(startSum))) << " * " << per << " / 100 * 365) / " << daysInYear << " = ";

            SetConsoleTextAttribute(handleConsole, Green);
            cout << GetMoneySeparator(RoundStr(Round(result)));

            if (isLongYear)
                cout << " - Высокосный год";

            cout << endl;
        }

        return result;

    }

    //вычисляет «простые» проценты за количество указанных лет.
    double GetEasyProfit(int startSum, double per, int years, bool isLongYear, bool isPrint) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        double result = 0;

        int n = isLongYear ? 4 : 1;

        for (int i = 1; i <= years; ++i, ++n) {

            if (i != 1)
                isLongYear = n % 4 == 0;

            if (isPrint) {
                SetConsoleTextAttribute(handleConsole, White);
                cout << i << ") ";
            }

            result += GetEasyProfitYear(startSum, per, isLongYear, isPrint);

        }


        return result;
    }

    //вычисляет «сложные» проценты за количество указанных лет.
    double GetHardProfit(int startSum, double per, int years, bool isPrint) {

        double total = startSum;
        double charges = 0;

        const int WIDTH_YEAR = 4;
        const int WIDTH_CHARGES = 4;
        const int WIDTH_TOTAL = 6;

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        if (isPrint) {
            SetConsoleTextAttribute(handleConsole, White);
            cout.width(WIDTH_YEAR);
            cout << left << "год ";

            SetConsoleTextAttribute(handleConsole, Green);
            cout.width(WIDTH_CHARGES);
            cout << " " << left << "начисления ";

            SetConsoleTextAttribute(handleConsole, Yellow);
            cout.width(WIDTH_TOTAL);
            cout << " " << right << "итоговая сумма" << endl;
        }

        string strTotal, strCharges;
        string strPer = RoundStr(Round(per));

        for (int i = 0; i < years; ++i) {

            charges = total / 100 * per;
            total += charges;

            if (isPrint) {

                strTotal = GetMoneySeparator(RoundStr(Round(total)));
                strCharges = GetMoneySeparator(RoundStr(Round(charges)));

                SetConsoleTextAttribute(handleConsole, White);
                cout.width(WIDTH_YEAR);
                cout << left << (i + 1);

                SetConsoleTextAttribute(handleConsole, Green);
                cout.width(WIDTH_CHARGES);
                cout << " " << left << strTotal << " / 100 * " << strPer << " = " << strCharges;

                SetConsoleTextAttribute(handleConsole, Yellow);
                cout.width(WIDTH_TOTAL);
                cout << " " << right << strTotal << " + " << strCharges << " = " << strTotal << endl;
            }

        }

        double result = total - startSum;

        if (isPrint) {
            PrintInfo("\nНачисления:", GetMoneySeparator(RoundStr(Round(result))));
            PrintInfo("Итоговая сумма", GetMoneySeparator(RoundStr(Round(total))));
            cout << endl;
        }

        return result;
    }
public:

    void Init() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        cout << "Определить прибыль от вклада под p% годовых через N лет для случаев простых или сложных процентов" << endl << endl;

        MyQuestion myQuestion = *new MyQuestion();
        bool isRandom = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);
        bool isEasy = myQuestion.isQuestion("Провести расчеты по простым процентам [y/n]: ");
        bool isShowCalc = myQuestion.isQuestion(myQuestion.QUESTION_SHOW_CALC);

        int startSum, years;
        double per;

        bool isLongYear = false;

        if (isRandom) {
            MyRandom myRandom = *new MyRandom();
            startSum = myRandom.GetRandom(START_RAND_SUM_MIN, START_RAND_SUM_MAX);

            startSum *= 1000;

            years = myRandom.GetRandom(N_YEAR_MIN, N_YEAR_MAX);
            per = myRandom.GetRandom(PER_MIN, PER_MAX);
        }
        else {

            int defaultYear;

            if (isEasy) {
                defaultYear = DEFAULT_EASY_N_YEAR;
                MyQuestion myQuestion = *new MyQuestion();
                isLongYear = myQuestion.isQuestion("Год високосный? [y/n]: ");
            }
            else
                defaultYear = DEFAULT_HARD_N_YEAR;

            MyInput myInput = *new MyInput();
            startSum = myInput.InputIntData("Введите начальную сумму вклада [ по умолчанию " + GetMoneySeparator(RoundStr(Round(DEFAULT_START_SUM))) + " ]: ", START_SUM_MIN, START_SUM_MAX, DEFAULT_START_SUM);
            years = myInput.InputIntData("Введите срок вклада (в годах) [ по умолчанию " + to_string(defaultYear) + " ]: ", N_YEAR_MIN, N_YEAR_MAX, defaultYear);
            per = myInput.InputDoubleData("Введите % вклада [ по умолчанию " + to_string(DEFAULT_PER) + " ]: ", PER_MIN, PER_MAX, DEFAULT_PER);
        }

        string strAlgoritm;
        
        if (isEasy)
            strAlgoritm = "простым";
        else
            strAlgoritm = "сложным";

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "\nИсходные данные:" << endl;

        PrintInfo("Расчет по", strAlgoritm, "процентам");
        PrintInfo("Начальная сумма вклада:", GetMoneySeparator(RoundStr(Round(startSum))), "руб.");
        PrintInfo("Срок вклада:", to_string(years), GetWordYear(years));
        PrintInfo("Под:", RoundStr(Round(per)) + "%", "годовых");

        cout << endl;

        double result;

        if (isEasy)
            result = GetEasyProfit(startSum, per, years, isLongYear, isShowCalc);
        else
            result = GetHardProfit(startSum, per, years, isShowCalc);

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "\nРезультат" << endl;

        PrintInfo("Прибыль от вклада составляет:", GetMoneySeparator(RoundStr(Round(result))), "руб.");

    }
};

class Task26 {
private:
    const int R_DEFAULT = 2;
    const int R_MIN = 1;
    const int R_MAX = 10000;

    struct myPoint {
        double x = 0;
        double y = 0;
    };

    //определяет равны ли точки p1 и p2.
    bool isQual(myPoint p1, myPoint p2) {
        return p1.x == p2.x && p1.y == p2.y;
    }

    //определяет равна ли хотя бы одна точка p1, p2 и p3.
    bool isQual(myPoint p1, myPoint p2, myPoint p3) {
        return isQual(p1, p2) || isQual(p2, p3) || isQual(p1, p3);
    }

    //запрашивает у пользователя координаты точки.
    myPoint InputPoint(string namePoint, int min, int max) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "\nТочка: " << namePoint << endl;

        MyInput myInput = *new MyInput();
        myPoint point;

        point.x = myInput.InputIntData("Введите X: ", min, max);
        point.y = myInput.InputIntData("Введите Y: ", min, max);

        return point;
    }
 
    //генерирует случайную точку.
    myPoint RandomPoint(int min, int max) {

        MyRandom myRandom = *new MyRandom();

        myPoint point;
        point.x = myRandom.GetRandom(min, max);
        point.y = myRandom.GetRandom(min, max);

        return point;
    }

    //декартовы координаты в полярные.
    void PrintPoint(myPoint point, string namePoint, bool isEndl = true) {

        cout << namePoint << "(" << point.x << "; " << point.y << ")";

        if (isEndl)
            cout << endl;
    }

    /*
    выводит на экран информации о координатах в декартовой и полярной системах, 
    так-же выводит информацию о том лежит ли точка с полярными координатами polar внутри круга с радиусом r.
    */
    void PrintResultPoint(myPoint decart, myPoint polar, string namePoint, double r) {

        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

        SetConsoleTextAttribute(handleConsole, Yellow);
        PrintPoint(decart, namePoint, false);

        SetConsoleTextAttribute(handleConsole, Blue);
        cout << " = ";

        SetConsoleTextAttribute(handleConsole, White);
        PrintPoint(polar, namePoint, false);

        double polarR = polar.x;

        if (polarR < r) {
            SetConsoleTextAttribute(handleConsole, Green);
            cout << " - лежит внутри круга";
        }
        else if (polarR > r) {
            SetConsoleTextAttribute(handleConsole, Red);
            cout << " - не лежит внутри круга";
        }
        else {
            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << " - лежит на границе круга";
        }

        cout << endl;
    }

    //декартовы координаты в полярные.
    myPoint polarCoord(myPoint coord, string namePoint, bool isPrint) {

        int x = coord.x;
        int y = coord.y;

        //===============r=================
        int x2 = x * x;
        int y2 = y * y;

        int underRoot = x2 + y2;
        double r = sqrt(underRoot);
        //===========end r=================

        //============O===================
        double divisionYX = y / x;

        double O = 0;
        
        if (divisionYX != 0) {
            O = DegreeToRadian(divisionYX);
            O = pow(tan(O), -1);
            O = RadianToDegree(O);
        }
        else if (x == 0 && y == 0) {
            O = 0;
        }
        else if (x == 0 && y != 0) {
            O = 1.5707963267949; // 90 градусов
        }
        else if (x != 0 && y == 0) {
            O = PI; // 180 градусов
        }
        
        //========end O===================

        if (isPrint) {

            SetConsoleTextAttribute(handleConsole, Yellow);
            PrintPoint(coord, namePoint);

            SetConsoleTextAttribute(handleConsole, Green);
            cout << "r = Корень из (" << x << "^2 + " << y << "^2)" << endl;
            cout << "r = Корень из (" << x2 << " + " << y2 << ")" << endl;
            cout << "r = Корень из (" << underRoot << ")" << endl;
            cout << "r = " << r << endl << endl;

            SetConsoleTextAttribute(handleConsole, Blue);
            cout << "O = tg^-1 (" << y << " / " << x << ")" << endl;
            cout << "O = tg^-1 (" << divisionYX << ")" << endl;
            cout << "O = " << O << endl;

        }

        myPoint result;
        result.x = r;
        result.y = O;

        if (isPrint) {
            SetConsoleTextAttribute(handleConsole, Yellow);
            cout << "Полярные координаты: ";
            PrintPoint(result, namePoint);
            cout << endl;
        }

        return result;
    }

    double myRound(double val, int n) {
        return round(val * n) / n;
    }

    double RadianToDegree(double val) {
        return (val * 180) / PI;
    }

    double DegreeToRadian(double val) {
        return (val * PI) / 180;
    }

public:
    void Init() {
        HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
        SetConsoleTextAttribute(handleConsole, White);

        int r;
        myPoint m1, m2, m3;

        MyQuestion myQuestion = *new MyQuestion();
        bool isRandom = myQuestion.isQuestion(myQuestion.QUESTION_RANDOM_DATA);
        bool isShowCalc = myQuestion.isQuestion(myQuestion.QUESTION_SHOW_CALC);

        if (isRandom) {
            r = R_DEFAULT + 1;

            while (isQual(m1, m2, m3)) {
                m1 = RandomPoint(-r, r);
                m2 = RandomPoint(-r, r);
                m3 = RandomPoint(-r, r);
            }
        }
        else {
            MyInput myInput = *new MyInput();
            r = myInput.InputIntData("Введите радиус (R) круга [по умолчанию " + to_string(R_DEFAULT) + "]: ", R_MIN, R_MAX, R_DEFAULT);

            bool isGo = true;

            while (isGo) {
                m1 = InputPoint("Точка M1", -R_MAX, R_MAX);
                m2 = InputPoint("Точка M2", -R_MAX, R_MAX);
                m3 = InputPoint("Точка M3", -R_MAX, R_MAX);

                isGo = isQual(m1, m2, m3);

                if (isGo) {
                    SetConsoleTextAttribute(handleConsole, Red);
                    cout << "Точки не должны иметь общие координаты!" << endl;
                    SetConsoleTextAttribute(handleConsole, White);
                }
            }

        }

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "\nИсходные данные:" << endl;

        SetConsoleTextAttribute(handleConsole, Yellow);
        cout << "R = " << r << endl;
        PrintPoint(m1, "M1");
        PrintPoint(m2, "M2");
        PrintPoint(m3, "M3");

        if (isShowCalc) {
            SetConsoleTextAttribute(handleConsole, Green);
            cout << "\nr = Корень из (x^2 + y^2)" << endl;

            SetConsoleTextAttribute(handleConsole, Blue);
            cout << "O = tg^-1 (y / x)" << endl;

            SetConsoleTextAttribute(handleConsole, White);
            cout << "(x; y) = (r; O)" << endl << endl;
        }

        myPoint M1 = polarCoord(m1, "M1", isShowCalc);
        myPoint M2 = polarCoord(m2, "M2", isShowCalc);
        myPoint M3 = polarCoord(m3, "M3", isShowCalc);

        SetConsoleTextAttribute(handleConsole, Green);
        cout << "\nРезультаты вычислений (при R = " + to_string(r) + "):" << endl;

        PrintResultPoint(m1, M1, "M1", r);
        PrintResultPoint(m2, M2, "M2", r);
        PrintResultPoint(m3, M3, "M3", r);
    }
};


int main()
{
    handleConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    setlocale(LC_ALL, "Russian");

    SetConsoleTextAttribute(handleConsole, White);

    cout << "Тема 5. Подпрограммы" << endl;

    bool isGo = true;
    string select;

    while (isGo)
    {
        SetConsoleTextAttribute(handleConsole, White);

        cout << "\nВведите номер задачи" << endl;
        cout << "6)	Вычислить скалярное произведение векторов произвольной размерности" << endl << endl;

        cout << "16) Определить прибыль от вклада под p% годовых через N лет  для случаев простых или сложных процентов" << endl << endl;
       
        cout << "26) Вычислить полярные координаты для точек, лежащих внутри круга радиусом R с центром в начале координат" << endl << endl;

        cout << endl << "Для выхода введите \"0\": ";

        select = GetLine();

        if (select == "6") {
            Task6 task6 = *new Task6();
            task6.Init();
        }
        else if (select == "16") {
            Task16 task16 = *new Task16();
            task16.Init();
        } 
        else if (select == "26") {
            Task26 task26 = *new Task26();
            task26.Init();
        }
        else if (select == "0") {
            isGo = false;
        }
        else {
            SetConsoleTextAttribute(handleConsole, Red);
            cout << "\nНекорректные данные!" << endl;
            SetConsoleTextAttribute(handleConsole, White);
        }

    }
}

