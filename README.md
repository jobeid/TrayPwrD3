# TrayPwrD3
**Keeps dGPU on but idle** until needed. This addresses the Windows 10 (64bit) stutter/mouse freeze on dual GPU laptops (intel or AMD CPU with built in GPU in combination with NVIDIA). TrayPwrD3 runs in the background and stays in a tray icon.   ![Tray icon](screenshots/traypwr_icon1.png?raw=true "Tray icon")

**Other functionality**: single click turns monitor off to save power until you wake it up by moving the mouse or using the keyboard; provides several menu options on right click; more details in Help menu.

This is to address the [Windows 10 and Optimus problem of stutters and mouse hangs](https://forums.geforce.com/default/topic/860554/geforce-mobile-gpus/windows-10-and-optimus/15/). For more about this issue see description on answers.microsoft [Intel + NVIDIA Laptop Freeze Problem](https://answers.microsoft.com/en-us/windows/forum/windows_10-hardware/mobile-gtx-1060-freeze-problem/93e7004a-62b1-4211-8e37-4c136608865e). Note: it is not for stutters in games. It only addresses the desktop stutters in Windows 10 (right click -> display settings, battery icon, chrome and firefox tabs, PowerPoint slideshow etc).  If you don't have any of these, then you don't need it. 

**Windows 10 May 2020 Update:** With this latest update Microsoft fixed the latency issue. However, you will need NVIDIA driver version 451.48 or later. For more information see this Windows Latest post: [Nvidia GeForce 451.48 adds Windows 10 GPU scheduling feature](https://www.windowslatest.com/2020/06/24/nvidia-geforce-451-48-windows-10-hardware-accelerated-gpu-scheduling/).
  
**Update 7/14/2020:** Still noticing some brief hanging during some activities with explorer. So latency issue not totally fixed.

**Update 11/12/2020:** Still noticing some major hanging during some activities with explorer and when running certain apps e.g. Media Player Classic on AMD Ryzen systems. So NVIDIA GPU activation latency remains a problem. The updated version of TrayPwrD3 (v2020-11) now supports AMD Ryzen systems and prevents the GPU stutter or latency on both Intel and AMD Ryzen systems.

# Installation
Simply download the [executable](https://github.com/jobeid/TrayPwrD3/tree/master/executable), uncompress it in any folder and run it. 
Notes:
- The executable was compliled for 64bit windows. 
- Direct3D version 12 is required.
- If you don't already have [Visual C++ Redistributable for Visual Studio](https://support.microsoft.com/en-us/help/2977003/the-latest-supported-visual-c-downloads) installed, then you could download the 64bit version [directly from Microsoft](https://aka.ms/vs/16/release/vc_redist.x64.exe) and install it.
- Optional: If you would like to start TrayPwrD3 when windows starts, you can place a shortcut of the executable in the startup folder (located at %USERPROFILE%\AppData\Roaming\Microsoft\Windows\Start Menu\Programs\STARTUP). You can quickly open the startup folder by tapping the Windows key+R to bring up the "Run" dialog, and type `shell:startup` then enter.

# Using the App
When you start the app it simply runs in the background in a tray icon, keeping the dGPU on but idle. This addresses the well known Windows 10 stutter/mouse freeze on some dual GPU laptops.

**To stop the app** simpley right-click the icon and click exit. **There is _no_ need to reboot the system**.

# Screenshots

Tray icon:   ![Tray icon](screenshots/traypwr_icon1.png?raw=true "Tray icon")


Right click menu:  ![Menu](screenshots/traypwr_rightclick1.png?raw=true "Menu")
