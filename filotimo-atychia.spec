Name:          filotimo-atychia
Version:       0.7
Release:       1%{?dist}
License:       GPL-2.0-or-later
Summary:       A utility akin to Ctrl-Alt-Delete on Windows that allows a user to recover from a broken session, rather than being forced to a TTY.
URL:           https://github.com/filotimo-project/atychia
Source:        https://github.com/filotimo-project/atychia/archive/refs/tags/%{name}-%{version}-1.tar.gz

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

Requires: hicolor-icon-theme
Requires: cage
Requires: qt6qml(org.kde.coreaddons)
Requires: qt6qml(org.kde.kirigami)
Requires: qt6qml(org.kde.kirigamiaddons.formcard)


%description
A utility akin to Ctrl-Alt-Delete on Windows that allows a user to recover from a broken session, rather than being forced to a TTY. This automatically registers a shortcut with KDE, and is designed for use with KDE. Registers Meta+Ctrl+Alt+Del by default. A shortcut is also registered to relaunch Plasmashell - Meta+Ctrl+Shift+B.

%prep
%autosetup -p1 -n atychia-filotimo-atychia-%{version}-1

%build
%cmake_kf6
%cmake_build

%install
%cmake_install
# no translations yet
#%find_lang %{name} --with-man --with-qt --all-name

%check
desktop-file-validate %{buildroot}%{_kf6_datadir}/applications/org.filotimoproject.atychia.desktop
desktop-file-validate %{buildroot}%{_kf6_datadir}/applications/org.filotimoproject.atychia-restart-plasmashell.desktop

%files
#-f %{name}.lang
%license LICENSES/*
%{_kf6_bindir}/atychia
%{_kf6_bindir}/atychia-launch
%{_kf6_bindir}/atychia-restart-plasmashell
%{_kf6_datadir}/applications/org.filotimoproject.atychia.desktop
%{_kf6_datadir}/applications/org.filotimoproject.atychia-restart-plasmashell.desktop
%{_kf6_datadir}/kglobalaccel/org.filotimoproject.atychia.desktop
%{_kf6_datadir}/kglobalaccel/org.filotimoproject.atychia-restart-plasmashell.desktop
%{_kf6_datadir}/polkit-1/actions/org.filotimoproject.atychia-launch.policy
%{_kf6_datadir}/polkit-1/rules.d/99-filotimo-atychia.rules

%changelog
* Thu Dec 12 2024 Thomas Duckworth <tduck973564@gmail.com> 0.7-1
- new package built with tito

