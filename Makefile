TARGET_1=nod32_nautilus_ext.so
OBJECTS_1=plugin.o

TARGET_2=NOD32_EXT

LIBDIR=/usr/lib/x86_64-linux-gnu

CFLAGS_1= -fPIC -g \
	$(shell pkg-config --cflags gtk+-3.0 libnautilus-extension)

LDFLAGS_1=-shared \
	$(shell pkg-config --libs gtk+-3.0 libnautilus-extension)

GLIB= `pkg-config --cflags --libs glib-2.0`

all: $(TARGET_1) $(TARGET_2)

$(TARGET_1): $(OBJECTS_1)
		gcc $(LDFLAGS_1) $(OBJECTS_1) -o $(TARGET_1)

$(TARGET_2): 
		gcc tool.c $(GLIB) -o $(TARGET_2)

install: all
	mkdir -p $(LIBDIR)/nautilus/extensions-3.0/
	mkdir -p $(HOME)/.config/NOD32_Extention/
	sudo mv $(TARGET_1) $(LIBDIR)/nautilus/extensions-3.0/
	mv $(TARGET_2) $(HOME)/.config/NOD32_Extention/

uninstall: all
	sudo rm -rf $(HOME)/.config/NOD32_Extention/
	rm -f $(LIBDIR)/nautilus/extensions-3.0/$(TARGET_1)

plugin.o: plugin.c
	gcc -c $(CFLAGS_1) plugin.c -o plugin.o

clean:
	rm -f $(OBJECTS_1) $(TARGET_1) $(TARGET_2)
