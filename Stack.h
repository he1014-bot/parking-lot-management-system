#ifndef STACK_H
#define STACK_H

#include "Car.h"

class Stack {
private:
    static const int MAX_SIZE = 100;  // 栈的最大容量
    Car data[MAX_SIZE];               // 存储车辆的数组
    int top;                          // 栈顶指针
    
public:
    // 构造函数
    Stack();
    
    // 判断栈是否为空
    bool isEmpty() const;
    
    // 判断栈是否已满
    bool isFull() const;
    
    // 获取栈中元素数量
    int size() const;
    
    // 入栈操作
    bool push(const Car& car);
    
    // 出栈操作
    bool pop();
    
    // 获取栈顶元素
    Car peek() const;
    
    // 查找车辆（通过车牌号）
    // 返回车辆在栈中的位置（从栈顶开始计数，1表示栈顶）
    int findCar(const std::string& licensePlate) const;
    
    // 获取指定位置的车辆（从栈顶开始计数，1表示栈顶）
    Car getCarAt(int position) const;
    
    // 移除指定位置的车辆（从栈顶开始计数，1表示栈顶）
    // 用于车辆离开时的操作
    bool removeCarAt(int position);
    
    // 显示栈中所有车辆信息
    void display() const;
};

#endif // STACK_H
