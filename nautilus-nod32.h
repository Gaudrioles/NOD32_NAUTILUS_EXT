#ifndef NAUTILUS_NOD32_H
#define NAUTILUS_NOD32_H

#include <glib-object.h>

G_BEGIN_DECLS

#define NAUTILUS_TYPE_NOD32  (nautilus_nod32_get_type ())
#define NAUTILUS_NOD32(o)    (G_TYPE_CHECK_INSTANCE_CAST ((o), NAUTILUS_TYPE_NOD32, NautilusNod32))
#define NAUTILUS_IS_NOD32(o) (G_TYPE_CHECK_INSTANCE_TYPE ((o), NAUTILUS_TYPE_NOD32))

typedef struct
{
    GObject __parent;
} NautilusNod32;

typedef struct
{
	GObjectClass __parent;
} NautilusNod32Class;

GType nautilus_nod32_get_type      (void);
void  nautilus_nod32_register_type (GTypeModule *module);

G_END_DECLS

#endif /* NAUTILUS_NOD32_H */
