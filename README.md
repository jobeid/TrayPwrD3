# TrayPwrD3
**Keeps dGPU on but idle** until needed. This addresses the Windows 10 stutter/mouse freeze on dual GPU laptops. TrayPwrD3 runs in the background and stays in a tray icon.   ![Tray icon](screenshots/traypwr_icon1.png?raw=true "Tray icon")

**Other functionality**: single click turns monitor off to save power until you wake it up by moving the mouse or using the keyboard; provides several menu options on right click; more details in Help menu.

This is to address the [Windows 10 and Optimus problem of stutters and mouse hangs](https://forums.geforce.com/default/topic/860554/geforce-mobile-gpus/windows-10-and-optimus/15/). For more about this issue see description on answers.microsoft [Intel + NVIDIA Laptop Freeze Problem](https://answers.microsoft.com/en-us/windows/forum/windows_10-hardware/mobile-gtx-1060-freeze-problem/93e7004a-62b1-4211-8e37-4c136608865e).

# Installation
Simply download the [executable](https://github.com/jobeid/TrayPwrD3/tree/master/executable), uncompress it in any folder and run it. 
Notes:
- The executable was compliled for 64bit windows.
- If you don't already have [Visual C++ Redistributable for Visual Studio 2015](https://www.microsoft.com/en-us/download/details.aspx?id=48145) installed, then you could download and install it from Microsoft [here](https://go.microsoft.com/fwlink/?LinkId=746572).)
- Optional: If you would like to start TrayPwrD3 when windows starts, you can place a shortcut of the executable in the startup folder (located at %USERPROFILE%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\STARTUP). You can quickly open the startup folder by tapping the Windows key+R to bring up the "Run" dialog, and type `shell:startup` then enter.

# Screenshots

Tray icon:   ![Tray icon](screenshots/traypwr_icon1.png?raw=true "Tray icon")


Right click menu:  ![Menu](screenshots/traypwr_rightclick.png?raw=true "Menu")
