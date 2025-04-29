/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2025 Thomas Duckworth <tduck@filotimoproject.org>
 */

#pragma once

#include <QDBusInterface>
#include <QErrorMessage>
#include <QObject>

using namespace Qt::Literals::StringLiterals;

class ActionContext : public QObject
{
    Q_OBJECT
    Q_PROPERTY(uint32_t ttyNumber READ ttyNumber CONSTANT)
    Q_PROPERTY(uint32_t userUid READ userUid CONSTANT)
    Q_PROPERTY(uint32_t seatNumber READ seatNumber CONSTANT)

public:
    explicit ActionContext(QObject *parent = nullptr, uint32_t tty_number = 0, uint32_t user_uid = 0, uint32_t seat_number = 0)
        : QObject(parent)
        , m_ttyNumber(tty_number)
        , m_userUid(user_uid)
        , m_seatNumber(seat_number)
    {
    }

    uint32_t ttyNumber() const
    {
        return m_ttyNumber;
    }
    uint32_t userUid() const
    {
        return m_userUid;
    }
    uint32_t seatNumber() const
    {
        return m_seatNumber;
    }

private:
    uint32_t m_ttyNumber;
    uint32_t m_userUid;
    uint32_t m_seatNumber;
};

class Action : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString buttonText READ buttonText CONSTANT)
    Q_PROPERTY(QString name READ name CONSTANT)
    Q_PROPERTY(QString description READ description CONSTANT)
    Q_PROPERTY(QString iconSource READ iconSource CONSTANT)
    Q_PROPERTY(bool canExecute READ canExecute CONSTANT)

public:
    using QObject::QObject;

    Action(QObject *parent = nullptr, ActionContext *ctx = nullptr)
        : QObject(parent)
        , m_ctx(ctx)
    {
    }

    virtual QString buttonText() const = 0;
    virtual QString name() const = 0;
    virtual QString description() const = 0;
    virtual QString iconSource() const = 0;
    virtual bool canExecute() const = 0;

    Q_INVOKABLE virtual void execute() = 0;
    Q_SIGNAL void errorOccurred(QString name, QString description) const;

protected:
    ActionContext *m_ctx = nullptr;
};
