#include <stdlib.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>

#include <nautilus-extension.h>
#include <gtk/gtk.h>

static GType provider_types[1];
static GType foo_extension_type;
static GObjectClass *parent_class;

typedef struct 
{
	GObject parent_slot;
} FooExtension;

typedef struct 
{
	GObjectClass parent_slot;
} FooExtensionClass;

void nautilus_module_initialize (GTypeModule  *module);
void nautilus_module_shutdown (void);
void nautilus_module_list_types (const GType **types, int *num_types);
GType foo_extension_get_type (void);

static void foo_extension_register_type (GTypeModule *module);

/* menu filler */
static GList * foo_extension_get_file_items (NautilusMenuProvider *provider,
                GtkWidget *window,
                GList *files);
#if 0
static GList * foo_extension_get_background_items (NautilusMenuProvider *provider,
                GtkWidget *window,
                NautilusFileInfo *current_folder);
static GList * foo_extension_get_toolbar_items (NautilusMenuProvider *provider,
                GtkWidget *window,
                NautilusFileInfo *current_folder);
#endif

/* command callback */
void fonction_extension(NautilusMenuItem *item, gpointer user_data);
void execution(char *CheminFichier);
gboolean CreationFichierSh(char *chemin);

void nautilus_module_initialize (GTypeModule  *module)
{
        foo_extension_register_type (module);

        provider_types[0] = foo_extension_get_type ();
}

void nautilus_module_shutdown (void)
{
        /* Any module-specific shutdown */
}

void nautilus_module_list_types (const GType **types,
                                 int *num_types)
{
        *types = provider_types;
        *num_types = G_N_ELEMENTS (provider_types);
}

GType foo_extension_get_type (void)
{
        return foo_extension_type;
}

static void foo_extension_instance_init (FooExtension *object)
{
}

static void foo_extension_class_init(FooExtensionClass *class)
{
	parent_class = g_type_class_peek_parent (class);
}

static void foo_extension_menu_provider_iface_init(
		NautilusMenuProviderIface *iface)
{
	iface->get_file_items = foo_extension_get_file_items;
}

static void foo_extension_register_type (GTypeModule *module)
{
        static const GTypeInfo info = {
                sizeof (FooExtensionClass),
                (GBaseInitFunc) NULL,
                (GBaseFinalizeFunc) NULL,
                (GClassInitFunc) foo_extension_class_init,
                NULL,
                NULL,
                sizeof (FooExtension),
                0,
                (GInstanceInitFunc) foo_extension_instance_init,
        };

	static const GInterfaceInfo menu_provider_iface_info = {
		(GInterfaceInitFunc) foo_extension_menu_provider_iface_init,
		NULL,
		NULL
	};

        foo_extension_type = g_type_module_register_type (module,
                             G_TYPE_OBJECT,
                             "FooExtension",
                             &info, 0);

	g_type_module_add_interface (module,
				     foo_extension_type,
				     NAUTILUS_TYPE_MENU_PROVIDER,
				     &menu_provider_iface_info);
}

static GList *foo_extension_get_file_items (NautilusMenuProvider *provider, GtkWidget *window, GList *files)
{
        NautilusMenuItem *item;
        GList *l;
        GList *ret;

        for (l = files; l != NULL; l = l->next) 
        {
                NautilusFileInfo *file = NAUTILUS_FILE_INFO (l->data);
                gchar *name;
                name = nautilus_file_info_get_name (file);
                g_print ("selected %s\n", name);
                g_free (name);
        }
        
        item = nautilus_menu_item_new("Eset NOD32", "Analyser avec ESET NOD32 Antivirus", "ESET NOD32", "/opt/eset/esets/share/menu/app_icon_128.png");

        g_signal_connect(item, "activate", G_CALLBACK(fonction_extension), provider);

        g_object_set_data_full((GObject*) item, "foo_extension_files", nautilus_file_info_list_copy (files), (GDestroyNotify)nautilus_file_info_list_free);

        ret = g_list_append (NULL, item);

        return ret;
}

void fonction_extension(NautilusMenuItem *item, gpointer user_data)
{
        GList *fichier;
        GList *liste;
        gchar *nom = NULL;
        gchar *parent = NULL;

        fichier = g_object_get_data ((GObject *) item, "foo_extension_files");

        for (liste = fichier; liste != NULL; liste = liste->next) 
        {
                
                NautilusFileInfo *file = NAUTILUS_FILE_INFO (liste->data);
                nom = nautilus_file_info_get_name(file);

                parent = g_file_get_path(nautilus_file_info_get_location(file));

                execution(parent);

                g_free(nom);
                g_free(parent);
        }

        return;
}

void execution(char *CheminFichier)
{
        gchar *tampon = NULL;
         
        const char *home = getenv("HOME");

        if(CreationFichierSh(CheminFichier) != TRUE)
        {
                return;
        }

        tampon = g_strdup_printf("%s/.config/NOD32_Extention/NOD32_EXT", home);

        system(tampon);

        g_free(tampon);
                
        return;
}

gboolean CreationFichierSh(char *chemin)
{
    FILE *fichier = NULL;
    gchar *tampon = NULL;

    const char *home = getenv("HOME");

    tampon = g_strdup_printf("%s/.config/NOD32_Extention/NOD32CMD.sh", home);
    
    fichier = fopen(tampon, "w");
        
    if(fichier == NULL)
    {
        return FALSE;
    }
    
    fprintf(fichier, "#!/bin/sh\n\nsh /opt/eset/esets/lib/esets_scan.sh %s", chemin);

    fclose(fichier);

    g_free(tampon);

    return TRUE;
}