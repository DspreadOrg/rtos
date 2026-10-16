# Embedded RTOS POS Terminal Application Development Guide

This repository contains the source code and related SDK for an embedded RTOS POS terminal application based on the Unisoc 8910 (RDA) platform. The project demonstrates how to use the Unisoc CSDK to develop high-performance payment terminal applications.

## **Project Introduction**

This project is a comprehensive POS terminal demo program that integrates core functionalities such as financial transaction processing, graphical user interface interaction, and network communication.

### **Core Features**

- **Financial Transactions**: Supports CardTrade, BarcodeTrade, and EMV L1/L2 processes.
- **Graphical Interface**: UI framework built on **LVGL 8.3.x**, supporting multi-level menus, message popups, and multi-language display.
- **Communication Capabilities**: Supports GPRS/LTE network connections, MQTT protocol communication, and WiFi configuration.
- **System Functions**: Integrated TMS remote upgrade, parameter management, log management, low power management, and driver abstraction layer (Battery, Key, Printer, GPS, etc.).
- **Multi-language Support**: Built-in Chinese, English, Portuguese, and other language resources with dynamic switching mechanism.

### **Tech Stack**

- **Platform**: Unisoc 8910 (UIX8910)
- **OS**: RTOS (with OSI abstraction layer)
- **Build System**: CMake
- **GUI Framework**: LVGL (Light and Versatile Graphics Library)
- **Language**: C

---

## **Environment Setup**

### **1. Hardware Preparation**

- **Debug Board**: Dedicated multi-functional debug adapter board.
- **Cables**: 2 × Micro-USB cables (for serial port and power), 1 × double-ended Type-C cable (to connect the POS terminal).
- **PC Requirements**: Windows OS, RAM ≥ 4GB, Dual-core processor or higher recommended.

### **2. Software Toolkits**

| Package Name                | Purpose                              | Storage Path                                                                                                                                   |
|:--------------------------- |:------------------------------------ |:---------------------------------------------------------------------------------------------------------------------------------------------- |
| `prebuilts.rar`             | Cross-compilation Toolchain (GCC)    | [Download Link](https://drive.google.com/file/d/1ov-yd-pDURo3wFRl8hCcGMDOAwWPl36o/view?usp=sharing) <br> (Must be extracted to `C:\prebuilts`) |
| `8910_module_usb_driver...` | MCU USB Driver                       | `02.SDK Envrmt/`                                                                                                                               |
| `CP210x_VCP_Windows.rar`    | USB to Virtual Serial Port Driver    | `02.SDK Envrmt/`                                                                                                                               |
| `Coolwatcher`               | Runtime Log Capture Tool             | [Download Link](https://drive.google.com/file/d/1_In2GXYtz3U9dJh6utiGerCKLq0gDXvY/view?usp=sharing) <br> (Located in `02.SDK Envrmt/`)         |
| `Flash_Tool`                | Firmware Flashing & File System Tool | `05.Tools/`                                                                                                                                    |

### **3. Deployment Steps**

1. **Install Toolchain**: Extract `prebuilts.rar` to the **root of C drive**. The path must be `C:\prebuilts`, otherwise CMake will not find the compiler.
2. **Connect Hardware**: Connect the POS terminal to the debug board, and the debug board to the PC.
3. **Install Drivers**:
   - In Device Manager, update the driver for the unrecognized `USB Serial Device`, pointing to the extracted MCU driver directory. It should display as `SPRD U2S Diag` after installation.
   - Install the CP210x driver to enable debug serial port output.

---

## **Quick Start Guide**

### **1. Source Code Structure**

- `app_csdk/`: SDK core and build scripts.
  - `examples/Demo/Application`: Business logic code (Transactions, Menus, MQTT, etc.).
  - `examples/Demo/Driver`: Driver adaptation layer (LCD, Printer, Keys, etc.).
  - `examples/Demo/Lib`: Pre-compiled financial kernel and algorithm libraries.
  - `components/`: SDK headers and static libraries.

### **2. Build Process**

In the `04.SDK_DEMO/app_csdk` directory, run the corresponding batch file based on the hardware model (A30R or D70R):

- **Debug Build (TRACE logs enabled)**:
  - A30R Model: `A30R_Lvgl_debug_build.bat`
  - D70R Model: `D70R_Lvgl_debug_build.bat`
- **Release Build (Logs disabled for performance)**:
  - A30R Model: `A30R_Lvgl_release_build.bat`
  - D70R Model: `D70R_Lvgl_release_build.bat`

The compiled firmware is located in the `out/xxx_debug/hex/` or `out/xxx_release/hex/` directory, mainly including `.pac` (Full Firmware) and `.img` (App-only Firmware).

### **3. Firmware Flashing**

1. **Enter Download Mode**: Toggle the **BOOT switch** on the debug board to the left and power on. The BOOT Mode LED will light up (device enters download mode).
2. **Use Tools**: Open `Flash_Tool` (located in `05.Tools/Flash_Tool/`) or `AppUpgradeTool`, and load the generated `.pac` file.
3. **Execute Download**: Click Start to begin flashing. Once the progress bar completes, toggle the BOOT switch back to the right and restart the device.

### **4. Log Debugging**

1. **Enable TRACE**: Ensure `SetTraceLogConfig(1);` is called in `main.c`, and a `release` script was NOT used for building.
2. **Open Coolwatcher**: Start the `Coolwatcher` tool.
3. **Configure Serial Port**: Select the correct debug serial port in Coolwatcher (usually the first COM port after driver installation).
4. **View Output**: You can now view real-time debug information output via the `TRACE()` macro in the code.

---

## **Related Documents**

- Environment Setup Manual:Locate in 01.DEB 
- SDK User Manual: Locate in 03.SDK UM
- Coolwatcher User Guide: Locate in 02.SDK Envrmt/
