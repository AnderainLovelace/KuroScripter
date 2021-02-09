#---------------------------------
# KuroScripter Makefile
#---------------------------------
# by Hikari no Uiharu
# for Ndless SDK
#---------------------------------
DEBUG       = FALSE
GCC         = nspire-gcc
AS          = as
GXX         = g++
LD          = nspire-ld
GCCFLAGS    = -Wall -W -marm -I../include
GENZEHN = genzehn

LIB         = libjpeg/jdmarker.o libjpeg/jidctred.o libjpeg/jdmaster.o libjpeg/jmemmgr.o libjpeg/jdmerge.o libjpeg/jmemnobs.o libjpeg/jdphuff.o libjpeg/jquant1.o libjpeg/jdapimin.o libjpeg/jdpostct.o libjpeg/jquant2.o libjpeg/jdapistd.o libjpeg/jdsample.o libjpeg/jutils.o libjpeg/jdatasrc.o libjpeg/jerror.o libjpeg/jdcoefct.o libjpeg/jdcolor.o libjpeg/jddctmgr.o libjpeg/jdhuff.o libjpeg/jidctflt.o libjpeg/jcomapi.o libjpeg/jdinput.o libjpeg/jidctfst.o libjpeg/jdmainct.o libjpeg/jidctint.o \
	         -lndls
	 
LDFLAGS     = $(LIB)
ZEHNFLAGS = --name "KuroScripter"

ifeq ($(DEBUG),FALSE)
	GCCFLAGS += -Os
else
	GCCFLAGS += -O0 -g
	LDFLAGS += --debug
endif

OBJS = error.o       kuro.o           scene_title.o  timer.o            \
       font.o        scene_data.o     script.o       scene_select.o     \
       data.o        graph.o          media.o        sprite.o           \
       dlist.o       input.o          oimg_jpeg.o    scene_talk.o       \
	   tar_reader.o  utils.o
	   
ifneq ($(strip $(CPPOBJS)),)
	LDFLAGS += --cpp
endif

EXE         = KuroScripter
DISTDIR     = .

vpath %.tns $(DISTDIR)
vpath %.elf $(DISTDIR)

all: $(EXE).tns

%.o: %.c
	$(GCC) $(GCCFLAGS) -c $< -o $@

%.o: %.cpp
	$(GXX) $(GCCFLAGS) -c $< -o $@
	
%.o: %.S
	$(AS) -c $< -o $@

$(EXE).elf: $(OBJS)
	mkdir -p $(DISTDIR)
	$(LD) $^ -o $@ $(LDFLAGS)

$(EXE).tns: $(EXE).elf
	$(GENZEHN) --input $^ --output $@.zehn $(ZEHNFLAGS)
	make-prg $@.zehn $@
	rm $@.zehn

clean:
	rm -f $(OBJS) $(DISTDIR)/$(EXE).tns $(DISTDIR)/$(EXE).elf $(DISTDIR)/$(EXE).zehn
