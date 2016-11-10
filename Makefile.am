## Process this file with automake to produce Makefile.in

SUBDIRS = src po
man_MANS = grsync.1 grsync-batch.1

EXTRA_DIST = \
	autogen.sh \
	grsync.glade \
	grsync-maemo-ui.xml \
	grsync.conf \
	grsync.desktop.in \
	grsync.xml.in \
	it.opbyte.grsync.service.in \
	grsync.spec.in \
	$(man_MANS) \
	intltool-extract.in \
	intltool-merge.in \
	intltool-update.in \
	src/grsync-batch \
	pixmaps \
	maemo-icons

distclean-local:
	rm -rf *.cache *~

am__tar = @am__tar@ --exclude=".svn"

if MAEMO
Applicationsdir = $(desktopentrydir)
Applications_DATA = grsync.desktop

Servicedir = $(dbusservicedir)
Service_DATA = it.opbyte.grsync.service

Backupdir = /etc/osso-backup/applications
Backup_DATA = grsync.conf

icon_26x26dir = $(datadir)/icons/hicolor/26x26/hildon
icon_26x26_DATA = maemo-icons/26x26/grsync.png

icon_40x40dir = $(datadir)/icons/hicolor/40x40/hildon
icon_40x40_DATA = maemo-icons/40x40/grsync.png

icon_48x48dir = $(datadir)/icons/hicolor/48x48/hildon
icon_48x48_DATA = maemo-icons/48x48/grsync.png

icon_64x64dir = $(datadir)/icons/hicolor/64x64/hildon
icon_64x64_DATA = maemo-icons/64x64/grsync.png

icon_scalabledir = $(datadir)/icons/hicolor/scalable/hildon
icon_scalable_DATA = maemo-icons/scalable/grsync.png
else
Applicationsdir = $(datadir)/applications
Applications_DATA = grsync.desktop
endif

Gladedir = $(datadir)/grsync
Glade_DATA = $(XMLFILE)

mimedir = $(datadir)/mime
mimepackagesdir = $(mimedir)/packages
dist_mimepackages_DATA = grsync.xml

pixmapdir = $(datadir)/pixmaps
pixmap_DATA = pixmaps/grsync.png

pixmapbusydir = $(datadir)/pixmaps
pixmapbusy_DATA = pixmaps/grsync-busy.png

mimeicondir = $(datadir)/icons/hicolor/48x48/mimetypes/
mimeicon_DATA = pixmaps/application-x-grsync-session.png
