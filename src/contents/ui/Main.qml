// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.filotimoproject.atychia
import org.kde.kirigami as Kirigami

Kirigami.ApplicationWindow {
    id: root

    objectName: "mainWindow"
    onClosing: Actions.returnToPrevTTYAndQuit()
    pageStack.initialPage: page
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.None

    // Error handling - I don't know how bad of an idea this is, but if it works, it works
    Connections {
        function onErrorOccured(name, description) {
            errorDialog.title = name;
            errorDialog.subtitle = description;
            errorDialog.open();
        }

        target: Actions
    }

    Kirigami.PromptDialog {
        id: errorDialog

        standardButtons: Kirigami.Dialog.Ignore
        onAccepted: {
            errorDialog.title = "";
            errorDialog.subtitle = "";
            errorDialog.close();
        }
        customFooterActions: [
            Kirigami.Action {
                text: i18n("Return to desktop")
                icon.name: "desktop-symbolic"
                onTriggered: {
                    Actions.returnToPrevTTYAndQuit();
                }
            }
        ]
    }

    ListModel {
        id: actionsModel

        ListElement {
            actionName: "Return"
            name: "Return to desktop"
            description: "Exits Desktop Recovery and returns to the desktop."
            iconSource: "desktop-symbolic"
        }

        ListElement {
            // TODO: Make this terminal-emulator-agnostic
            actionName: "Launch"
            name: "Open a terminal window"
            description: "Opens a Konsole window that allows you to log in and execute commands."
            iconSource: "utilities-terminal-symbolic"
        }

        ListElement {
            actionName: "Log out"
            name: "Log out of current user"
            description: "Immediately logs out of current user. All unsaved work will be lost."
            iconSource: "system-log-out-symbolic"
        }

        ListElement {
            actionName: "Power off"
            name: "Power off computer"
            description: "Immediately powers off computer. All unsaved work will be lost."
            iconSource: "system-shutdown-symbolic"
        }

        ListElement {
            actionName: "Reboot"
            name: "Reboot computer"
            description: "Immediately reboots computer. All unsaved work will be lost."
            iconSource: "system-reboot-symbolic"
        }

        ListElement {
            actionName: "Reboot"
            name: "Reboot to Firmware Setup"
            description: "Immediately reboots to Firmware Setup. All unsaved work will be lost."
            iconSource: "preferences-system-symbolic"
        }

    }

    Component {
        id: actionsDelegate

        Kirigami.AbstractCard {
            function canDoAction() {
                switch (actionName) {
                case "Power off":
                    return Actions.canDoAction(Actions.CheckableAction.PowerOff);
                case "Reboot":
                    return Actions.canDoAction(Actions.CheckableAction.Reboot);
                case "Reboot to setup":
                    return Actions.canDoAction(Actions.CheckableAction.RebootToFirmwareSetup);
                default:
                    return true;
                }
            }

            visible: canDoAction()

            contentItem: Item {
                Layout.fillWidth: true
                implicitWidth: delegateLayout.implicitWidth
                implicitHeight: delegateLayout.implicitHeight

                GridLayout {
                    id: delegateLayout

                    rowSpacing: Kirigami.Units.largeSpacing
                    columnSpacing: Kirigami.Units.largeSpacing
                    columns: width > Kirigami.Units.gridUnit * 20 ? 4 : 2

                    anchors {
                        left: parent.left
                        top: parent.top
                        right: parent.right
                    }

                    Kirigami.Icon {
                        source: iconSource
                        Layout.fillHeight: true
                        Layout.maximumHeight: Kirigami.Units.iconSizes.huge
                        Layout.preferredWidth: height
                    }

                    ColumnLayout {
                        Kirigami.Heading {
                            text: i18n(name)
                            level: 2
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
                        function activate() {
                            switch (actionName) {
                            case "Return":
                                // TODO: This should be in the ListElement somehow
                                Actions.returnToPrevTTYAndQuit();
                                break;
                            case "Log out":
                                Actions.logout();
                                break;
                            case "Power off":
                                Actions.powerOff();
                                break;
                            case "Reboot":
                                Actions.reboot();
                                break;
                            case "Reboot to setup":
                                Actions.rebootToFirmwareSetup();
                                break;
                            case "Launch terminal":
                                Actions.launchKonsole();
                                break;
                            }
                        }

                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.columnSpan: 2
                        text: actionName
                        onClicked: activate()
                        Keys.onEnterPressed: activate()
                        Keys.onReturnPressed: activate()
                    }

                }

            }

        }

    }

    Kirigami.ScrollablePage {
        id: page

        Item {
            anchors.centerIn: parent
            width: contentColumn.implicitWidth
            height: contentColumn.implicitHeight

            ColumnLayout {
                id: contentColumn

                spacing: Kirigami.Units.largeSpacing * 2

                Kirigami.Heading {
                    text: i18n("For keyboard navigation, hold Alt and press the underlined letter on the button.")
                    level: 4
                }

                Repeater {
                    model: actionsModel
                    delegate: actionsDelegate
                }

            }

        }

    }

}