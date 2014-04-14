/*
 * Copyright (C) 2014  Daniel Vrátil <dvratil@redhat.com>
 *
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program; if not, write to the Free Software Foundation, Inc.,
 * 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
 *
 */

#include <QApplication>
#include <QQuickView>
#include <QIcon>
#include <QStandardPaths>
#include <QDebug>
#include <QQmlEngine>

#include <KLocalizedString>

#include <Plasma/Package>
#include <Plasma/PackageStructure>
#include <Plasma/Plasma>
#include <Plasma/PluginLoader>


void prependEnv(const char *env, const QByteArray &value)
{
    QByteArray values = qgetenv(env);
    if (!values.contains(value + ":")) {
        values = value + ":" + values;
    }
    qputenv(env, values);
}

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    app.setApplicationName(QLatin1String("fedora-plasma-first-login"));
    app.setApplicationVersion(QLatin1String(FPFL_VERSION));
    app.setOrganizationName(QLatin1String("Fedora Plasma"));
    app.setOrganizationDomain(QLatin1String("org.fedoraproject.kde"));

    // As long as we install KDE 5 into prefix, we need to override XDG_DATA_DIRS
    // so that we can use QStandardPaths to locate stuff from kde5-workspace.
    prependEnv("XDG_DATA_DIRS", "/opt/kde5/share");
    // FIXME: OUCH!
    prependEnv("QML2_IMPORT_PATH", "/usr/lib64/qml");
    prependEnv("QML2_IMPORT_PATH", "/opt/kde5/lib64/qml");
    // 'Coz my setup is cozy
    prependEnv("QML2_IMPORT_PATH",  "/opt/kde-devel/lib64/qml/");

    // Set QT_MESSAGE_PATTERN to be useful
    qputenv("QT_MESSAGE_PATTERN", "[%{type}] %{appname} (%{file}:%{line}) - %{message}");

    // Don't use default KDEHOME. We initialize various KDE5 stuff and it could
    // clash with KDE4 home.
    qputenv("KDEHOME", "$HOME/.kde5-firstlogin");

    Plasma::Package package = Plasma::PluginLoader::self()->loadPackage(QLatin1String("Plasma/Applet"));
    QString appletPath = QStandardPaths::locate(QStandardPaths::GenericDataLocation,
                                                QLatin1String("fedora-plasma-first-login/applet"),
                                                QStandardPaths::LocateDirectory);
    // For development purposes, does not require installing into /usr
    if (appletPath.isEmpty()) {
        appletPath = QLatin1String(CMAKE_SOURCE_DIR "/src/qml/applet");
    }
    package.setPath(appletPath);
    if (!package.isValid()) {
        qFatal("Failed to load Fedora Plasma First Login Applet");
    }
    qDebug() << "Found applet:" << package.metadata().pluginName() << package.defaultPackageRoot();

    QQuickView view;
    view.setIcon(QIcon::fromTheme(QLatin1String("start-here-kde-fedora")));
    view.setTitle(i18n("Fedora Plasma First Login Guide"));
    view.setResizeMode(QQuickView::SizeRootObjectToView);
    view.resize(800, 600);
    view.setSource(QUrl::fromLocalFile(package.filePath("ui", QLatin1String("main.qml"))));
    QObject::connect(view.engine(), &QQmlEngine::quit, &app, &QApplication::quit);
    view.show();

    return app.exec();
}
