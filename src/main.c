#ifdef HAVE_CONFIG_H
#  include <config.h>
#endif

#include <gtk/gtk.h>
#include "support.h"
#include "callbacks.h"
#ifdef MAEMO
#	include <hildon/hildon.h>
#	include <libosso.h>
#endif

int main (int argc, char *argv[]) {
#ifdef ENABLE_NLS
	bindtextdomain(GETTEXT_PACKAGE, PACKAGE_LOCALE_DIR);
	bind_textdomain_codeset(GETTEXT_PACKAGE, "UTF-8");
	textdomain(GETTEXT_PACKAGE);
#endif

	gtk_set_locale();
#ifdef MAEMO
	hildon_gtk_init(&argc, &argv);
#else
	gtk_init(&argc, &argv);
#endif

	gint i = 1, result;
	GError *gerror = NULL;
#ifdef MAEMO
	osso_context_t *osso_cont;
	HildonProgram *grsync_prog;
	GtkWidget *toolbar1;
	GtkWidget *appmenu;
#endif

	cmdline_session = FALSE;
	cmdline_execute = FALSE;
	cmdline_stayopen = FALSE;
	cmdline_import = FALSE;
	argv_filename = NULL;

	while (i < argc && argv[i] != NULL) {
		if (argv[i][0] == '-') {
			if (argv[i][1] == 'e') cmdline_execute = TRUE;
			if (argv[i][1] == 's') cmdline_stayopen = TRUE;
			if (argv[i][1] == 'i') cmdline_import = TRUE;
		} else {
			if (cmdline_import) argv_filename = g_strconcat(argv[i], NULL);
			else {
				argv_session = g_strconcat(argv[i], NULL);
				cmdline_session = TRUE;
			}
		}
		i++;
	}

	if (cmdline_execute && cmdline_import) {
		printf(_("Error: conflicting arguments\n"));
		return 22;
	}
	if (!cmdline_session) argv_session = g_strconcat("default", NULL);

	if (cmdline_import && argv_filename == NULL) {
		// disabled because of grsync.desktop Exec option
		/*
		printf(_("Error: missing filename\n"));
		return 2;
		*/
		cmdline_import = FALSE;
	}

	icon = g_file_test(ICON_SOURCE, G_FILE_TEST_EXISTS) ? ICON_SOURCE : ICON_PACKAGE;
	icon_busy = g_file_test(ICON_SOURCE_BUSY, G_FILE_TEST_EXISTS) ? ICON_SOURCE_BUSY : ICON_PACKAGE_BUSY;
	gtk_window_set_default_icon_from_file(icon, NULL);

	builder = gtk_builder_new();
	result = gtk_builder_add_from_file(builder, XMLFILE, &gerror) || gtk_builder_add_from_file(builder, PACKAGE_DATA_DIR "/" PACKAGE "/" XMLFILE, NULL);
	if (result) {
#ifdef MAEMO
		/* Initialize OSSO context */
		osso_cont = osso_initialize("it.opbyte." PACKAGE, VERSION, TRUE, NULL);
		if (osso_cont == NULL) {
			g_printerr("ERROR: Unable to properly initialize Maemo environment.\n");
			return OSSO_ERROR;
		}
		/* Create HildonProgram and add the HildonWindow to it */
		grsync_prog = HILDON_PROGRAM (hildon_program_get_instance ());
		main_window = (GtkWidget*) gtk_builder_get_object(builder, "main_window");
		hildon_program_add_window (grsync_prog, HILDON_WINDOW (main_window));
		/* Create HildonAppMenu and add it to the HildonWindow */
		appmenu = (GtkWidget*) gtk_builder_get_object(builder, "appmenu");
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "switch_source_with_destination1")));
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "commandline")));
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "preferences1")));
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "import1")));
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "export1")));
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "about1")));
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "rsync_info")));
		hildon_app_menu_append(HILDON_APP_MENU (appmenu),GTK_BUTTON (gtk_builder_get_object(builder, "contribute")));
		hildon_window_set_app_menu(HILDON_WINDOW (main_window),HILDON_APP_MENU (appmenu));
		gtk_widget_show_all(appmenu);
		/* Add the GtkToolbar to the HildonWindow */
		toolbar1 = (GtkWidget*) gtk_builder_get_object(builder, "toolbar1");
		gtk_container_remove(GTK_CONTAINER (gtk_builder_get_object(builder,"vbox7")),toolbar1);
		hildon_window_add_toolbar(HILDON_WINDOW (main_window),GTK_TOOLBAR (toolbar1));
		gtk_widget_show_all(toolbar1);
#else
		main_window = (GtkWidget*) gtk_builder_get_object(builder, "main_window");
#endif
		liststore_session = (GtkListStore*) gtk_builder_get_object(builder, "liststore_session");
		gtk_builder_connect_signals(builder, NULL);
		on_main_create((GtkWindow*) main_window, NULL);		// I run the callback now because the "show" signal seems not to be emitted with gtkbuilder
		// gtk_widget_show_all(main_window);		// disabled because it showed widgets hidden by gtk calls in the program; was needed on some systems, need to check again
		gtk_main();
	} else {
		g_printerr("Error loading GtkBuilder XML file: %s - error code %u (%s)\n", XMLFILE, gerror->code, gerror->message);
	}

	g_free(argv_session);
	g_object_unref(G_OBJECT(builder));
#ifdef MAEMO
	/* De-initialize OSSO context */
	osso_deinitialize(osso_cont);
#endif
	return 0;
}
