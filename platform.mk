.SUFFIXES : .c .cc .cpp .obj

.cpp.obj:
	$(CCC) $(CCFLAGS) -c $(IPATHS) $<

.c.obj:
	$(CC) $(CFLAGS) -c $(IPATHS) $<

.cc.obj:
	$(CC) $(CFLAGS) -c $(IPATHS) $<

# The following symbols should be valid on all Windows systems
CC	  = cl
CCC	  = cl
CFLAGS_	  = -nologo -EHsc -W3 -Zm200
CVARS0_	  = -DWIN32 -D_WIN32 -GF
CVARSDLL_ = $(CVARSRTDLL_) -DCBORDLL
COPTIMIZE0_ = -Ob2 -Gy -Ox 
CDEBUG0_  = -Zi -D_DEBUG
FS	  = ;
LIBCMD	  = lib /NOLOGO /OUT:$@
LIBADD	  = lib /NOLOGO /OUT:$@ $@
LINK	  = cl
LINKOPT0  = /nologo /link /OUT:$@ /OPT:REF  
LINKDBG_  = /MAP /DEBUG 
LINKOPTM_ = /OPT:REF  
OBJOUT	  = -Fo$@
PLATFORM  = WIN32
PS	  = \#
PURECVAR  = -TC

CVARSMTR_ = $(CVARS0_) -D_MT -MT
CVARSMDR_ = $(CVARS0_) -D_MT -D_DLL -MD
CVARSR_	  = $(CVARSMDR_)

CVARSMTD_ = $(CVARS0_) -D_MT -MTd
CVARSMDD_ = $(CVARS0_) -D_MT -D_DLL -MDd
CVARSD_	  = $(CVARSMDD_)

LINKOPTR_  = $(LINKOPT0)
LINKOPT2R  = /nologo /link /OUT:$@ /OPT:REF

LINKOPTD_  = $(LINKOPT0) $(LINKDBG_)
LINKOPT2D  = /nologo /link /OUT:$@ /OPT:REF $(LINKDBG_)
COPTIMIZE_ = $(COPTIMIZE0_) -D_OPTIMIZED

# START RELEASE
#CDEV_     = -D_TRACE -Od
#CDEBUG_   = $(CDEV_) $(CDEBUG0_)
#CBLDTYPE_ = $(COPTIMIZE_)
#CVARS_    = $(CVARSR_)
#CVARSMT_  = $(CVARSMTR_)
#CVARSMD_  = $(CVARSMDR_)
#LINKOPT_  = $(LINKOPTR_)
#LINKOPT2  = $(LINKOPT2R)
# END RELEASE

# START DEBUG
CDEV_     = -D_TRACE -Od $(CDEBUG0_)
CDEBUG_	  = $(CDEV_)
CBLDTYPE_ = $(CDEBUG_)
CVARS_    = $(CVARSD_)
CVARSMT_  = $(CVARSMTD_)
CVARSMD_  = $(CVARSMDD_)
LINKOPT_  = $(LINKOPTD_)
LINKOPT2  = $(LINKOPT2D)
# END DEBUG

# File extensions
EXE	= .exe
OBJ	= .obj

# Run-time library
LIBPFX	=
LIBEXT	= lib
LPPFX	= -LIBPATH:
LLPFX   =
LLEXT   = .lib
LLAEXT  = _a.$(LIBEXT)
A       = _a.$(LIBEXT)
MTA     = mt_a.$(LIBEXT)
MDA     = md_a.$(LIBEXT)
IMP     = .$(LIBEXT)
DLL     = .dll
RTDIRSFX =

# O/S commands
COPY	 = -copy
MOVE	 = -move
MV	 = $(MOVE)
RM	 = -del
STRIP	 = strip -g -S
MAKE     = nmake
RMDIR = -rmdir /S /Q
MKDIR = -mkdir

LLCBOR	= oortcbor$(A)
LLSYS	= 
