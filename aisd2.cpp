#include <iostream>
#include <fstream>
#include <limits>
#include <chrono>

using namespace std;
struct Node { // Структура узла
    double value;
    Node* next;
    Node* prev;
    Node(double val) : value(val), next(nullptr), prev(nullptr) {}
};

class Deque { // Класс дека
private:
    Node* head;
	Node* tail;
    int size_;

public:
    Deque() : head(nullptr), tail(nullptr), size_(0) {}
	// Добавление в начало
    void pushFront(double x) {
        Node* new_node = new Node(x);
        if (!head) { // Если дек пуст
            head = tail = new_node;
        }
		else { // Сдвиг первого элемента вправо
            new_node->next = head;
            head->prev = new_node;
            head = new_node;
        }
        size_++;
    }
	// Добавление в конец
    void pushBack(double x) {
        Node* new_node = new Node(x);
        if (!tail) { // Если дек пуст
            head = tail = new_node;
        }
		else { // Сдвиг конца на новый элемент
            new_node->prev = tail;
            tail->next = new_node;
            tail = new_node;
        }
        size_++;
    }
	// Удаление из начала
    double popFront() {
        if (!head) { // Проверка на пустоту
        	throw runtime_error("Deque is empty");
		}
        double val = head->value;
        Node* temp = head;
        head = head->next; // Перенос головы на след. узел
        if (head) {  // Обнуляем старую голову
        	head->prev = nullptr;
		}
        else { // Обнуляем хвост
        	tail = nullptr;
		}
        delete temp; // Удаляем старую голову
        size_--;
        return val;
    }
	// Удаление с конца
    double popBack() {
        if (!tail) { // Проверка на пустоту
        	throw runtime_error("Deque is empty");
		}
        double val = tail->value;
        Node* temp = tail;
        tail = tail->prev; // Перенос хвоста на пред. узел
        if (tail) { // Обнуляем старый хвост
        	tail->next = nullptr;	
		}
        else { // Обнуляем  голову
        	head  = nullptr;
		}
        delete temp; // Удаляем старый хвост
        size_--;
        return val;
    }
    // Возвращение значения по индексу
	double get(int index) const {
        if (index < 0 || index >= size_) {
        	throw out_of_range("Index out of range");
		}
        Node* cur = head;
        for (int i = 0; i < index; ++i) {
            cur = cur->next;
        }
        return cur->value;
    }
	// Установление значения по индексу
    void set(int index, double value) {
        if (index < 0 || index >= size_) {
        	throw out_of_range("Index out of range");
		}
        Node* cur = head;
        for (int i = 0; i < index; ++i) {
            cur = cur->next;
        }
        cur->value = value;
    }
	// Размер дека
    int size() const { return size_; }
    // Проверка на пустоту
    bool empty() const { return size_ == 0; }
    // Вывод дека
	void print() const{
		Node* cur=head;
		while (cur != nullptr) {
	        cout << cur->value << " ";
	        cur=cur->next;
	    }
	    cout << endl;
	}
};
void merge(Deque& dq, int left, int mid, int right){// Слияние двух частей
	int it1=0, it2=0;
	// Вспомогательный дек
	Deque result;
	// Добавление меньших чисел из двух частей
	while(left+it1<mid and mid+it2<right){
		// Меньшее число из левой части
		if(dq.get(left+it1)<dq.get(mid+it2)){
			result.pushBack(dq.get(left+it1));
			it1++;
		}
		// Меньшее число из правой части
		else {
			result.pushBack(dq.get(mid+it2));
			it2++;
		}
	}
	// Добавление оставшихся чисел из одной из частей
	while(left+it1<mid){
		result.pushBack(dq.get(left+it1));
		it1++;
	}
	while(mid+it2<right){
		result.pushBack(dq.get(mid+it2));
		it2++;
	}
	// Перезаписываем исходный дек
	for(int i=0;i<it1+it2;i++){
		dq.set(left+i,result.get(i));
	}
}
void sorting(Deque& dq){
	int n=dq.size(); 
   	// Вывод содержимого неотсортированного дека
    cout << "Unsorted deque:" << endl;
   	//dq.print();	
   	
	auto start = std::chrono::high_resolution_clock::now();
   	// Сортировка
    // Определяем срезы дека и проходимся сортировкой по ним
	for(int i=1;i<n+1;i*=2){
		for(int j=0;j<n-1;j+=2*i){
			merge(dq,j,min(j+i,n),min(j+2*i,n));
		}
	}
    auto end = chrono::high_resolution_clock::now();
    auto ms = chrono::duration_cast<chrono::microseconds>(end-start);
	cout<<"Lead time: "<<ms.count()<<endl;
   	// Вывод содержимого отсортированного дека
    cout << "Sorted deque:" << endl;
   	//dq.print();	
}

int main() {
	string file_path="test.txt";
    // Открываем файл для чтения
    ifstream file(file_path);
    // Проверка на ошибку при открытии
    if (!file.is_open()) {
        cerr << "Error opening file" << endl;
        return 1;
    }
   	Deque dq;
   	long double num;
    while (file >> num) {
    	// Проверка на корректность данных
    	if(num<numeric_limits<double>::max() and num>numeric_limits<double>::lowest()){
    		dq.pushBack(num);
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
    if(dq.size()==0){
    	cout<<"Error: empty file";
    	return 1;
	}
    sorting(dq);
}