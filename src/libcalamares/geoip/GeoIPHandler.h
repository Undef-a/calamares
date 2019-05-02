/* === This file is part of Calamares - <http://github.com/calamares> ===
 *
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

#ifndef GEOIP_GEOIPHANDLER_H
#define GEOIP_GEOIPHANDLER_H

#include "GeoIP.h"

namespace CalamaresUtils
{

/** @brief Handle one complete GeoIP lookup.
 *
 * This class handles one complete GeoIP lookup. Create it with
 * suitable configuration values, then call lookup(). This is a
 * synchronous API and will return an invalid zone pair on
 * error or if the configuration is not understood/
 */
class GeoIPHandler
{
public:
    /** @brief An unconfigured handler; this always returns errors. */
    GeoIPHandler();

    GeoIP::RegionZonePair query() const;

    bool isValid() const;
};

}  // namespace
#endif

