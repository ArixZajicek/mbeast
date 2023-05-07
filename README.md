# UnityVisor/mbeast

This project contains a variety of tools for interfacing a Unity application with hardware on a Raspberry Pi. Most notably, it allows sending pixel data directly from a Unity application to a HUB75 based LED matrix. Other additions are planned, such as serial communication with an attached microcontroller for further hardware control and inputs.

# LibCostume

This is the core of the project. It is a library written in C that provides a native interface to call from a C# script in Unity. The library then provides easy access to the HUB75 LED matrix interface or a debug view via SDL if not running on a live device.