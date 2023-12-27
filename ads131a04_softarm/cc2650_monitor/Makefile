#--------------------------------------------------------------
#                Linux
#--------------------------------------------------------------
ROOT_SDK_PATH ?= $(shell cd $(shell pwd)/../../..; pwd)
ROOTFS_DIR  ?= $(ROOT_SDK_PATH)/ql-ol-sdk/ql-ol-rootfs
USRDATA_DIR ?= $(ROOT_SDK_PATH)/ql-ol-sdk/ql-ol-usrdata
HOME_DIR ?= $(ROOTFS_DIR)/home/root
APPS_DIR ?= $(HOME_DIR)/apps

QL_SDK_PATH ?= $(ROOT_SDK_PATH)/ql-ol-sdk/ql-ol-extsdk
KY_SDK_PATH ?= $(ROOT_SDK_PATH)/kunyue

QL_EXP_LDLIBS  = -lql_sys_log
#CFLAGS += -g

#QL_SDK_PATH   ?= $(shell pwd)/../..

QL_EXP_TARGETS = system_monitor

#--------------------------------------------------------
# TOOLS BASIC CONFIG
# Note: No Need to change them
#--------------------------------------------------------
#CPPFLAGS = -I./ -I$(SDKTARGETSYSROOT)/usr/include/ -I$(QL_SDK_PATH)/include
#CPPFLAGS = -I./ -I$(QL_SDK_PATH)/include -I$(SDKTARGETSYSROOT)/usr/include/json-c
CPPFLAGS += -I./                     \
            -I./inc                  \
            -I../../include          \
            -I$(QL_SDK_PATH)/include           \
            -I$(QL_SDK_PATH)/lib/interface/inc        \
            -I$(SDKTARGETSYSROOT)/usr/include               \
            -I$(SDKTARGETSYSROOT)/usr/include               \
            -I$(SDKTARGETSYSROOT)/usr/include/data          \
            -I$(SDKTARGETSYSROOT)/usr/include/dsutils       \
            -I$(SDKTARGETSYSROOT)/usr/include/qmi           \
            -I$(SDKTARGETSYSROOT)/usr/include/qmi-framework \
            -I$(SDKTARGETSYSROOT)/usr/include/ql-manager	

USR_LIB=$(QL_SDK_PATH)/lib/libql_peripheral.a
STD_LIB= $(SDKTARGETSYSROOT)/usr/lib/libql_mgmt_client.so
DUAL_LIB=$(QL_SDK_PATH)/lib/libql_common_api.a


LDFLAGS = -L./ -L$(QL_SDK_PATH)/lib ${QL_EXP_LDLIBS} -lpthread -lrt 

	
LOCAL_SRC_FILES = system_monitor.c
all: $(QL_EXP_TARGETS)
.PHPNY: all


$(QL_EXP_TARGETS): 
	$(COMPILE.c) $(CPPFLAGS) $(LDFLAGS) $(LOCAL_SRC_FILES)
	$(LINK.o) *.o $(LDFLAGS) $(USR_LIB) $(DUAL_LIB) $(STD_LIB) -o $@

install : all
	install -d ${APPS_DIR}
	install -m 777 $(QL_EXP_TARGETS) ${APPS_DIR}/$(QL_EXP_TARGETS)

uninstall :
	-rm -f ${APPS_DIR}/$(QL_EXP_TARGETS)

clean:
	rm -rf $(QL_EXP_TARGETS) *.o

.PHONY:checkmake
checkmake:  
	@echo -e "CURDIR =		\n	${CURDIR}"  
	@echo -e "\nMAKE_VERSION =	\n	${MAKE_VERSION}"  
	@echo -e "\nMAKEFILE_LIST =	\n	${MAKEFILE_LIST}"  
	@echo -e "\nCOMPILE.c =		\n	${COMPILE.c}"
	@echo -e "\nCOMPILE.cc =	\n	${COMPILE.cc}"
	@echo -e "\nCOMPILE.cpp =	\n	${COMPILE.cpp}"
	@echo -e "\nLINK.cc =		\n	${LINK.cc}"
	@echo -e "\nLINK.o =		\n	${LINK.o}"
	@echo -e "\nCPPFLAGS =		\n	${CPPFLAGS}"
	@echo -e "\nCFLAGS =		\n	${CFLAGS}"
	@echo -e "\nCXXFLAGS =		\n	${CXXFLAGS}"
	@echo -e "\nLDFLAGS =		\n	${LDFLAGS}"
	@echo -e "\nLDLIBS =		\n	${LDLIBS}"

