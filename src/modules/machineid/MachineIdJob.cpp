/* === This file is part of Calamares - <https://github.com/calamares> ===
 *
 *   Copyright 2014, Kevin Kofler <kevin.kofler@chello.at>
 *   Copyright 2016, Philip Müller <philm@manjaro.org>
 *   Copyright 2017, Alf Gaida <agaida@siduction.org>
 *   Copyright 2019, Adriaan de Groot <groot@kde.org>
 *
 *   Calamares is free software: you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation, either version 3 of the License, or
 *   (at your option) any later version.
 *
 *   Calamares is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with Calamares. If not, see <http://www.gnu.org/licenses/>.
 */

#include "MachineIdJob.h"

#include "utils/CalamaresUtilsSystem.h"
#include "utils/Logger.h"
#include "utils/Variant.h"

#include "GlobalStorage.h"
#include "JobQueue.h"

#include <QFile>

MachineIdJob::MachineIdJob( QObject* parent )
    : Calamares::CppJob( parent )
{
}


MachineIdJob::~MachineIdJob() {}


QString
MachineIdJob::prettyName() const
{
    return tr( "Generate machine-id." );
}

// might need to use a helper to remove the file
static void
removeFile( const QString& fileName )
{
    QFile::remove( fileName );
}

Calamares::JobResult
MachineIdJob::exec()
{
    QString root;

    Calamares::GlobalStorage* gs = Calamares::JobQueue::instance()->globalStorage();
    if ( gs && gs->contains( "rootMountPoint" ) )
    {
        root = gs->value( "rootMountPoint" ).toString();
    }
    else
    {
        cWarning() << "No *rootMountPoint* defined.";
        return Calamares::JobResult::internalError( tr( "Configuration Error" ),
                                                    tr( "No root mount point is set for MachineId." ),
                                                    Calamares::JobResult::InvalidConfiguration );
    }

    QString target_systemd_machineid_file = root + QStringLiteral( "/etc/machine-id" );
    QString target_dbus_machineid_file = root + QStringLiteral( "/var/lib/dbus/machine-id" );
    QString target_entropy_file = root + QStringLiteral( "/var/lib/urandom/random-seed" );

    // Clear existing files
    if ( m_entropy )
    {
        removeFile( target_entropy_file );
    }
    if ( m_dbus )
    {
        removeFile( target_dbus_machineid_file );
    }
    if ( m_systemd )
    {
        removeFile( target_systemd_machineid_file );
    }

    return Calamares::JobResult::ok();
}


void
MachineIdJob::setConfigurationMap( const QVariantMap& map )
{
    m_systemd = CalamaresUtils::getBool( map, "systemd", false );

    m_dbus = CalamaresUtils::getBool( map, "dbus", false );
    if ( map.contains( "dbus-symlink" ) )
    {
        m_dbus_symlink = CalamaresUtils::getBool( map, "dbus-symlink", false );
    }
    else if ( map.contains( "symlink" ) )
    {
        m_dbus_symlink = CalamaresUtils::getBool( map, "symlink", false );
        cWarning() << "MachineId: configuration setting *symlink* is deprecated, use *dbus-symlink*.";
    }
    // else it's still false from the constructor

    // ignore it, though, if dbus is false
    m_dbus_symlink = m_dbus && m_dbus_symlink;

    m_entropy = CalamaresUtils::getBool( map, "entropy", false );
    m_entropy_copy = CalamaresUtils::getBool( map, "entropy-copy", false );
}

CALAMARES_PLUGIN_FACTORY_DEFINITION( MachineIdJobFactory, registerPlugin< MachineIdJob >(); )
