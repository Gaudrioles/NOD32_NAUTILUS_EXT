#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <glib-2.0/glib.h>

int main(void)
{
    char *home = NULL;
    gchar *tampon = NULL;

    home = getenv("HOME");
    tampon = g_strdup_printf("%s/.config/NOD32_Extention/NOD32CMD.sh", home);

    execl("/bin/sh", "sh", tampon, (char *) NULL);

    g_free(tampon);
    return 0;
}