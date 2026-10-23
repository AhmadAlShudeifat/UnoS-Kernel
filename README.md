# UnoS - ATmega328P Micro-Operating System
![UnoS Banner](https://via.placeholder.com/800x200/222222/00FF00?text=DEVELOPMENT:+PAUSED-FOR+NOW)

README MADE BY AI

![UnoS Banner](https://via.placeholder.com/800x200/222222/00FF00?text=UnoS:+Bare-Metal+Micro-OS+for+Arduino)

UnoS is a lightweight, bare-metal operating system built entirely from scratch for the Arduino Uno (ATmega328P). It brings seamless multitasking, dynamic memory management, and a sandboxed execution environment to an 8-bit microcontroller with only 2,048 bytes of RAM.

By bypassing standard Arduino overhead, UnoS acts as a fully standalone computer, complete with its own command-line interface, software protection rings, and virtual memory manager.

## 🚀 What It Does

UnoS transforms an Arduino into a standalone multitasking system that doesn't need a PC to run or switch programs. 

*   **Custom Memory Management (MMU):** Dynamically allocates and frees RAM using a custom slotted heap system. User programs never touch raw physical memory directly.
*   **Software Protection Rings:** Implements a Ring 0 (Kernel) to Ring 3 (Userland) architecture entirely in software to prevent apps from crashing the core OS.
*   **Bytecode Virtual Machine:** User applications are stored in the 1KB internal EEPROM and executed via a custom bytecode interpreter engine, saving precious RAM.
*   **Dual-Monitor CLI:** Features an on-board terminal using a 0.96" OLED display and a 5-button D-Pad for hardware-independent control, while simultaneously mirroring telemetry to a PC via Serial.

![UnoS Architecture](https://via.placeholder.com/600x300/333333/00FFFF?text=Ring+0+(Kernel)+%7C+Ring+3+(EEPROM+Userland))

## 🧠 How It Works

The ATmega328P lacks a hardware MMU and has strict memory boundaries built into the silicon. UnoS works around this by strictly partitioning the physical memory map:

*   `0x0100 - 0x02FF`: **Kernel Static RAM** (Process tables, memory allocation bitmaps).
*   `0x0300 - 0x06FF`: **Managed Dynamic Heap** (128 individual 8-byte slots for user programs).
*   `0x0700 - 0x08FF`: **Hardware Call Stack**.

The OS boots through a tiny `.ino` launcher and immediately hands control over to the pure C++ kernel (`kernel.cpp`). User programs request memory or hardware access via system calls (e.g., `sys_write_slot`), ensuring the Ring 0 kernel remains the absolute authority over the hardware.

## ⚙️ Hardware Requirements

*   **Arduino Uno** (ATmega328P)
*   **0.96" OLED Display** (SSD1306, I2C)
*   **5x Tactile Push Buttons** (For the on-screen keyboard/D-Pad navigation)
*   *(Optional)* PC with a Serial Monitor for secondary logging.

![OLED Terminal Demo](https://via.placeholder.com/400x300/111111/FFFF00?text=OLED+Terminal+Interface+Demo)

## ⚡ How to Flash It

Because UnoS handles its own compilation linking, flashing is incredibly straightforward and doesn't require any complex makefiles.

1.  **Prepare the Directory:** Ensure all UnoS files (`sketch.ino`, `kernel.h`, `kernel.cpp`, `pyinterp.cpp`, `compile++.cpp`) are grouped in a single folder named exactly the same as your `.ino` file (e.g., `sketch`).
2.  **Install Dependencies:** Open the Arduino IDE and install the **SSD1306Ascii** library from the Library Manager (this is required for the zero-RAM display driver).
3.  **Connect Hardware:** Wire your OLED display via I2C (SDA to A4, SCL to A5) and your D-Pad buttons to the configured digital pins.
4.  **Compile and Upload:** Open `sketch.ino` in the Arduino IDE, select your Arduino Uno board and COM port, and hit **Upload**. 
5.  **Disconnect & Run:** Once flashed, the OS lives permanently on the chip. You can disconnect it from your PC, power it from a battery, and use the OLED/D-Pad interface as a standalone computer.

README MADE BY AI
