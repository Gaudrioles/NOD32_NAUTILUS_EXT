#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib-2.0/glib.h>

#include <string.h>
#include <gio/gio.h>
#include <nautilus-extension.h>
#include <locale.h>
#include "nautilus-nod32.h"


static GObjectClass *parent_class;


static void scan_to_callback (NautilusMenuItem *item, gpointer user_data)
{
	GList *files, *scan;
	NautilusFileInfo *file;
	GString *cmd;
	
	files = g_object_get_data (G_OBJECT (item), "files");
	file = files->data;

	for (scan = files; scan; scan = scan->next)
	{
		NautilusFileInfo *file = scan->data;
		char *uri;

		uri = g_file_get_path(nautilus_file_info_get_location(file));

		cmd = g_string_new ("sh");
		g_string_append_printf (cmd, " /opt/eset/esets/lib/esets_scan.sh %s", g_shell_quote (uri));
		g_free (uri);
	}
	
	g_spawn_command_line_async (cmd->str, NULL);

	g_string_free (cmd, TRUE);
	
	return;
}

static GList *nautilus_nod32_get_file_items (NautilusMenuProvider *provider, GtkWidget *window, GList *files)
{
	GList    *items = NULL;
	GList    *scan;

    NautilusMenuItem *item;

	if (files == NULL)
		return NULL;

	for (scan = files; scan; scan = scan->next) 
    {
		NautilusFileInfo *file = NAUTILUS_FILE_INFO (scan->data);
		gchar *name;

		name = nautilus_file_info_get_name (file);
		
		g_free (name);
	}
    
    item = nautilus_menu_item_new ("Eset NOD32", "Analyser avec ESET NOD32 Antivirus", "ESET NOD32", "drive-harddisk");

	g_signal_connect (item, "activate", G_CALLBACK (scan_to_callback), provider);

	g_object_set_data_full (G_OBJECT (item), "files", nautilus_file_info_list_copy (files), (GDestroyNotify) nautilus_file_info_list_free);

	items = g_list_append (items, item);

	return items;
}


static void nautilus_nod32_menu_provider_iface_init (NautilusMenuProviderIface *iface)
{
	iface->get_file_items = nautilus_nod32_get_file_items;
}


static void nautilus_nod32_instance_init (NautilusNod32 *nod32)
{
}


static void nautilus_nod32_class_init (NautilusNod32Class *class)
{
	parent_class = g_type_class_peek_parent (class);
}


static GType nod32_type = 0;


GType nautilus_nod32_get_type (void)
{
	return nod32_type;
}


void nautilus_nod32_register_type (GTypeModule *module)
{
	static const GTypeInfo info = {
		sizeof (NautilusNod32Class),
		(GBaseInitFunc) NULL,
		(GBaseFinalizeFunc) NULL,
		(GClassInitFunc) nautilus_nod32_class_init,
		NULL,
		NULL,
		sizeof (NautilusNod32),
		0,
		(GInstanceInitFunc) nautilus_nod32_instance_init,
	};

	static const GInterfaceInfo menu_provider_iface_info = {
		(GInterfaceInitFunc) nautilus_nod32_menu_provider_iface_init,
		NULL,
		NULL
	};

	nod32_type = g_type_module_register_type (module,
					       G_TYPE_OBJECT,
					       "NautilusNod32",
					       &info, 0);

	g_type_module_add_interface (module,
				     nod32_type,
				     NAUTILUS_TYPE_MENU_PROVIDER,
				     &menu_provider_iface_info);
}
