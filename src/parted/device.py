#
# device.py
# Python bindings for libparted (built on top of the _ped Python module).
#
# Copyright (C) 2009  Red Hat, Inc.
#
# This copyrighted material is made available to anyone wishing to use,
# modify, copy, or redistribute it subject to the terms and conditions of
# the GNU General Public License v.2, or (at your option) any later version.
# This program is distributed in the hope that it will be useful, but WITHOUT
# ANY WARRANTY expressed or implied, including the implied warranties of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General
# Public License for more details.  You should have received a copy of the
# GNU General Public License along with this program; if not, write to the
# Free Software Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
# 02110-1301, USA.  Any Red Hat trademarks that are incorporated in the
# source code or documentation are not subject to the GNU General Public
# License and may only be used or replicated with the express permission of
# Red Hat, Inc.
#
# Red Hat Author(s): David Cantrell <dcantrell@redhat.com>
#

import math
import _ped
import parted

class Device(object):
    """Device()

       Device represents a phyiscal piece of hardware in the system, e.g. a
       disk.  A Device should be considered a low-level and operating system
       specific interface to the hardware.

       A number of read-only properties about the Device are available.

       For information on the individual methods, see help(Device.METHODNAME)"""

    def __init__(self, path=None, PedDevice=None):
        """Create a new Device object based on the specified path or the
           already existing _ped.Device object.  You must provide either a
           path (e.g., "/dev/sda") or an existing _ped.Device object, but
           not both."""

        if PedDevice:
            self.__device = PedDevice
        elif path is not None:
            self.__device = _ped.device_get(path)
        else:
            raise parted.DeviceException, "no path or PedDevice specified"

    def __eq__(self, other):
        return not self.__ne__(other)

    def __ne__(self, other):
        if hash(self) == hash(other):
            return False

        if type(self) != type(other):
            return True

        return self.model != other.model or self.path != other.path or self.type != other.type or self.length != other.length

    def __getCHS(self, geometry):
        return (geometry.cylinders, geometry.heads, geometry.sectors)

    @property
    def model(self):
        """Model name and vendor of this device."""
        return self.__device.model

    @property
    def path(self):
        """Filesystem node path of this device (e.g., /dev/sda)."""
        return self.__device.path

    @property
    def type(self):
        """Type of this device.  An integer constant corresponding
           to one of the parted.DEVICE_* values.
        """
        return self.__device.type

    @property
    def sectorSize(self):
        """Sector size (in bytes) for this device."""
        return self.__device.sector_size

    @property
    def physicalSectorSize(self):
        """Physical sector size (in bytes) for this device.  Not always
           the same as sectorSize, but is a multiple of sectorSize.
        """
        return self.__device.phys_sector_size

    @property
    def length(self):
        """The size of this device in sectors."""
        return self.__device.length

    @property
    def openCount(self):
        """How many times the open() method has been called on this device."""
        return self.__device.open_count

    @property
    def readOnly(self):
        """True if the device is currently in read-only mode, False
           otherwise.
        """
        return bool(self.__device.read_only)

    @property
    def externalMode(self):
        """True if external access mode is currently activated on this
           device, False otherwise.  External access mode has to be used
           if you want to use an external command on the device while
           you are currently using it in pyparted.
        """
        return bool(self.__device.external_mode)

    @property
    def dirty(self):
        """True if the device is marked dirty, False otherwise."""
        return bool(self.__device.dirty)

    @property
    def bootDirty(self):
        """True if the device is marked boot dirty, False otherwise."""
        return bool(self.__device.boot_dirty)

    @property
    def host(self):
        """The host value of this device."""
        return self.__device.host

    @property
    def did(self):
        """The did value of this device."""
        return self.__device.did

    @property
    def busy(self):
        """True if this device is busy, False otherwise."""
        return bool(self.__device.busy)

    @property
    def hardwareGeometry(self):
        """A 3-tuple representing the hardware geometry of this device.
           The tuple is in order of cylinders, heads, and sectors.
        """
        return self.__getCHS(self.__device.hw_geom)

    @property
    def biosGeometry(self):
        """A 3-tuple representing the BIOS geometry of this device.
           The tuple is in order of cylinders, heads, and sectors.
        """
        return self.__getCHS(self.__device.bios_geom)

    def __str__(self):
        s = ("parted.Device instance --\n"
             "  model: %(model)s  path: %(path)s  type: %(type)s\n"
             "  sectorSize: %(sectorSize)s  physicalSectorSize:  %(physSectorSize)s\n"
             "  length: %(length)s  openCount: %(openCount)s  readOnly: %(readOnly)s\n"
             "  externalMode: %(external)s  dirty: %(dirty)s  bootDirty: %(bootDirty)s\n"
             "  host: %(host)s  did: %(did)s  busy: %(busy)s\n"
             "  hardwareGeometry: %(hardwareGeom)s  biosGeometry: %(biosGeom)s\n"
             "  PedDevice: %(ped)r" %
             {"model": self.model, "path": self.path, "type": self.type,
              "sectorSize": self.sectorSize, "physSectorSize": self.physicalSectorSize,
              "length": self.length, "openCount": self.openCount, "readOnly": self.readOnly,
              "external": self.externalMode, "dirty": self.dirty, "bootDirty": self.bootDirty,
              "host": self.host, "did": self.did, "busy": self.busy,
              "hardwareGeom": self.hardwareGeometry, "biosGeom": self.biosGeometry,
              "ped": self.__device})
        return s

    def clobber(self, type=None):
        """Remove all identifying signatures of the partition table.  If type
           is not None, remove all identifying signatures of the partition
           table, except for partition tables of that type.  type must be a
           string matching a valid key in the diskType hash."""
        if type is None:
            return self.__device.clobber()
        else:
            return self.__device.clobber_exclude(diskType[type])

    def open(self):
        """Open this Device for read operations."""

        return self.__device.open()

    def close(self):
        """Close this Device.  Used after open() method calls."""

        return self.__device.close()

    def destroy(self):
        """Destroy this Device.  Operating system specific."""

        return self.__device.destroy()

    def removeFromCache(self):
        """Remove this Device from the internal libparted device cache."""

        return self.__device.cache_remove()

    def beginExternalAccess(self):
        """Set up the Device for use by an external program.  Call this method
           before running an external program that uses the Device."""

        return self.__device.begin_external_access()

    def endExternalAccess(self):
        """Turn off external access mode for the Device.  Call this method once
           your external program has finished using the Device."""

        return self.__device.end_external_access()

    def read(self, start, count):
        """From the sector indentified by start, read and return count sectors
           from the Device."""

        return self.__device.read(start, count)

    def write(self, buffer, start, count):
        """From the sector identified by start, write count sectors from
           buffer to the Device."""

        return self.__device.write(buffer, start, count)

    def sync(self, fast=False):
        """Perform a operating-system specific sync(2) operation on the
           Device.  If fast is True, try to perform a fast sync(2)."""

        if fast:
            return self.__device.sync_fast()
        else:
            return self.__device.sync()

    def check(self, start, count):
        """From the sector identified by start, perform an operating
           system specific check on count sectors."""
        return self.__device.check(start, count)

    def startSectorToCylinder(self, sector):
        """Return the closest cylinder (round down) to sector on
           this Device."""
        (cylinders, heads, sectors) = self.biosGeometry
        return long(math.floor((float(sector) / (heads * sectors)) + 1))

    def endSectorToCylinder(self, sector):
        """Return the closest cylinder (round up) to sector on
           this Device."""
        (cylinders, heads, sectors) = self.biosGeometry
        return long(math.ceil(float((sector + 1)) / (heads * sectors)))

    def startCylinderToSector(self, cylinder):
        """Return the sector corresponding to cylinder as a
           starting cylinder on this Device."""
        (cylinders, heads, sectors) = self.biosGeometry
        return long((cylinder - 1) * (heads * sectors))

    def endCylinderToSector(self, cylinder):
        """Return the sector corresponding to cylinder as an
           ending cylinder on this Device."""
        (cylinders, heads, sectors) = self.biosGeometry
        return long(((cylinder) * (heads * sectors)) - 1)

    def getSize(self, unit="MB"):
        """Return the size of the Device in the unit specified.  The unit
           is given as a string corresponding to one of the following
           abbreviations:  b (bytes), KB (kilobytes), MB (megabytes), GB
           (gigabytes), TB (terabytes).  An invalid unit string will raise a
           SyntaxError exception.  The default unit is MB."""
        lunit = unit.lower()

        if lunit not in parted._exponent.keys():
            raise SyntaxError, "invalid unit %s given" % (unit,)

        (cylinders, heads, sectors) = self.biosGeometry
        size = float(heads * cylinders * sectors)
        size /= math.pow(1024.0, parted._exponent[lunit])
        size *= self.physicalSectorSize

        return size

    def getConstraint(self):
        """Return a Constraint defining the limitations imposed by
           this Device."""
        return parted.Constraint(PedConstraint=self.__device.get_constraint())

    def getPedDevice(self):
        """Return the _ped.Device object contained in this Device.
           For internal module use only."""
        return self.__device
