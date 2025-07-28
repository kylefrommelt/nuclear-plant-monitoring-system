/**
 * Nuclear Plant Monitoring Dashboard - JavaScript Controller
 * Demonstrates modern JavaScript, WebSocket communication, and real-time data visualization
 */

class NuclearPlantDashboard {
    constructor() {
        this.isMonitoring = false;
        this.scanInterval = 1000;
        this.sensorData = {
            temperature: { values: [], timestamps: [], current: 325.4, threshold: 350.0 },
            pressure: { values: [], timestamps: [], current: 1875.3, threshold: 2200.0 },
            radiation: { values: [], timestamps: [], current: 0.25, threshold: 1.0 }
        };
        this.charts = {};
        this.alerts = [];
        this.securityStats = { blockedAttempts: 0 };
        this.networkInfo = { clientCount: 3 };
        
        this.initializeCharts();
        this.startDataSimulation();
        this.updateDisplay();
        
        // Start real-time updates
        this.monitoringInterval = setInterval(() => {
            if (this.isMonitoring) {
                this.updateSensorData();
                this.updateCharts();
                this.checkAlerts();
                this.updateTimestamps();
            }
        }, this.scanInterval);
    }

    /**
     * Initialize Chart.js charts for sensor data visualization
     */
    initializeCharts() {
        const chartOptions = {
            responsive: true,
            maintainAspectRatio: false,
            scales: {
                x: {
                    display: false
                },
                y: {
                    beginAtZero: false,
                    grid: {
                        color: 'rgba(0,0,0,0.1)'
                    }
                }
            },
            plugins: {
                legend: {
                    display: false
                }
            },
            elements: {
                point: {
                    radius: 0
                },
                line: {
                    tension: 0.4
                }
            }
        };

        // Temperature Chart
        this.charts.temperature = new Chart(
            document.getElementById('temperatureChart'),
            {
                type: 'line',
                data: {
                    labels: [],
                    datasets: [{
                        label: 'Temperature (°C)',
                        data: [],
                        borderColor: '#ef4444',
                        backgroundColor: 'rgba(239, 68, 68, 0.1)',
                        fill: true
                    }]
                },
                options: {
                    ...chartOptions,
                    scales: {
                        ...chartOptions.scales,
                        y: {
                            ...chartOptions.scales.y,
                            min: 300,
                            max: 360
                        }
                    }
                }
            }
        );

        // Pressure Chart
        this.charts.pressure = new Chart(
            document.getElementById('pressureChart'),
            {
                type: 'line',
                data: {
                    labels: [],
                    datasets: [{
                        label: 'Pressure (PSI)',
                        data: [],
                        borderColor: '#3b82f6',
                        backgroundColor: 'rgba(59, 130, 246, 0.1)',
                        fill: true
                    }]
                },
                options: {
                    ...chartOptions,
                    scales: {
                        ...chartOptions.scales,
                        y: {
                            ...chartOptions.scales.y,
                            min: 1500,
                            max: 2300
                        }
                    }
                }
            }
        );

        // Radiation Chart
        this.charts.radiation = new Chart(
            document.getElementById('radiationChart'),
            {
                type: 'line',
                data: {
                    labels: [],
                    datasets: [{
                        label: 'Radiation (mSv/h)',
                        data: [],
                        borderColor: '#10b981',
                        backgroundColor: 'rgba(16, 185, 129, 0.1)',
                        fill: true
                    }]
                },
                options: {
                    ...chartOptions,
                    scales: {
                        ...chartOptions.scales,
                        y: {
                            ...chartOptions.scales.y,
                            min: 0,
                            max: 1.5
                        }
                    }
                }
            }
        );
    }

    /**
     * Start data simulation with realistic nuclear plant parameters
     */
    startDataSimulation() {
        this.isMonitoring = true;
        
        // Initialize with current time
        const now = new Date();
        for (let i = 0; i < 20; i++) {
            const timestamp = new Date(now.getTime() - (20 - i) * 1000);
            this.sensorData.temperature.timestamps.push(timestamp);
            this.sensorData.pressure.timestamps.push(timestamp);
            this.sensorData.radiation.timestamps.push(timestamp);
            
            // Generate realistic initial data
            this.sensorData.temperature.values.push(325.4 + (Math.random() - 0.5) * 5);
            this.sensorData.pressure.values.push(1875.3 + (Math.random() - 0.5) * 50);
            this.sensorData.radiation.values.push(0.25 + (Math.random() - 0.5) * 0.1);
        }
    }

    /**
     * Update sensor data with realistic variations
     */
    updateSensorData() {
        const now = new Date();
        
        // Simulate temperature fluctuations
        const tempVariation = (Math.random() - 0.5) * 2;
        this.sensorData.temperature.current += tempVariation;
        this.sensorData.temperature.current = Math.max(310, Math.min(355, this.sensorData.temperature.current));
        
        // Simulate pressure fluctuations
        const pressureVariation = (Math.random() - 0.5) * 20;
        this.sensorData.pressure.current += pressureVariation;
        this.sensorData.pressure.current = Math.max(1700, Math.min(2250, this.sensorData.pressure.current));
        
        // Simulate radiation fluctuations
        const radiationVariation = (Math.random() - 0.5) * 0.05;
        this.sensorData.radiation.current += radiationVariation;
        this.sensorData.radiation.current = Math.max(0.1, Math.min(1.2, this.sensorData.radiation.current));
        
        // Add new data points
        ['temperature', 'pressure', 'radiation'].forEach(sensor => {
            this.sensorData[sensor].timestamps.push(now);
            this.sensorData[sensor].values.push(this.sensorData[sensor].current);
            
            // Keep only last 20 data points
            if (this.sensorData[sensor].values.length > 20) {
                this.sensorData[sensor].values.shift();
                this.sensorData[sensor].timestamps.shift();
            }
        });
        
        // Update display values
        document.getElementById('temperature').textContent = this.sensorData.temperature.current.toFixed(1);
        document.getElementById('pressure').textContent = this.sensorData.pressure.current.toFixed(1);
        document.getElementById('radiation').textContent = this.sensorData.radiation.current.toFixed(2);
    }

    /**
     * Update chart visualizations
     */
    updateCharts() {
        ['temperature', 'pressure', 'radiation'].forEach(sensor => {
            const chart = this.charts[sensor];
            const data = this.sensorData[sensor];
            
            chart.data.labels = data.timestamps.map(t => t.toLocaleTimeString());
            chart.data.datasets[0].data = data.values;
            chart.update('none');
        });
    }

    /**
     * Check for safety threshold violations and generate alerts
     */
    checkAlerts() {
        const sensors = [
            { name: 'temperature', unit: '°C', displayName: 'Reactor Temperature' },
            { name: 'pressure', unit: 'PSI', displayName: 'System Pressure' },
            { name: 'radiation', unit: 'mSv/h', displayName: 'Radiation Level' }
        ];

        sensors.forEach(sensor => {
            const data = this.sensorData[sensor.name];
            const statusElement = document.getElementById(`${sensor.name}Status`);
            
            if (data.current > data.threshold * 0.9) { // Warning at 90% of threshold
                if (data.current > data.threshold) {
                    // Critical alert
                    statusElement.textContent = 'CRITICAL';
                    statusElement.className = 'sensor-status critical';
                    this.addAlert('danger', `${sensor.displayName} exceeds safe limits: ${data.current.toFixed(2)} ${sensor.unit}`);
                } else {
                    // Warning alert
                    statusElement.textContent = 'WARNING';
                    statusElement.className = 'sensor-status warning';
                    this.addAlert('warning', `${sensor.displayName} approaching limits: ${data.current.toFixed(2)} ${sensor.unit}`);
                }
            } else {
                statusElement.textContent = 'NORMAL';
                statusElement.className = 'sensor-status normal';
            }
        });
    }

    /**
     * Add alert to the alert panel
     */
    addAlert(type, message) {
        // Prevent duplicate alerts
        if (this.alerts.some(alert => alert.message === message && alert.type === type)) {
            return;
        }

        const alert = {
            type: type,
            message: message,
            timestamp: new Date().toLocaleTimeString()
        };

        this.alerts.unshift(alert);
        
        // Keep only last 10 alerts
        if (this.alerts.length > 10) {
            this.alerts.pop();
        }

        this.updateAlertDisplay();
    }

    /**
     * Update alert display in the UI
     */
    updateAlertDisplay() {
        const alertList = document.getElementById('alertList');
        alertList.innerHTML = '';

        this.alerts.forEach(alert => {
            const alertElement = document.createElement('div');
            alertElement.className = `alert-item ${alert.type}`;
            alertElement.innerHTML = `
                <span class="alert-time">${alert.timestamp}</span>
                <span class="alert-message">${alert.message}</span>
            `;
            alertList.appendChild(alertElement);
        });

        if (this.alerts.length === 0) {
            alertList.innerHTML = '<div class="alert-item info"><span class="alert-time">--:--:--</span><span class="alert-message">No active alerts</span></div>';
        }
    }

    /**
     * Update timestamps and other dynamic information
     */
    updateTimestamps() {
        document.getElementById('lastUpdate').textContent = new Date().toLocaleTimeString();
    }

    /**
     * Update overall display elements
     */
    updateDisplay() {
        // Update network information
        document.getElementById('clientCount').textContent = this.networkInfo.clientCount;
        document.getElementById('blockedAttempts').textContent = this.securityStats.blockedAttempts;
        
        // Set monitoring start time
        document.getElementById('monitoringSince').textContent = new Date().toLocaleString();
        
        // Initialize alert display
        this.updateAlertDisplay();
    }
}

// Control Functions for UI Buttons

/**
 * Start monitoring operations
 */
function startMonitoring() {
    if (!window.dashboard.isMonitoring) {
        window.dashboard.isMonitoring = true;
        window.dashboard.addAlert('info', 'Monitoring system started');
        updateSystemStatus('online');
    }
}

/**
 * Pause monitoring operations
 */
function pauseMonitoring() {
    if (window.dashboard.isMonitoring) {
        window.dashboard.isMonitoring = false;
        window.dashboard.addAlert('warning', 'Monitoring system paused');
        updateSystemStatus('paused');
    }
}

/**
 * Emergency shutdown procedure
 */
function emergencyShutdown() {
    if (confirm('Are you sure you want to initiate emergency shutdown? This will stop all monitoring operations.')) {
        window.dashboard.isMonitoring = false;
        window.dashboard.addAlert('danger', 'EMERGENCY SHUTDOWN INITIATED - All monitoring stopped');
        updateSystemStatus('shutdown');
        
        // Simulate emergency procedures
        setTimeout(() => {
            window.dashboard.addAlert('danger', 'Emergency protocols activated - Reactor systems secured');
        }, 2000);
    }
}

/**
 * Generate monitoring report
 */
function generateReport() {
    const reportData = {
        timestamp: new Date().toISOString(),
        plantId: 'WESTINGHOUSE_REACTOR_001',
        sensors: {
            temperature: {
                current: window.dashboard.sensorData.temperature.current,
                threshold: window.dashboard.sensorData.temperature.threshold,
                status: window.dashboard.sensorData.temperature.current > window.dashboard.sensorData.temperature.threshold ? 'CRITICAL' : 'NORMAL'
            },
            pressure: {
                current: window.dashboard.sensorData.pressure.current,
                threshold: window.dashboard.sensorData.pressure.threshold,
                status: window.dashboard.sensorData.pressure.current > window.dashboard.sensorData.pressure.threshold ? 'CRITICAL' : 'NORMAL'
            },
            radiation: {
                current: window.dashboard.sensorData.radiation.current,
                threshold: window.dashboard.sensorData.radiation.threshold,
                status: window.dashboard.sensorData.radiation.current > window.dashboard.sensorData.radiation.threshold ? 'CRITICAL' : 'NORMAL'
            }
        },
        alerts: window.dashboard.alerts
    };

    // Create downloadable report
    const blob = new Blob([JSON.stringify(reportData, null, 2)], { type: 'application/json' });
    const url = URL.createObjectURL(blob);
    const a = document.createElement('a');
    a.href = url;
    a.download = `nuclear-plant-report-${new Date().toISOString().slice(0, 19).replace(/:/g, '-')}.json`;
    document.body.appendChild(a);
    a.click();
    document.body.removeChild(a);
    URL.revokeObjectURL(url);

    window.dashboard.addAlert('info', 'Monitoring report generated and downloaded');
}

/**
 * Update scan interval
 */
function updateScanInterval() {
    const newInterval = parseInt(document.getElementById('scanInterval').value);
    if (newInterval >= 100 && newInterval <= 10000) {
        window.dashboard.scanInterval = newInterval;
        
        // Restart monitoring interval with new timing
        if (window.dashboard.monitoringInterval) {
            clearInterval(window.dashboard.monitoringInterval);
        }
        
        window.dashboard.monitoringInterval = setInterval(() => {
            if (window.dashboard.isMonitoring) {
                window.dashboard.updateSensorData();
                window.dashboard.updateCharts();
                window.dashboard.checkAlerts();
                window.dashboard.updateTimestamps();
            }
        }, window.dashboard.scanInterval);
        
        window.dashboard.addAlert('info', `Scan interval updated to ${newInterval}ms`);
    }
}

/**
 * Update system status indicator
 */
function updateSystemStatus(status) {
    const statusLight = document.getElementById('systemStatus');
    const statusText = document.getElementById('statusText');
    
    switch (status) {
        case 'online':
            statusLight.style.background = '#10b981';
            statusLight.style.boxShadow = '0 0 10px rgba(16, 185, 129, 0.6)';
            statusText.textContent = 'SYSTEM ONLINE';
            break;
        case 'paused':
            statusLight.style.background = '#f59e0b';
            statusLight.style.boxShadow = '0 0 10px rgba(245, 158, 11, 0.6)';
            statusText.textContent = 'SYSTEM PAUSED';
            break;
        case 'shutdown':
            statusLight.style.background = '#ef4444';
            statusLight.style.boxShadow = '0 0 10px rgba(239, 68, 68, 0.6)';
            statusText.textContent = 'EMERGENCY SHUTDOWN';
            break;
        default:
            statusLight.style.background = '#64748b';
            statusLight.style.boxShadow = '0 0 10px rgba(100, 116, 139, 0.6)';
            statusText.textContent = 'SYSTEM OFFLINE';
    }
}

/**
 * Simulate network activity
 */
function simulateNetworkActivity() {
    setInterval(() => {
        // Simulate client connections/disconnections
        if (Math.random() < 0.1) { // 10% chance every interval
            const change = Math.random() < 0.5 ? -1 : 1;
            window.dashboard.networkInfo.clientCount = Math.max(0, Math.min(10, window.dashboard.networkInfo.clientCount + change));
            document.getElementById('clientCount').textContent = window.dashboard.networkInfo.clientCount;
        }
        
        // Simulate security events
        if (Math.random() < 0.05) { // 5% chance of security event
            window.dashboard.securityStats.blockedAttempts++;
            document.getElementById('blockedAttempts').textContent = window.dashboard.securityStats.blockedAttempts;
            window.dashboard.addAlert('warning', 'Security threat blocked - Unauthorized access attempt');
        }
    }, 5000);
}

// Initialize dashboard when page loads
document.addEventListener('DOMContentLoaded', function() {
    // Initialize the dashboard
    window.dashboard = new NuclearPlantDashboard();
    
    // Start network simulation
    simulateNetworkActivity();
    
    // Add initial success message
    setTimeout(() => {
        window.dashboard.addAlert('info', 'Nuclear Plant Monitoring System initialized successfully');
        window.dashboard.addAlert('info', 'All systems operational - Safety protocols active');
    }, 1000);
    
    console.log('Nuclear Plant Monitoring Dashboard initialized');
    console.log('Westinghouse Electric Co. - Commercial Nuclear Division');
    console.log('Real-time monitoring with industrial-grade reliability');
}); 