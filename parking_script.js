// 停车场模拟系统 - Web版 JavaScript

// 系统状态
const systemState = {
    totalCapacity: 10,
    parkingLot: [], // 停车场栈
    waitingLane: [], // 候车道队列
    smallSpaces: 3,
    mediumSpaces: 3,
    largeSpaces: 4,
    usedSmallSpaces: 0,
    usedMediumSpaces: 0,
    usedLargeSpaces: 0,
    hourlyRate: 5.0
};

// 车型枚举
const CarType = {
    SMALL: 0,
    MEDIUM: 1,
    LARGE: 2
};

// 车位类型枚举
const SpaceType = {
    SMALL: 0,
    MEDIUM: 1,
    LARGE: 2
};

// 车辆类
class Car {
    constructor(licensePlate, carType, arrivalTime) {
        this.licensePlate = licensePlate;
        this.carType = carType;
        this.arrivalTime = arrivalTime || new Date();
        this.exitTime = null;
        this.spaceType = null;
    }
    
    getTypeString() {
        switch(this.carType) {
            case CarType.SMALL: return "小型车";
            case CarType.MEDIUM: return "中型车";
            case CarType.LARGE: return "大型车";
            default: return "未知";
        }
    }
    
    getSpaceTypeString() {
        switch(this.spaceType) {
            case SpaceType.SMALL: return "小型车位";
            case SpaceType.MEDIUM: return "中型车位";
            case SpaceType.LARGE: return "大型车位";
            default: return "未知";
        }
    }
    
    getTypeColor() {
        switch(this.carType) {
            case CarType.SMALL: return "small";
            case CarType.MEDIUM: return "medium";
            case CarType.LARGE: return "large";
            default: return "";
        }
    }
}

// DOM元素
const domElements = {
    totalCapacity: document.getElementById('total-capacity'),
    currentParked: document.getElementById('current-parked'),
    availableSpaces: document.getElementById('available-spaces'),
    waitingCount: document.getElementById('waiting-count'),
    smallUsed: document.getElementById('small-used'),
    smallTotal: document.getElementById('small-total'),
    mediumUsed: document.getElementById('medium-used'),
    mediumTotal: document.getElementById('medium-total'),
    largeUsed: document.getElementById('large-used'),
    largeTotal: document.getElementById('large-total'),
    smallBar: document.getElementById('small-bar'),
    mediumBar: document.getElementById('medium-bar'),
    largeBar: document.getElementById('large-bar'),
    parkingLot: document.getElementById('parking-lot'),
    waitingLane: document.getElementById('waiting-lane'),
    logContainer: document.getElementById('log-container'),
    btnArrival: document.getElementById('btn-arrival'),
    btnDeparture: document.getElementById('btn-departure'),
    btnTest: document.getElementById('btn-test'),
    btnReset: document.getElementById('btn-reset'),
    arrivalModal: document.getElementById('arrival-modal'),
    departureModal: document.getElementById('departure-modal'),
    licensePlateInput: document.getElementById('license-plate'),
    departureLicenseInput: document.getElementById('departure-license'),
    btnCancelArrival: document.getElementById('btn-cancel-arrival'),
    btnConfirmArrival: document.getElementById('btn-confirm-arrival'),
    btnCancelDeparture: document.getElementById('btn-cancel-departure'),
    btnConfirmDeparture: document.getElementById('btn-confirm-departure'),
    carTypeButtons: document.querySelectorAll('.car-type-btn')
};

// 初始化系统
function initSystem() {
    systemState.parkingLot = [];
    systemState.waitingLane = [];
    systemState.usedSmallSpaces = 0;
    systemState.usedMediumSpaces = 0;
    systemState.usedLargeSpaces = 0;
    
    updateUI();
    addLog("系统已初始化", "welcome");
}

// 更新UI
function updateUI() {
    // 更新状态概览
    domElements.totalCapacity.textContent = systemState.totalCapacity;
    domElements.currentParked.textContent = systemState.parkingLot.length;
    domElements.availableSpaces.textContent = systemState.totalCapacity - systemState.parkingLot.length;
    domElements.waitingCount.textContent = systemState.waitingLane.length;
    
    // 更新车位使用情况
    domElements.smallUsed.textContent = systemState.usedSmallSpaces;
    domElements.smallTotal.textContent = systemState.smallSpaces;
    domElements.mediumUsed.textContent = systemState.usedMediumSpaces;
    domElements.mediumTotal.textContent = systemState.mediumSpaces;
    domElements.largeUsed.textContent = systemState.usedLargeSpaces;
    domElements.largeTotal.textContent = systemState.largeSpaces;
    
    // 更新进度条
    domElements.smallBar.style.width = `${(systemState.usedSmallSpaces / systemState.smallSpaces) * 100}%`;
    domElements.mediumBar.style.width = `${(systemState.usedMediumSpaces / systemState.mediumSpaces) * 100}%`;
    domElements.largeBar.style.width = `${(systemState.usedLargeSpaces / systemState.largeSpaces) * 100}%`;
    
    // 更新停车场可视化
    updateParkingLotVisualization();
    
    // 更新候车道可视化
    updateWaitingLaneVisualization();
}

// 更新停车场可视化
function updateParkingLotVisualization() {
    const parkingLot = domElements.parkingLot;
    parkingLot.innerHTML = '';
    
    // 我们需要跟踪哪些车位被占用
    // 创建数组来跟踪每种类型的被占用车位
    const occupiedSmallSpaces = [];
    const occupiedMediumSpaces = [];
    const occupiedLargeSpaces = [];
    
    // 首先，根据车辆的spaceType将车辆分配到车位
    systemState.parkingLot.forEach(car => {
        if (car.spaceType === SpaceType.SMALL) {
            occupiedSmallSpaces.push(car);
        } else if (car.spaceType === SpaceType.MEDIUM) {
            occupiedMediumSpaces.push(car);
        } else if (car.spaceType === SpaceType.LARGE) {
            occupiedLargeSpaces.push(car);
        }
    });
    
    // 创建车位
    let spaceIndex = 0;
    
    // 小型车位 (0-2)
    for (let i = 0; i < systemState.smallSpaces; i++) {
        const space = document.createElement('div');
        space.className = 'parking-space small';
        
        if (i < occupiedSmallSpaces.length) {
            const car = occupiedSmallSpaces[i];
            space.classList.add('occupied');
            space.innerHTML = `
                <div class="space-number">车位 ${spaceIndex + 1}</div>
                <div class="space-type">小型车位</div>
                <div class="car-info">
                    <div class="car-license">${car.licensePlate}</div>
                    <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
                </div>
            `;
        } else {
            space.classList.add('empty');
            space.innerHTML = `
                <div class="space-number">车位 ${spaceIndex + 1}</div>
                <div class="space-type">小型车位</div>
                <div class="car-info">空位</div>
            `;
        }
        
        parkingLot.appendChild(space);
        spaceIndex++;
    }
    
    // 中型车位 (3-5)
    for (let i = 0; i < systemState.mediumSpaces; i++) {
        const space = document.createElement('div');
        space.className = 'parking-space medium';
        
        if (i < occupiedMediumSpaces.length) {
            const car = occupiedMediumSpaces[i];
            space.classList.add('occupied');
            space.innerHTML = `
                <div class="space-number">车位 ${spaceIndex + 1}</div>
                <div class="space-type">中型车位</div>
                <div class="car-info">
                    <div class="car-license">${car.licensePlate}</div>
                    <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
                </div>
            `;
        } else {
            space.classList.add('empty');
            space.innerHTML = `
                <div class="space-number">车位 ${spaceIndex + 1}</div>
                <div class="space-type">中型车位</div>
                <div class="car-info">空位</div>
            `;
        }
        
        parkingLot.appendChild(space);
        spaceIndex++;
    }
    
    // 大型车位 (6-9)
    for (let i = 0; i < systemState.largeSpaces; i++) {
        const space = document.createElement('div');
        space.className = 'parking-space large';
        
        if (i < occupiedLargeSpaces.length) {
            const car = occupiedLargeSpaces[i];
            space.classList.add('occupied');
            space.innerHTML = `
                <div class="space-number">车位 ${spaceIndex + 1}</div>
                <div class="space-type">大型车位</div>
                <div class="car-info">
                    <div class="car-license">${car.licensePlate}</div>
                    <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
                </div>
            `;
        } else {
            space.classList.add('empty');
            space.innerHTML = `
                <div class="space-number">车位 ${spaceIndex + 1}</div>
                <div class="space-type">大型车位</div>
                <div class="car-info">空位</div>
            `;
        }
        
        parkingLot.appendChild(space);
        spaceIndex++;
    }
}

// 更新候车道可视化
function updateWaitingLaneVisualization() {
    const waitingLane = domElements.waitingLane;
    
    if (systemState.waitingLane.length === 0) {
        waitingLane.innerHTML = `
            <div class="lane-empty">
                <i class="fas fa-car-side fa-3x"></i>
                <p>候车道为空</p>
            </div>
        `;
        return;
    }
    
    waitingLane.innerHTML = '';
    
    systemState.waitingLane.forEach((car, index) => {
        const carElement = document.createElement('div');
        carElement.className = 'waiting-car';
        carElement.innerHTML = `
            <div class="car-license">${car.licensePlate}</div>
            <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
            <div class="waiting-position">等待 #${index + 1}</div>
        `;
        waitingLane.appendChild(carElement);
    });
}

// 添加日志
function addLog(message, type = 'info') {
    const now = new Date();
    const timeString = `${now.getHours().toString().padStart(2, '0')}:${now.getMinutes().toString().padStart(2, '0')}:${now.getSeconds().toString().padStart(2, '0')}`;
    
    const logEntry = document.createElement('div');
    logEntry.className = `log-entry ${type}`;
    logEntry.innerHTML = `
        <div class="log-time">${timeString}</div>
        <div class="log-message">${message}</div>
    `;
    
    domElements.logContainer.appendChild(logEntry);
    domElements.logContainer.scrollTop = domElements.logContainer.scrollHeight;
}

// 检查是否有合适的车位
function hasSuitableSpace(carType) {
    switch(carType) {
        case CarType.SMALL:
            return systemState.usedSmallSpaces < systemState.smallSpaces;
        case CarType.MEDIUM:
            return systemState.usedMediumSpaces < systemState.mediumSpaces;
        case CarType.LARGE:
            return systemState.usedLargeSpaces < systemState.largeSpaces;
        default:
            return false;
    }
}

// 分配车位
function allocateParkingSpace(carType) {
    switch(carType) {
        case CarType.SMALL:
            if (systemState.usedSmallSpaces < systemState.smallSpaces) {
                systemState.usedSmallSpaces++;
                return SpaceType.SMALL;
            }
            break;
        case CarType.MEDIUM:
            if (systemState.usedMediumSpaces < systemState.mediumSpaces) {
                systemState.usedMediumSpaces++;
                return SpaceType.MEDIUM;
            }
            break;
        case CarType.LARGE:
            if (systemState.usedLargeSpaces < systemState.largeSpaces) {
                systemState.usedLargeSpaces++;
                return SpaceType.LARGE;
            }
            break;
    }
    return -1;
}

// 释放车位
function releaseParkingSpace(spaceType) {
    switch(spaceType) {
        case SpaceType.SMALL:
            if (systemState.usedSmallSpaces > 0) systemState.usedSmallSpaces--;
            break;
        case SpaceType.MEDIUM:
            if (systemState.usedMediumSpaces > 0) systemState.usedMediumSpaces--;
            break;
        case SpaceType.LARGE:
            if (systemState.usedLargeSpaces > 0) systemState.usedLargeSpaces--;
            break;
    }
}

// 车辆到达
function carArrival(licensePlate, carType) {
    // 检查车辆是否已存在
    const existingCar = [...systemState.parkingLot, ...systemState.waitingLane]
        .find(car => car.licensePlate === licensePlate);
    
    if (existingCar) {
        addLog(`错误：车牌号 ${licensePlate} 已存在！`, "error");
        return -1;
    }
    
    const car = new Car(licensePlate, carType);
    
    // 检查停车场是否有空位
    if (hasSuitableSpace(carType) && systemState.parkingLot.length < systemState.totalCapacity) {
        // 分配车位
        const spaceType = allocateParkingSpace(carType);
        if (spaceType === -1) {
            addLog(`错误：无法分配车位！`, "error");
            return -1;
        }
        
        car.spaceType = spaceType;
        
        // 将车辆停入停车场（栈顶）
        systemState.parkingLot.unshift(car);
        
        addLog(`车辆 ${licensePlate} 已停入停车场，车位类型：${car.getSpaceTypeString()}`, "arrival");
        updateUI();
        return 0;
    } else {
        // 停车场已满，停入候车道
        systemState.waitingLane.push(car);
        addLog(`停车场已满，车辆 ${licensePlate} 已进入候车道等待`, "arrival");
        updateUI();
        return 1;
    }
}

// 车辆离开
function carDeparture(licensePlate) {
    // 在停车场查找
    const parkingIndex = systemState.parkingLot.findIndex(car => car.licensePlate === licensePlate);
    
    if (parkingIndex !== -1) {
        const car = systemState.parkingLot[parkingIndex];
        
        // 设置离开时间
        car.exitTime = new Date();
        
        // 计算费用（简化计算）
        const duration = Math.floor((car.exitTime - car.arrivalTime) / 1000 / 60); // 分钟
        const hours = Math.max(1, Math.ceil(duration / 60));
        let coefficient = 1.0;
        
        // 根据车型调整系数
        switch(car.carType) {
            case CarType.SMALL: coefficient *= 1.0; break;
            case CarType.MEDIUM: coefficient *= 1.5; break;
            case CarType.LARGE: coefficient *= 2.0; break;
        }
        
        // 根据车位类型调整系数
        switch(car.spaceType) {
            case SpaceType.SMALL: coefficient *= 1.0; break;
            case SpaceType.MEDIUM: coefficient *= 1.2; break;
            case SpaceType.LARGE: coefficient *= 1.5; break;
        }
        
        const fee = hours * systemState.hourlyRate * coefficient;
        
        // 释放车位
        releaseParkingSpace(car.spaceType);
        
        // 移除车辆
        systemState.parkingLot.splice(parkingIndex, 1);
        
        addLog(`车辆 ${licensePlate} 离开，停车时长：${duration}分钟，应付费用：${fee.toFixed(2)}元`, "departure");
        
        // 检查是否有候车道车辆可以进入
        moveFromWaitingLaneToParkingLot();
        
        updateUI();
        return true;
    } else {
        // 在候车道查找
        const waitingIndex = systemState.waitingLane.findIndex(car => car.licensePlate === licensePlate);
        if (waitingIndex !== -1) {
            // 车辆在候车道中，直接移除
            systemState.waitingLane.splice(waitingIndex, 1);
            addLog(`车辆 ${licensePlate} 从候车道中离开，不收费`, "departure");
            updateUI();
            return true;
        } else {
            addLog(`错误：车牌号 ${licensePlate} 不存在！`, "error");
            return false;
        }
    }
}

// 从候车道移动车辆到停车场
function moveFromWaitingLaneToParkingLot() {
    if (systemState.waitingLane.length === 0 || systemState.parkingLot.length >= systemState.totalCapacity) {
        return false;
    }
    
    // 获取候车道第一辆车
    const car = systemState.waitingLane[0];
    
    // 检查是否有合适的车位
    if (hasSuitableSpace(car.carType)) {
        // 分配车位
        const spaceType = allocateParkingSpace(car.carType);
        if (spaceType === -1) {
            return false;
        }
        
        car.spaceType = spaceType;
        
        // 从候车道移除
        systemState.waitingLane.shift();
        
        // 停入停车场
        systemState.parkingLot.unshift(car);
        
        addLog(`提示：候车道车辆 ${car.licensePlate} 已进入停车场，车位类型：${car.getSpaceTypeString()}`, "info");
        updateUI();
        return true;
    }
    
    return false;
}

// 运行测试数据
function runTestData() {
    addLog("开始运行测试数据...", "test");
    
    // 测试数据
    const testData = [
        { action: 'A', licensePlate: '1', carType: CarType.SMALL },
        { action: 'A', licensePlate: '2', carType: CarType.SMALL },
        { action: 'D', licensePlate: '1', carType: CarType.SMALL },
        { action: 'A', licensePlate: '3', carType: CarType.MEDIUM },
        { action: 'A', licensePlate: '4', carType: CarType.LARGE },
        { action: 'A', licensePlate: '5', carType: CarType.SMALL },
        { action: 'D', licensePlate: '2', carType: CarType.SMALL },
        { action: 'D', licensePlate: '4', carType: CarType.LARGE }
    ];
    
    let delay = 0;
    
    testData.forEach((data, index) => {
        setTimeout(() => {
            if (data.action === 'A') {
                carArrival(data.licensePlate, data.carType);
            } else if (data.action === 'D') {
                carDeparture(data.licensePlate);
            }
        }, delay);
        
        delay += 1000; // 1秒间隔
    });
    
    setTimeout(() => {
        addLog("测试数据运行完成！", "test");
    }, delay);
}

// 事件监听器
function setupEventListeners() {
    // 车辆到达按钮
    domElements.btnArrival.addEventListener('click', () => {
        domElements.arrivalModal.classList.add('active');
        domElements.licensePlateInput.focus();
    });
    
    // 车辆离开按钮
    domElements.btnDeparture.addEventListener('click', () => {
        domElements.departureModal.classList.add('active');
        domElements.departureLicenseInput.focus();
    });
    
    // 运行测试数据按钮
    domElements.btnTest.addEventListener('click', () => {
        runTestData();
    });
    
    // 重置系统按钮
    domElements.btnReset.addEventListener('click', () => {
        if (confirm("确定要重置系统吗？所有数据将丢失。")) {
            initSystem();
        }
    });
    
    // 取消到达
    domElements.btnCancelArrival.addEventListener('click', () => {
        domElements.arrivalModal.classList.remove('active');
        domElements.licensePlateInput.value = '';
    });
    
    // 确认到达
    domElements.btnConfirmArrival.addEventListener('click', () => {
        const licensePlate = domElements.licensePlateInput.value.trim();
        if (!licensePlate) {
            alert("请输入车牌号！");
            return;
        }
        
        // 获取选中的车型
        const selectedCarTypeBtn = document.querySelector('.car-type-btn.active');
        const carType = parseInt(selectedCarTypeBtn.dataset.type);
        
        const result = carArrival(licensePlate, carType);
        
        domElements.arrivalModal.classList.remove('active');
        domElements.licensePlateInput.value = '';
    });
    
    // 取消离开
    domElements.btnCancelDeparture.addEventListener('click', () => {
        domElements.departureModal.classList.remove('active');
        domElements.departureLicenseInput.value = '';
    });
    
    // 确认离开
    domElements.btnConfirmDeparture.addEventListener('click', () => {
        const licensePlate = domElements.departureLicenseInput.value.trim();
        if (!licensePlate) {
            alert("请输入车牌号！");
            return;
        }
        
        carDeparture(licensePlate);
        
        domElements.departureModal.classList.remove('active');
        domElements.departureLicenseInput.value = '';
    });
    
    // 车型选择按钮
    domElements.carTypeButtons.forEach(button => {
        button.addEventListener('click', () => {
            domElements.carTypeButtons.forEach(btn => btn.classList.remove('active'));
            button.classList.add('active');
        });
    });
    
    // 模态框外部点击关闭
    window.addEventListener('click', (event) => {
        if (event.target === domElements.arrivalModal) {
            domElements.arrivalModal.classList.remove('active');
            domElements.licensePlateInput.value = '';
        }
        if (event.target === domElements.departureModal) {
            domElements.departureModal.classList.remove('active');
            domElements.departureLicenseInput.value = '';
        }
    });
    
    // 输入框回车确认
    domElements.licensePlateInput.addEventListener('keypress', (event) => {
        if (event.key === 'Enter') {
            domElements.btnConfirmArrival.click();
        }
    });
    
    domElements.departureLicenseInput.addEventListener('keypress', (event) => {
        if (event.key === 'Enter') {
            domElements.btnConfirmDeparture.click();
        }
    });
}

// 页面加载完成后初始化
document.addEventListener('DOMContentLoaded', () => {
    initSystem();
    setupEventListeners();
    addLog("欢迎使用停车场模拟系统 Web版！", "welcome");
});
