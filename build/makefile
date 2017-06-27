# Build static run-time libraries

include ../platform.mk

OOROOTDIR = ..
OBJDIR = .
RTXSRCDIR = $(OOROOTDIR)/rtxsrc
CBORSRCDIR = $(OOROOTDIR)/rtcborsrc
JSONSRCDIR = $(OOROOTDIR)/rtjsonsrc

COMFLAGS = $(CVARS_) $(MCFLAGS) $(CBLDTYPE_)
CFLAGS	 = $(COMFLAGS) $(CFLAGS_)
RTXCFLAGS = $(CFLAGS)
IPATHS	 = -I$(OOROOTDIR) $(IPATHS_)
LPATHS	 = $(LPATHS_)

# START RTBUILD 
all : common_libs 

common_libs : ../lib/$(LIBPFX)oortcbor$(A)

include ../rtxsrc/objects.mk
include ../rtcborsrc/objects.mk
include ../rtjsonsrc/objects.mk

OBJECTS = $(RTXOBJECTS) $(OSRT_CBOR_RTOBJECTS) $(OSRT_JSON_RTOBJECTS)

../lib/$(LIBPFX)oortcbor$(A) : $(OBJECTS) 
	$(LIBCMD) $(OBJECTS) 

include ../rtxsrc/rules.mk
include ../rtcborsrc/rules.mk
include ../rtjsonsrc/rules.mk

clean :
	$(RM) *$(OBJ) 
	$(RM) *.pdb 
	$(RM) *~ 
