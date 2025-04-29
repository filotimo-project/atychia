/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck@filotimoproject.org>
 */

#pragma once

#include <QDBusInterface>
#include <QErrorMessage>
#include <QObject>

using namespace Qt::Literals::StringLiterals;

class Actions : public QObject
{
    Q_OBJECT

    void returnToTTYNumberAndQuit(uint32_t ttyNum) const;
    void showErrorMessage(QString name, QString message) const;

public:
    explicit Actions(QObject *parent = nullptr, uint32_t tty_number = 0, uint32_t user_uid = 0, uint32_t seat_number = 0);

    enum class CheckableAction : int { RebootToFirmwareSetup, Reboot, PowerOff };
    Q_ENUM(CheckableAction)

    uint32_t ttyNumber;
    uint32_t userUid;
    uint32_t seatNumber;

    Q_SLOT bool canDoAction(Actions::CheckableAction action) const;

    Q_SLOT void returnToPrevTTYAndQuit() const;

    Q_SLOT void logout() const;

    Q_SLOT void powerOff() const;
    Q_SLOT void reboot() const;
    Q_SLOT void rebootToFirmwareSetup() const;

    Q_SLOT void launchKonsole() const;

    Q_SIGNAL void errorOccured(QString name, QString description) const;
};
