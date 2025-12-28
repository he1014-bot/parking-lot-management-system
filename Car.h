#ifndef CAR_H
#define CAR_H

#include <string>
#include <ctime>

// 车型枚举
enum CarType {
    SMALL,      // 小型车
    MEDIUM,     // 中型车
    LARGE       // 大型车
};

// 车位类型枚举
enum ParkingSpaceType {
    SPACE_SMALL,    // 小型车位
    SPACE_MEDIUM,   // 中型车位
    SPACE_LARGE     // 大型车位
};

class Car {
private:
    std::string licensePlate;  // 车牌号
    CarType type;              // 车型
    time_t entryTime;          // 进入时间
    time_t exitTime;           // 离开时间（如果已离开）
    ParkingSpaceType spaceType; // 停放的车位类型
    
public:
    // 构造函数
    Car();
    Car(const std::string& plate, CarType carType, time_t entry);
    
    // 获取车牌号
    std::string getLicensePlate() const;
    
    // 获取车型
    CarType getType() const;
    
    // 获取车型字符串表示
    std::string getTypeString() const;
    
    // 获取进入时间
    time_t getEntryTime() const;
    
    // 设置离开时间
    void setExitTime(time_t exit);
    
    // 获取离开时间
    time_t getExitTime() const;
    
    // 设置车位类型
    void setSpaceType(ParkingSpaceType space);
    
    // 获取车位类型
    ParkingSpaceType getSpaceType() const;
    
    // 获取车位类型字符串表示
    std::string getSpaceTypeString() const;
    
    // 计算停车时长（秒）
    long getParkingDuration() const;
    
    // 计算费用（根据车型和车位类型）
    double calculateFee(double hourlyRate) const;
    
    // 检查车辆是否可以停放在指定类型的车位
    bool canParkIn(ParkingSpaceType space) const;
};

#endif // CAR_H
