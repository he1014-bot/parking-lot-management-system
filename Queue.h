#ifndef QUEUE_H
#define QUEUE_H

#include "Car.h"

// 队列节点结构
struct QueueNode {
    Car car;            // 车辆数据
    QueueNode* next;    // 指向下一个节点的指针
    
    QueueNode(const Car& c) : car(c), next(nullptr) {}
};

class Queue {
private:
    QueueNode* front;   // 队头指针
    QueueNode* rear;    // 队尾指针
    int count;          // 队列中元素数量
    
public:
    // 构造函数
    Queue();
    
    // 析构函数
    ~Queue();
    
    // 判断队列是否为空
    bool isEmpty() const;
    
    // 获取队列中元素数量
    int size() const;
    
    // 入队操作
    void enqueue(const Car& car);
    
    // 出队操作
    bool dequeue();
    
    // 获取队头元素
    Car getFront() const;
    
    // 查找车辆（通过车牌号）
    // 返回车辆在队列中的位置（从队头开始计数，1表示队头）
    int findCar(const std::string& licensePlate) const;
    
    // 获取指定位置的车辆（从队头开始计数，1表示队头）
    Car getCarAt(int position) const;
    
    // 显示队列中所有车辆信息
    void display() const;
    
    // 清空队列
    void clear();
};

#endif // QUEUE_H
