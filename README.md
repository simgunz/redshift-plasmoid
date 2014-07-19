Redshift Plasmoid
=================
Redshift Plasmoid provide a graphical configuration interface to redshift and it allows to
start/stop the redshift daemon. The program automatically retrieves the user coordinates
from the KDE settings, permitting redshift to determine the current position of the sun,
needed to perform the color regulation.

The program extends the original redshift with the following extra features:

* **KDE activities support** A different behavior of redshift can be set for different activities.
The possible behaviors are: Always On, Always Off or Auto. As an example redshift can be
automatically turned off when the user switches to the movie activity, so that the movie can
be seen without altering the original colors.

* **Manual color adjustment** Sometimes the color temperature set does not match the current
environmental light. It can happen when we are in a different location, where there are different
types of light, or when there is a gloomy day and the sunlight is not very bright. In these cases
it is possible to manually adjust the color temperature by spinning the mouse wheel over the widget.
The program will then switch to Manual Mode and won’t adjust the temperature automatically, but
will keep the current. To exit Manual Mode you just click on the widget.

Installation instructions
=========================
From redshift root directory execute:

    mkdir build
    cd build
    cmake -DCMAKE_INSTALL_PREFIX=$(kde4-config --prefix) ..
    make
    sudo make install

To start using the plasmoid reboot you machine or run the following command:

    redshiftautostart

Dependencies
------------
* KDE SC 4.x
* Redshift

Build dependecies
-----------------
* Essential build tools (gcc, cmake, and so on)

Required packages for Debian like distributions:
------------------------------------------------

* build-essentials
* cmake
* kde-workspace-dev
* redshift

Installation troubleshoots:
---------------------------
If make fails to build because it try to build it against Qt5, try:

    cmake -DQT_QMAKE_EXECUTABLE=/usr/bin/qmake-qt4 -DCMAKE_INSTALL_PREFIX=$(kde4-config --prefix) ..)

If your distribution doesn't use bash as default shell, try:

    su -c 'make install'

How to use
==========

Basic commands
--------------
* Click on the redshift widget to toggle the screen color regulation on/off
* Scroll the mouse wheel over the redshift widget to switch to Manual Mode and keep the screen
  color temperature constant throughout the day

Configuration pages
-------------------
* **General configuration page**
  The general redshift parameters can be set from this page. A tooltip explains each parameter.
* **Activities configuration page**
  In this page a different behavior of redshift can be set for each kde activity. The possible behaviors are:
  - **Auto** Redshift can be turned on/off
  - **Always on** Redshift is active and can't be turned off
  - **Always off** Redshift is inactive and can't be turned on
* **Shortcut configuration page**
  In this page it's possible to set a global keyboard shortcut to toggle redshift on/off.

Author
======
Simone Gaiarin \<simgunz AT gmail DOT com\>

License
=======
Software distributed under the GNU General Public License version 3 (GNU GPL v3).

http://www.gnu.org/copyleft/gpl.html

References
=============

* Redshift plasmoid:
  - [Official project page] (http://simgunz.org/projects/redshift-plasmoid/)
  - [Kde-apps page] (http://kde-apps.org/content/show.php/Redshift+Plasmoid?content=148737)
  - [Github page] (https://github.com/simgunz/redshift-plasmoid)

* Redshift:
  - [Official project page] (http://jonls.dk/redshift/)

