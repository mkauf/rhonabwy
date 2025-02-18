#
# Iddawc library
#
# Makefile used to build all programs
#
# Copyright 2020-2022 Nicolas Mora <mail@babelouest.org>
#
# This program is free software; you can redistribute it and/or
# modify it under the terms of the GNU Lesser General Public License
# as published by the Free Software Foundation;
# version 2.1 of the License.
#
# This library is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	See the
# GNU GENERAL PUBLIC LICENSE for more details.
#
# You should have received a copy of the GNU General Public
# License along with this library.	If not, see <http://www.gnu.org/licenses/>.
#

LIBIDDAWC_LOCATION=./src
TESTS_LOCATION=./test
RNBYC_LOCATION=./tools/rnbyc

all:
	cd $(LIBIDDAWC_LOCATION) && $(MAKE) $*
	cd $(RNBYC_LOCATION) && $(MAKE) $*

debug:
	cd $(LIBIDDAWC_LOCATION) && $(MAKE) debug $*
	cd $(RNBYC_LOCATION) && $(MAKE) debug $*

clean:
	cd $(LIBIDDAWC_LOCATION) && $(MAKE) clean
	cd $(TESTS_LOCATION) && $(MAKE) clean
	cd $(RNBYC_LOCATION) && $(MAKE) clean
	rm -rf doc/html $(TESTS_LOCATION)/cert/*.crt $(TESTS_LOCATION)/cert/*.key $(TESTS_LOCATION)/cert/*.log

install:
	cd $(LIBIDDAWC_LOCATION) && $(MAKE) install
	cd $(RNBYC_LOCATION) && $(MAKE) install

uninstall:
	cd $(LIBIDDAWC_LOCATION) && $(MAKE) uninstall
	cd $(RNBYC_LOCATION) && $(MAKE) uninstall

check:
	cd $(TESTS_LOCATION) && $(MAKE)

doxygen:
	doxygen doc/doxygen.cfg
