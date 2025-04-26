#include <iostream>
#include <fstream>
#include <string>
#include <cmath>
#include <limits>

using namespace std;
void print(int n, double *nums) {// Вывод массива
	for (int i = 0; i < n; ++i) {
        cout << nums[i] << " ";
    }
    cout << endl;
}
void input(string file_path, int size, double *array){// Чтение чисел из файла
    double value;
    ifstream file(file_path);
    for (int i = 0; i < size; ++i) {
    	file >> array[i];
   	}
}
void merge(double *array, int left, int mid, int right){// Слияние двух частей
	int it1=0, it2=0;
	double* result = new double[right-left];  // Вспомогательный динамический массив
	// Добавление меньших чисел из двух частей
	while(left+it1<mid and mid+it2<right){
		// Меньшее число из левой части
		if(array[left+it1]<array[mid+it2]){
			result[it1+it2]=array[left+it1];
			it1+=1;
		}
		// Меньшее число из правой части
		else {
			result[it1+it2]=array[mid+it2];
			it2+=1;
		}
	}
	// Добавление оставшихся чисел из одной из частей
	while(left+it1<mid){
		result[it1+it2]=array[left+it1];
		it1+=1;
	}
	while(mid+it2<right){
		result[it1+it2]=array[mid+it2];
		it2+=1;
	}
	// Перезаписываем исходный массив
	for(int i=0;i<it1+it2;i++){
		array[left+i]=result[i];
	}
	// Освобождение памяти из-под вспомогательного массива
	delete[] result;
}
void sorting(int n,double *array){	
   	// Вывод содержимого неотсортированного массива
    cout << "Unsorted array:" << endl;
   	print(n,array);	
   	// Сортировка
    // Определяем срезы массива и проходимся сортировкой по ним
	for(int i=1;i<n+1;i*=2){
		for(int j=0;j<n-1;j+=2*i){
			merge(array,j,j+i,min(j+2*i,n));
		}
	}
   	// Вывод содержимого отсортированного массива
    cout << "Sorted array:" << endl;
   	print(n,array);
}

int main()
{    
	string file_path="data.txt";
    // Открываем файл для чтения
    ifstream file(file_path);
    // Проверка на ошибку при открытии
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
    // Определяем количество чисел в файле
   	long double tempnum;
   	int size=0; // Инициализация размерности массива
    while (file >> tempnum) {
    	// Проверка на корректность данных
    	if(tempnum<numeric_limits<double>::max() and tempnum>numeric_limits<double>::lowest()){
    		size++; // Увеличиваем количество чисел
    		continue;
		}
		cout<<"Error: numbers out of the range of type double";
    	return 1;
    }
	// Проверка на ввод всех символов
    file.clear(); // Сброс возможных ошибок
    int cur=file.tellg(); // Сохраняем позицию курсора
    file.seekg(0, ios::end); // Перемещаем курсор в конец файла
    if(cur!=file.tellg()) { //Сравниваем сохраненный курсор с концом файла
    	cout<<"Error: non-numeric data";
    	return 1;
	}
	
    // Проверка на наличие данных
    if(size==0){
    	cout<<"Error: empty file";
    	return 1;
	}
	
	// Ввод типа массива
    string choice;
    cout << "Select array type (s - static, d - dynamic): ";
    cin >> choice;

    // Обработка выбора пользователя
    if(choice=="s") {// Статический массив
        double array[size]; // Ограничение на размер массива
        input(file_path,size,array);
        sorting(size,array);
    }
    
    else if(choice=="d") {// Динамический массив
        double* array = new double[size]; // Выделение памяти
        input(file_path,size,array);
        sorting(size,array);
        delete[] array;// Освобождение выделенной памяти
    }
    else{ //Ошибка выбора типа массива
        cerr << "Error: incorrect input";
        return 1;
    }
    //Закрываем файл
	file.close();
	return 0;
}