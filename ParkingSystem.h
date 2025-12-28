#ifndef PARKINGSYSTEM_H
#define PARKINGSYSTEM_H

#include "Stack.h"
#include "Queue.h"
#include <string>
#include <vector>

class ParkingSystem {
private:
    Stack parkingLot;           // 停车场（栈）
    Queue waitingLane;          // 候车道（队列）
    Stack tempStack;            // 临时栈（用于车辆离开时临时存放）
    
    int maxCapacity;            // 停车场最大容量
    double hourlyRate;          // 每小时费率
    
    // 车位配置
    int smallSpaces;           // 小型车位数量
    int mediumSpaces;          // 中型车位数量
    int largeSpaces;           // 大型车位数量
    
    int usedSmallSpaces;       // 已使用的小型车位
    int usedMediumSpaces;      // 已使用的中型车位
    int usedLargeSpaces;       // 已使用的大型车位
    
public:
    // 构造函数
    ParkingSystem(int capacity = 10, double rate = 5.0);
    
    // 设置车位配置
    void setParkingSpaces(int small, int medium, int large);
    
    // 车辆到达
    // 返回：0-成功停入停车场，1-停入候车道，-1-失败
    int carArrival(const std::string& licensePlate, int carType, time_t arrivalTime);
    
    // 车辆离开
    // 返回：true-成功离开，false-失败（车辆不存在）
    bool carDeparture(const std::string& licensePlate, time_t departureTime);
    
    // 查询停车场状态
    void displayParkingStatus() const;
    
    // 查询候车道状态
    void displayWaitingLaneStatus() const;
    
    // 查询所有状态
    void displayAllStatus() const;
    
    // 查找车辆
    // 返回：0-未找到，1-在停车场，2-在候车道
    int findCar(const std::string& licensePlate) const;
    
    // 获取停车场可用车位数量
    int getAvailableSpaces() const;
    
    // 获取停车场总容量
    int getTotalCapacity() const;
    
    // 获取候车道长度
    int getWaitingLaneLength() const;
    
    // 检查是否有空位
    bool hasAvailableSpace() const;
    
    // 检查停车场是否已满
    bool isParkingLotFull() const;
    
    // 从候车道移动车辆到停车场（当停车场有空位时）
    bool moveFromWaitingLaneToParkingLot();
    
    // 保存数据到文件
    bool saveToFile(const std::string& filename) const;
    
    // 从文件加载数据
    bool loadFromFile(const std::string& filename);
    
    // 获取停车场中所有车辆的信息（用于图形界面）
    // 返回一个向量，包含停车场中所有车辆的信息字符串
    std::vector<std::string> getParkingLotInfo() const;
    
    // 获取候车道中所有车辆的信息（用于图形界面）
    std::vector<std::string> getWaitingLaneInfo() const;
    
    // 获取车位配置信息
    void getSpaceInfo(int& small, int& medium, int& large, 
                     int& usedSmall, int& usedMedium, int& usedLarge) const;
    
private:
    // 检查是否有合适的车位可用
    bool hasSuitableSpace(int carType) const;
    
    // 分配车位
    // 返回分配的车位类型，如果无法分配返回-1
    int allocateParkingSpace(int carType);
    
    // 释放车位
    void releaseParkingSpace(int spaceType);
    
    // 将时间转换为字符串
    std::string timeToString(time_t time) const;
};

#endif // PARKINGSYSTEM_H
