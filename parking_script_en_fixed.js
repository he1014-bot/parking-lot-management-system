// Parking System Simulation - Web Version JavaScript

// System state
const systemState = {
    totalCapacity: 10,
    parkingLot: [], // Parking lot stack
    waitingLane: [], // Waiting lane queue
    smallSpaces: 3,
    mediumSpaces: 3,
    largeSpaces: 4,
    usedSmallSpaces: 0,
    usedMediumSpaces: 0,
    usedLargeSpaces: 0,
    hourlyRate: 5.0
};

// Car type enumeration
const CarType = {
    SMALL: 0,
    MEDIUM: 1,
    LARGE: 2
};

// Space type enumeration
const SpaceType = {
    SMALL: 0,
    MEDIUM: 1,
    LARGE: 2
};

// Car class
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
            case CarType.SMALL: return "Small Car";
            case CarType.MEDIUM: return "Medium Car";
            case CarType.LARGE: return "Large Car";
            default: return "Unknown";
        }
    }
    
    getSpaceTypeString() {
        switch(this.spaceType) {
            case SpaceType.SMALL: return "Small Space";
            case SpaceType.MEDIUM: return "Medium Space";
            case SpaceType.LARGE: return "Large Space";
            default: return "Unknown";
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

// DOM elements
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

// Initialize system
function initSystem() {
    systemState.parkingLot = [];
    systemState.waitingLane = [];
    systemState.usedSmallSpaces = 0;
    systemState.usedMediumSpaces = 0;
    systemState.usedLargeSpaces = 0;
    
    updateUI();
    addLog("System initialized", "welcome");
}

// Update UI
function updateUI() {
    // Update status overview
    domElements.totalCapacity.textContent = systemState.totalCapacity;
    domElements.currentParked.textContent = systemState.parkingLot.length;
    domElements.availableSpaces.textContent = systemState.totalCapacity - systemState.parkingLot.length;
    domElements.waitingCount.textContent = systemState.waitingLane.length;
    
    // Update parking space usage
    domElements.smallUsed.textContent = systemState.usedSmallSpaces;
    domElements.smallTotal.textContent = systemState.smallSpaces;
    domElements.mediumUsed.textContent = systemState.usedMediumSpaces;
    domElements.mediumTotal.textContent = systemState.mediumSpaces;
    domElements.largeUsed.textContent = systemState.usedLargeSpaces;
    domElements.largeTotal.textContent = systemState.largeSpaces;
    
    // Update progress bars
    domElements.smallBar.style.width = `${(systemState.usedSmallSpaces / systemState.smallSpaces) * 100}%`;
    domElements.mediumBar.style.width = `${(systemState.usedMediumSpaces / systemState.mediumSpaces) * 100}%`;
    domElements.largeBar.style.width = `${(systemState.usedLargeSpaces / systemState.largeSpaces) * 100}%`;
    
    // Update parking lot visualization
    updateParkingLotVisualization();
    
    // Update waiting lane visualization
    updateWaitingLaneVisualization();
}

// Update parking lot visualization
function updateParkingLotVisualization() {
    const parkingLot = domElements.parkingLot;
    parkingLot.innerHTML = '';
    
    // Create parking spaces
    for (let i = 0; i < systemState.totalCapacity; i++) {
        const space = document.createElement('div');
        space.className = 'parking-space';
        
        // Determine space type
        let spaceType = '';
        let spaceTypeClass = '';
        if (i < systemState.smallSpaces) {
            spaceType = 'Small Space';
            spaceTypeClass = 'small';
        } else if (i < systemState.smallSpaces + systemState.mediumSpaces) {
            spaceType = 'Medium Space';
            spaceTypeClass = 'medium';
        } else {
            spaceType = 'Large Space';
            spaceTypeClass = 'large';
        }
        
        space.classList.add(spaceTypeClass);
        
        // Check if space is occupied
        const carIndex = systemState.parkingLot.findIndex(car => {
            // Simplified handling, should be based on actual space allocation
            return systemState.parkingLot.indexOf(car) === i;
        });
        
        if (carIndex !== -1 && carIndex < systemState.parkingLot.length) {
            const car = systemState.parkingLot[carIndex];
            space.classList.add('occupied');
            
            space.innerHTML = `
                <div class="space-number">Space ${i + 1}</div>
                <div class="space-type">${spaceType}</div>
                <div class="car-info">
                    <div class="car-license">${car.licensePlate}</div>
                    <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
                </div>
            `;
        } else {
            space.classList.add('empty');
            space.innerHTML = `
                <div class="space-number">Space ${i + 1}</div>
                <div class="space-type">${spaceType}</div>
                <div class="car-info">Empty</div>
            `;
        }
        
        parkingLot.appendChild(space);
    }
}

// Update waiting lane visualization
function updateWaitingLaneVisualization() {
    const waitingLane = domElements.waitingLane;
    
    if (systemState.waitingLane.length === 0) {
        waitingLane.innerHTML = `
            <div class="lane-empty">
                <i class="fas fa-car-side fa-3x"></i>
                <p>Waiting lane is empty</p>
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
            <div class="waiting-position">Waiting #${index + 1}</div>
        `;
        waitingLane.appendChild(carElement);
    });
}

// Add log
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

// Check if suitable space is available
function hasSuitableSpace(carType) {
    switch(carType) {
        case CarType.SMALL:
            return systemState.usedSmallSpaces < systemState.smallSpaces;
        case CarType.MEDIUM:
            return (systemState.usedMediumSpaces < systemState.mediumSpaces) || 
                   (systemState.usedSmallSpaces < systemState.smallSpaces);
        case CarType.LARGE:
            return (systemState.usedLargeSpaces < systemState.largeSpaces) || 
                   (systemState.usedMediumSpaces < systemState.mediumSpaces) || 
                   (systemState.usedSmallSpaces < systemState.smallSpaces);
        default:
            return false;
    }
}

// Allocate parking space
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
            } else if (systemState.usedSmallSpaces < systemState.smallSpaces) {
                systemState.usedSmallSpaces++;
                return SpaceType.SMALL;
            }
            break;
        case CarType.LARGE:
            if (systemState.usedLargeSpaces < systemState.largeSpaces) {
                systemState.usedLargeSpaces++;
                return SpaceType.LARGE;
            } else if (systemState.usedMediumSpaces < systemState.mediumSpaces) {
                systemState.usedMediumSpaces++;
                return SpaceType.MEDIUM;
            } else if (systemState.usedSmallSpaces < systemState.smallSpaces) {
                systemState.usedSmallSpaces++;
                return SpaceType.SMALL;
            }
            break;
    }
    return -1;
}

// Release parking space
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

// Car arrival
function carArrival(licensePlate, carType) {
    // Check if car already exists
    const existingCar = [...systemState.parkingLot, ...systemState.waitingLane]
        .find(car => car.licensePlate === licensePlate);
    
    if (existingCar) {
        addLog(`Error: License plate ${licensePlate} already exists!`, "error");
        return -1;
    }
    
    const car = new Car(licensePlate, carType);
    
    // Check if parking lot has available space
    if (hasSuitableSpace(carType) && systemState.parkingLot.length < systemState.totalCapacity) {
        // Allocate parking space
        const spaceType = allocateParkingSpace(carType);
        if (spaceType === -1) {
            addLog(`Error: Cannot allocate parking space!`, "error");
            return -1;
        }
        
        car.spaceType = spaceType;
        
        // Park car in parking lot (top of stack)
        systemState.parkingLot.unshift(car);
        
        addLog(`Car ${licensePlate} parked in parking lot, space type: ${car.getSpaceTypeString()}`, "arrival");
        updateUI();
        return 0;
    } else {
        // Parking lot is full, go to waiting lane
        systemState.waitingLane.push(car);
        addLog(`Parking lot full, car ${licensePlate} entered waiting lane`, "arrival");
        updateUI();
        return 1;
    }
}

// Car departure
function carDeparture(licensePlate) {
    // Search in parking lot
    const parkingIndex = systemState.parkingLot.findIndex(car => car.licensePlate === licensePlate);
    
    if (parkingIndex !== -1) {
        const car = systemState.parkingLot[parkingIndex];
        
        // Set departure time
        car.exitTime = new Date();
        
        // Calculate fee (simplified calculation)
        const duration = Math.floor((car.exitTime - car.arrivalTime) / 1000 / 60); // minutes
        const hours = Math.max(1, Math.ceil(duration / 60));
        let coefficient = 1.0;
        
        // Adjust coefficient based on car type
        switch(car.carType) {
            case CarType.SMALL: coefficient *= 1.0; break;
            case CarType.MEDIUM: coefficient *= 1.5; break;
            case CarType.LARGE: coefficient *= 2.0; break;
        }
        
        // Adjust coefficient based on space type
        switch(car.spaceType) {
            case SpaceType.SMALL: coefficient *= 1.0; break;
            case SpaceType.MEDIUM: coefficient *= 1.2; break;
            case SpaceType.LARGE: coefficient *= 1.5; break;
        }
        
        const fee = hours * systemState.hourlyRate * coefficient;
        
        // Release parking space
        releaseParkingSpace(car.spaceType);
        
        // Remove car
        systemState.parkingLot.splice(parkingIndex, 1);
        
        addLog(`Car ${licensePlate} departed, parking duration: ${duration} minutes, fee: ${fee.toFixed(2)}Ԫ`, "departure");
        
        // Check if any waiting lane car can enter
        moveFromWaitingLaneToParkingLot();
        
        updateUI();
        return true;
    } else {
        // Search in waiting lane
        const waitingIndex = systemState.waitingLane.findIndex(car => car.licensePlate === licensePlate);
        if (waitingIndex !== -1) {
            // Car is in waiting lane, remove directly
            systemState.waitingLane.splice(waitingIndex, 1);
            addLog(`Car ${licensePlate} left from waiting lane, no charge`, "departure");
            updateUI();
            return true;
        } else {
            addLog(`Error: License plate ${licensePlate} not found!`, "error");
            return false;
        }
    }
}

// Move car from waiting lane to parking lot
function moveFromWaitingLaneToParkingLot() {
    if (systemState.waitingLane.length === 0 || systemState.parkingLot.length >= systemState.totalCapacity) {
        return false;
    }
    
    // Get first car in waiting lane
    const car = systemState.waitingLane[0];
    
    // Check if suitable space is available
    if (hasSuitableSpace(car.carType)) {
        // Allocate parking space
        const spaceType = allocateParkingSpace(car.carType);
        if (spaceType === -1) {
            return false;
        }
        
        car.spaceType = spaceType;
        
        // Remove from waiting lane
        systemState.waitingLane.shift();
        
        // Park in parking lot
        systemState.parkingLot.unshift(car);
        
        addLog(`Info: Waiting lane car ${car.licensePlate} entered parking lot, space type: ${car.getSpaceTypeString()}`, "info");
        updateUI();
        return true;
    }
    
    return false;
}

// Run test data
function runTestData() {
    addLog("Starting test data...", "test");
    
    // Test data
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
        
        delay += 1000; //
