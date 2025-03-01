# Atychia

A utility akin to Ctrl-Alt-Delete on Windows that allows a user to recover from a broken session, rather than being forced to a TTY.
This automatically registers a shortcut with KDE, and is designed for use with KDE. Registers `Meta+Ctrl+Alt+Del` by default.
A shortcut is also registered to relaunch Plasmashell - `Meta+Ctrl+Shift+B`.

Attempts to look a bit like the Plymouth boot screen - you'll find that it takes the watermark from the spinner theme, and has a black background for consistency.

![Screenshot of the application](screenshot.png)

Requires an up to date version of KDE Frameworks 6, Qt 6, systemd, logind, and Polkit.
Requires `cage`, the Wayland kiosk compositor, to be installed.

You can build and install this with the following:
```
mkdir -p build && cd build
cmake .. -G "Kate - Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
sudo ninja-build -C . install
```
