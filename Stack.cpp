#include "Stack.h"
#include <iostream>
#include <iomanip>

using namespace std;

// 构造函数
Stack::Stack() : top(-1) {}

// 判断栈是否为空
bool Stack::isEmpty() const {
    return top == -1;
}

// 判断栈是否已满
bool Stack::isFull() const {
    return top == MAX_SIZE - 1;
}

// 获取栈中元素数量
int Stack::size() const {
    return top + 1;
}

// 入栈操作
bool Stack::push(const Car& car) {
    if (isFull()) {
        return false;
    }
    data[++top] = car;
    return true;
}

// 出栈操作
bool Stack::pop() {
    if (isEmpty()) {
        return false;
    }
    top--;
    return true;
}

// 获取栈顶元素
Car Stack::peek() const {
    if (isEmpty()) {
        // 返回一个默认的Car对象
        return Car();
    }
    return data[top];
}

// 查找车辆（通过车牌号）
int Stack::findCar(const string& licensePlate) const {
    for (int i = top; i >= 0; i--) {
        if (data[i].getLicensePlate() == licensePlate) {
            // 返回位置（从栈顶开始计数，1表示栈顶）
            return top - i + 1;
        }
    }
    return 0; // 未找到
}

// 获取指定位置的车辆（从栈顶开始计数，1表示栈顶）
Car Stack::getCarAt(int position) const {
    if (position < 1 || position > size()) {
        // 位置无效，返回默认Car对象
        return Car();
    }
    // 计算数组索引：栈顶是data[top]，位置1对应data[top]
    int index = top - position + 1;
    return data[index];
}

// 移除指定位置的车辆（从栈顶开始计数，1表示栈顶）
bool Stack::removeCarAt(int position) {
    if (position < 1 || position > size()) {
        return false;
    }
    
    // 计算要移除的车辆的数组索引
    int removeIndex = top - position + 1;
    
    // 将后面的车辆向前移动
    for (int i = removeIndex; i < top; i++) {
        data[i] = data[i + 1];
    }
    
    top--;
    return true;
}

// 显示栈中所有车辆信息
void Stack::display() const {
    if (isEmpty()) {
        cout << "停车场为空" << endl;
        return;
    }
    
    cout << "停车场状态（栈顶在最前面）：" << endl;
    cout << "==========================================" << endl;
    cout << left << setw(15) << "位置" 
         << setw(15) << "车牌号" 
         << setw(15) << "车型" 
         << setw(15) << "车位类型" 
         << setw(20) << "进入时间" << endl;
    cout << "------------------------------------------" << endl;
    
    for (int i = top; i >= 0; i--) {
        int position = top - i + 1;
        Car car = data[i];
        
        // 转换时间为字符串
        char entryTimeStr[20];
        time_t entryTime = car.getEntryTime();
        struct tm* timeinfo = localtime(&entryTime);
        strftime(entryTimeStr, sizeof(entryTimeStr), "%Y-%m-%d %H:%M:%S", timeinfo);
        
        cout << left << setw(15) << position
             << setw(15) << car.getLicensePlate()
             << setw(15) << car.getTypeString()
             << setw(15) << car.getSpaceTypeString()
             << setw(20) << entryTimeStr << endl;
    }
    cout << "==========================================" << endl;
}
