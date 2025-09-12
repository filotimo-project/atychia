// SPDX-License-Identifier: GPL-2.0-or-later
// SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>

import QtQuick
import QtQuick.Controls as Controls
import QtQuick.Layouts
import org.kde.kirigami as Kirigami

import org.filotimoproject.atychia

Kirigami.ApplicationWindow {
    id: root

    visibility: "FullScreen"
    flags: Qt.FramelessWindowHint
         | Qt.WindowStaysOnBottomHint


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

        ColumnLayout {
            anchors.fill: parent
            anchors.margins: Kirigami.Units.largeSpacing * 2

            ColumnLayout {
                id: contentColumn

                Layout.fillWidth: false
                Layout.fillHeight: false
                Layout.alignment: Qt.AlignHCenter | Qt.AlignVCenter

                width: contentColumn.implicitWidth
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

            Image {
                id: watermarkImage
                Layout.bottomMargin: Kirigami.Units.largeSpacing * 2
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom

                source: "file:///usr/share/plymouth/themes/spinner/watermark.png"
                fillMode: Image.PreserveAspectFit

                property real contentBottom: contentColumn.y + contentColumn.height
                property int availableHeight: page.height - contentBottom - anchors.bottomMargin

                // Set the image's height to be the smaller of its natural
                // size or the calculated available space.
                height: Math.max(32, Math.min(sourceSize.height, availableHeight))
            }
        }
    }
}
