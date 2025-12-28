// Parking System Simulation - Web Version JavaScript

// System state
const systemState = {
    totalCapacity: 10,
    parkingLot: [], // Parking lot stack (newest car at index 0, oldest at last index)
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
    
    // Sort cars by arrival time (earliest first)
    const sortedCars = [...systemState.parkingLot].sort((a, b) => a.arrivalTime - b.arrivalTime);
    
    // We need to track which spaces are occupied
    // Create arrays to track occupied spaces for each type
    const occupiedSmallSpaces = [];
    const occupiedMediumSpaces = [];
    const occupiedLargeSpaces = [];
    
    // First, assign cars to spaces based on their spaceType
    sortedCars.forEach(car => {
        if (car.spaceType === SpaceType.SMALL) {
            occupiedSmallSpaces.push(car);
        } else if (car.spaceType === SpaceType.MEDIUM) {
            occupiedMediumSpaces.push(car);
        } else if (car.spaceType === SpaceType.LARGE) {
            occupiedLargeSpaces.push(car);
        }
    });
    
    // Create parking spaces
    let spaceIndex = 0;
    
    // Small spaces (1-3) - First small car in Space 1, second in Space 2, etc.
    for (let i = 0; i < systemState.smallSpaces; i++) {
        const space = document.createElement('div');
        space.className = 'parking-space small';
        
        // First small car (earliest) should be in Space 1, second in Space 2, etc.
        // occupiedSmallSpaces is sorted by arrival time (earliest first)
        if (i < occupiedSmallSpaces.length) {
            const car = occupiedSmallSpaces[i]; // i=0 is earliest small car
            space.classList.add('occupied');
            space.innerHTML = `
                <div class="space-number">Space ${spaceIndex + 1}</div>
                <div class="space-type">Small Space</div>
                <div class="car-info">
                    <div class="car-license">${car.licensePlate}</div>
                    <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
                    <div class="arrival-time">Arrived: ${new Date(car.arrivalTime).toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'})}</div>
                    <div class="arrival-order">Small car #${i + 1}</div>
                </div>
            `;
        } else {
            space.classList.add('empty');
            space.innerHTML = `
                <div class="space-number">Space ${spaceIndex + 1}</div>
                <div class="space-type">Small Space</div>
                <div class="car-info">Empty</div>
            `;
        }
        
        parkingLot.appendChild(space);
        spaceIndex++;
    }
    
    // Medium spaces (4-6) - First medium car in Space 4, second in Space 5, etc.
    for (let i = 0; i < systemState.mediumSpaces; i++) {
        const space = document.createElement('div');
        space.className = 'parking-space medium';
        
        // First medium car (earliest) should be in Space 4, second in Space 5, etc.
        // occupiedMediumSpaces is sorted by arrival time (earliest first)
        if (i < occupiedMediumSpaces.length) {
            const car = occupiedMediumSpaces[i]; // i=0 is earliest medium car
            space.classList.add('occupied');
            space.innerHTML = `
                <div class="space-number">Space ${spaceIndex + 1}</div>
                <div class="space-type">Medium Space</div>
                <div class="car-info">
                    <div class="car-license">${car.licensePlate}</div>
                    <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
                    <div class="arrival-time">Arrived: ${new Date(car.arrivalTime).toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'})}</div>
                    <div class="arrival-order">Medium car #${i + 1}</div>
                </div>
            `;
        } else {
            space.classList.add('empty');
            space.innerHTML = `
                <div class="space-number">Space ${spaceIndex + 1}</div>
                <div class="space-type">Medium Space</div>
                <div class="car-info">Empty</div>
            `;
        }
        
        parkingLot.appendChild(space);
        spaceIndex++;
    }
    
    // Large spaces (7-10) - First large car in Space 7, second in Space 8, etc.
    for (let i = 0; i < systemState.largeSpaces; i++) {
        const space = document.createElement('div');
        space.className = 'parking-space large';
        
        // First large car (earliest) should be in Space 7, second in Space 8, etc.
        // occupiedLargeSpaces is sorted by arrival time (earliest first)
        if (i < occupiedLargeSpaces.length) {
            const car = occupiedLargeSpaces[i]; // i=0 is earliest large car
            space.classList.add('occupied');
            space.innerHTML = `
                <div class="space-number">Space ${spaceIndex + 1}</div>
                <div class="space-type">Large Space</div>
                <div class="car-info">
                    <div class="car-license">${car.licensePlate}</div>
                    <div class="car-type ${car.getTypeColor()}">${car.getTypeString()}</div>
                    <div class="arrival-time">Arrived: ${new Date(car.arrivalTime).toLocaleTimeString([], {hour: '2-digit', minute:'2-digit'})}</div>
                    <div class="arrival-order">Large car #${i + 1}</div>
                </div>
            `;
        } else {
            space.classList.add('empty');
            space.innerHTML = `
                <div class="space-number">Space ${spaceIndex + 1}</div>
                <div class="space-type">Large Space</div>
                <div class="car-info">Empty</div>
            `;
        }
        
        parkingLot.appendChild(space);
        spaceIndex++;
    }
    
    // Add explanation about arrangement
    if (sortedCars.length > 0) {
        const explanation = document.createElement('div');
        explanation.className = 'arrangement-explanation';
        explanation.style.gridColumn = '1 / -1';
        explanation.style.textAlign = 'center';
        explanation.style.marginTop = '15px';
        explanation.style.padding = '10px';
        explanation.style.backgroundColor = 'rgba(52, 152, 219, 0.1)';
        explanation.style.borderRadius = '5px';
        explanation.style.fontSize = '0.9rem';
        explanation.style.color = 'var(--dark-color)';
        explanation.innerHTML = `
            <i class="fas fa-info-circle"></i> 
            <strong>Arrangement:</strong> Cars are arranged from North to South. 
            Earliest car (${sortedCars[0].licensePlate}) is at the top (North), 
            latest car (${sortedCars[sortedCars.length - 1].licensePlate}) is at the bottom (South, near gate).
        `;
        parkingLot.appendChild(explanation);
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
            return systemState.usedMediumSpaces < systemState.mediumSpaces;
        case CarType.LARGE:
            return systemState.usedLargeSpaces < systemState.largeSpaces;
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
        // Determine which space this car should occupy based on car type and arrival order
        // Small cars: Space 1, 2, 3 (in order of arrival)
        // Medium cars: Space 4, 5, 6 (in order of arrival)
        // Large cars: Space 7, 8, 9, 10 (in order of arrival)
        
        // Count how many cars of this type are already parked
        const carsOfSameType = systemState.parkingLot.filter(c => c.carType === carType);
        const positionInType = carsOfSameType.length; // 0-based position (0 = first car of this type)
        
        // Determine target space number
        let targetSpaceNumber;
        if (carType === CarType.SMALL) {
            targetSpaceNumber = positionInType; // 0, 1, 2 -> Space 1, 2, 3
        } else if (carType === CarType.MEDIUM) {
            targetSpaceNumber = systemState.smallSpaces + positionInType; // 3, 4, 5 -> Space 4, 5, 6
        } else { // LARGE
            targetSpaceNumber = systemState.smallSpaces + systemState.mediumSpaces + positionInType; // 6, 7, 8, 9 -> Space 7, 8, 9, 10
        }
        
        addLog(`DEBUG: Car ${licensePlate} (${car.getTypeString()}) should go to Space ${targetSpaceNumber + 1}, position in type: ${positionInType}`, "info");
        
        // Check if there are cars blocking the path to the target space
        // Cars are stored in parkingLot with newest at index 0 (south end, near gate)
        // Target space is at position targetSpaceNumber from the north end (index length-1-targetSpaceNumber)
        const targetIndex = systemState.parkingLot.length - targetSpaceNumber - 1;
        
        // If targetIndex is negative, it means the space is beyond current parking lot (empty space)
        // We need to check if there are cars blocking the path (cars at indices < targetIndex)
        if (targetIndex >= 0) {
            // There is already a car in the target space or cars blocking the path
            // We need to move blocking cars to waiting lane
            return carArrivalWithMakeWay(car, carType, targetSpaceNumber, targetIndex);
        } else {
            // Target space is empty and not blocked, can park directly
            return carArrivalDirectly(car, carType);
        }
    } else {
        // Parking lot is full, go to waiting lane
        systemState.waitingLane.push(car);
        addLog(`Parking lot full, car ${licensePlate} entered waiting lane (position ${systemState.waitingLane.length})`, "arrival");
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
        
        // Check if car is at the top (south end, near gate)
        if (parkingIndex === 0) {
            // Car is at the top, can depart directly
            return departCarDirectly(car, parkingIndex);
        } else {
            // Car is not at the top, need to move cars above it to waiting lane
            return departCarWithMakeWay(car, parkingIndex);
        }
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

// Depart car directly (car is at top/south end)
function departCarDirectly(car, index) {
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
    systemState.parkingLot.splice(index, 1);
    
    addLog(`Car ${car.licensePlate} departed, parking duration: ${duration} minutes, fee: $${fee.toFixed(2)}`, "departure");
    
    // Check if any waiting lane car can enter
    moveFromWaitingLaneToParkingLot();
    
    updateUI();
    return true;
}

// Depart car with make way (cars above need to move to waiting lane)
function departCarWithMakeWay(car, index) {
    // Cars above the departing car (index 0 to index-1) need to move to waiting lane
    // These are the cars that arrived after the departing car (south of it, closer to gate)
    const carsToMove = systemState.parkingLot.slice(0, index);
    
    addLog(`DEBUG: Car ${car.licensePlate} at index ${index}, ${carsToMove.length} cars need to move to waiting lane`, "info");
    
    if (carsToMove.length > 0) {
        // Move cars to waiting lane (temporarily)
        // We need to move them in reverse order to maintain original order when re-entering
        const movedCars = [];
        
        addLog(`=== Make way process for ${car.licensePlate} departure ===`, "info");
        addLog(`Step 1: ${carsToMove.length} cars need to move to waiting lane (in order):`, "info");
        
        for (let i = 0; i < carsToMove.length; i++) {
            const carToMove = carsToMove[i];
            // Release parking space
            releaseParkingSpace(carToMove.spaceType);
            movedCars.unshift(carToMove); // Add to beginning to maintain order
            // Add to waiting lane so user can see them
            systemState.waitingLane.unshift(carToMove);
            addLog(`  ${i+1}. Car ${carToMove.licensePlate} (${carToMove.getTypeString()}) moved to waiting lane`, "info");
        }
        
        addLog(`Waiting lane order (from front to back): ${movedCars.map(c => c.licensePlate).reverse().join(' -> ')}`, "info");
        
        // Remove moved cars from parking lot
        systemState.parkingLot.splice(0, index);
        
        // Update UI to show cars in waiting lane
        updateUI();
        
        // Now the target car is at index 0 (top/south end)
        // Depart the target car after a short delay so user can see the waiting lane
        setTimeout(() => {
            departCarDirectly(car, 0);
            
            // Re-enter moved cars back to parking lot (in original order) after another delay
            // They should enter in the same order they were before (FIFO for re-entry)
            movedCars.reverse(); // Reverse to get original order (最早移出的先回)
            
            addLog(`DEBUG: Re-entering ${movedCars.length} cars from waiting lane`, "info");
            
            // Re-enter cars one by one with delay so user can see the process
            let delay = 0;
            for (const movedCar of movedCars) {
                setTimeout(() => {
                    // Remove from waiting lane
                    const waitingIndex = systemState.waitingLane.findIndex(c => c.licensePlate === movedCar.licensePlate);
                    if (waitingIndex !== -1) {
                        systemState.waitingLane.splice(waitingIndex, 1);
                    }
                    
                    // Check if suitable space is available
                    if (hasSuitableSpace(movedCar.carType)) {
                        // Allocate parking space
                        const spaceType = allocateParkingSpace(movedCar.carType);
                        if (spaceType !== -1) {
                            movedCar.spaceType = spaceType;
                            // Park car in parking lot (top of stack)
                            systemState.parkingLot.unshift(movedCar);
                            addLog(`Re-enter: Car ${movedCar.licensePlate} re-entered parking lot`, "arrival");
                        } else {
                            // If no suitable space, keep in waiting lane
                            systemState.waitingLane.unshift(movedCar); // Add to front of waiting lane
                            addLog(`Waiting: Car ${movedCar.licensePlate} waiting in lane for suitable space`, "arrival");
                        }
                    } else {
                        // No suitable space, keep in waiting lane
                        systemState.waitingLane.unshift(movedCar); // Add to front of waiting lane
                        addLog(`Waiting: Car ${movedCar.licensePlate} waiting in lane for suitable space`, "arrival");
                    }
                    
                    updateUI();
                }, delay);
                
                delay += 1000; // 1 second delay between each re-entry
            }
        }, 1500); // 1.5 second delay before departing target car
        
        return true;
    } else {
        // No cars to move, depart directly
        return departCarDirectly(car, index);
    }
}

// Car arrival directly (no cars blocking the path)
function carArrivalDirectly(car, carType) {
    // Allocate parking space
    const spaceType = allocateParkingSpace(carType);
    if (spaceType === -1) {
        addLog(`Error: Cannot allocate parking space!`, "error");
        return -1;
    }
    
    car.spaceType = spaceType;
    
    // Park car in parking lot (top of stack - newest car at index 0)
    systemState.parkingLot.unshift(car);
    
    addLog(`Car ${car.licensePlate} parked in parking lot, space type: ${car.getSpaceTypeString()}`, "arrival");
    updateUI();
    return 0;
}

// Car arrival with make way (cars blocking the path need to move to waiting lane)
function carArrivalWithMakeWay(car, carType, targetSpaceNumber, targetIndex) {
    addLog(`DEBUG: Car ${car.licensePlate} needs to go to Space ${targetSpaceNumber + 1}, but there are cars blocking the path`, "info");
    
    // Cars blocking the path are at indices 0 to targetIndex (inclusive)
    // These are the cars that are south of the target space (closer to gate)
    const carsToMove = systemState.parkingLot.slice(0, targetIndex + 1);
    
    addLog(`Make way for arrival: ${carsToMove.length} cars need to move to waiting lane for ${car.licensePlate} to enter Space ${targetSpaceNumber + 1}`, "info");
    
    if (carsToMove.length > 0) {
        // Move cars to waiting lane (temporarily)
        // We need to move them in reverse order to maintain original order when re-entering
        const movedCars = [];
        
        addLog(`=== Make way process for ${car.licensePlate} arrival to Space ${targetSpaceNumber + 1} ===`, "info");
        addLog(`Step 1: ${carsToMove.length} cars need to move to waiting lane (in order):`, "info");
        
        for (let i = 0; i < carsToMove.length; i++) {
            const carToMove = carsToMove[i];
            // Release parking space
            releaseParkingSpace(carToMove.spaceType);
            movedCars.unshift(carToMove); // Add to beginning to maintain order
            // Add to waiting lane so user can see them
            systemState.waitingLane.unshift(carToMove);
            addLog(`  ${i+1}. Car ${carToMove.licensePlate} (${carToMove.getTypeString()}) moved to waiting lane`, "info");
        }
        
        addLog(`Waiting lane order (from front to back): ${movedCars.map(c => c.licensePlate).reverse().join(' -> ')}`, "info");
        
        // Remove moved cars from parking lot
        systemState.parkingLot.splice(0, targetIndex + 1);
        
        // Update UI to show cars in waiting lane
        updateUI();
        
        // Now we can park the new car
        // Allocate parking space
        const spaceType = allocateParkingSpace(carType);
        if (spaceType === -1) {
            addLog(`Error: Cannot allocate parking space!`, "error");
            return -1;
        }
        
        car.spaceType = spaceType;
        
        // Park the new car at the correct position (from the north end)
        // We need to insert at position targetSpaceNumber from the north end
        // Since we removed the blocking cars, the parking lot now has fewer cars
        // The new car should be at index systemState.parkingLot.length - targetSpaceNumber
        const insertIndex = systemState.parkingLot.length - targetSpaceNumber;
        if (insertIndex < 0) {
            // If insertIndex is negative, add to the beginning
            systemState.parkingLot.unshift(car);
        } else {
            systemState.parkingLot.splice(insertIndex, 0, car);
        }
        
        addLog(`Car ${car.licensePlate} parked in Space ${targetSpaceNumber + 1} after making way`, "arrival");
        updateUI();
        
        // Re-enter moved cars back to parking lot (in original order) after a delay
        movedCars.reverse(); // Reverse to get original order (最早移出的先回)
        
        addLog(`DEBUG: Re-entering ${movedCars.length} cars from waiting lane after ${car.licensePlate} entered`, "info");
        
        // Re-enter cars one by one with delay so user can see the process
        let delay = 0;
        for (const movedCar of movedCars) {
            setTimeout(() => {
                // Remove from waiting lane
                const waitingIndex = systemState.waitingLane.findIndex(c => c.licensePlate === movedCar.licensePlate);
                if (waitingIndex !== -1) {
                    systemState.waitingLane.splice(waitingIndex, 1);
                }
                
                // Check if suitable space is available
                if (hasSuitableSpace(movedCar.carType)) {
                    // Allocate parking space
                    const spaceType = allocateParkingSpace(movedCar.carType);
                    if (spaceType !== -1) {
                        movedCar.spaceType = spaceType;
                        // Park car in parking lot (top of stack)
                        systemState.parkingLot.unshift(movedCar);
                        addLog(`Re-enter after arrival: Car ${movedCar.licensePlate} re-entered parking lot`, "arrival");
                    } else {
                        // If no suitable space, keep in waiting lane
                        systemState.waitingLane.unshift(movedCar); // Add to front of waiting lane
                        addLog(`Waiting after arrival: Car ${movedCar.licensePlate} waiting in lane for suitable space`, "arrival");
                    }
                } else {
                    // No suitable space, keep in waiting lane
                    systemState.waitingLane.unshift(movedCar); // Add to front of waiting lane
                    addLog(`Waiting after arrival: Car ${movedCar.licensePlate} waiting in lane for suitable space`, "arrival");
                }
                
                updateUI();
            }, delay);
            
            delay += 1000; // 1 second delay between each re-entry
        }
        
        return 0;
    } else {
        // No cars to move, park directly
        return carArrivalDirectly(car, carType);
    }
}

// Move car from waiting lane to parking lot
function moveFromWaitingLaneToParkingLot() {
    if (systemState.waitingLane.length === 0 || systemState.parkingLot.length >= systemState.totalCapacity) {
        return false;
    }
    
    // Get first car in waiting lane (FIFO - First In First Out)
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
        
        addLog(`Waiting lane rule: Car ${car.licensePlate} (${car.getTypeString()}) entered parking lot from waiting lane to ${car.getSpaceTypeString()}`, "arrival");
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
        
        delay += 1000; // 1 second interval
    });
    
    setTimeout(() => {
        addLog("Test data completed!", "test");
    }, delay);
}

// Event listeners
function setupEventListeners() {
    // Car arrival button
    domElements.btnArrival.addEventListener('click', () => {
        domElements.arrivalModal.classList.add('active');
        domElements.licensePlateInput.focus();
    });
    
    // Car departure button
    domElements.btnDeparture.addEventListener('click', () => {
        domElements.departureModal.classList.add('active');
        domElements.departureLicenseInput.focus();
    });
    
    // Run test data button
    domElements.btnTest.addEventListener('click', () => {
        runTestData();
    });
    
    // Reset system button
    domElements.btnReset.addEventListener('click', () => {
        if (confirm("Are you sure you want to reset the system? All data will be lost.")) {
            initSystem();
        }
    });
    
    // Cancel arrival
    domElements.btnCancelArrival.addEventListener('click', () => {
        domElements.arrivalModal.classList.remove('active');
        domElements.licensePlateInput.value = '';
    });
    
    // Confirm arrival
    domElements.btnConfirmArrival.addEventListener('click', () => {
        const licensePlate = domElements.licensePlateInput.value.trim();
        if (!licensePlate) {
            alert("Please enter license plate!");
            return;
        }
        
        // Get selected car type
        const selectedCarTypeBtn = document.querySelector('.car-type-btn.active');
        const carType = parseInt(selectedCarTypeBtn.dataset.type);
        
        const result = carArrival(licensePlate, carType);
        
        domElements.arrivalModal.classList.remove('active');
        domElements.licensePlateInput.value = '';
    });
    
    // Cancel departure
    domElements.btnCancelDeparture.addEventListener('click', () => {
        domElements.departureModal.classList.remove('active');
        domElements.departureLicenseInput.value = '';
    });
    
    // Confirm departure
    domElements.btnConfirmDeparture.addEventListener('click', () => {
        const licensePlate = domElements.departureLicenseInput.value.trim();
        if (!licensePlate) {
            alert("Please enter license plate!");
            return;
        }
        
        carDeparture(licensePlate);
        
        domElements.departureModal.classList.remove('active');
        domElements.departureLicenseInput.value = '';
    });
    
    // Car type selection buttons
    domElements.carTypeButtons.forEach(button => {
        button.addEventListener('click', () => {
            domElements.carTypeButtons.forEach(btn => btn.classList.remove('active'));
            button.classList.add('active');
        });
    });
    
    // Modal outside click to close
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
    
    // Input enter key confirmation
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

// Initialize when page loads
document.addEventListener('DOMContentLoaded', () => {
    initSystem();
    setupEventListeners();
    addLog("Welcome to Parking System Simulation Web Version!", "welcome");
});
