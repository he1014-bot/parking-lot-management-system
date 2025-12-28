#include "Queue.h"
#include <iostream>
#include <iomanip>

using namespace std;

// 构造函数
Queue::Queue() : front(nullptr), rear(nullptr), count(0) {}

// 析构函数
Queue::~Queue() {
    clear();
}

// 判断队列是否为空
bool Queue::isEmpty() const {
    return front == nullptr;
}

// 获取队列中元素数量
int Queue::size() const {
    return count;
}

// 入队操作
void Queue::enqueue(const Car& car) {
    QueueNode* newNode = new QueueNode(car);
    
    if (isEmpty()) {
        front = rear = newNode;
    } else {
        rear->next = newNode;
        rear = newNode;
    }
    
    count++;
}

// 出队操作
bool Queue::dequeue() {
    if (isEmpty()) {
        return false;
    }
    
    QueueNode* temp = front;
    front = front->next;
    
    if (front == nullptr) {
        rear = nullptr;
    }
    
    delete temp;
    count--;
    return true;
}

// 获取队头元素
Car Queue::getFront() const {
    if (isEmpty()) {
        // 返回一个默认的Car对象
        return Car();
    }
    return front->car;
}

// 查找车辆（通过车牌号）
int Queue::findCar(const string& licensePlate) const {
    QueueNode* current = front;
    int position = 1;
    
    while (current != nullptr) {
        if (current->car.getLicensePlate() == licensePlate) {
            return position;
        }
        current = current->next;
        position++;
    }
    
    return 0; // 未找到
}

// 获取指定位置的车辆（从队头开始计数，1表示队头）
Car Queue::getCarAt(int position) const {
    if (position < 1 || position > count) {
        // 位置无效，返回默认Car对象
        return Car();
    }
    
    QueueNode* current = front;
    for (int i = 1; i < position; i++) {
        current = current->next;
    }
    
    return current->car;
}

// 显示队列中所有车辆信息
void Queue::display() const {
    if (isEmpty()) {
        cout << "候车道为空" << endl;
        return;
    }
    
    cout << "候车道状态（队头在最前面）：" << endl;
    cout << "==========================================" << endl;
    cout << left << setw(15) << "位置" 
         << setw(15) << "车牌号" 
         << setw(15) << "车型" 
         << setw(20) << "进入时间" << endl;
    cout << "------------------------------------------" << endl;
    
    QueueNode* current = front;
    int position = 1;
    
    while (current != nullptr) {
        Car car = current->car;
        
        // 转换时间为字符串
        char entryTimeStr[20];
        time_t entryTime = car.getEntryTime();
        struct tm* timeinfo = localtime(&entryTime);
        strftime(entryTimeStr, sizeof(entryTimeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        
        cout << left << setw(15) << position
             << setw(15) << car.getLicensePlate()
             << setw(15) << car.getTypeString()
             << setw(20) << entryTimeStr << endl;
        
        current = current->next;
        position++;
    }
    cout << "==========================================" << endl;
}

// 清空队列
void Queue::clear() {
    while (!isEmpty()) {
        dequeue();
    }
}
