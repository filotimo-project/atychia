/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>
 */

#pragma once

#include <QErrorMessage>
#include <QObject>

using namespace Qt::Literals::StringLiterals;

class Actions : public QObject
{
    Q_OBJECT

    void returnToTTYNumber(uint32_t ttyNum) const;
    void showErrorMessage(QString name, QString message) const;

public:
    explicit Actions(QObject *parent = nullptr, uint32_t tty_number = 0, uint32_t user_uid = 0, uint32_t seat_number = 0);

    uint32_t ttyNumber;
    uint32_t userUid;
    uint32_t seatNumber;

    // "public slots:" doesn't work
    Q_SLOT void returnToTTYAndQuit() const;
    Q_SLOT void logout() const;
    Q_SLOT void shutdown() const;
    Q_SLOT void reboot() const;
    Q_SLOT void rebootToFirmwareSetup() const;
    Q_SLOT void launchKonsole() const;
};
