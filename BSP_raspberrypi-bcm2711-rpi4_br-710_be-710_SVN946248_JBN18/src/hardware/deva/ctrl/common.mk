ifndef QCONFIG
QCONFIG=qconfig.mk
endif
include $(QCONFIG)

ifndef CLASS
CLASS=ctrl
endif

NAME=deva-$(CLASS)-$(SECTION)
USEFILE=$(PROJECT_ROOT)/$(SECTION)/$(NAME).use
EXTRA_SILENT_VARIANTS+=$(SECTION)

ifneq ($(DEBUG),)
ifneq ($(origin DEBUG),undefined)
CPPFLAGS += -DADO_DEBUG=1
endif
endif

LIBS_mx += drvr
LIBS += $(LIBS_$(SECTION))

define PINFO
PINFO DESCRIPTION=
endef

include $(MKFILES_ROOT)/qmacros.mk
-include $(PROJECT_ROOT)/roots.mk
ifndef AUDIO_ROOT
AUDIO_ROOT=$(PRODUCT_ROOT)
endif
include $(PROJECT_ROOT)/$(SECTION)/pinfo.mk
#Removes the default -Wl,-Bsymbolic from the link commandline
LDVFLAG_dll= -L.

#####AUTO-GENERATED by packaging script... do not checkin#####
   INSTALL_ROOT_nto = $(PROJECT_ROOT)/../../../../install
   USE_INSTALL_ROOT=1
##############################################################

include $(MKFILES_ROOT)/qtargets.mk


ifneq ($(QNX_ALLOW_WERROR), 0)
CCFLAGS += -Werror
endif
CCFLAGS += -Wno-unused-parameter

# Define HAVE_SMMU_H if smmu.h is found in the INCVPATH
# Used by audio public headers and drivers to selectively compile in smmu features only if smmu is available
CPPFLAGS += $(if $(strip $(foreach F,$(INCVPATH),$(wildcard $(F)/smmu.h*))),-DHAVE_SMMU_H)

ifeq ($(filter dll, $(VARIANTS)),)
CPPFLAGS += -Dctrl_version=ctrl_version_$(SECTION)
CPPFLAGS += -Dctrl_init=ctrl_init_$(SECTION)
CPPFLAGS += -Dctrl_destroy=ctrl_destroy_$(SECTION)
endif
