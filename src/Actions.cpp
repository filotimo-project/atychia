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
    QDBusInterface logind{u"org.freedesktop.login1"_s,
                          u"/org/freedesktop/login1/seat/seat%1"_s.arg(seatNumber),
                          u"org.freedesktop.login1.Seat"_s,
                          QDBusConnection::systemBus()};

    logind.callWithArgumentList(QDBus::Block, u"SwitchTo"_s, {ttyNum});

    QCoreApplication::exit(0);
}

void Actions::showErrorMessage(QString name, QString message) const
{
    QMessageBox messageBox;
    messageBox.critical(nullptr, name, message);
    messageBox.setFixedSize(400, 200);
    messageBox.show();
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

    QCoreApplication::exit(0);
}

void Actions::shutdown() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, u"CanPowerOff"_s, {});
    QDBusPendingReply<QString> canPowerOff = message;

    Q_ASSERT(canPowerOff.isFinished());
    if (canPowerOff.isError()) {
        const auto error = canPowerOff.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        showErrorMessage(error.name(), error.message());
    }

    if (canPowerOff.value() == u"yes"_s) {
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
        }

    } else {
        qCritical().noquote() << i18n("poweroff", u"Can't power off: CanPowerOff() result is %1"_s).arg(canPowerOff.value());
        showErrorMessage(i18n("poweroff"), i18n("Can't power off: CanPowerOff() result is %1", canPowerOff.value()));
    }
}

void Actions::reboot() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, u"CanReboot"_s, {});
    QDBusPendingReply<QString> canReboot = message;

    Q_ASSERT(canReboot.isFinished());
    if (canReboot.isError()) {
        const auto error = canReboot.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        showErrorMessage(error.name(), error.message());
    }

    if (canReboot.value() == u"yes"_s) {
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
        }

    } else {
        qCritical().noquote() << i18n("reboot", u"Can't reboot: CanReboot() result is %1"_s).arg(canReboot.value());
        showErrorMessage(i18n("reboot"), i18n("Can't reboot: CanReboot() result is %1", canReboot.value()));
    }
}

void Actions::rebootToFirmwareSetup() const
{
    QDBusInterface logind{u"org.freedesktop.login1"_s, u"/org/freedesktop/login1"_s, u"org.freedesktop.login1.Manager"_s, QDBusConnection::systemBus()};
    const auto message = logind.callWithArgumentList(QDBus::Block, u"CanRebootToFirmwareSetup"_s, {});
    QDBusPendingReply<QString> canReboot = message;

    Q_ASSERT(canReboot.isFinished());
    if (canReboot.isError()) {
        const auto error = canReboot.error();
        qWarning().noquote() << i18n("Asynchronous call finished with error: %1 (%2)").arg(error.name(), error.message());
        showErrorMessage(error.name(), error.message());
    }

    if (canReboot.value() == u"yes"_s) {
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
        }

    } else {
        qCritical().noquote() << i18n("reboot", u"Can't reboot to firmware setup: CanRebootToFirmwareSetup() result is %1"_s).arg(canReboot.value());
        showErrorMessage(i18n("reboot"), i18n("Can't reboot to firmware setup: CanRebootToFirmwareSetup() result is %1", canReboot.value()));
    }
}

void Actions::launchKonsole() const
{
    QStringList args{u"-e"_s, u"$SHELL -c %1/atychia-interactive-su"_s.arg(QString::fromUtf8(LIBEXECDIR))};
    QProcess process;
    process.startDetached(u"%1/konsole"_s.arg(QString::fromUtf8(BINDIR)), args);
}
