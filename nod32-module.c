#define GETTEXT_PACKAGE "gtk3"

#include <nautilus-extension.h>
#include "nautilus-nod32.h"

void nautilus_module_initialize (GTypeModule*module)
{
	nautilus_nod32_register_type (module);
}


void nautilus_module_shutdown (void)
{

}


void nautilus_module_list_types (const GType **types, int *num_types)
{
        static GType type_list[1];

	type_list[0] = NAUTILUS_TYPE_NOD32;
	*types = type_list;
	*num_types = 1;
}