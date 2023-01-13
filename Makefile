# call make w/ one of the following defined for cross compilation:
#CROSS := i686  LINK_TYPE will be static or shared
#CROSS := x86_64

BFDHEAD := elf64-x86-64

ifneq ($(CROSS),)
	FULLCROSS := $(CROSS)-w64-mingw32.$(LINK_TYPE)-
	EXE_PFX := wine 
	EXE_SFX := _$(CROSS)-w64-mingw32.$(LINK_TYPE).exe
	RELEASE_DIR := true
	ifeq ($(CROSS), i686)
		BFDHEAD := pei-i386
	else
		BFDHEAD := pei-x86-64
	endif
	OTHERFLAGS := -fpermissive -mwindows
endif

CC=$(FULLCROSS)gcc
LD=$(FULLCROSS)ld
AR=$(FULLCROSS)ar
PKG_CONFIG=$(FULLCROSS)pkg-config
CXX		  := $(FULLCROSS)g++
CPPSTANDARD:= -std=c++17

OBJCPY 	:= $(LD) -r -b binary
RSCCPY	:= $(FULLCROSS)windres -O coff
REL 	:= rel
DBG		:= dbg
BIN     := bin
SRC		:= src
INC		:= inc
RSC 	:= rsc
CWD		:= $(PWD)
# ACT_ONS are the roots of the directory trees with all the files that MAKE will act on 
ACT_ONS := $(SRC) $(RSC)

ifneq (,$(RELEASE_DIR))
	BLD := $(BIN)/$(REL)
else
	BLD := $(BIN)/$(DBG)
	DEBUGFLAG := -ggdb
endif

CXX_FLAGS := $(CPPSTANDARD) $(OTHERFLAGS) $(DEBUGFLAG)

CFLAGS	:= $(shell $(PKG_CONFIG) gtkmm-3.0 --cflags)
LIBRARIES	:= $(shell $(PKG_CONFIG) gtkmm-3.0 epoxy glu --libs)
# -lcuda library for parallel processing
#  glx epoxy opengl gl glew sdl2 glu

EXE := $(notdir $(CWD))$(EXE_SFX)

ICONFILE := $(CWD)/share/icons/small.ico
USETERMINAL := false
LOGFILE := $(CWD)/log.txt
DIRFILE := $(CWD)/dir.txt

#updating base directory structure:
BDIRS := $(BIN) $(SRC) $(RSC) $(INC) $(BIN)/$(DBG) $(BIN)/$(REL) 
BD_CHK := $(foreach BD, $(BDIRS), [$(shell [ -d $(BD) ] && echo $(BD)_exists || mkdir $(BD) -pv )])
BD_ADDS:= $(filter-out %_exists], $(BD_CHK))
ifneq (,$(BD_ADDS))
BUFF := $(file >> $(DIRFILE),[directories created=$(BD_ADDS)])
endif
#updating directory structure in build subdirectory:
ODIRS := $(shell find -type d)
ODIRS := $(ODIRS:./.%=) 
ODIRS := $(ODIRS:./%=%)
ODIRS := $(ODIRS:.=)
ODIRS := $(foreach OD, $(ACT_ONS), $(filter $(OD)%, $(ODIRS)))
ODIRS := $(addprefix $(BLD)/, $(ODIRS))
ADIRS := $(shell find $(BLD) -type d)
ADIRS := $(ADIRS:$(BLD)=)
ADIRS := $(filter-out $(ODIRS), $(ADIRS))
ADIRS := $(strip $(ADIRS))
MD_RES := $(foreach OD, $(ODIRS), [$(shell [ -d $(OD) ] && echo $(OD)_PRESENT  || mkdir $(OD) -pv )])
BR_ADDS:= $(filter-out %_PRESENT],$(MD_RES))
ifneq (,$(BR_ADDS))
BUFF := $(file >> $(DIRFILE),[added branches=$(BR_ADDS)])
endif
RF_RES := $(foreach AD, $(ADIRS), [$(shell $(RM) -v `find $(AD) -type f`)])
RD_RES := $(foreach AD, $(ADIRS),[$(shell rmdir $(AD) -pv 2>&1 | sed 's/^rmdir: failed to remove .*: Directory not empty$$/ - failed/g')])
ifneq (,$(ADIRS)) 
BUFF := $(file >> $(DIRFILE),[trees to prune=$(ADIRS)] - [files deleted=$(RF_RES)] - [directories removed=$(RD_RES)])
endif

#derived file variables:
allinBIN := $(shell find $(BIN) -type f)  #$(wildcard $(BIN)/$(DBG)/*) $(wildcard $(BIN)/$(REL)/*)
allinBLD := $(shell find $(BLD) -type f)
allinRSC := $(shell find $(RSC) -type f)
allinSRC := $(shell find $(SRC) -type f)
SRCS :=  $(filter %.cpp, $(allinSRC))
DEPS := $(filter %.d, $(allinBLD))
DEL_BLD := $(filter-out $(DEPS:%.d=%.o), $(filter $(BLD)/$(SRC)/%.o, $(allinBLD)))
OBJS := $(addprefix $(BLD)/, $(addsuffix .o, $(SRCS)))
OBJS += $(addprefix $(BLD)/, $(addsuffix .o, $(allinRSC)))
DEL_BLD += $(filter-out $(BLD)/$(EXE), $(filter-out $(DEPS), $(filter-out $(OBJS), $(allinBLD))))
RUN_DT := $(EXE).desktop
RUN_SH := $(EXE).sh

.PHONY: all
all: logsetup compile
	@echo all completed >> $(LOGFILE)

.PHONY: run
run: all
	$(EXE_PFX)./$(BLD)/$(EXE)
	@echo run completed >> $(LOGFILE)

.PHONY: compile
compile: prepbld make_exe
	@printf \# > $(RUN_DT)
	@echo !/usr/bin/env xdg-open >> $(RUN_DT)
	@echo [Desktop Entry] >> $(RUN_DT)
	@echo Name=Run $(EXE) >> $(RUN_DT)
	@echo Comment=A Nemo Work-around >> $(RUN_DT)
	@echo Exec=$(CWD)/$(EXE).sh >> $(RUN_DT)
	@echo Icon=$(ICONFILE) >> $(RUN_DT)
	@echo Terminal=$(USETERMINAL) >> $(RUN_DT)
	@echo Type=Application >> $(RUN_DT)
	@printf \# > $(RUN_SH)
	@echo !/bin/bash >> $(RUN_SH)
	@echo cd $(CWD) >> $(RUN_SH)
	@echo $(EXE_PFX)./$(BLD)/$(EXE) >> $(RUN_SH)
	@echo echo \"Press 'Enter' To Exit\" >> $(RUN_SH)
	@echo read extKey >> $(RUN_SH)
	@chmod a+x $(EXE).*
	@echo compile completed >> $(LOGFILE)

.PHONY: clean
clean: 
	@$(RM) $(allinBIN)
	@echo deleted $(words $(allinBIN)) files [$(allinBIN)] >> $(LOGFILE)

PHONY: prepbld
prepbld:
	@$(RM) $(DEL_BLD)
	@echo deleted $(words $(DEL_BLD)) build files [$(DEL_BLD)] >> $(LOGFILE)

.PHONY: make_exe
make_exe: $(BLD)/$(EXE)
	@echo make_exe completed >> $(LOGFILE)

$(BLD)/$(EXE): $(OBJS)
	@$(CXX) $^ -o $@ $(LIBRARIES)
	@chmod a+x $(BLD)/$(EXE)
	@echo linked all objects into $@ >> $(LOGFILE)

$(BLD)/$(RSC)/%.o: $(RSC)/%
	@if [ $(suffix $<) = ".rc" ]; then \
	$(RSCCPY) $< $@ ; \
	else \
	$(OBJCPY) $< -o $@ ; fi ;
	@echo resource $< made into object $@ >> $(LOGFILE) ;

$(BLD)/$(SRC)/%.o: $(SRC)/%
	@$(CXX) -I$(INC) $< -MM -MT '$@' -MF $(subst .o,.d,$@)
	@sed -i 's/$(SRC)\/$* //g' $(subst .o,.d,$@)
	@$(CXX) $(CXX_FLAGS) -I$(INC) $(CFLAGS) -c $< -o $@
	@echo new object + depend file compiled [$@ + .d] due to [$?] >> $(LOGFILE)

.PHONY: logsetup
logsetup:
	@echo BUILD LOG `date` > $(LOGFILE)
	@echo -----\>Cross Compile Variables: >> $(LOGFILE)
	@echo [FULLCROSS=$(FULLCROSS)] - [BFDHEAD=$(BFDHEAD)] - [LINK_TYPE=$(LINK_TYPE)] >> $(LOGFILE) 
	@echo -----\>User Variables: >> $(LOGFILE)
	@echo [RELEASE_DIR=$(RELEASE_DIR)] - [MAKECMDGOALS=$(MAKECMDGOALS)] - [MAKE_RESTARTS=$(MAKE_RESTARTS)] [MAKEFILE_LIST=$(MAKEFILE_LIST)] >> $(LOGFILE)
	@echo [necessary build directories=$(ODIRS)] - [checks=$(MD_RES)] >> $(LOGFILE)
	@if [ -f $(DIRFILE) ]; then \
	echo -------\>UPDATED THESE FILES AND DIRECTORIES: >> $(LOGFILE) ; \
	cat $(DIRFILE) >> $(LOGFILE) ; \
	$(RM) $(DIRFILE) ; fi ; 
	@echo [CXX=$(CXX)] - [CXX_FLAGS=$(CXX_FLAGS)] >> $(LOGFILE)
	@echo [BIN=$(BIN)] - [BLD=$(BLD)] - [SRC=$(SRC)] >> $(LOGFILE)
	@echo [INC=$(INC)] - [DBG=$(DBG)] - [REL=$(REL)] >> $(LOGFILE)
	@echo [RSC=$(RSC)] - [CWD=$(CWD)] >> $(LOGFILE)
	@echo [CFLAGS=$(CFLAGS)] >> $(LOGFILE)
	@echo [EXE=$(EXE)] - [ICONFILE=$(ICONFILE)] >> $(LOGFILE)
	@echo [LIBRARIES=$(LIBRARIES)] >> $(LOGFILE)
	@echo [USETERMINAL=$(USETERMINAL)] - [LOGFILE=$(LOGFILE)] >> $(LOGFILE)
	@echo [DEBUGFLAG=$(DEBUGFLAG)] >> $(LOGFILE)
	@echo -----\>Calculated/Environmental Variables: >> $(LOGFILE)
	@echo [SRCS=$(SRCS)] >> $(LOGFILE)
	@echo [DEPS=$(DEPS)] >> $(LOGFILE)
	@echo [DEL_BLD=$(DEL_BLD)] >> $(LOGFILE)
	@echo [allinBIN=$(allinBIN)] >> $(LOGFILE)
	@echo [allinBLD=$(allinBLD)] >> $(LOGFILE)
	@echo [allinRSC=$(allinRSC)] >> $(LOGFILE)
	@echo [allinSRC=$(allinSRC)] >> $(LOGFILE)
	@echo [OBJS=$(OBJS)] >> $(LOGFILE)
	@echo -----\>MAKE OUTPUT FOLLOWS: >> $(LOGFILE)

.PHONY: logclear
logclear:
	@$(RM) $(LOGFILE)

#including all the depends files only if we're compiling
ifeq (,$(findstring $(MAKECMDGOALS),clean-logsetup-logclear-prepbld))
include $(DEPS)
endif

