// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import org.filotimoproject.atychia

Kirigami.ApplicationWindow {
    id: root

    objectName: "mainWindow"
    pageStack.initialPage: page
    pageStack.globalToolBar.style: Kirigami.ApplicationHeaderStyle.None

    Keys.onEscapePressed: {
        Actions.returnToDesktop.execute();
    }

    // Error handling - I don't know how bad of an idea this is, but if it works, it works
    Connections {
        target: Actions

        function onErrorOccurred(name, description) {
            errorDialog.title = name;
            errorDialog.subtitle = description;
            errorDialog.open();
        }

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
                    Actions.returnToDesktop.execute();
                }
            }
        ]
    }

    Component {
        id: actionsDelegate

        Kirigami.AbstractCard {
            visible: canExecute

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
                            text: name
                            level: 2
                        }

                        Kirigami.Separator {
                            Layout.fillWidth: true
                        }

                        Controls.Label {
                            Layout.fillWidth: true
                            wrapMode: Text.WordWrap
                            text: description
                        }

                    }

                    Controls.Button {
                        Layout.alignment: Qt.AlignRight | Qt.AlignVCenter
                        Layout.columnSpan: 2
                        text: buttonText
                        onClicked: actionObject.execute()
                        Keys.onEnterPressed: actionObject.execute()
                        Keys.onReturnPressed: actionObject.execute()
                    }

                }

            }

        }

    }

    Kirigami.ScrollablePage {
        id: page

        background: Rectangle {
            color: "black"
        }

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
                    model: Actions
                    delegate: actionsDelegate
                }

            }

        }

        ColumnLayout {
            anchors.bottom: parent.bottom
            anchors.bottomMargin: Kirigami.Units.largeSpacing * 4

            Image {
                Layout.alignment: Qt.AlignBottom | Qt.AlignHCenter
                source: "file:///usr/share/plymouth/themes/spinner/watermark.png"
            }

        }

    }

}
