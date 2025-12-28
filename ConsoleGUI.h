#ifndef CONSOLEGUI_H
#define CONSOLEGUI_H

#include "ParkingSystem.h"
#include <string>
#include <vector>

class ConsoleGUI {
private:
    // 图形界面尺寸
    static const int WIDTH = 80;
    static const int HEIGHT = 24;
    
    // 颜色代码（Windows控制台）
    enum Color {
        BLACK = 0,
        BLUE = 1,
        GREEN = 2,
        CYAN = 3,
        RED = 4,
        MAGENTA = 5,
        YELLOW = 6,
        WHITE = 7,
        GRAY = 8,
        BRIGHT_BLUE = 9,
        BRIGHT_GREEN = 10,
        BRIGHT_CYAN = 11,
        BRIGHT_RED = 12,
        BRIGHT_MAGENTA = 13,
        BRIGHT_YELLOW = 14,
        BRIGHT_WHITE = 15
    };
    
    ParkingSystem& parkingSystem; // 停车场系统引用
    
public:
    // 构造函数
    ConsoleGUI(ParkingSystem& system);
    
    // 显示主界面
    void displayMainMenu(const ParkingSystem& system);
    
    // 显示停车场可视化
    void displayParkingVisualization(const ParkingSystem& system);
    
    // 显示候车道可视化
    void displayWaitingLaneVisualization(const ParkingSystem& system);
    
    // 显示完整系统可视化
    void displayFullVisualization(const ParkingSystem& system);
    
    // 显示车辆到达界面
    void displayArrivalInterface();
    
    // 显示车辆离开界面
    void displayDepartureInterface();
    
    // 显示查找车辆界面
    void displayFindCarInterface();
    
    // 显示测试数据运行界面
    void displayTestDataInterface();
    
    // 显示文件操作界面
    void displayFileOperationsInterface();
    
    // 清屏
    void clearScreen();
    
    // 设置控制台颜色
    void setColor(Color foreground, Color background = BLACK);
    
    // 重置颜色
    void resetColor();
    
    // 在指定位置输出文本
    void drawText(int x, int y, const std::string& text);
    
    // 绘制矩形框
    void drawBox(int x, int y, int width, int height, const std::string& title = "");
    
    // 绘制停车场车位
    void drawParkingSpace(int x, int y, int width, int height, 
                         const std::string& label, 
                         bool occupied, 
                         const std::string& carInfo = "");
    
    // 绘制车辆
    void drawCar(int x, int y, const std::string& licensePlate, int carType);
    
    // 绘制候车道
    void drawWaitingLane(int x, int y, int length, const std::vector<std::string>& cars);
    
private:
    // 获取车型颜色
    Color getCarTypeColor(int carType);
    
    // 获取车位类型颜色
    Color getSpaceTypeColor(int spaceType);
    
    // 获取车型符号
    std::string getCarTypeSymbol(int carType);
};

#endif // CONSOLEGUI_H
