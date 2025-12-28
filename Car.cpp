#include "Car.h"
#include <sstream>
#include <iomanip>
#include <cmath>

using namespace std;

// 构造函数
Car::Car() : licensePlate(""), type(SMALL), entryTime(0), exitTime(0), spaceType(SPACE_SMALL) {}

Car::Car(const string& plate, CarType carType, time_t entry) 
    : licensePlate(plate), type(carType), entryTime(entry), exitTime(0), spaceType(SPACE_SMALL) {}

// 获取车牌号
string Car::getLicensePlate() const {
    return licensePlate;
}

// 获取车型
CarType Car::getType() const {
    return type;
}

// 获取车型字符串表示
string Car::getTypeString() const {
    switch(type) {
        case SMALL: return "小型车";
        case MEDIUM: return "中型车";
        case LARGE: return "大型车";
        default: return "未知";
    }
}

// 获取进入时间
time_t Car::getEntryTime() const {
    return entryTime;
}

// 设置离开时间
void Car::setExitTime(time_t exit) {
    exitTime = exit;
}

// 获取离开时间
time_t Car::getExitTime() const {
    return exitTime;
}

// 设置车位类型
void Car::setSpaceType(ParkingSpaceType space) {
    spaceType = space;
}

// 获取车位类型
ParkingSpaceType Car::getSpaceType() const {
    return spaceType;
}

// 获取车位类型字符串表示
string Car::getSpaceTypeString() const {
    switch(spaceType) {
        case SPACE_SMALL: return "小型车位";
        case SPACE_MEDIUM: return "中型车位";
        case SPACE_LARGE: return "大型车位";
        default: return "未知";
    }
}

// 计算停车时长（秒）
long Car::getParkingDuration() const {
    if (exitTime == 0) {
        // 如果车辆还未离开，使用当前时间
        time_t now = time(nullptr);
        return difftime(now, entryTime);
    }
    return difftime(exitTime, entryTime);
}

// 计算费用（根据车型和车位类型）
double Car::calculateFee(double hourlyRate) const {
    // 基础费率：每小时hourlyRate元
    // 不同车型和车位类型有不同的费率系数
    double coefficient = 1.0;
    
    // 根据车型调整系数
    switch(type) {
        case SMALL: coefficient *= 1.0; break;
        case MEDIUM: coefficient *= 1.5; break;
        case LARGE: coefficient *= 2.0; break;
    }
    
    // 根据车位类型调整系数
    switch(spaceType) {
        case SPACE_SMALL: coefficient *= 1.0; break;
        case SPACE_MEDIUM: coefficient *= 1.2; break;
        case SPACE_LARGE: coefficient *= 1.5; break;
    }
    
    // 计算停车时长（小时）
    double hours = getParkingDuration() / 3600.0;
    
    // 不足1小时按1小时计算
    if (hours < 1.0) hours = 1.0;
    
    // 计算费用
    return hours * hourlyRate * coefficient;
}

// 检查车辆是否可以停放在指定类型的车位
bool Car::canParkIn(ParkingSpaceType space) const {
    // 规则：上一级别的车不能在下一级别的停车位停车
    // 即：小型车只能停小型车位，中型车可停中型和小型车位，大型车可停大、中、小型车位
    
    switch(type) {
        case SMALL:
            return space == SPACE_SMALL;
        case MEDIUM:
            return space == SPACE_SMALL || space == SPACE_MEDIUM;
        case LARGE:
            return true; // 大型车可以停任何类型的车位
        default:
            return false;
    }
}
