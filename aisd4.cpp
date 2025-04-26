#include <iostream>
#include <fstream>
#include <limits>
#include <stdexcept>
#include "red&black.cpp"

using namespace std;
const int N=50;
// Структура
struct node {  
	double data; 
	node *next;
};
class stack {
private:
	node *top;
	int size_;
public:
	stack(): top(nullptr), size_(0) {}
	// Добавление элемента
	void push (double x) {
		node *Newnode = new node;
		Newnode->data=x;
		Newnode->next=top;
		top = Newnode;
		size_++;
	}
	// Удаление элемента
	double pop() {
		double x;
		node *q=top;
		if(top==nullptr){//пустой стек
			throw runtime_error("Stack is empty\n");	
		}
		x = top->data;//запоминаем удаленное значение
		top = top->next;//смещение указателя
		delete q;//освобождаем память
		size_--;
		return x;
	}
	// Возвращение значения по индексу
	double top_value() { return top->data;}
	// Размер стека
    int size() const { return size_; }
    // Проверка на пустоту
    bool empty() const { return size_ == 0; }
    void print() const {
        node* cur = top;//указатель на вершину
        while (cur != nullptr) {
            cout<<cur->data<<" ";
            cur = cur->next;
        }
        cout<<endl;
    }
};
void merge(stack &s, stack &left, stack &right) {
	stack result;
	//Добавление меньших чисел
	while (!left.empty() && !right.empty()) {
		// Сравнение первых элементов из частей и добавление меньшего
        if (left.top_value()<right.top_value()) {
            result.push(left.pop());
        }
		else {
            result.push(right.pop());
        }
    }
    // Добавление оставшихся элементов
    while (!left.empty()) result.push(left.pop());
    while (!right.empty()) result.push(right.pop());
    // Разворачиваем стек
    while (!result.empty()) s.push(result.pop());
}
// Рекурсивная сортировка слиянием
void merge_sort(stack &s) {
    if(s.size() <= 1) return;
    // Разделение очереди на две части
    stack left, right;
    int half = s.size() / 2;
    // Заполнение первой части
    for (int i=0; i<half; i++) {
        left.push(s.pop());
    }
    // Заполнение второй части
    while (!s.empty()) {
        right.push(s.pop());
    }
    // Сортировка каждой части
    merge_sort(left);
    merge_sort(right);
    // Слияние двух отсортированных частей
    merge(s, left, right);
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
   	stack s;
   	long double num;
    RedBlackTree tree;
    while (file >> num) {
    	// Проверка на корректность данных
    	if(num<numeric_limits<double>::max() and num>numeric_limits<double>::lowest()){
    		s.push(num);
    		tree.insert(num);
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
    if(s.size()==0){
    	cout<<"Error: empty file";
    	return 1;
	}
	cout<<"Unsorted stack:\n";
	s.print();
    merge_sort(s);
	cout<<"Sorted stack:\n";
	s.print();
	tree.printTree();
return 0;
}