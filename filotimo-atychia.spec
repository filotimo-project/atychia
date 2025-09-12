Name:          filotimo-atychia
Version:       0.17
Release:       1%{?dist}
License:       GPL-2.0-or-later
Summary:       A utility akin to Ctrl-Alt-Delete on Windows that allows a user to recover from a broken session, rather than being forced to a TTY.
URL:           https://github.com/filotimo-project/atychia
Source:        %{name}-%{version}.tar.gz

BuildRequires: extra-cmake-modules
BuildRequires: gcc-c++
BuildRequires: kf6-rpm-macros
BuildRequires: desktop-file-utils
BuildRequires: gettext

BuildRequires: cmake(Qt6Core)
BuildRequires: cmake(Qt6Quick)
BuildRequires: cmake(Qt6Gui)
BuildRequires: cmake(Qt6Qml)
BuildRequires: cmake(Qt6QuickControls2)
BuildRequires: cmake(Qt6DBus)
BuildRequires: cmake(Qt6Svg)
BuildRequires: cmake(Qt6Widgets)

BuildRequires: cmake(KF6Kirigami)
BuildRequires: cmake(KF6CoreAddons)
BuildRequires: cmake(KF6I18n)
BuildRequires: cmake(KF6KirigamiAddons)
BuildRequires: cmake(KF6Config)
BuildRequires: cmake(KF6ColorScheme)
BuildRequires: cmake(KF6Service)
BuildRequires: cmake(KF6KIO)

Requires: hicolor-icon-theme
Requires: qt6qml(org.kde.coreaddons)
Requires: qt6qml(org.kde.kirigami)
Requires: qt6qml(org.kde.kirigamiaddons.formcard)


%description
A utility akin to Ctrl-Alt-Delete on Windows that allows a user to recover from a broken session, rather than being forced to a TTY. This automatically registers a shortcut with KDE, and is designed for use with KDE. Registers Meta+Ctrl+Alt+Del by default. A shortcut is also registered to relaunch Plasmashell - Meta+Ctrl+Shift+B.

%prep
%setup -q

%build
%cmake_kf6
%cmake_build

%install
%cmake_install
# no translations yet
#%find_lang %{name} --with-man --with-qt --all-name

%check
desktop-file-validate %{buildroot}%{_kf6_datadir}/applications/org.filotimoproject.atychia.desktop

# -f %{name}.lang - no translations yet
%files
%license LICENSES/*
%{_kf6_bindir}/atychia
%{_kf6_bindir}/atychia-launch
%{_kf6_datadir}/applications/org.filotimoproject.atychia.desktop
%{_kf6_datadir}/kglobalaccel/org.filotimoproject.atychia.desktop
%{_kf6_datadir}/polkit-1/actions/org.filotimoproject.atychia-launch.policy

%changelog
* Fri Sep 12 2025 Thomas Duckworth <tduck@filotimoproject.org> 0.17-1
- make a bunch of improvements, make it use kwin instead of cage
  (tduck@filotimoproject.org)

* Sun Aug 31 2025 Thomas Duckworth <tduck@filotimoproject.org> 0.16-1
- chore: update readme (tduck@filotimoproject.org)
- fix: make logout work multi-user and more robustly
  (tduck@filotimoproject.org)
- fix: get i18n to work (tduck@filotimoproject.org)

* Mon Jul 14 2025 Thomas Duckworth <tduck@filotimoproject.org> 0.15-1
- feat: better i18n including desktop files (tduck@filotimoproject.org)

* Tue Apr 29 2025 Thomas Duckworth <tduck@filotimoproject.org> 0.14-1
- chore: bump (tduck@filotimoproject.org)
- refactor: Make Action representation robust (tduck@filotimoproject.org)
- chore: Update my email (tduck@filotimoproject.org)
- Automatic commit of package [filotimo-atychia] release [0.13-1].
  (tduck@filotimoproject.org)
- bump (tduck@filotimoproject.org)
- Update atychia-launch.in (tduck973564@gmail.com)

* Sun Mar 09 2025 Thomas Duckworth <tduck@filotimoproject.org> 0.13-1
- bump (tduck@filotimoproject.org)
- Set KDE_HOME_READONLY, don't create home folder (tduck@filotimoproject.org)

* Sun Mar 09 2025 Thomas Duckworth <tduck@filotimoproject.org> 0.12-1
- bump version (tduck@filotimoproject.org)
- add bottom margin to logo (tduck@filotimoproject.org)
- Make this more reliable (tduck@filotimoproject.org)
- Update README.md (tduck@filotimoproject.org)

* Sun Feb 02 2025 Thomas Duckworth <tduck@filotimoproject.org> 0.11-1
- return to sddm on log out (tduck@filotimoproject.org)

* Thu Dec 12 2024 Thomas Duckworth <tduck@filotimoproject.org> 0.10-1
- fix spec (tduck@filotimoproject.org)
- bump version (tduck@filotimoproject.org)

* Thu Dec 12 2024 Thomas Duckworth <tduck@filotimoproject.org> 0.9-1
- Automatic commit of package [filotimo-atychia] release [0.8-1].
  (tduck@filotimoproject.org)
- maybe fix spec (tduck@filotimoproject.org)
- Automatic commit of package [filotimo-atychia] release [0.8-1].
  (tduck@filotimoproject.org)

* Thu Dec 12 2024 Thomas Duckworth <tduck@filotimoproject.org> 0.8-1
- maybe fix spec (tduck@filotimoproject.org)
- tito (tduck@filotimoproject.org)

* Thu Dec 12 2024 Thomas Duckworth <tduck@filotimoproject.org> 0.7-1
- new package built with tito

