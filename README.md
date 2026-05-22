# Makcu Hardware Control Box SDK for .NET

This documentation describes how to integrate and use the `Makcu-CSharp.dll` wrapper in your C# projects. The wrapper bridges the native Win32 asynchronous serial communication library with the .NET environment using C++/CLI, providing a high-performance, clean, and thread-safe API.

---

### CRITICAL REQUIREMENT
> Since this library encapsulates native C++ code, your C# project **MUST NOT** target `Any CPU`. You **MUST** force your C# project's platform target to **`x64`** (or `x86`), matching the architecture of the DLL. Otherwise, a `System.BadImageFormatException` will be thrown at runtime.

---

## API Reference

### `MakcuProxy::MakcuLib` (Static Class)

| Method | Return Type | Description |
| :--- | :--- | :--- |
| `Init(int comPort, int baudRate)` | `bool` | Initializes the serial port and connects to the hardware box. |
| `Close()` | `void` | Closes the serial port and releases hardware resources. |
| `MouseMove(int x, int y)` | `bool` | Relative mouse movement. |
| `MouseMoveMock(int x, int y, int time)` | `bool` | Simulates human-like mouse movement curves with a specified duration. |
| `MouseMoveto(int x, int y)` | `bool` | Absolute mouse movement. |
| `MouseLeft(int type)` | `bool` | Simulates mouse left button state (`1` for Press down, `0` for Release up). |
| `MouseRight(int type)` | `bool` | Simulates mouse right button state (`1` for Press down, `0` for Release up). |
| `UpdateMouseState()` | `bool` | Synchronizes and updates the hardware physical key states. Must be called continuously. |
| `GetMouseKeyState(int key)` | `bool` | Queries the state of a specific key. Keys: `0`: Left, `1`: Right, `2`: Middle, `3`: Side2 (Upper), `4`: Side1 (Lower). |

---

## C# Integration Guide

### 1. Project Configuration
1. Copy `Makcu-CSharp.dll` into your C# project directory.
2. In Visual Studio, right-click your C# project -> **Add** -> **Reference...** -> **Browse** and select `MakcuProxy.dll`.
3. Open **Configuration Manager** in Visual Studio, change your active solution platform from `Any CPU` to **`x64`**.

### 2. Implementation Example

Here is a comprehensive example demonstrating device initialization, asynchronous state background monitoring, relative movement, and hardware button state interception.

```csharp
using System;
using System.Threading;
using MakcuProxy; // Import the hardware proxy namespace

class Program
{
    static void Main(string[] args)
    {
        Console.WriteLine("[System] Initializing Makcu Hardware Box...");

        // 1. Initialize the Hardware Box (COM Port 10, Baudrate 115200)
        int comPort = 10;
        int baudRate = 115200;

        if (!MakcuLib.Init(comPort, baudRate))
        {
            Console.WriteLine($"[Error] Failed to open COM{comPort}. Please check connection.");
            return;
        }
        Console.WriteLine($"[Success] Makcu Hardware Box connected on COM{comPort}.");

        // 2. Start a background thread to continuously poll physical mouse button states
        Thread monitorThread = new Thread(() =>
        {
            Console.WriteLine("[Monitor] Physical mouse state listener thread started.");
            while (true)
            {
                // Synchronize raw physical data from hardware registers
                MakcuLib.UpdateMouseState(); 
                
                // Sleep for 1ms to prevent high CPU utilization
                Thread.Sleep(1); 
            }
        });
        monitorThread.IsBackground = true;
        monitorThread.Start();

        // 3. Main execution and key interception loop
        Console.WriteLine("[System] Hardware is ready. Loop started.");
        try
        {
            while (true)
            {
                // Key 0: Physical Left Click Hold Interception
                if (MakcuLib.GetMouseKeyState(0))
                {
                    // Execute a relative mouse movement offset of (15, 15)
                    MakcuLib.MouseMove(15, 15);
                }

                // Key 3: Physical Upper Side Button Interception
                if (MakcuLib.GetMouseKeyState(3))
                {
                    // Perform a relative move down-right
                    MakcuLib.MouseMove(15, 15);

                    // Execute a rapid click simulation sequence via hardware
                    MakcuLib.MouseLeft(1);  // Hardware left mouse down
                    Thread.Sleep(1);        // Wait 1 millisecond
                    MakcuLib.MouseLeft(0);  // Hardware left mouse up
                }

                // Standard polling interval pacing
                Thread.Sleep(1);
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine($"[Runtime Exception] {ex.Message}");
        }
        finally
        {
            // 4. Safely release resources on shutdown
            MakcuLib.Close();
            Console.WriteLine("[System] COM port safely closed. Stream terminated.");
        }
    }
}
