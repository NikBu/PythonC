// Test.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <python.h>
#include <iostream>
#include <vector>
#include <numpy/arrayobject.h>

using namespace std;

int main()
{
    //Вектор и массив для датасета
    vector <int> VectorDS = { 1,2,3,4,5,6,7,8,9,10 };
    int ArrayDS[10] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10 };
    cout << "Vector:\n";
    for (int i = 0; i < 10; i++) {
        cout << VectorDS.at(i) << " ";
    }
    cout << "\nArray\n";
    for (int i = 0; i < 10; i++) {
        cout << ArrayDS[i] << " ";
    }
    cout << endl;
    try
    {
        //Работа с Питоном
        Py_Initialize();//Инициализация
        import_array(); //Для работты с NpArray

        PyObject* Py_main = PyImport_AddModule("__main__"); //Работаем с модулем main
        PyObject* Py_ds = PyList_New(10); //Переводим наш массив/Вектор в питон 
        for (int i = 0; i < 10; ++i) //Заполнение списка
        {
            PyObject* python_int = Py_BuildValue("i", VectorDS[i]);
            PyList_SetItem(Py_ds, i, python_int);
        };
        //Код функции обучения
        PyRun_SimpleString("from statsmodels.tsa.api import SimpleExpSmoothing\n"
            "def test(Pyds):\n"
            "\tprint('Transformed array: ',Pyds)\n"
            "\tmodel = SimpleExpSmoothing(Pyds, initialization_method = 'heuristic').fit(optimized = True)\n"
            "\tfit = model.fittedvalues\n"
            "\tpred = model.forecast()\n"
            "\tprint('Fit: ',fit)\n"
            "\tprint('Prediction: ',pred)\n"
            "\treturn (pred,fit)\n");

        PyObject* pFunc = PyObject_GetAttrString(Py_main, "test");//Получение строки функции из кода питон
        if (pFunc && PyCallable_Check(pFunc)) //Проверка функции
        {
            PyObject* pArgs = PyTuple_New(1);//Создание кортежа аргументов функции
            PyTuple_SetItem(pArgs, 0, Py_ds);//Помещение туда вектора/Массива
            PyObject* pReturn = PyObject_CallObject(pFunc, pArgs);//Вызов функции
            Py_DECREF(pFunc);

            double pred;
            PyObject* pFit;//Возвращённый fit как PyObject
            PyArg_ParseTuple(pReturn, "dO", &pred, &pFit);//Разделение значений из полученного кортежа(pred и fit)
            cout << "Returned Prediction: " << pred << endl;//Вывод предсказания
            Py_DECREF(pArgs);
            Py_DECREF(pReturn);

            vector <double> fit;//Fit как вектор С++
            cout << "Returned Fit: ";
            for (int i = 0; i < PyArray_Size(pFit); i++) {
                fit.push_back(*((double*)PyArray_GETPTR1(pFit, i)));
                cout << fit[i] << "  ";
            }
            cout << endl;
        }

        Py_Finalize();
    }
    catch (const std::exception&)
    {
        cout << " Error detected while processing python" << endl;
    }

    std::system("pause");

}
