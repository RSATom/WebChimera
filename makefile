.PHONY: all prep build install clean

INSTALLDIR = $(DESTDIR)/usr/lib/mozilla/plugins

all: prep build

dprep:
        ./deps/firebreath/prepmake.sh src build -dcmake_build_type=debug

prep:
	./deps/firebreath/prepmake.sh src build

build:
	cd ./build ./ && $(MAKE)

install:
	test -d $(INSTALLDIR) || mkdir -p $(INSTALLDIR)
	install  ./build/bin/WebChimera/npWebChimera.so $(INSTALLDIR)

clean:
	rm -rf ./build
