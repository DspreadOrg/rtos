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

# 

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

#### **2.1 Hardware Model Mapping**

The SDK uses an internal hardware model code, while the same product is marketed under a different public name:

| Marketing Name | Hardware Model | Build Macros |
|:-------------- |:-------------- |:------------ |
| QPOS Plus      | A30R           | `-DHW_A30R`  |
| D70            | D70R           | `-DHW_D70R`  |

#### **2.2 Prerequisites**

1. The cross-compilation toolchain is extracted to `C:\prebuilts` (contains `win32\bin`, `cmake`, `python3`, `gcc-arm-none-eabi`, and `ninja`). The build scripts add these to `PATH` automatically.
2. The pre-compiled model libraries exist under `examples/Demo/Lib/`: `libSDK_<MODEL>.a` and `libSelfCheck_<MODEL>.a` (e.g. `libSDK_A30R.a`, `libSelfCheck_A30R.a`). The scripts abort with a clear message if they are missing.
3. All build commands must be run from the `04.SDK_DEMO/app_csdk` directory in a Windows Command Prompt.

#### **2.3 Build Scripts**

Run the batch file that matches the hardware model and the desired build type:

| Model          | Build Type                         | Script                         | Output Directory     |
|:-------------- |:---------------------------------- |:------------------------------ |:-------------------- |
| A30R (QPOS Plus)| Debug (TRACE logs enabled)        | `A30R_Lvgl_debug_build.bat`    | `out/A30R_debug/`    |
| A30R (QPOS Plus)| Release (logs disabled, faster)   | `A30R_Lvgl_release_build.bat`  | `out/A30R_release/`  |
| D70R (D70)     | Debug (TRACE logs enabled)         | `D70R_Lvgl_debug_build.bat`    | `out/D70R_debug/`    |
| D70R (D70)     | Release (logs disabled, faster)    | `D70R_Lvgl_release_build.bat`  | `out/D70R_release/`  |

For example, to build a release firmware for the D70 (D70R hardware):

```bat
cd 04.SDK_DEMO\app_csdk
D70R_Lvgl_release_build.bat
```

#### **2.4 What a Build Script Does**

Each script performs the same pipeline, parameterized by model and build type:

1. **Check model libraries**: verifies `libSDK_<MODEL>.a` and `libSelfCheck_<MODEL>.a` exist.
2. **Set compile macros** (`MY_PRJ`): the model macro `-DHW_<MODEL>` and `-DXML_MENU` are always defined; the release build additionally defines `-D__NOTRACE` to compile out TRACE logging.
3. **Generate version header** (`script\svn_ver.bat`): writes `examples/Demo/Application/Include/version.h` from the SVN revision template.
4. **Prepare environment** (`tools\core_launch.bat`): adds the `C:\prebuilts` toolchain, CMake, Python 3, and Ninja to `PATH`, then creates the `out/<MODEL>_<type>` directory.
5. **Configure and compile**: runs `cmake ..\.. -G Ninja` to configure the Ninja project and `ninja` to build it.
6. **Package and rename** (`script\rename.bat`): the build generates the application image and the full `.pac` package in `out/<MODEL>_<type>/hex/`; the script then copies the application `.img` to a version-stamped filename and removes the intermediate `.pac`.

#### **2.5 Output Artifacts**

The final deliverables are placed in `out/<MODEL>_<type>/hex/`:

- **`<MODEL>_APP_<version>.<R|D><svn>.img`** — version-stamped application firmware image (the file to flash). The `R`/`D` flag indicates Release/Debug and the trailing number is the SVN revision.
  - Example: `A30R_APP_V1.00.02.R19792.img`, `D70R_APP_V1.00.03.R19792.img`.
- **`mainapp_flash.img`** — the raw application-only image produced during the build.
- **`mainapp_flash.pac`** — the full firmware package (application + FDL) produced during the build; it is deleted by the packaging step, so keep a copy if a full-package file is needed.

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
