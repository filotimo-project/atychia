// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami
import org.filotimoproject.atychia


Kirigami.ApplicationWindow {
    id: root
    objectName: "mainWindow"

    minimumWidth: Kirigami.Units.gridUnit * 35
    minimumHeight: Kirigami.Units.gridUnit * 35

    // Error handling - I don't know how bad of an idea this is, but if it works, it works
    Connections {
        target: Actions
        function onErrorOccured(name, description) {
            errorDialog.title = name;
            errorDialog.subtitle = description;
            errorDialog.open()
        }
    }
    Kirigami.PromptDialog {
        id: errorDialog
        standardButtons: Kirigami.Dialog.Ok

        onAccepted: {
            errorDialog.title = ""
            errorDialog.subtitle = ""inu
            errorDialog.close()
        }
    }

    onClosing: Actions.returnToPrevTTYAndQuit();

    globalDrawer: Kirigami.GlobalDrawer {
        isMenu: !Kirigami.Settings.isMobile
        actions: [
            Kirigami.Action {
                text: i18n("About Desktop Recovery")
                icon.name: "help-about"
                onTriggered: root.pageStack.pushDialogLayer(Qt.resolvedUrl("About.qml"))
                Keys.onEnterPressed: root.pageStack.pushDialogLayer(Qt.resolvedUrl("About.qml"))
                Keys.onReturnPressed: root.pageStack.pushDialogLayer(Qt.resolvedUrl("About.qml"))
            },
            Kirigami.Action {
                text: i18n("Quit")
                icon.name: "application-exit"
                shortcut: StandardKey.Quit
                onTriggered: Actions.returnToPrevTTYAndQuit()
                Keys.onEnterPressed: Actions.returnToPrevTTYAndQuit()
                Keys.onReturnPressed: Actions.returnToPrevTTYAndQuit()
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
            // TODO: Make this terminal-emulator-agnostic
            actionName: "Launch terminal"
            name: "Open a terminal window"
            description: "Opens a Konsole window that allows you to log in and execute commands."
            iconSource: "utilities-terminal"
        }
        ListElement {
            actionName: "Log out"
            name: "Log out of current user"
            description: "Immediately logs out of current user. All unsaved work will be lost."
            iconSource: "system-log-out"
        }
        ListElement {
            actionName: "Power off"
            name: "Power off computer"
            description: "Immediately powers off computer. All unsaved work will be lost."
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
    }

    Component {
        id: actionsDelegate
        Kirigami.AbstractCard {
            function canDoAction() {
                switch (actionName) {
                    case "Power off": return Actions.canDoAction(Actions.CheckableAction.PowerOff);
                    case "Reboot": return Actions.canDoAction(Actions.CheckableAction.Reboot);
                    case "Reboot to setup": return Actions.canDoAction(Actions.CheckableAction.RebootToFirmwareSetup);
                    default: return true;
                }
            }
            visible: canDoAction()

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

                        function activate() {
                            switch (actionName) { // TODO: This should be in the ListElement somehow
                                case "Return": Actions.returnToPrevTTYAndQuit(); break;
                                case "Log out": Actions.logout(); break;
                                case "Power off": Actions.powerOff(); break;
                                case "Reboot": Actions.reboot(); break;
                                case "Reboot to setup": Actions.rebootToFirmwareSetup(); break;
                                case "Launch terminal": Actions.launchKonsole(); break;
                            }
                        }

                        onClicked: activate()
                        Keys.onEnterPressed: activate()
                        Keys.onReturnPressed: activate()
                    }
                }
            }
        }
    }

    pageStack.initialPage: page

    Kirigami.ScrollablePage {
        id: page

        title: i18n("Desktop Recovery")

        header: Kirigami.InlineViewHeader {
            width: listView.width
            text: i18n("Hold Alt and press underlined letters on buttons with the keyboard for keyboard navigation.")
        }

        Kirigami.CardsListView {
            keyNavigationEnabled: true
            id: listView
            model: actionsModel
            delegate: actionsDelegate
        }
    }
}
