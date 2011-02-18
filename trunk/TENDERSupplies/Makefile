# $Id: Makefile 27887 2008-08-11 08:33:57Z pchrist $

include Makefile.arch

PACKAGE = TENDERSupplies

TENDERSupplies: lib$(PACKAGE).so

ALICEINC      = -I.

# only if no par file was loaded before

ifneq ($(TENDER_INCLUDE),)
 ALICEINC += -I../$(TENDER_INCLUDE)
endif

ifneq ($(ALICE_ROOT),)
  ALICEINC += -I$(ALICE_ROOT)/TPC
endif

ifneq ($(ALICE_ROOT),)
  ALICEINC += -I$(ALICE_ROOT)/TOF
endif

ifneq ($(ALICE_ROOT),)
  ALICEINC += -I$(ALICE_ROOT)/TRD
endif

ifneq ($(ALICE_ROOT),)
  ALICEINC += -I$(ALICE_ROOT)/TRD/Cal
endif

ifneq ($(ALICE_ROOT),)
  ALICEINC += -I$(ALICE_ROOT)/T0
endif

ifneq ($(ALICE_ROOT),)
  ALICEINC += -I$(ALICE_ROOT)/VZERO
endif

ifneq ($(ALICE_ROOT),)
  ALICEINC += -I$(ALICE_ROOT)/include
endif

# for building of PWG0dep.par


CXXFLAGS     += $(ALICEINC) -g
F77FLAGS     += $(ALICEINC) -g -fPIC

#ALICEINC += -IFEMTOSCOPY/AliFemto -IANALYSIS -IFEMTOSCOPY/AliFemtoUser

include lib$(PACKAGE).pkg

DHDR_PWG2 := $(DHDR)
HDRS_PWG2 := $(HDRS)
SRCS_PWG2 := $(SRCS) G__$(PACKAGE).cxx
FSCRS_PWG2 := $(FSRCS)
OBJS_PWG2 := $(SRCS_PWG2:.cxx=.o) $(FSRCS:.F=.o)

PARFILE       = $(PACKAGE).par

lib$(PACKAGE).so: $(OBJS_PWG2)
	@echo "Linking" $@ ...
	@/bin/rm -f $@
ifeq ($(ARCH),macosx)
	@$(LD) -bundle -undefined $(UNDEFOPT) $(LDFLAGS) $^ -o $@
else
	@$(LD) $(SOFLAGS) $(LDFLAGS) $^ -o $@
endif
	@chmod a+x $@
	@echo "done"

%.o:    %.cxx %.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

%.o:    %.F
	$(F77) $(F77FLAGS) -c $< -o $@

G__$(PACKAGE).cxx G__$(PACKAGE).h: $(HDRS_PWG2) $(DHDR_PWG2)
	@echo "Generating dictionary ..."
	rootcint -f $@ -c $(CINTFLAGS) $(ALICEINC) $^

clean:
	@rm -f $(OBJS_PWG2) *.so G__$(PACKAGE).* $(PARFILE)

### CREATE PAR FILE

$(PARFILE): $(patsubst %,$(PACKAGE)/%,$(filter-out G__%, $(HDRS_PWG2) $(SRCS_PWG2) $(DHDR_PWG2) Makefile Makefile.arch lib$(PACKAGE).pkg PROOF-INF))
	@echo "Creating archive" $@ ...
	@tar cfzh $@ $(PACKAGE)
	@rm -rf $(PACKAGE)
	@echo "done"

$(PACKAGE)/Makefile: Makefile #.$(PACKAGE)
	@echo Copying $< to $@ with transformations
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@sed 's/include \$$(ROOTSYS)\/test\/Makefile.arch/include Makefile.arch/' < $^ > $@

$(PACKAGE)/Makefile.arch: $(ROOTSYS)/test/Makefile.arch
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a $^ $@

$(PACKAGE)/PROOF-INF: PROOF-INF.$(PACKAGE)
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a -r $^ $@

$(PACKAGE)/%: %
	@echo Copying $< to $@
	@[ -d $(dir $@) ] || mkdir -p $(dir $@)
	@cp -a $< $@

test-%.par: %.par
	@echo "INFO: The file $< is now tested, in case of an error check in par-tmp."
	@mkdir -p par-tmp
	@cd par-tmp; tar xfz ../$<;	cd $(subst .par,,$<); PROOF-INF/BUILD.sh
	@rm -rf par-tmp
	@echo "INFO: Testing succeeded (already cleaned up)"
