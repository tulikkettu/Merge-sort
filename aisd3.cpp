#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>

using namespace std;
const int N=10000;
class queue {
public: 
	int data[N];
	int first; //указатель на начало
	int last; //указатель на конец
	queue(): first(1), last(1) {}//оставляем зазор в виде одного элемента
	bool empty() {//проверка очереди на пустоту
		if (last==first) return true;
		else return false;
	}
	// Добавление элемента
	void add(double value) {
		if ((last%(N-1))+1==first){
			throw runtime_error("The queue is full\n");
		}
		else {
			data[last]=value;
			last=(last%(N-1))+1;// Сдвиг last вправо
		}
	}
	// Удаление элемента
	double pop() {
		if (last==first){
			throw runtime_error("The queue is empty\n");	
		}
		else {
			double x=data[first];
			first=(first%(N-1))+1;// Сдвиг first вправо
			return x;
		}
	}
	// Размер очереди
	int size() {
		if (first>last)
			return (N-1)-(first-last);
		else
			return last-first;
	}
	// Вывод очереди
	void print() {
		if (first<last){
			for(int i=first;i<last;i++){
				cout<<data[i]<<" ";
			}
			cout<<endl;
		}
		else {
			int i;
			for(i=first;i<N;i++){
				cout<<data[i]<<" ";
			}
			for(i=1;i<last;i++){
				cout<<data[i]<<" ";
			}
			cout<<endl;
		}
	}
};
void merge(queue &q, queue &left, queue &right) {
	//Добавление меньших чисел
	while (!left.empty() && !right.empty()) {
		// Сравнение первых элементов из частей и добавление меньшего
        if (left.data[left.first]<right.data[right.first]) {
            q.add(left.pop());
        }
		else {
            q.add(right.pop());
        }
    }
    // Добавление оставшихся элементов
    while (!left.empty()) q.add(left.pop());
    while (!right.empty()) q.add(right.pop());
}
// Рекурсивная сортировка слиянием
void merge_sort(queue &q) {
    if(q.size() <= 1) return;
    // Разделение очереди на две части
    queue left, right;
    int half = q.size() / 2;
    // Заполнение первой части
    for (int i=0; i<half; i++) {
        left.add(q.pop());
    }
    // Заполнение второй части
    while (!q.empty()) {
        right.add(q.pop());
    }
    // Сортировка каждой части
    merge_sort(left);
    merge_sort(right);
    // Слияние двух отсортированных частей
    merge(q, left, right);
}

int main() {
	string file_path="data.txt";
    // Открываем файл для чтения
    ifstream file(file_path);
    // Проверка на ошибку при открытии
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
	queue q;
    long double num;
    while (file >> num) {
    	// Проверка на корректность данных
    	if(num<numeric_limits<double>::max() and num>numeric_limits<double>::lowest()){
    		q.add(num);
    		continue;
		}
		cout<<"Error: numbers out of range";
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
    if(q.size()==0){
    	cout<<"Error: empty file";
    	return 1;
	}
	cout<<"Unsorted queue:\n";
	q.print();
    merge_sort(q);
	cout<<"Sorted queue:\n";
	q.print();
return 0;
}