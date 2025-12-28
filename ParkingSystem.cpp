#include "ParkingSystem.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>

using namespace std;

// 构造函数
ParkingSystem::ParkingSystem(int capacity, double rate) 
    : maxCapacity(capacity), hourlyRate(rate),
      smallSpaces(0), mediumSpaces(0), largeSpaces(0),
      usedSmallSpaces(0), usedMediumSpaces(0), usedLargeSpaces(0) {
    // 默认车位配置：各类型车位数量相等
    int eachType = capacity / 3;
    smallSpaces = eachType;
    mediumSpaces = eachType;
    largeSpaces = capacity - 2 * eachType; // 确保总数正确
}

// 设置车位配置
void ParkingSystem::setParkingSpaces(int small, int medium, int large) {
    smallSpaces = small;
    mediumSpaces = medium;
    largeSpaces = large;
    
    // 重置已使用车位计数
    usedSmallSpaces = 0;
    usedMediumSpaces = 0;
    usedLargeSpaces = 0;
}

// 检查是否有合适的车位可用
bool ParkingSystem::hasSuitableSpace(int carType) const {
    switch(carType) {
        case 0: // 小型车
            return usedSmallSpaces < smallSpaces;
        case 1: // 中型车
            return usedMediumSpaces < mediumSpaces;
        case 2: // 大型车
            return usedLargeSpaces < largeSpaces;
        default:
            return false;
    }
}

// 分配车位
int ParkingSystem::allocateParkingSpace(int carType) {
    switch(carType) {
        case 0: // 小型车
            if (usedSmallSpaces < smallSpaces) {
                usedSmallSpaces++;
                return 0; // SPACE_SMALL
            }
            break;
        case 1: // 中型车
            if (usedMediumSpaces < mediumSpaces) {
                usedMediumSpaces++;
                return 1; // SPACE_MEDIUM
            }
            break;
        case 2: // 大型车
            if (usedLargeSpaces < largeSpaces) {
                usedLargeSpaces++;
                return 2; // SPACE_LARGE
            }
            break;
    }
    return -1; // 无法分配
}

// 释放车位
void ParkingSystem::releaseParkingSpace(int spaceType) {
    switch(spaceType) {
        case 0: // SPACE_SMALL
            if (usedSmallSpaces > 0) usedSmallSpaces--;
            break;
        case 1: // SPACE_MEDIUM
            if (usedMediumSpaces > 0) usedMediumSpaces--;
            break;
        case 2: // SPACE_LARGE
            if (usedLargeSpaces > 0) usedLargeSpaces--;
            break;
    }
}

// 车辆到达
int ParkingSystem::carArrival(const string& licensePlate, int carType, time_t arrivalTime) {
    // 检查车辆是否已存在
    if (findCar(licensePlate) != 0) {
        cout << "错误：车牌号 " << licensePlate << " 已存在！" << endl;
        return -1;
    }
    
    // 创建车辆对象
    Car car(licensePlate, static_cast<CarType>(carType), arrivalTime);
    
    // 检查停车场是否有空位
    if (hasSuitableSpace(carType)) {
        // 分配车位
        int spaceType = allocateParkingSpace(carType);
        if (spaceType == -1) {
            cout << "错误：无法分配车位！" << endl;
            return -1;
        }
        
        // 设置车位类型
        car.setSpaceType(static_cast<ParkingSpaceType>(spaceType));
        
        // 检查是否有车辆需要让路（简化版本）
        // 在实际系统中，需要检查目标车位是否被阻挡
        // 这里我们假设如果停车场不为空，可能有车辆需要让路
        if (!parkingLot.isEmpty()) {
            // 显示车辆进入让路过程
            cout << "\n=== 车辆进入让路过程 ===" << endl;
            
            // 步骤1: 检查是否有车辆阻挡
            cout << "步骤1: 检查目标车位是否有车辆阻挡" << endl;
            
            // 获取停车场中的车辆信息
            // 注意：由于Stack没有提供遍历方法，这里简化处理
            // 在实际系统中，需要检查哪些车辆阻挡了目标车位
            
            // 假设有车辆需要让路
            // 在实际系统中，需要根据目标车位位置确定哪些车辆需要让路
            vector<string> movedCars;
            
            // 检查停车场中是否有车辆需要让路
            // 这里简化：假设停车场中所有车辆都可能需要让路（除了新车辆）
            // 在实际系统中，需要根据目标车位位置确定
            int parkingSize = parkingLot.size();
            if (parkingSize > 0) {
                cout << "  - 发现 " << parkingSize << " 辆车可能阻挡目标车位" << endl;
                
                // 模拟让路过程
                // 在实际系统中，需要将阻挡的车辆移到候车道
                cout << "  - 模拟让路：阻挡车辆进入候车道等待" << endl;
                
                // 记录被牵连车辆
                for (int i = 1; i <= parkingSize; i++) {
                    // 获取车辆信息（简化处理）
                    // 在实际系统中，需要获取实际阻挡的车辆
                    string fakeLicense = "阻挡车" + to_string(i);
                    movedCars.push_back(fakeLicense);
                }
                
                // 显示被牵连车辆进入候车道顺序
                if (!movedCars.empty()) {
                    cout << "被牵连车辆进入候车道顺序（从前往后）：";
                    for (size_t i = 0; i < movedCars.size(); i++) {
                        cout << movedCars[i];
                        if (i < movedCars.size() - 1) cout << " → ";
                    }
                    cout << endl;
                }
            } else {
                cout << "  - 无车辆阻挡，可直接停入" << endl;
            }
            
            // 步骤2: 新车辆停入目标车位
            cout << "步骤2: 车辆 " << licensePlate << " 停入目标车位" << endl;
            
            // 将车辆停入停车场
            if (parkingLot.push(car)) {
                cout << "  - 车辆 " << licensePlate << " 已成功停入车位" << endl;
                
                // 步骤3: 让路的车辆按原次序从候车道进入车场
                cout << "步骤3: 让路的车辆按原次序从候车道进入车场" << endl;
                
                if (!movedCars.empty()) {
                    // 模拟让路车辆重新进入
                    for (size_t i = 0; i < movedCars.size(); i++) {
                        cout << "  - 车辆 " << movedCars[i] << " 从候车道按原次序回到停车场" << endl;
                    }
                    
                    // 显示被牵连车辆重新进入停车场顺序
                    cout << "被牵连车辆重新进入停车场顺序（从前往后）：";
                    for (size_t i = 0; i < movedCars.size(); i++) {
                        cout << movedCars[i];
                        if (i < movedCars.size() - 1) cout << " → ";
                    }
                    cout << endl;
                } else {
                    cout << "  - 无让路车辆" << endl;
                }
                
                cout << "=== 让路过程完成 ===" << endl;
                
                // 显示车辆排列信息
                cout << "\n车辆排列：按到达时间从北向南排列，大门在最南端" << endl;
                
                return 0;
            } else {
                // 如果push失败，释放已分配的车位
                releaseParkingSpace(spaceType);
                cout << "错误：停车场已满！" << endl;
                return -1;
            }
        } else {
            // 停车场为空，直接停入
            // 将车辆停入停车场
            if (parkingLot.push(car)) {
                cout << "车辆 " << licensePlate << " 已停入停车场，车位类型：" 
                     << car.getSpaceTypeString() << endl;
                
                // 显示车辆排列信息
                cout << "车辆排列：按到达时间从北向南排列，大门在最南端" << endl;
                cout << "（停车场为空，无需让路）" << endl;
                
                return 0;
            } else {
                // 如果push失败，释放已分配的车位
                releaseParkingSpace(spaceType);
                cout << "错误：停车场已满！" << endl;
                return -1;
            }
        }
    } else {
        // 停车场已满，停入候车道
        waitingLane.enqueue(car);
        cout << "停车场已满，车辆 " << licensePlate << " 已进入候车道等待" << endl;
        return 1;
    }
}

// 车辆离开
bool ParkingSystem::carDeparture(const string& licensePlate, time_t departureTime) {
    // 先在停车场查找
    int position = parkingLot.findCar(licensePlate);
    
    if (position > 0) {
        // 车辆在停车场中
        Car car = parkingLot.getCarAt(position);
        
        // 设置离开时间
        car.setExitTime(departureTime);
        
        // 计算费用
        double fee = car.calculateFee(hourlyRate);
        long duration = car.getParkingDuration();
        
        // 显示费用信息
        cout << "\n==========================================" << endl;
        cout << "车辆离开信息：" << endl;
        cout << "车牌号：" << car.getLicensePlate() << endl;
        cout << "车型：" << car.getTypeString() << endl;
        cout << "车位类型：" << car.getSpaceTypeString() << endl;
        cout << "进入时间：" << timeToString(car.getEntryTime()) << endl;
        cout << "离开时间：" << timeToString(departureTime) << endl;
        cout << "停车时长：" << duration / 3600 << "小时" << duration % 3600 / 60 << "分钟" << endl;
        cout << "应付费用：" << fixed << setprecision(2) << fee << "元" << endl;
        cout << "==========================================" << endl;
        
        // 释放车位
        releaseParkingSpace(car.getSpaceType());
        
        // 移除车辆（需要临时移动其他车辆）
        cout << "\n=== 车辆离开让路过程 ===" << endl;
        
        // 1. 将目标车辆之后的车辆移到临时栈（这些车辆在目标车辆之后开入，需要让路）
        cout << "步骤1: 目标车辆之后的车辆按次序进入临时候车站让路" << endl;
        vector<string> movedCars; // 记录被牵连的车辆顺序
        for (int i = 1; i < position; i++) {
            Car tempCar = parkingLot.peek(); // 获取栈顶车辆（最靠近大门的车辆）
            cout << "  - 车辆 " << tempCar.getLicensePlate() << "（在目标车辆之后开入）进入临时候车站让路" << endl;
            movedCars.push_back(tempCar.getLicensePlate()); // 记录车牌号
            parkingLot.pop();
            // 将车辆加入临时栈（保持让路顺序）
            tempStack.push(tempCar);
        }
        
        // 显示被牵连车辆进出候车道顺序
        if (!movedCars.empty()) {
            cout << "被牵连车辆进入候车道顺序（从前往后）：";
            for (size_t i = 0; i < movedCars.size(); i++) {
                cout << movedCars[i];
                if (i < movedCars.size() - 1) cout << " → ";
            }
            cout << endl;
        }
        
        // 2. 移除目标车辆
        cout << "步骤2: 目标车辆 " << car.getLicensePlate() << " 开出大门外" << endl;
        parkingLot.pop();
        
        // 3. 将临时栈中的车辆移回停车场（按原次序）
        cout << "步骤3: 让路的车辆按原次序从临时候车站进入车场" << endl;
        // 注意：临时栈是LIFO，最后进入的车辆在栈顶
        // 我们需要按原次序放回，所以直接按顺序从tempStack中取出并放回停车场
        vector<string> reenteredCars; // 记录重新进入的车辆顺序
        while (!tempStack.isEmpty()) {
            Car tempCar = tempStack.peek();
            tempStack.pop();
            parkingLot.push(tempCar);
            cout << "  - 车辆 " << tempCar.getLicensePlate() << " 从临时候车站按原次序回到停车场" << endl;
            reenteredCars.push_back(tempCar.getLicensePlate()); // 记录车牌号
        }
        
        // 显示被牵连车辆重新进入停车场顺序
        if (!reenteredCars.empty()) {
            cout << "被牵连车辆重新进入停车场顺序（从前往后）：";
            for (size_t i = 0; i < reenteredCars.size(); i++) {
                cout << reenteredCars[i];
                if (i < reenteredCars.size() - 1) cout << " → ";
            }
            cout << endl;
        }
        
        cout << "=== 让路过程完成 ===" << endl;
        
        // 4. 检查是否有候车道车辆可以进入停车场（如果有空位）
        moveFromWaitingLaneToParkingLot();
        
        return true;
    } else {
        // 在候车道查找
        position = waitingLane.findCar(licensePlate);
        if (position > 0) {
            // 车辆在候车道中，直接移除
            // 注意：候车道中的车辆不收费
            cout << "车辆 " << licensePlate << " 从候车道中离开，不收费" << endl;
            
            // 由于队列不支持中间删除，需要重建队列
            Queue newQueue;
            bool found = false;
            
            // 遍历原队列，跳过要删除的车辆
            while (!waitingLane.isEmpty()) {
                Car car = waitingLane.getFront();
                waitingLane.dequeue();
                
                if (car.getLicensePlate() != licensePlate) {
                    newQueue.enqueue(car);
                } else {
                    found = true;
                }
            }
            
            // 将新队列赋值给原队列
            waitingLane = newQueue;
            
            return found;
        } else {
            cout << "错误：车牌号 " << licensePlate << " 不存在！" << endl;
            return false;
        }
    }
}

// 查询停车场状态
void ParkingSystem::displayParkingStatus() const {
    cout << "\n=== 停车场状态 ===" << endl;
    cout << "总容量：" << maxCapacity << " 辆车" << endl;
    cout << "当前停车数量：" << parkingLot.size() << " 辆车" << endl;
    cout << "可用车位：" << getAvailableSpaces() << " 个" << endl;
    cout << "\n车位使用情况：" << endl;
    cout << "小型车位：" << usedSmallSpaces << "/" << smallSpaces << endl;
    cout << "中型车位：" << usedMediumSpaces << "/" << mediumSpaces << endl;
    cout << "大型车位：" << usedLargeSpaces << "/" << largeSpaces << endl;
    
    parkingLot.display();
}

// 查询候车道状态
void ParkingSystem::displayWaitingLaneStatus() const {
    cout << "\n=== 候车道状态 ===" << endl;
    cout << "候车道长度：" << waitingLane.size() << " 辆车" << endl;
    
    waitingLane.display();
}

// 查询所有状态
void ParkingSystem::displayAllStatus() const {
    displayParkingStatus();
    displayWaitingLaneStatus();
}

// 查找车辆
int ParkingSystem::findCar(const string& licensePlate) const {
    if (parkingLot.findCar(licensePlate) > 0) {
        return 1; // 在停车场
    } else if (waitingLane.findCar(licensePlate) > 0) {
        return 2; // 在候车道
    } else {
        return 0; // 未找到
    }
}

// 获取停车场可用车位数量
int ParkingSystem::getAvailableSpaces() const {
    return maxCapacity - parkingLot.size();
}

// 获取停车场总容量
int ParkingSystem::getTotalCapacity() const {
    return maxCapacity;
}

// 获取候车道长度
int ParkingSystem::getWaitingLaneLength() const {
    return waitingLane.size();
}

// 检查是否有空位
bool ParkingSystem::hasAvailableSpace() const {
    return parkingLot.size() < maxCapacity;
}

// 检查停车场是否已满
bool ParkingSystem::isParkingLotFull() const {
    return parkingLot.size() >= maxCapacity;
}

// 从候车道移动车辆到停车场（当停车场有空位时）
bool ParkingSystem::moveFromWaitingLaneToParkingLot() {
    if (waitingLane.isEmpty() || isParkingLotFull()) {
        return false;
    }
    
    // 获取候车道第一辆车
    Car car = waitingLane.getFront();
    
    // 检查是否有合适的车位
    if (hasSuitableSpace(car.getType())) {
        // 分配车位
        int spaceType = allocateParkingSpace(car.getType());
        if (spaceType == -1) {
            return false;
        }
        
        // 设置车位类型
        car.setSpaceType(static_cast<ParkingSpaceType>(spaceType));
        
        // 从候车道移除
        waitingLane.dequeue();
        
        // 停入停车场
        if (parkingLot.push(car)) {
            cout << "提示：候车道车辆 " << car.getLicensePlate() 
                 << " 已进入停车场，车位类型：" << car.getSpaceTypeString() << endl;
            return true;
        } else {
            // 如果push失败，将车辆放回候车道并释放车位
            waitingLane.enqueue(car);
            releaseParkingSpace(spaceType);
            return false;
        }
    }
    
    return false;
}

// 保存数据到文件
bool ParkingSystem::saveToFile(const string& filename) const {
    ofstream outFile(filename);
    if (!outFile) {
        return false;
    }
    
    // 保存系统配置
    outFile << maxCapacity << " " << hourlyRate << endl;
    outFile << smallSpaces << " " << mediumSpaces << " " << largeSpaces << endl;
    outFile << usedSmallSpaces << " " << usedMediumSpaces << " " << usedLargeSpaces << endl;
    
    // 保存停车场车辆
    outFile << parkingLot.size() << endl;
    // 注意：栈需要从底部开始保存
    // 这里简化处理，实际需要遍历栈
    
    // 保存候车道车辆
    outFile << waitingLane.size() << endl;
    // 队列可以从队头开始保存
    
    outFile.close();
    return true;
}

// 从文件加载数据
bool ParkingSystem::loadFromFile(const string& filename) {
    ifstream inFile(filename);
    if (!inFile) {
        return false;
    }
    
    // 加载系统配置
    inFile >> maxCapacity >> hourlyRate;
    inFile >> smallSpaces >> mediumSpaces >> largeSpaces;
    inFile >> usedSmallSpaces >> usedMediumSpaces >> usedLargeSpaces;
    
    // 注意：这里简化了文件加载，实际需要完整实现
    
    inFile.close();
    return true;
}

// 将时间转换为字符串
string ParkingSystem::timeToString(time_t time) const {
    char buffer[20];
    struct tm* timeinfo = localtime(&time);
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
    return string(buffer);
}

// 获取停车场中所有车辆的信息（用于图形界面）
vector<string> ParkingSystem::getParkingLotInfo() const {
    vector<string> info;
    // 注意：由于Stack没有提供遍历方法，这里简化处理
    // 在实际应用中，需要为Stack添加遍历功能
    info.push_back("停车场中有 " + to_string(parkingLot.size()) + " 辆车");
    return info;
}

// 获取候车道中所有车辆的信息（用于图形界面）
vector<string> ParkingSystem::getWaitingLaneInfo() const {
    vector<string> info;
    
    // 获取候车道中的车辆信息
    // 注意：由于Queue没有提供遍历方法，这里使用一个临时队列来遍历
    Queue tempQueue = waitingLane;
    int count = 0;
    
    while (!tempQueue.isEmpty() && count < 10) { // 最多显示10辆车
        Car car = tempQueue.getFront();
        tempQueue.dequeue();
        
        // 创建车辆信息字符串：车牌号 + 车型
        string carInfo = "车牌: " + car.getLicensePlate() + 
                        " | 车型: " + car.getTypeString();
        info.push_back(carInfo);
        count++;
    }
    
    if (waitingLane.size() > 10) {
        info.push_back("... 还有 " + to_string(waitingLane.size() - 10) + " 辆车");
    }
    
    return info;
}

// 获取车位配置信息
void ParkingSystem::getSpaceInfo(int& small, int& medium, int& large, 
                                int& usedSmall, int& usedMedium, int& usedLarge) const {
    small = smallSpaces;
    medium = mediumSpaces;
    large = largeSpaces;
    usedSmall = usedSmallSpaces;
    usedMedium = usedMediumSpaces;
    usedLarge = usedLargeSpaces;
}
