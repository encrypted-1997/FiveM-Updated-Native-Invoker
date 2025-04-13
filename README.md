**My Discord**
_encrypt3d.

**Credits**
Original Invoker code from : [SuckaSkidding](https://github.com/SuckaSkidding/New-FiveM-Cheat/blob/main/native.hpp)
Native class from : [GTAV Native DB](https://alloc8or.re/gta5/nativedb/)

**Detected Banned for 300 days**  
Use at your own risk If you do not care about ban from CFX

## How to Update the invoker

### 1. Locate the `MapNative` Function

Refer to the official FiveM GitHub repository for the implementation of the `MapNative` function:

[MapNative Function on GitHub](https://github.com/citizenfx/fivem/blob/master/code/components/rage-scripting-five/src/TableBuilder.cpp#L27)

### 2. Find the MapNative Function in `rage-scripting-five.dll`

- Navigate to your FiveM installation folder and locate the `rage-scripting-five.dll` file
- Load it to IDA or any debugger (you can use WinDBG also)
- start finding uint64_t MapNative(uint64_t inNative) like in the picture, I've uploaded
- Once youve found it (as shown in the picture i uploaded), click on the address once to highlight it in yellow
- Then click at Hex View Tab You’ll see the bytes highlighted in green
- Copy those bytes and replace the mapped_hash_addy pattern in the code(E9 BB FB FF FF) with your copied bytes
- successfully updated it!

![Alt text1](https://i.imgur.com/d7GxXA9.png)
![Alt text2](https://i.imgur.com/pa22Euj.png)

