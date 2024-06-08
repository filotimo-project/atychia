/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>
 */

#pragma once

#include <QObject>

using namespace Qt::Literals::StringLiterals;

class Actions : public QObject
{
    Q_OBJECT

    void returnToTTYNumber(QString ttyNum) const;

public:
    explicit Actions(QObject *parent = nullptr, QString tty_number = u""_s, QString user_uid = u""_s, QString session_id = u""_s);

    QString ttyNumber;
    QString userUid;
    QString sessionId;

    // "public slots:" doesn't work
    Q_SLOT void returnToTTYAndQuit() const;
    Q_SLOT void relaunchPlasma() const;
    Q_SLOT void logout() const;
    Q_SLOT void shutdown() const;
    Q_SLOT void reboot() const;
    Q_SLOT void rebootToFirmwareSetup() const;
};
