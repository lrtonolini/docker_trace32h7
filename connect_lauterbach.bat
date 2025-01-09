@echo off
setlocal

rem Define the USB VID:PID to look for
set "VIDPID1=0897:0005"
set "VIDPID2=0897:0004"

rem Try to find the first device with VID:PID1
for /f "tokens=2 delims=:" %%i in ('usbipd list ^| findstr /i "%VIDPID1%"') do (
    rem Bind the device using VID:PID
    echo Device with VID:PID %VIDPID1% found. Binding it...
    usbipd bind -i %VIDPID1%

    rem Attach the device to WSL
    echo Attaching device with VID:PID %VIDPID1% to WSL...
    usbipd attach --wsl -i %VIDPID1%
    goto :done
)

rem If the first device was not found, try the second device with VID:PID2
for /f "tokens=2 delims=:" %%i in ('usbipd list ^| findstr /i "%VIDPID2%"') do (
    rem Bind the device using VID:PID
    echo Device with VID:PID %VIDPID2% found. Binding it...
    usbipd bind -i %VIDPID2%

    rem Attach the device to WSL
    echo Attaching device with VID:PID %VIDPID2% to WSL...
    usbipd attach --wsl -i %VIDPID2%
    goto :done
)

rem If no matching device is found
echo No matching USB device found with VID:PID %VIDPID1% or %VIDPID2%.

:done

rem Set the WSL environment variable for the current session
echo Setting WSLENV for the current session...
setx WSLENV "MNTWSL:pu"

rem Run the startup.sh script in WSL
echo Running startup.sh in WSL...
wsl bash "%MNTWSL%/startup.sh"

endlocal