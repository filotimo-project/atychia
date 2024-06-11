# Atychia

A utility akin to Ctrl-Alt-Delete on Windows that allows a user to recover from a broken session, rather than being forced to a TTY.
This automatically registers a shortcut with KDE, and is designed for use with KDE. Registers `Meta+Ctrl+Alt+Del` by default.

Keyboard navigation needs to be worked on to make this foolproof as well as a potential text-based UI where a Wayland session can't be launched.

![Screenshot of the application](screenshot.png)
![Recording of the application](atychia.gif)

Requires a system user `atychia` (UID below 1000, and no home dir) which is in the `atychia` and `video` group.
You can make this with `sudo useradd -r atychia && sudo usermod -a -G video atychia`.

Requires an up to date version of KDE Frameworks 6, Qt 6, systemd, logind, and Polkit.

You can build and install this with the following:
```
mkdir build && cd build
cmake .. -G "Kate - Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
sudo ninja-build -C . install
```
