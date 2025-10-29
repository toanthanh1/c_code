/*
 * Device Node Management System using Linked List
 * This program implements device node adding and deletion for a system
 * that manages hardware devices using a linked list structure.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

// Enumeration for device types
typedef enum {
    DEVICE_USB,
    DEVICE_PCI,
    DEVICE_NETWORK,
    DEVICE_AUDIO,
    DEVICE_VIDEO,
    DEVICE_STORAGE,
    DEVICE_INPUT,
    DEVICE_BLUETOOTH
} DeviceType;

// Enumeration for device status
typedef enum {
    STATUS_ACTIVE,
    STATUS_INACTIVE,
    STATUS_ERROR,
    STATUS_PENDING
} DeviceStatus;

// Structure to represent a device node
typedef struct DeviceNode {
    int deviceId;
    char deviceName[50];
    char manufacturer[30];
    DeviceType type;
    DeviceStatus status;
    char driverVersion[20];
    time_t installTime;
    int priority;
    struct DeviceNode* next;
} DeviceNode;

// Structure to represent the device management system
typedef struct DeviceManager {
    DeviceNode* head;
    int totalDevices;
    int activeDevices;
    int nextDeviceId;
} DeviceManager;

// Function to get device type string
const char* getDeviceTypeString(DeviceType type) {
    switch (type) {
        case DEVICE_USB: return "USB";
        case DEVICE_PCI: return "PCI";
        case DEVICE_NETWORK: return "Network";
        case DEVICE_AUDIO: return "Audio";
        case DEVICE_VIDEO: return "Video";
        case DEVICE_STORAGE: return "Storage";
        case DEVICE_INPUT: return "Input";
        case DEVICE_BLUETOOTH: return "Bluetooth";
        default: return "Unknown";
    }
}

// Function to get device status string
const char* getDeviceStatusString(DeviceStatus status) {
    switch (status) {
        case STATUS_ACTIVE: return "Active";
        case STATUS_INACTIVE: return "Inactive";
        case STATUS_ERROR: return "Error";
        case STATUS_PENDING: return "Pending";
        default: return "Unknown";
    }
}

// Function to create a new device node
DeviceNode* createDeviceNode(int id, char* name, char* manufacturer, 
                            DeviceType type, char* driverVersion, int priority) {
    DeviceNode* newNode = (DeviceNode*)malloc(sizeof(DeviceNode));
    newNode->deviceId = id;
    strcpy(newNode->deviceName, name);
    strcpy(newNode->manufacturer, manufacturer);
    newNode->type = type;
    newNode->status = STATUS_PENDING;
    strcpy(newNode->driverVersion, driverVersion);
    newNode->installTime = time(NULL);
    newNode->priority = priority;
    newNode->next = NULL;
    return newNode;
}

// Function to create device manager
DeviceManager* createDeviceManager() {
    DeviceManager* manager = (DeviceManager*)malloc(sizeof(DeviceManager));
    manager->head = NULL;
    manager->totalDevices = 0;
    manager->activeDevices = 0;
    manager->nextDeviceId = 1001;
    return manager;
}

// Function to add device node (sorted by priority, highest first)
void addDevice(DeviceManager* manager, char* name, char* manufacturer, 
               DeviceType type, char* driverVersion, int priority) {
    
    DeviceNode* newDevice = createDeviceNode(manager->nextDeviceId++, name, 
                                           manufacturer, type, driverVersion, priority);
    
    // Insert in priority order (highest priority first)
    if (manager->head == NULL || manager->head->priority < priority) {
        newDevice->next = manager->head;
        manager->head = newDevice;
    } else {
        DeviceNode* current = manager->head;
        while (current->next != NULL && current->next->priority >= priority) {
            current = current->next;
        }
        newDevice->next = current->next;
        current->next = newDevice;
    }
    
    manager->totalDevices++;
    printf("Device added: %s (ID: %d, Priority: %d)\n", 
           name, newDevice->deviceId, priority);
}

// Function to find device by ID
DeviceNode* findDeviceById(DeviceManager* manager, int deviceId) {
    DeviceNode* current = manager->head;
    while (current != NULL) {
        if (current->deviceId == deviceId) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to find device by name
DeviceNode* findDeviceByName(DeviceManager* manager, char* name) {
    DeviceNode* current = manager->head;
    while (current != NULL) {
        if (strcmp(current->deviceName, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

// Function to delete device by ID
int deleteDeviceById(DeviceManager* manager, int deviceId) {
    if (manager->head == NULL) {
        return 0;
    }
    
    DeviceNode* current = manager->head;
    DeviceNode* previous = NULL;
    
    while (current != NULL && current->deviceId != deviceId) {
        previous = current;
        current = current->next;
    }
    
    if (current == NULL) {
        return 0; // Device not found
    }
    
    // Remove device from list
    if (previous == NULL) {
        manager->head = current->next;
    } else {
        previous->next = current->next;
    }
    
    if (current->status == STATUS_ACTIVE) {
        manager->activeDevices--;
    }
    
    printf("Device removed: %s (ID: %d)\n", current->deviceName, current->deviceId);
    free(current);
    manager->totalDevices--;
    return 1; // Success
}

// Function to delete device by name
int deleteDeviceByName(DeviceManager* manager, char* name) {
    DeviceNode* device = findDeviceByName(manager, name);
    if (device != NULL) {
        return deleteDeviceById(manager, device->deviceId);
    }
    return 0;
}

// Function to activate device
void activateDevice(DeviceManager* manager, int deviceId) {
    DeviceNode* device = findDeviceById(manager, deviceId);
    if (device == NULL) {
        printf("Device not found!\n");
        return;
    }
    
    if (device->status == STATUS_ACTIVE) {
        printf("Device %s is already active!\n", device->deviceName);
        return;
    }
    
    device->status = STATUS_ACTIVE;
    manager->activeDevices++;
    printf("Device activated: %s (ID: %d)\n", device->deviceName, deviceId);
}

// Function to deactivate device
void deactivateDevice(DeviceManager* manager, int deviceId) {
    DeviceNode* device = findDeviceById(manager, deviceId);
    if (device == NULL) {
        printf("Device not found!\n");
        return;
    }
    
    if (device->status != STATUS_ACTIVE) {
        printf("Device %s is not active!\n", device->deviceName);
        return;
    }
    
    device->status = STATUS_INACTIVE;
    manager->activeDevices--;
    printf("Device deactivated: %s (ID: %d)\n", device->deviceName, deviceId);
}

// Function to set device error status
void setDeviceError(DeviceManager* manager, int deviceId) {
    DeviceNode* device = findDeviceById(manager, deviceId);
    if (device == NULL) {
        printf("Device not found!\n");
        return;
    }
    
    if (device->status == STATUS_ACTIVE) {
        manager->activeDevices--;
    }
    
    device->status = STATUS_ERROR;
    printf("Device error status set: %s (ID: %d)\n", device->deviceName, deviceId);
}

// Function to display device details
void displayDeviceDetails(DeviceNode* device) {
    if (device == NULL) {
        printf("Device not found!\n");
        return;
    }
    
    char timeStr[100];
    strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", localtime(&device->installTime));
    
    printf("\n=== DEVICE DETAILS ===\n");
    printf("Device ID: %d\n", device->deviceId);
    printf("Name: %s\n", device->deviceName);
    printf("Manufacturer: %s\n", device->manufacturer);
    printf("Type: %s\n", getDeviceTypeString(device->type));
    printf("Status: %s\n", getDeviceStatusString(device->status));
    printf("Driver Version: %s\n", device->driverVersion);
    printf("Priority: %d\n", device->priority);
    printf("Install Time: %s\n", timeStr);
    printf("=====================\n\n");
}

// Function to display all devices
void displayAllDevices(DeviceManager* manager) {
    if (manager->head == NULL) {
        printf("No devices found!\n");
        return;
    }
    
    printf("\n=== ALL DEVICES (Priority Order) ===\n");
    printf("%-6s %-20s %-15s %-10s %-10s %-8s %-15s\n", 
           "ID", "Name", "Manufacturer", "Type", "Status", "Priority", "Driver");
    printf("--------------------------------------------------------------------------------\n");
    
    DeviceNode* current = manager->head;
    while (current != NULL) {
        printf("%-6d %-20s %-15s %-10s %-10s %-8d %-15s\n",
               current->deviceId,
               current->deviceName,
               current->manufacturer,
               getDeviceTypeString(current->type),
               getDeviceStatusString(current->status),
               current->priority,
               current->driverVersion);
        current = current->next;
    }
    printf("\n");
}

// Function to display devices by status
void displayDevicesByStatus(DeviceManager* manager, DeviceStatus status) {
    printf("\n=== %s DEVICES ===\n", getDeviceStatusString(status));
    printf("%-6s %-20s %-15s %-10s %-8s\n", 
           "ID", "Name", "Manufacturer", "Type", "Priority");
    printf("------------------------------------------------------------\n");
    
    DeviceNode* current = manager->head;
    int count = 0;
    
    while (current != NULL) {
        if (current->status == status) {
            printf("%-6d %-20s %-15s %-10s %-8d\n",
                   current->deviceId,
                   current->deviceName,
                   current->manufacturer,
                   getDeviceTypeString(current->type),
                   current->priority);
            count++;
        }
        current = current->next;
    }
    
    if (count == 0) {
        printf("No devices with status: %s\n", getDeviceStatusString(status));
    }
    printf("Total: %d devices\n\n", count);
}

// Function to display devices by type
void displayDevicesByType(DeviceManager* manager, DeviceType type) {
    printf("\n=== %s DEVICES ===\n", getDeviceTypeString(type));
    printf("%-6s %-20s %-15s %-10s %-8s\n", 
           "ID", "Name", "Manufacturer", "Status", "Priority");
    printf("----------------------------------------------------------\n");
    
    DeviceNode* current = manager->head;
    int count = 0;
    
    while (current != NULL) {
        if (current->type == type) {
            printf("%-6d %-20s %-15s %-10s %-8d\n",
                   current->deviceId,
                   current->deviceName,
                   current->manufacturer,
                   getDeviceStatusString(current->status),
                   current->priority);
            count++;
        }
        current = current->next;
    }
    
    if (count == 0) {
        printf("No %s devices found\n", getDeviceTypeString(type));
    }
    printf("Total: %d devices\n\n", count);
}

// Function to show system statistics
void showSystemStats(DeviceManager* manager) {
    printf("\n=== SYSTEM STATISTICS ===\n");
    printf("Total devices: %d\n", manager->totalDevices);
    printf("Active devices: %d\n", manager->activeDevices);
    printf("Inactive devices: %d\n", manager->totalDevices - manager->activeDevices);
    
    // Count by type
    int typeCounts[8] = {0};
    DeviceNode* current = manager->head;
    
    while (current != NULL) {
        typeCounts[current->type]++;
        current = current->next;
    }
    
    printf("\nDevices by type:\n");
    for (int i = 0; i < 8; i++) {
        if (typeCounts[i] > 0) {
            printf("  %s: %d\n", getDeviceTypeString((DeviceType)i), typeCounts[i]);
        }
    }
    
    printf("Next device ID: %d\n", manager->nextDeviceId);
    printf("========================\n\n");
}

// Function to populate sample devices
void loadSampleDevices(DeviceManager* manager) {
    printf("Loading sample devices...\n\n");
    
    addDevice(manager, "USB Keyboard", "Logitech", DEVICE_INPUT, "v2.1.0", 5);
    addDevice(manager, "USB Mouse", "Microsoft", DEVICE_INPUT, "v1.8.2", 5);
    addDevice(manager, "Ethernet Adapter", "Intel", DEVICE_NETWORK, "v3.2.1", 9);
    addDevice(manager, "Audio Codec", "Realtek", DEVICE_AUDIO, "v6.0.9", 7);
    addDevice(manager, "Graphics Card", "NVIDIA", DEVICE_VIDEO, "v12.4.0", 10);
    addDevice(manager, "SSD Drive", "Samsung", DEVICE_STORAGE, "v4.1.2", 8);
    addDevice(manager, "WiFi Adapter", "Broadcom", DEVICE_NETWORK, "v2.9.3", 8);
    addDevice(manager, "Bluetooth Module", "Intel", DEVICE_BLUETOOTH, "v5.2.1", 6);
    addDevice(manager, "USB Hub", "D-Link", DEVICE_USB, "v1.0.5", 4);
    addDevice(manager, "Webcam", "Logitech", DEVICE_VIDEO, "v2.3.7", 6);
    
    // Activate some devices
    activateDevice(manager, findDeviceByName(manager, "USB Keyboard")->deviceId);
    activateDevice(manager, findDeviceByName(manager, "USB Mouse")->deviceId);
    activateDevice(manager, findDeviceByName(manager, "Ethernet Adapter")->deviceId);
    activateDevice(manager, findDeviceByName(manager, "Graphics Card")->deviceId);
    activateDevice(manager, findDeviceByName(manager, "SSD Drive")->deviceId);
    
    // Set one device to error
    setDeviceError(manager, findDeviceByName(manager, "WiFi Adapter")->deviceId);
    
    printf("Sample devices loaded!\n");
}

// Function to free device manager memory
void freeDeviceManager(DeviceManager* manager) {
    DeviceNode* current = manager->head;
    while (current != NULL) {
        DeviceNode* next = current->next;
        free(current);
        current = next;
    }
    free(manager);
}

// Main function with menu-driven interface
int main() {
    DeviceManager* deviceManager = createDeviceManager();
    int choice, deviceId, priority, type;
    char name[50], manufacturer[30], driverVersion[20];
    DeviceNode* foundDevice;
    
    printf("⚙️  Welcome to Device Node Management System! ⚙️\n");
    printf("Manage your system devices efficiently!\n\n");
    
    do {
        printf("\n=== DEVICE MANAGEMENT MENU ===\n");
        printf("1. Add device\n");
        printf("2. Remove device by ID\n");
        printf("3. Remove device by name\n");
        printf("4. Find device by ID\n");
        printf("5. Find device by name\n");
        printf("6. Activate device\n");
        printf("7. Deactivate device\n");
        printf("8. Set device error\n");
        printf("9. Display all devices\n");
        printf("10. Display devices by status\n");
        printf("11. Display devices by type\n");
        printf("12. Show system statistics\n");
        printf("13. Load sample devices\n");
        printf("14. Exit\n");
        printf("Enter your choice: ");
        
        scanf("%d", &choice);
        getchar(); // consume newline
        
        switch (choice) {
            case 1:
                printf("Enter device name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                printf("Enter manufacturer: ");
                fgets(manufacturer, sizeof(manufacturer), stdin);
                manufacturer[strcspn(manufacturer, "\n")] = 0;
                
                printf("Device types:\n");
                printf("0-USB, 1-PCI, 2-Network, 3-Audio, 4-Video, 5-Storage, 6-Input, 7-Bluetooth\n");
                printf("Enter device type (0-7): ");
                scanf("%d", &type);
                getchar();
                
                printf("Enter driver version: ");
                fgets(driverVersion, sizeof(driverVersion), stdin);
                driverVersion[strcspn(driverVersion, "\n")] = 0;
                
                printf("Enter priority (1-10): ");
                scanf("%d", &priority);
                
                if (type >= 0 && type <= 7 && priority >= 1 && priority <= 10) {
                    addDevice(deviceManager, name, manufacturer, (DeviceType)type, driverVersion, priority);
                } else {
                    printf("Invalid type or priority!\n");
                }
                break;
                
            case 2:
                printf("Enter device ID to remove: ");
                scanf("%d", &deviceId);
                
                if (deleteDeviceById(deviceManager, deviceId)) {
                    printf("Device removed successfully!\n");
                } else {
                    printf("Device not found!\n");
                }
                break;
                
            case 3:
                printf("Enter device name to remove: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                if (deleteDeviceByName(deviceManager, name)) {
                    printf("Device removed successfully!\n");
                } else {
                    printf("Device not found!\n");
                }
                break;
                
            case 4:
                printf("Enter device ID: ");
                scanf("%d", &deviceId);
                
                foundDevice = findDeviceById(deviceManager, deviceId);
                displayDeviceDetails(foundDevice);
                break;
                
            case 5:
                printf("Enter device name: ");
                fgets(name, sizeof(name), stdin);
                name[strcspn(name, "\n")] = 0;
                
                foundDevice = findDeviceByName(deviceManager, name);
                displayDeviceDetails(foundDevice);
                break;
                
            case 6:
                printf("Enter device ID to activate: ");
                scanf("%d", &deviceId);
                activateDevice(deviceManager, deviceId);
                break;
                
            case 7:
                printf("Enter device ID to deactivate: ");
                scanf("%d", &deviceId);
                deactivateDevice(deviceManager, deviceId);
                break;
                
            case 8:
                printf("Enter device ID to set error: ");
                scanf("%d", &deviceId);
                setDeviceError(deviceManager, deviceId);
                break;
                
            case 9:
                displayAllDevices(deviceManager);
                break;
                
            case 10:
                printf("Status types: 0-Active, 1-Inactive, 2-Error, 3-Pending\n");
                printf("Enter status type (0-3): ");
                scanf("%d", &type);
                
                if (type >= 0 && type <= 3) {
                    displayDevicesByStatus(deviceManager, (DeviceStatus)type);
                } else {
                    printf("Invalid status type!\n");
                }
                break;
                
            case 11:
                printf("Device types: 0-USB, 1-PCI, 2-Network, 3-Audio, 4-Video, 5-Storage, 6-Input, 7-Bluetooth\n");
                printf("Enter device type (0-7): ");
                scanf("%d", &type);
                
                if (type >= 0 && type <= 7) {
                    displayDevicesByType(deviceManager, (DeviceType)type);
                } else {
                    printf("Invalid device type!\n");
                }
                break;
                
            case 12:
                showSystemStats(deviceManager);
                break;
                
            case 13:
                loadSampleDevices(deviceManager);
                break;
                
            case 14:
                printf("Thanks for using Device Management System!\n");
                break;
                
            default:
                printf("Invalid choice! Please try again.\n");
        }
    } while (choice != 14);
    
    freeDeviceManager(deviceManager);
    return 0;
}