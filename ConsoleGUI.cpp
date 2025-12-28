#include "ConsoleGUI.h"
#include <iostream>
#include <iomanip>
#include <sstream>
#include <windows.h> // 用于Windows控制台API
#include <algorithm>

using namespace std;

// 构造函数
ConsoleGUI::ConsoleGUI(ParkingSystem& system) : parkingSystem(system) {
    // 初始化控制台
    clearScreen();
}

// 清屏
void ConsoleGUI::clearScreen() {
    system("cls");
}

// 设置控制台颜色（Windows特定）
void ConsoleGUI::setColor(Color foreground, Color background) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, foreground | (background << 4));
}

// 重置颜色
void ConsoleGUI::resetColor() {
    setColor(WHITE, BLACK);
}

// 在指定位置输出文本
void ConsoleGUI::drawText(int x, int y, const string& text) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
    cout << text;
}

// 绘制矩形框
void ConsoleGUI::drawBox(int x, int y, int width, int height, const string& title) {
    // 绘制上边框
    drawText(x, y, "┌");
    for (int i = 1; i < width - 1; i++) {
        drawText(x + i, y, "─");
    }
    drawText(x + width - 1, y, "┐");
    
    // 绘制标题
    if (!title.empty()) {
        drawText(x + 2, y, " " + title + " ");
    }
    
    // 绘制左右边框
    for (int i = 1; i < height - 1; i++) {
        drawText(x, y + i, "│");
        drawText(x + width - 1, y + i, "│");
    }
    
    // 绘制下边框
    drawText(x, y + height - 1, "└");
    for (int i = 1; i < width - 1; i++) {
        drawText(x + i, y + height - 1, "─");
    }
    drawText(x + width - 1, y + height - 1, "┘");
}

// 绘制停车场车位
void ConsoleGUI::drawParkingSpace(int x, int y, int width, int height, 
                                 const string& label, 
                                 bool occupied, 
                                 const string& carInfo) {
    // 设置颜色
    if (occupied) {
        setColor(RED, BLACK);
    } else {
        setColor(GREEN, BLACK);
    }
    
    // 绘制车位边框
    drawBox(x, y, width, height, label);
    
    // 绘制车位内部
    if (occupied && !carInfo.empty()) {
        // 显示车辆信息
        vector<string> lines;
        stringstream ss(carInfo);
        string line;
        int lineNum = 0;
        
        while (getline(ss, line) && lineNum < height - 2) {
            if (line.length() > width - 4) {
                line = line.substr(0, width - 4);
            }
            drawText(x + 2, y + 1 + lineNum, line);
            lineNum++;
        }
    } else if (!occupied) {
        drawText(x + width/2 - 2, y + height/2, "空位");
    }
    
    resetColor();
}

// 绘制车辆
void ConsoleGUI::drawCar(int x, int y, const string& licensePlate, int carType) {
    Color color = getCarTypeColor(carType);
    setColor(color, BLACK);
    
    // 简单车辆图形
    string carSymbol = getCarTypeSymbol(carType);
    drawText(x, y, "[" + carSymbol + "]");
    drawText(x, y + 1, licensePlate.substr(0, 5));
    
    resetColor();
}

// 绘制候车道
void ConsoleGUI::drawWaitingLane(int x, int y, int length, const vector<string>& cars) {
    drawBox(x, y, 30, 5, "候车道");
    
    if (cars.empty()) {
        drawText(x + 2, y + 2, "候车道为空");
        return;
    }
    
    // 显示候车道中的车辆
    int startX = x + 2;
    int startY = y + 2;
    
    for (size_t i = 0; i < cars.size() && i < 5; i++) {
        drawText(startX + i * 6, startY, "[" + cars[i].substr(0, 3) + "]");
    }
    
    if (cars.size() > 5) {
        drawText(startX + 30, startY, "...+" + to_string(cars.size() - 5));
    }
}

// 获取车型颜色
ConsoleGUI::Color ConsoleGUI::getCarTypeColor(int carType) {
    switch(carType) {
        case 0: return BRIGHT_GREEN; // 小型车 - 绿色
        case 1: return BRIGHT_YELLOW; // 中型车 - 黄色
        case 2: return BRIGHT_RED;    // 大型车 - 红色
        default: return WHITE;
    }
}

// 获取车位类型颜色
ConsoleGUI::Color ConsoleGUI::getSpaceTypeColor(int spaceType) {
    switch(spaceType) {
        case 0: return GREEN;  // 小型车位 - 绿色
        case 1: return YELLOW; // 中型车位 - 黄色
        case 2: return RED;    // 大型车位 - 红色
        default: return WHITE;
    }
}

// 获取车型符号
string ConsoleGUI::getCarTypeSymbol(int carType) {
    switch(carType) {
        case 0: return "S"; // 小型车
        case 1: return "M"; // 中型车
        case 2: return "L"; // 大型车
        default: return "?";
    }
}

// 显示主界面
void ConsoleGUI::displayMainMenu(const ParkingSystem& system) {
    clearScreen();
    
    // 绘制标题
    setColor(BRIGHT_CYAN, BLACK);
    drawBox(20, 1, 40, 3, "停车场模拟系统");
    drawText(22, 2, "图形界面版本 v1.0");
    resetColor();
    
    // 显示系统状态概览
    drawBox(5, 5, 70, 8, "系统状态概览");
    
    stringstream status;
    status << "停车场: " << system.getAvailableSpaces() << "/" << system.getTotalCapacity() << " 个空位";
    status << " | 候车道: " << system.getWaitingLaneLength() << " 辆车等待";
    status << " | 费率: " << fixed << setprecision(2) << 5.0 << "元/小时";
    
    drawText(7, 7, status.str());
    
    // 显示菜单选项
    drawBox(5, 14, 70, 10, "菜单");
    
    drawText(7, 16, "1. 车辆到达 (图形界面)");
    drawText(7, 17, "2. 车辆离开 (图形界面)");
    drawText(7, 18, "3. 查看停车场可视化");
    drawText(7, 19, "4. 查看候车道可视化");
    drawText(7, 20, "5. 查看完整系统可视化");
    drawText(7, 21, "6. 运行测试数据");
    drawText(7, 22, "7. 返回文本界面");
    drawText(7, 23, "0. 退出系统");
    
    drawText(7, 25, "请选择操作 (0-7): ");
}

// 显示停车场可视化
void ConsoleGUI::displayParkingVisualization(const ParkingSystem& parkingSystem) {
    clearScreen();
    
    drawBox(5, 1, 70, 22, "停车场可视化 - 南北方向排列");
    
    // 绘制方向说明
    setColor(BRIGHT_CYAN, BLACK);
    drawText(20, 3, "=== 车辆排列规则：按到达时间从北向南排列 ===");
    resetColor();
    
    // 获取车位配置信息
    int smallSpaces, mediumSpaces, largeSpaces;
    int usedSmall, usedMedium, usedLarge;
    parkingSystem.getSpaceInfo(smallSpaces, mediumSpaces, largeSpaces, 
                              usedSmall, usedMedium, usedLarge);
    
    // 显示车位使用情况
    drawText(10, 5, "车位配置: 小型车位 " + to_string(smallSpaces) + 
                    " | 中型车位 " + to_string(mediumSpaces) + 
                    " | 大型车位 " + to_string(largeSpaces));
    
    drawText(10, 6, "已使用: 小型 " + to_string(usedSmall) + "/" + to_string(smallSpaces) + 
                    " | 中型 " + to_string(usedMedium) + "/" + to_string(mediumSpaces) + 
                    " | 大型 " + to_string(usedLarge) + "/" + to_string(largeSpaces));
    
    drawText(10, 7, "总容量: " + to_string(parkingSystem.getTotalCapacity()) + 
                    " | 当前停车: " + to_string(parkingSystem.getTotalCapacity() - parkingSystem.getAvailableSpaces()) +
                    " | 可用车位: " + to_string(parkingSystem.getAvailableSpaces()));
    
    // 绘制方向指示
    setColor(BRIGHT_YELLOW, BLACK);
    drawText(25, 9, "↑ 北 (North) - 最早到达的车辆");
    drawText(25, 10, "↓ 南 (South) - 大门 (Gate)");
    resetColor();
    
    // 绘制停车场布局 - 按南北方向排列
    drawBox(10, 12, 60, 8, "停车场布局 - 从北到南排列 (S-小型车位, M-中型车位, L-大型车位)");
    
    // 显示车位状态 - 按南北方向排列
    int startY = 14;
    int startX = 12;
    
    // 显示小型车位 (Space 1-3) - 最北端
    drawText(startX, startY, "小型车位 (Space 1-3) - 最北端:");
    for (int i = 0; i < smallSpaces; i++) {
        if (i < usedSmall) {
            drawText(startX + 25 + i * 4, startY, "[S●]"); // 占用的小型车
        } else {
            drawText(startX + 25 + i * 4, startY, "[S○]"); // 空位
        }
    }
    
    // 显示中型车位 (Space 4-6) - 中间
    drawText(startX, startY + 2, "中型车位 (Space 4-6) - 中间:");
    for (int i = 0; i < mediumSpaces; i++) {
        if (i < usedMedium) {
            drawText(startX + 25 + i * 4, startY + 2, "[M●]"); // 占用的中型车
        } else {
            drawText(startX + 25 + i * 4, startY + 2, "[M○]"); // 空位
        }
    }
    
    // 显示大型车位 (Space 7-10) - 最南端（靠近大门）
    drawText(startX, startY + 4, "大型车位 (Space 7-10) - 最南端（靠近大门）:");
    for (int i = 0; i < largeSpaces; i++) {
        if (i < usedLarge) {
            drawText(startX + 25 + i * 4, startY + 4, "[L●]"); // 占用的大型车
        } else {
            drawText(startX + 25 + i * 4, startY + 4, "[L○]"); // 空位
        }
    }
    
    // 在底部绘制大门图标
    setColor(BRIGHT_RED, BLACK);
    drawText(30, startY + 6, "╔══════════════════╗");
    drawText(30, startY + 7, "║    大  门        ║");
    drawText(30, startY + 8, "║  (唯一出入口)    ║");
    drawText(30, startY + 9, "╚══════════════════╝");
    resetColor();
    
    // 图例
    drawText(startX, startY + 11, "图例: S● - 占用的小型车 | M● - 占用的中型车 | L● - 占用的大型车");
    drawText(startX, startY + 12, "      S○ - 空的小型车位 | M○ - 空的中型车位 | L○ - 空的大型车位");
    
    // 显示规则说明
    setColor(BRIGHT_WHITE, BLACK);
    drawText(10, startY + 14, "规则说明:");
    drawText(12, startY + 15, "1. 车辆按到达时间从北向南排列，最早的车在最北端");
    drawText(12, startY + 16, "2. 小型车停在Space 1-3，中型车停在Space 4-6，大型车停在Space 7-10");
    drawText(12, startY + 17, "3. 大门在最南端，是唯一的出入口");
    resetColor();
    
    // 显示提示信息
    drawText(10, startY + 19, "按任意键返回主菜单...");
    
    // 等待按键
    ::system("pause > nul");
}

// 显示候车道可视化
void ConsoleGUI::displayWaitingLaneVisualization(const ParkingSystem& parkingSystem) {
    clearScreen();
    
    drawBox(5, 1, 70, 22, "候车道可视化");
    
    // 获取候车道信息
    vector<string> waitingCars = parkingSystem.getWaitingLaneInfo();
    
    drawText(10, 3, "候车道长度: " + to_string(parkingSystem.getWaitingLaneLength()) + " 辆车");
    
    // 绘制候车道示意图
    drawBox(10, 5, 60, 14, "候车道中的车辆");
    
    if (waitingCars.empty()) {
        drawText(12, 7, "候车道为空，没有车辆等待");
    } else {
        // 显示候车道中的车辆信息
        int startY = 7;
        for (size_t i = 0; i < waitingCars.size(); i++) {
            if (startY + i < 18) { // 确保不超出显示区域
                drawText(12, startY + i, to_string(i + 1) + ". " + waitingCars[i]);
            }
        }
    }
    
    // 显示提示信息
    drawText(10, 20, "按任意键返回主菜单...");
    
    // 等待按键
    ::system("pause > nul");
}

// 显示完整系统可视化
void ConsoleGUI::displayFullVisualization(const ParkingSystem& parkingSystem) {
    clearScreen();
    
    drawBox(5, 1, 70, 22, "完整系统可视化 - 停车场模拟系统");
    
    // 绘制标题和说明
    setColor(BRIGHT_CYAN, BLACK);
    drawText(20, 3, "=== 停车场核心规则 ===");
    resetColor();
    
    // 绘制方向指示 - 更加突出
    setColor(BRIGHT_YELLOW, BLACK);
    drawText(15, 5, "┌─────────────────────────────────────────────────────┐");
    drawText(15, 6, "│               车辆排列方向：北 → 南                │");
    drawText(15, 7, "│  最早到达的车辆在最北端，最新到达的车辆在最南端   │");
    drawText(15, 8, "└─────────────────────────────────────────────────────┘");
    resetColor();
    
    // 绘制方向箭头
    setColor(BRIGHT_GREEN, BLACK);
    drawText(35, 10, "↑ 北 (North)");
    drawText(35, 11, "│");
    drawText(35, 12, "│ 最早到达的车辆");
    drawText(35, 13, "│ (First Arrived Car)");
    drawText(35, 14, "│");
    drawText(35, 15, "│");
    drawText(35, 16, "│ 最新到达的车辆");
    drawText(35, 17, "│ (Last Arrived Car)");
    drawText(35, 18, "│");
    drawText(35, 19, "↓ 南 (South)");
    resetColor();
    
    // 绘制停车场（从北到南排列）
    drawBox(10, 10, 20, 10, "停车场");
    drawText(12, 12, "容量: " + to_string(parkingSystem.getTotalCapacity()));
    drawText(12, 13, "当前: " + to_string(parkingSystem.getTotalCapacity() - parkingSystem.getAvailableSpaces()));
    
    // 在停车场底部绘制大门图标 - 更加突出
    setColor(BRIGHT_RED, BLACK);
    drawText(12, 19, "┌──────────────────┐");
    drawText(12, 20, "│   ★ 大 门 ★     │");
    drawText(12, 21, "│ (唯一出入口)     │");
    drawText(12, 22, "└──────────────────┘");
    resetColor();
    
    // 绘制大门说明
    setColor(BRIGHT_YELLOW, BLACK);
    drawText(10, 23, "★ 只有一个大门可供汽车进出 ★");
    resetColor();
    
    // 绘制候车道
    drawBox(55, 10, 15, 10, "候车道");
    drawText(57, 12, "等待: " + to_string(parkingSystem.getWaitingLaneLength()));
    
    // 绘制车辆进出方向
    setColor(BRIGHT_CYAN, BLACK);
    drawText(32, 20, "↘ 车辆进入 (Car Entry)");
    drawText(32, 21, "↙ 车辆离开 (Car Exit)");
    resetColor();
    
    // 绘制详细规则说明
    drawBox(5, 25, 70, 8, "系统核心规则说明");
    
    setColor(BRIGHT_WHITE, BLACK);
    drawText(7, 26, "1. 只有一个大门：所有车辆必须通过此大门进出停车场");
    drawText(7, 27, "2. 南北方向排列：车辆按到达时间从北向南排列");
    drawText(7, 28, "3. 最早车辆在北：最先到达的第一辆车停放在车场的最北端");
    drawText(7, 29, "4. 大门在最南端：大门位于停车场最南端，靠近最新到达的车辆");
    drawText(7, 30, "5. 让路规则：车辆离开时，阻挡的车辆必须先退出车场让路");
    resetColor();
    
    // 显示提示信息
    drawText(10, 33, "按任意键返回主菜单...");
    
    // 等待按键
    ::system("pause > nul");
}

// 显示车辆到达界面
void ConsoleGUI::displayArrivalInterface() {
    clearScreen();
    drawBox(5, 1, 70, 22, "车辆到达");
    
    drawText(10, 3, "请在下方输入车辆信息：");
    
    // 显示输入区域
    drawBox(10, 5, 60, 12, "输入车辆信息");
    
    // 获取车牌号
    drawText(12, 7, "车牌号: ");
    std::string licensePlate;
    std::cin >> licensePlate;
    
    // 获取车型
    drawText(12, 9, "车型 (0-小型车, 1-中型车, 2-大型车): ");
    int carType;
    std::cin >> carType;
    
    // 验证车型输入
    if (carType < 0 || carType > 2) {
        drawText(12, 12, "错误：车型输入无效！");
        drawText(12, 14, "按任意键返回...");
        ::system("pause > nul");
        return;
    }
    
    // 使用当前时间作为到达时间
    time_t arrivalTime = time(nullptr);
    
    // 调用ParkingSystem的carArrival方法
    int result = parkingSystem.carArrival(licensePlate, carType, arrivalTime);
    
    // 显示结果
    if (result == 0) {
        // 获取车位配置信息以显示详细计数
        int smallSpaces, mediumSpaces, largeSpaces;
        int usedSmall, usedMedium, usedLarge;
        parkingSystem.getSpaceInfo(smallSpaces, mediumSpaces, largeSpaces, 
                                  usedSmall, usedMedium, usedLarge);
        
        string carTypeStr;
        switch(carType) {
            case 0: carTypeStr = "小型车"; break;
            case 1: carTypeStr = "中型车"; break;
            case 2: carTypeStr = "大型车"; break;
            default: carTypeStr = "未知车型";
        }
        
        stringstream ss;
        ss << "? " << carTypeStr << " (" << licensePlate << ") 已成功停入停车场！";
        drawText(12, 12, ss.str());
        
        // 显示当前车位使用情况
        stringstream ss2;
        ss2 << "当前车位使用: 小型车 " << usedSmall << "/" << smallSpaces 
            << " | 中型车 " << usedMedium << "/" << mediumSpaces 
            << " | 大型车 " << usedLarge << "/" << largeSpaces;
        drawText(12, 13, ss2.str());
    } else if (result == 1) {
        drawText(12, 12, "? 停车场已满，" + licensePlate + " 已进入候车道等待！");
    } else {
        drawText(12, 12, "? 车辆到达失败！");
    }
    
    drawText(12, 14, "按任意键返回主菜单...");
    ::system("pause > nul");
}

// 显示车辆离开界面
void ConsoleGUI::displayDepartureInterface() {
    clearScreen();
    drawBox(5, 1, 70, 22, "车辆离开");
    
    drawText(10, 3, "请在下方输入车辆信息：");
    
    // 显示输入区域
    drawBox(10, 5, 60, 12, "输入车辆信息");
    
    // 获取车牌号
    drawText(12, 7, "车牌号: ");
    std::string licensePlate;
    std::cin >> licensePlate;
    
    // 使用当前时间作为离开时间
    time_t departureTime = time(nullptr);
    
    // 调用ParkingSystem的carDeparture方法
    bool result = parkingSystem.carDeparture(licensePlate, departureTime);
    
    // 显示结果
    if (result) {
        drawText(12, 10, "? 车辆离开成功！");
    } else {
        drawText(12, 10, "? 车辆离开失败！车辆可能不存在。");
    }
    
    drawText(12, 12, "按任意键返回主菜单...");
    ::system("pause > nul");
}

// 显示查找车辆界面
void ConsoleGUI::displayFindCarInterface() {
    clearScreen();
    drawBox(5, 1, 70, 22, "查找车辆");
    
    drawText(10, 3, "此功能需要从文本界面输入数据");
    drawText(10, 5, "请返回文本界面使用选项6");
    
    drawText(10, 20, "按任意键返回主菜单...");
    ::system("pause > nul");
}

// 显示测试数据运行界面
void ConsoleGUI::displayTestDataInterface() {
    clearScreen();
    drawBox(5, 1, 70, 22, "运行测试数据");
    
    drawText(10, 3, "正在运行测试数据...");
    drawText(10, 5, "测试数据来自课题要求：");
    drawText(10, 6, "设n=2，输入数据为：");
    drawText(10, 7, "('A',1,5),('A',2,10),('D',1,15),('A',3,20),");
    drawText(10, 8, "('A',4,25),('A',5,30),('D',2,35),('D',4,40)");
    
    // 创建一个临时的ParkingSystem对象来运行测试数据
    // 注意：这里我们修改了当前的parkingSystem对象
    // 设置停车场容量为2
    parkingSystem = ParkingSystem(2, 5.0); // 2个车位，每小时5元
    
    // 测试数据
    struct TestData {
        char action;
        int licensePlate;
        int timeValue;
    };
    
    TestData testData[] = {
        {'A', 1, 5},   // 车辆1到达，时间5
        {'A', 2, 10},  // 车辆2到达，时间10
        {'D', 1, 15},  // 车辆1离开，时间15
        {'A', 3, 20},  // 车辆3到达，时间20
        {'A', 4, 25},  // 车辆4到达，时间25
        {'A', 5, 30},  // 车辆5到达，时间30
        {'D', 2, 35},  // 车辆2离开，时间35
        {'D', 4, 40}   // 车辆4离开，时间40
    };
    
    int dataCount = sizeof(testData) / sizeof(testData[0]);
    
    // 确保测试数据不超过10组
    if (dataCount > 10) {
        drawText(10, 10, "警告：测试数据超过10组，将只运行前10组");
        dataCount = 10;
    }
    
    for (int i = 0; i < dataCount; i++) {
        char action = testData[i].action;
        int licensePlate = testData[i].licensePlate;
        int timeValue = testData[i].timeValue;
        
        stringstream ss;
        ss << "操作 " << (i+1) << "/" << dataCount << ": " << action << ", 车牌: " << licensePlate << ", 时间: " << timeValue;
        drawText(10, 12 + i, ss.str());
        
        if (action == 'A') {
            // 车辆到达
            string plateStr = to_string(licensePlate);
            parkingSystem.carArrival(plateStr, 0, timeValue);
        } else if (action == 'D') {
            // 车辆离开
            string plateStr = to_string(licensePlate);
            parkingSystem.carDeparture(plateStr, timeValue);
        }
    }
    
    drawText(10, 12 + dataCount + 1, "测试数据运行完成！");
    drawText(10, 12 + dataCount + 2, "按任意键返回主菜单...");
    ::system("pause > nul");
}

// 显示文件操作界面
void ConsoleGUI::displayFileOperationsInterface() {
    clearScreen();
    drawBox(5, 1, 70, 22, "文件操作");
    
    drawText(10, 3, "此功能需要从文本界面操作");
    drawText(10, 5, "请返回文本界面使用选项8或9");
    
    drawText(10, 20, "按任意键返回主菜单...");
    ::system("pause > nul");
}
