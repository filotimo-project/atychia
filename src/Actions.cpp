/*
 *  SPDX-License-Identifier: GPL-2.0-or-later
 *  SPDX-FileCopyrightText: 2024 Thomas Duckworth <tduck973564@gmail.com>
 */

#include "Actions.h"
#include "directories.h"

#include <KLocalizedString>
#include <QCoreApplication>
#include <QDBusInterface>
#include <QDBusPendingReply>
#include <QMessageBox>
#include <QProcess>
#include <QThread>
#include <csignal>
#include <pwd.h>
#include <unistd.h>

using namespace Qt::Literals::StringLiterals;

Actions::Actions(QObject *parent, uint32_t tty_number, uint32_t user_uid, uint32_t seat_number)
    : QObject(parent)
{
    ttyNumber = tty_number;
    userUid = user_uid;
    seatNumber = seat_number;
}

void Actions::returnToTTYNumberAndQuit(uint32_t ttyNum) const
{
    QDBusInterface seatd{u"org.freedesktop.login1"_s,
                         u"/org/freedesktop/login1/seat/seat%1"_s.arg(seatNumber),
                         u"org.freedesktop.login1.Seat"_s,
                         QDBusConnection::systemBus()};

    seatd.callWithArgumentList(QDBus::Block, u"SwitchTo"_s, {ttyNum});

    QCoreApplication::exit(0);
}

void Actions::showErrorMessage(QString name, QString message) const
{
    Q_EMIT errorOccured(name, message);
}

bool Actions::canDoAction(CheckableAction action) const
{
    QString actionText;
    switch (action) {
    case CheckableAction::RebootToFirmwareSetup:
        actionText = u"CanRebootToFirmwareSetup"_s;
        break;
    case CheckableAction::Reboot:
        actionText = u"CanReboot"_s;
        break;
    case CheckableAction::PowerOff:
        actionText = u"CanPowerOff"_s;
        break;
    }

    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, actionText, {});
    QDBusPendingReply<QString> canDoAction = message;

    Q_ASSERT(canDoAction.isFinished());
    if (canDoAction.isError()) {
        const auto error = canDoAction.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        showErrorMessage(error.name(), error.message());
        return false;
    }

    if (canDoAction.value() == u"yes"_s) {
        return true;
    } else {
        return false;
    }
    return false;
}

void Actions::returnToPrevTTYAndQuit() const
{
    returnToTTYNumberAndQuit(ttyNumber);
}

void Actions::logout() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block,
                                                     u"KillUser"_s,
                                                     {
                                                         userUid,
                                                         SIGTERM,
                                                     });

    QDBusPendingReply<QString> killedUser = message;

    Q_ASSERT(killedUser.isFinished());
    if (killedUser.isError()) {
        const auto error = killedUser.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        showErrorMessage(error.name(), error.message());
    }

    returnToTTYNumberAndQuit(1);
}

void Actions::powerOff() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};

    if (canDoAction(CheckableAction::PowerOff)) {
        QDBusPendingReply<> powerOff = logind.callWithArgumentList(QDBus::Block,
                                                                   u"PowerOff"_s,
                                                                   {
                                                                       true,
                                                                   });

        Q_ASSERT(powerOff.isFinished());
        if (powerOff.isError()) {
            const auto error = powerOff.error();
            qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
            showErrorMessage(error.name(), error.message());
            return;
        }

        QCoreApplication::exit(0);

    } else {
        qCritical().noquote() << i18n("poweroff", u"Can't power off: canDoAction(PowerOff) result is false"_s);
        showErrorMessage(i18n("poweroff"), i18n("Can't power off: canDoAction(PowerOff) result is false"));
    }
}

void Actions::reboot() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};

    if (canDoAction(CheckableAction::Reboot)) {
        QDBusPendingReply<> reboot = logind.callWithArgumentList(QDBus::Block,
                                                                 u"Reboot"_s,
                                                                 {
                                                                     true,
                                                                 });
        Q_ASSERT(reboot.isFinished());
        if (reboot.isError()) {
            const auto error = reboot.error();
            qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
            showErrorMessage(error.name(), error.message());
            return;
        }

        QCoreApplication::exit(0);

    } else {
        qCritical().noquote() << i18n("reboot", u"Can't reboot: canDoAction(Reboot) result is false"_s);
        showErrorMessage(i18n("reboot"), i18n("Can't reboot: canDoAction(Reboot) result is false"));
    }
}

void Actions::rebootToFirmwareSetup() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};

    if (canDoAction(CheckableAction::RebootToFirmwareSetup)) {
        const auto message = logind.callWithArgumentList(QDBus::Block,
                                                         u"SetRebootToFirmwareSetup"_s,
                                                         {
                                                             true,
                                                         });
        QDBusPendingReply<QString> setRebootToFirmwareSetup = message;

        Q_ASSERT(setRebootToFirmwareSetup.isFinished());
        if (setRebootToFirmwareSetup.isError()) {
            const auto error = setRebootToFirmwareSetup.error();
            qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
            showErrorMessage(error.name(), error.message());
            return;
        }

        QDBusPendingReply<> reboot = logind.callWithArgumentList(QDBus::Block,
                                                                 u"Reboot"_s,
                                                                 {
                                                                     true,
                                                                 });
        Q_ASSERT(reboot.isFinished());
        if (reboot.isError()) {
            const auto error = reboot.error();
            qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
            showErrorMessage(error.name(), error.message());
            return;
        }

        QCoreApplication::exit(0);

    } else {
        qCritical().noquote() << i18n("reboot", u"Can't reboot to firmware setup: canDoAction(RebootToFirmwareSetup) result is false"_s);
        showErrorMessage(i18n("reboot"), i18n("Can't reboot to firmware setup: canDoAction(RebootToFirmwareSetup) result is false"));
    }
}

void Actions::launchKonsole() const
{
    struct passwd *pwd = getpwuid(userUid);
    QStringList args{
        u"-e"_s,
        u"/%1/su"_s.arg(QString::fromUtf8(BINDIR)),
        u"-"_s,
        u"%1"_s.arg(QString::fromUtf8(pwd->pw_name)),
    };
    QProcess process;
    process.startDetached(u"/%1/konsole"_s.arg(QString::fromUtf8(BINDIR)), args);
}
