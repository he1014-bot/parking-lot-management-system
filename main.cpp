#include "ParkingSystem.h"
#include "ConsoleGUI.h"
#include <iostream>
#include <string>
#include <ctime>
#include <vector>
#include <sstream>
#include <iomanip>
#include <cstring>

using namespace std;

// 显示菜单
void displayMenu() {
    cout << "\n========== 停车场管理系统 ==========" << endl;
    cout << "1. 车辆到达" << endl;
    cout << "2. 车辆离开" << endl;
    cout << "3. 查询停车场状态" << endl;
    cout << "4. 查询候车道状态" << endl;
    cout << "5. 查询所有状态" << endl;
    cout << "6. 查找车辆" << endl;
    cout << "7. 运行测试数据" << endl;
    cout << "8. 保存数据到文件" << endl;
    cout << "9. 从文件加载数据" << endl;
    cout << "10. 图形界面模式" << endl;
    cout << "0. 退出系统" << endl;
    cout << "=====================================" << endl;
    cout << "请选择操作 (0-10): ";
}

// 获取车型输入
int getCarType() {
    int type;
    while (true) {
        cout << "请选择车型 (0-小型车, 1-中型车, 2-大型车): ";
        cin >> type;
        if (type >= 0 && type <= 2) {
            return type;
        }
        cout << "输入错误，请重新输入！" << endl;
    }
}

// 获取时间输入
time_t getTimeInput(const string& prompt) {
    string timeStr;
    cout << prompt << " (格式: YYYY-MM-DD HH:MM:SS，直接回车使用当前时间): ";
    cin.ignore(); // 清除输入缓冲区
    getline(cin, timeStr);
    
    if (timeStr.empty()) {
        return time(nullptr); // 使用当前时间
    }
    
    // 解析时间字符串 - 手动解析，因为get_time在C++11中可能不可用
    struct tm tm;
    memset(&tm, 0, sizeof(struct tm));
    
    // 简单解析格式：YYYY-MM-DD HH:MM:SS
    int year, month, day, hour, minute, second;
    if (sscanf(timeStr.c_str(), "%d-%d-%d %d:%d:%d", 
               &year, &month, &day, &hour, &minute, &second) == 6) {
        tm.tm_year = year - 1900;
        tm.tm_mon = month - 1;
        tm.tm_mday = day;
        tm.tm_hour = hour;
        tm.tm_min = minute;
        tm.tm_sec = second;
        tm.tm_isdst = -1; // 自动判断夏令时
        
        return mktime(&tm);
    } else {
        cout << "时间格式错误，使用当前时间" << endl;
        return time(nullptr);
    }
}

// 运行测试数据
void runTestData(ParkingSystem& system) {
    cout << "\n=== 运行测试数据 ===" << endl;
    cout << "测试数据来自课题要求：" << endl;
    cout << "设n=2，输入数据为：" << endl;
    cout << "('A',1,5),('A',2,10),('D',1,15),('A',3,20)," << endl;
    cout << "('A',4,25),('A',5,30),('D',2,35),('D',4,40)" << endl;
    cout << "注意：测试数据最多不超过10组" << endl;
    
    // 设置停车场容量为2
    system = ParkingSystem(2, 5.0); // 2个车位，每小时5元
    
    // 测试数据 - 使用简单结构代替tuple
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
        cout << "警告：测试数据超过10组，将只运行前10组" << endl;
        dataCount = 10;
    }
    
    for (int i = 0; i < dataCount; i++) {
        char action = testData[i].action;
        int licensePlate = testData[i].licensePlate;
        int timeValue = testData[i].timeValue;
        
        cout << "\n操作 " << (i+1) << "/" << dataCount << ": " << action << ", 车牌: " << licensePlate << ", 时间: " << timeValue << endl;
        
        if (action == 'A') {
            // 车辆到达
            // 将车牌号转换为字符串
            string plateStr = to_string(licensePlate);
            // 使用小型车作为测试
            system.carArrival(plateStr, 0, timeValue);
        } else if (action == 'D') {
            // 车辆离开
            string plateStr = to_string(licensePlate);
            system.carDeparture(plateStr, timeValue);
        }
        
        // 每2次操作显示一次简要状态，避免刷屏
        if ((i + 1) % 2 == 0 || i == dataCount - 1) {
            cout << "  当前状态: 停车场 " << (system.getTotalCapacity() - system.getAvailableSpaces()) 
                 << "/" << system.getTotalCapacity() << ", 候车道 " 
                 << system.getWaitingLaneLength() << " 辆车" << endl;
        }
    }
    
    cout << "\n测试数据运行完成！共运行了 " << dataCount << " 组数据。" << endl;
    cout << "\n最终状态:" << endl;
    system.displayAllStatus();
    cout << "\n按任意键返回主菜单..." << endl;
    ::system("pause > nul");
}

// 图形界面模式
void graphicalInterfaceMode(ParkingSystem& system) {
    ConsoleGUI gui(system);
    bool inGraphicalMode = true;
    
    while (inGraphicalMode) {
        gui.displayMainMenu(system);
        
        int choice;
        cin >> choice;
        
        switch (choice) {
            case 0: // 退出系统
                inGraphicalMode = false;
                cout << "退出图形界面模式..." << endl;
                break;
                
            case 1: // 车辆到达（图形界面）
                gui.displayArrivalInterface();
                break;
                
            case 2: // 车辆离开（图形界面）
                gui.displayDepartureInterface();
                break;
                
            case 3: // 查看停车场可视化
                gui.displayParkingVisualization(system);
                break;
                
            case 4: // 查看候车道可视化
                gui.displayWaitingLaneVisualization(system);
                break;
                
            case 5: // 查看完整系统可视化
                gui.displayFullVisualization(system);
                break;
                
            case 6: // 运行测试数据
                gui.displayTestDataInterface();
                break;
                
            case 7: // 返回文本界面
                inGraphicalMode = false;
                cout << "返回文本界面模式..." << endl;
                break;
                
            default:
                cout << "无效的选择，请重新输入！" << endl;
                ::system("pause");
                break;
        }
    }
}

int main() {
    // 初始化停车场系统
    ParkingSystem system(10, 5.0); // 10个车位，每小时5元
    
    // 设置车位配置：3个小型车位，3个中型车位，4个大型车位
    system.setParkingSpaces(3, 3, 4);
    
    int choice;
    bool running = true;
    
    cout << "欢迎使用停车场模拟系统！" << endl;
    cout << "系统已初始化：10个车位（3小/3中/4大），费率：5元/小时" << endl;
    
    while (running) {
        displayMenu();
        cin >> choice;
        
        switch (choice) {
            case 0: // 退出系统
                running = false;
                cout << "感谢使用停车场模拟系统，再见！" << endl;
                break;
                
            case 1: { // 车辆到达
                string licensePlate;
                cout << "请输入车牌号: ";
                cin >> licensePlate;
                
                int carType = getCarType();
                time_t arrivalTime = getTimeInput("请输入到达时间");
                
                int result = system.carArrival(licensePlate, carType, arrivalTime);
                if (result == 0) {
                    cout << "车辆已成功停入停车场！" << endl;
                } else if (result == 1) {
                    cout << "车辆已进入候车道等待！" << endl;
                } else {
                    cout << "车辆到达失败！" << endl;
                }
                break;
            }
                
            case 2: { // 车辆离开
                string licensePlate;
                cout << "请输入车牌号: ";
                cin >> licensePlate;
                
                time_t departureTime = getTimeInput("请输入离开时间");
                
                bool success = system.carDeparture(licensePlate, departureTime);
                if (success) {
                    cout << "车辆离开成功！" << endl;
                } else {
                    cout << "车辆离开失败！" << endl;
                }
                break;
            }
                
            case 3: // 查询停车场状态
                system.displayParkingStatus();
                break;
                
            case 4: // 查询候车道状态
                system.displayWaitingLaneStatus();
                break;
                
            case 5: // 查询所有状态
                system.displayAllStatus();
                break;
                
            case 6: { // 查找车辆
                string licensePlate;
                cout << "请输入要查找的车牌号: ";
                cin >> licensePlate;
                
                int location = system.findCar(licensePlate);
                if (location == 1) {
                    cout << "车辆 " << licensePlate << " 在停车场中" << endl;
                } else if (location == 2) {
                    cout << "车辆 " << licensePlate << " 在候车道中" << endl;
                } else {
                    cout << "未找到车辆 " << licensePlate << endl;
                }
                break;
            }
                
            case 7: // 运行测试数据
                runTestData(system);
                break;
                
            case 8: { // 保存数据到文件
                string filename;
                cout << "请输入文件名: ";
                cin >> filename;
                
                if (system.saveToFile(filename)) {
                    cout << "数据保存成功！" << endl;
                } else {
                    cout << "数据保存失败！" << endl;
                }
                break;
            }
                
            case 9: { // 从文件加载数据
                string filename;
                cout << "请输入文件名: ";
                cin >> filename;
                
                if (system.loadFromFile(filename)) {
                    cout << "数据加载成功！" << endl;
                } else {
                    cout << "数据加载失败！" << endl;
                }
                break;
            }
                
            case 10: { // 图形界面模式
                graphicalInterfaceMode(system);
                break;
            }
                
            default:
                cout << "无效的选择，请重新输入！" << endl;
                break;
        }
    }
    
    return 0;
}
