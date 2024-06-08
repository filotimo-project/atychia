# Atychia

A utility akin to Ctrl-Alt-Delete on Windows that allows a user to recover from a broken session, rather than being forced to a TTY.
Bind any chosen keyboard shortcut on the desktop to `pkexec /usr/bin/atychia-launch`.

A daemon using libudev and libevdev, potentially utilising `interception-tools` will be implemented eventually.
Keyboard navigation also needs to be worked on to make this foolproof as well as a potential text-based UI where a Wayland session can't be launched.

![Screenshot of the application](screenshot.png)

Requires a system user `atychia` (UID below 1000, and no home dir) which is in the `atychia` and `video` group.
You can make this with `sudo useradd -r atychia && sudo usermod -a -G video atychia`.

Requires an up to date version of KDE Frameworks 6, systemd, logind, and PolicyKit.

You can build and install this with the following:
```
cd ./build
cmake .. -G "Kate - Ninja" -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
sudo ninja-build -C . install
```
