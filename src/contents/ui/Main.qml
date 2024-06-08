// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.filotimo.atychia

Kirigami.ApplicationWindow {
    id: root

    minimumWidth: Kirigami.Units.gridUnit * 20
    minimumHeight: Kirigami.Units.gridUnit * 20

    onClosing: Actions.returnToTTYAndQuit();

    globalDrawer: Kirigami.GlobalDrawer {
        isMenu: !Kirigami.Settings.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("About Desktop Recovery")
                icon.name: "help-about"
                onTriggered: root.pageStack.pushDialogLayer(Qt.resolvedUrl("About.qml"))
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                shortcut: StandardKey.Quit
                onTriggered: Actions.returnToTTYAndQuit()
            }
        ]
    }

    ListModel {
        id: actionsModel

        ListElement {
            actionName: "Return"
            name: "Return to desktop"
            description: "Exits Desktop Recovery and returns to the desktop."
            iconSource: "plasmashell"
        }
        ListElement {
            actionName: "Log out"
            name: "Log out of current user"
            description: "Immediately logs out of current user. All unsaved work will be lost."
            iconSource: "system-log-out"
        }
        ListElement {
            actionName: "Shut down"
            name: "Shutdown computer"
            description: "Immediately shuts down computer. All unsaved work will be lost."
            iconSource: "system-shutdown"
        }
        ListElement {
            actionName: "Reboot"
            name: "Reboot computer"
            description: "Immediately reboots computer. All unsaved work will be lost."
            iconSource: "system-reboot"
        }
        ListElement {
            actionName: "Reboot to setup"
            name: "Reboot to Firmware Setup"
            description: "Immediately reboots to Firmware Setup. All unsaved work will be lost."
            iconSource: "cpu"
        }
        ListElement {
            // TODO: Make this terminal-emulator-agnostic
            actionName: "Launch terminal"
            name: "Open a terminal window"
            description: "Opens a Konsole window that allows you to log in and execute commands."
            iconSource: "utilities-terminal"
        }
    }

    Component {
        id: actionsDelegate
        Kirigami.AbstractCard {
            contentItem: Item {
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight
                GridLayout {
                    id: delegateLayout
                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }
                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2
                    Kirigami.Icon {
                        source: iconSource
                        Layout.fillHeight: true
                        Layout.maximumHeight: Kirigami.Units.iconSizes.huge
                        Layout.preferredWidth: height
                    }
                    ColumnLayout {
                        Kirigami.Heading {
                            level: 2
                            text: i18n(name)
                        }
                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }
                        Controls.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: i18n(description)
                        }
                    }
                    Controls.Button {
                        Layout.alignment: Qt.AlignRight|Qt.AlignVCenter
                        Layout.columnSpan: 2
                        text: actionName
                        onClicked: switch (actionName) { // TODO: This should be in the ListElement somehow
                            case "Return": Actions.returnToTTYAndQuit(); break;
                            case "Log out": Actions.logout(); break;
                            case "Shut down": Actions.shutdown(); break;
                            case "Reboot": Actions.reboot(); break;
                            case "Reboot to setup": Actions.rebootToFirmwareSetup(); break;
                            case "Launch terminal": Actions.launchKonsole(); break;
                        }
                    }
                }
            }
        }
    }

    pageStack.initialPage: page

    Kirigami.ScrollablePage {
        id: page

        title: i18n("Desktop Recovery")

        Kirigami.CardsListView {
            keyNavigationEnabled: true
            id: listView
            model: actionsModel
            delegate: actionsDelegate

            header: Kirigami.InlineViewHeader {
                width: listView.width
                text: i18n("Hold Alt and press underlined letters on buttons with the keyboard for keyboard navigation.")
            }
        }
    }
}
