#
# OMNeT++/OMNEST Makefile for myWSN
#
# This file was generated with the command:
#  opp_makemake -f --deep -O out -I/usr/include/c++/4.5.2 -I/home/reu/Desktop/MiXiM/modules/phy -I/home/reu/Desktop/MiXiM/modules/nic -I/home/reu/Desktop/MiXiM/modules/messages -I/home/reu/Desktop/MiXiM/base/phyLayer -I/home/reu/Desktop/MiXiM/modules/mac -I/home/reu/Desktop/MiXiM/base/messages -I/home/reu/Desktop/MiXiM/base/modules -I/home/reu/Desktop/MiXiM/modules/power/battery -I/home/reu/Desktop/MiXiM/modules/analogueModel -I/home/reu/Desktop/MiXiM/base/connectionManager -I/home/reu/Desktop/MiXiM/base/utils -I/home/reu/Desktop/MiXiM/modules/utility -L/home/reu/Desktop/MiXiM/out/$(CONFIGNAME)/tests/testUtils -L/home/reu/Desktop/MiXiM/out/$(CONFIGNAME)/base -L/home/reu/Desktop/MiXiM/out/$(CONFIGNAME)/modules -lmiximtestUtils -lmiximbase -lmiximmodules -KMIXIM_PROJ=/home/reu/Desktop/MiXiM
#

# Name of target to be created (-o option)
TARGET = myWSN$(EXE_SUFFIX)

# User interface (uncomment one) (-u option)
USERIF_LIBS = $(ALL_ENV_LIBS) # that is, $(TKENV_LIBS) $(CMDENV_LIBS)
#USERIF_LIBS = $(CMDENV_LIBS)
#USERIF_LIBS = $(TKENV_LIBS)

# C++ include paths (with -I)
INCLUDE_PATH = \
    -I/usr/include/c++/4.5.2 \
    -I$(MIXIM_PROJ)/modules/phy \
    -I$(MIXIM_PROJ)/modules/nic \
    -I$(MIXIM_PROJ)/modules/messages \
    -I$(MIXIM_PROJ)/base/phyLayer \
    -I$(MIXIM_PROJ)/modules/mac \
    -I$(MIXIM_PROJ)/base/messages \
    -I$(MIXIM_PROJ)/base/modules \
    -I$(MIXIM_PROJ)/modules/power/battery \
    -I$(MIXIM_PROJ)/modules/analogueModel \
    -I$(MIXIM_PROJ)/base/connectionManager \
    -I$(MIXIM_PROJ)/base/utils \
    -I$(MIXIM_PROJ)/modules/utility \
    -I. \
    -Iresults

# Additional object and library files to link with
EXTRA_OBJS =

# Additional libraries (-L, -l options)
LIBS = -L$(MIXIM_PROJ)/out/$(CONFIGNAME)/tests/testUtils -L$(MIXIM_PROJ)/out/$(CONFIGNAME)/base -L$(MIXIM_PROJ)/out/$(CONFIGNAME)/modules  -lmiximtestUtils -lmiximbase -lmiximmodules
LIBS += -Wl,-rpath,`abspath $(MIXIM_PROJ)/out/$(CONFIGNAME)/tests/testUtils` -Wl,-rpath,`abspath $(MIXIM_PROJ)/out/$(CONFIGNAME)/base` -Wl,-rpath,`abspath $(MIXIM_PROJ)/out/$(CONFIGNAME)/modules`

# Output directory
PROJECT_OUTPUT_DIR = out
PROJECTRELATIVE_PATH =
O = $(PROJECT_OUTPUT_DIR)/$(CONFIGNAME)/$(PROJECTRELATIVE_PATH)

# Object files for local .cc and .msg files
OBJS = \
    $O/myAppl.o \
    $O/csmaControl.o \
    $O/stationAppl.o \
    $O/timeStationAppl.o \
    $O/timeFusionAppl.o \
    $O/myStationAppl.o \
    $O/myBattery.o \
    $O/fusionAppl.o \
    $O/myPhyLayerControl.o \
    $O/myData.o \
    $O/myFusionAppl.o \
    $O/wsnPkt_m.o

# Message files
MSGFILES = \
    wsnPkt.msg

# Other makefile variables (-K)
MIXIM_PROJ=/home/reu/Desktop/MiXiM

#------------------------------------------------------------------------------

# Pull in OMNeT++ configuration (Makefile.inc or configuser.vc)

ifneq ("$(OMNETPP_CONFIGFILE)","")
CONFIGFILE = $(OMNETPP_CONFIGFILE)
else
ifneq ("$(OMNETPP_ROOT)","")
CONFIGFILE = $(OMNETPP_ROOT)/Makefile.inc
else
CONFIGFILE = $(shell opp_configfilepath)
endif
endif

ifeq ("$(wildcard $(CONFIGFILE))","")
$(error Config file '$(CONFIGFILE)' does not exist -- add the OMNeT++ bin directory to the path so that opp_configfilepath can be found, or set the OMNETPP_CONFIGFILE variable to point to Makefile.inc)
endif

include $(CONFIGFILE)

# Simulation kernel and user interface libraries
OMNETPP_LIB_SUBDIR = $(OMNETPP_LIB_DIR)/$(TOOLCHAIN_NAME)
OMNETPP_LIBS = -L"$(OMNETPP_LIB_SUBDIR)" -L"$(OMNETPP_LIB_DIR)" $(USERIF_LIBS) $(KERNEL_LIBS) $(SYS_LIBS)

COPTS = $(CFLAGS)  $(INCLUDE_PATH) -I$(OMNETPP_INCL_DIR)
MSGCOPTS = $(INCLUDE_PATH)

#------------------------------------------------------------------------------
# User-supplied makefile fragment(s)
# >>>
# <<<
#------------------------------------------------------------------------------

# Main target
all: $(TARGET)

$(TARGET) : $O/$(TARGET)
	$(LN) $O/$(TARGET) .

$O/$(TARGET): $(OBJS)  $(wildcard $(EXTRA_OBJS)) Makefile
	@$(MKPATH) $O
	$(CXX) $(LDFLAGS) -o $O/$(TARGET)  $(OBJS) $(EXTRA_OBJS) $(WHOLE_ARCHIVE_ON) $(LIBS) $(WHOLE_ARCHIVE_OFF) $(OMNETPP_LIBS)

.PHONY:

.SUFFIXES: .cc

$O/%.o: %.cc
	@$(MKPATH) $(dir $@)
	$(CXX) -c $(COPTS) -o $@ $<

%_m.cc %_m.h: %.msg
	$(MSGC) -s _m.cc $(MSGCOPTS) $?

msgheaders: $(MSGFILES:.msg=_m.h)

clean:
	-rm -rf $O
	-rm -f myWSN myWSN.exe libmyWSN.so libmyWSN.a libmyWSN.dll libmyWSN.dylib
	-rm -f ./*_m.cc ./*_m.h
	-rm -f results/*_m.cc results/*_m.h

cleanall: clean
	-rm -rf $(PROJECT_OUTPUT_DIR)

depend:
	$(MAKEDEPEND) $(INCLUDE_PATH) -f Makefile -P\$$O/ -- $(MSG_CC_FILES)  ./*.cc results/*.cc

# DO NOT DELETE THIS LINE -- make depend depends on it.
$O/myData.o: myData.cc \
	myData.h
$O/myFusionAppl.o: myFusionAppl.cc \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	fusionAppl.h \
	myData.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/modules/nic/NicController.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/modules/nic/NicStateNotify.h \
	$(MIXIM_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	myAppl.h \
	wsnPkt_m.h \
	myBattery.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	myFusionAppl.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/modules/nic/NicCommandNotify.h \
	$(MIXIM_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/utils/NetwControlInfo.h
$O/myBattery.o: myBattery.cc \
	myBattery.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryStats.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h
$O/timeFusionAppl.o: timeFusionAppl.cc \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	timeFusionAppl.h \
	myData.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/modules/nic/NicController.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/modules/nic/NicStateNotify.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	myAppl.h \
	myBattery.h \
	wsnPkt_m.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_PROJ)/modules/nic/NicCommandNotify.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/utils/NetwControlInfo.h
$O/myAppl.o: myAppl.cc \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	myData.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/modules/nic/NicController.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/modules/nic/NicStateNotify.h \
	$(MIXIM_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	myAppl.h \
	myBattery.h \
	wsnPkt_m.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/modules/nic/NicCommandNotify.h \
	$(MIXIM_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/utils/NetwControlInfo.h
$O/stationAppl.o: stationAppl.cc \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	myData.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/modules/nic/NicController.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/modules/nic/NicStateNotify.h \
	$(MIXIM_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	myAppl.h \
	myBattery.h \
	wsnPkt_m.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	stationAppl.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/modules/nic/NicCommandNotify.h \
	$(MIXIM_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/utils/NetwControlInfo.h
$O/myPhyLayerControl.o: myPhyLayerControl.cc \
	$(MIXIM_PROJ)/modules/phy/PhyLayer.h \
	$(MIXIM_PROJ)/base/phyLayer/DeciderToPhyInterface.h \
	$(MIXIM_PROJ)/base/phyLayer/AnalogueModel.h \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	myPhyLayerControl.h \
	$(MIXIM_PROJ)/base/phyLayer/Decider.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_PROJ)/base/phyLayer/Interpolation.h \
	$(MIXIM_PROJ)/base/phyLayer/BaseDecider.h \
	$(MIXIM_PROJ)/modules/phy/PhyLayerBattery.h \
	$(MIXIM_PROJ)/base/utils/Move.h \
	$(MIXIM_PROJ)/base/phyLayer/ChannelState.h \
	$(MIXIM_PROJ)/base/messages/ChannelSenseRequest_m.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	$(MIXIM_PROJ)/base/phyLayer/PhyUtils.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	$(MIXIM_PROJ)/base/phyLayer/ChannelInfo.h \
	$(MIXIM_PROJ)/base/connectionManager/ChannelAccess.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/phyLayer/MappingBase.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/base/phyLayer/MacToPhyInterface.h \
	$(MIXIM_PROJ)/base/phyLayer/Mapping.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/base/modules/BaseWorldUtility.h \
	$(MIXIM_PROJ)/modules/phy/Decider80211Battery.h \
	$(MIXIM_PROJ)/modules/phy/Decider80211.h \
	$(MIXIM_PROJ)/base/connectionManager/NicEntry.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/base/phyLayer/BasePhyLayer.h \
	$(MIXIM_PROJ)/modules/phy/PhyLayerControl.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/modules/utility/Consts80211.h \
	$(MIXIM_PROJ)/base/phyLayer/MappingUtils.h \
	$(MIXIM_PROJ)/base/phyLayer/Signal_.h \
	$(MIXIM_PROJ)/base/messages/AirFrame_m.h \
	$(MIXIM_PROJ)/base/phyLayer/MacToPhyControlInfo.h \
	$(MIXIM_PROJ)/base/messages/MacPkt_m.h
$O/myStationAppl.o: myStationAppl.cc \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	myData.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/modules/nic/NicController.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/modules/nic/NicStateNotify.h \
	$(MIXIM_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	myAppl.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	myBattery.h \
	wsnPkt_m.h \
	myStationAppl.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	stationAppl.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/modules/nic/NicCommandNotify.h \
	$(MIXIM_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/utils/NetwControlInfo.h
$O/timeStationAppl.o: timeStationAppl.cc \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	myData.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/modules/nic/NicController.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	timeStationAppl.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/modules/nic/NicStateNotify.h \
	$(MIXIM_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	myAppl.h \
	wsnPkt_m.h \
	myBattery.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_PROJ)/modules/nic/NicCommandNotify.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/utils/NetwControlInfo.h
$O/csmaControl.o: csmaControl.cc \
	$(MIXIM_PROJ)/base/phyLayer/AnalogueModel.h \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BaseMacLayer.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_PROJ)/base/connectionManager/BaseConnectionManager.h \
	$(MIXIM_PROJ)/modules/mac/csma.h \
	$(MIXIM_PROJ)/base/phyLayer/Interpolation.h \
	$(MIXIM_PROJ)/base/utils/Move.h \
	$(MIXIM_PROJ)/base/phyLayer/ChannelState.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	$(MIXIM_PROJ)/base/phyLayer/PhyUtils.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	csmaControl.h \
	$(MIXIM_PROJ)/base/phyLayer/MappingBase.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/base/phyLayer/MacToPhyInterface.h \
	$(MIXIM_PROJ)/base/phyLayer/Mapping.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/modules/utility/DroppedPacket.h \
	$(MIXIM_PROJ)/base/connectionManager/NicEntry.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/phyLayer/MappingUtils.h \
	$(MIXIM_PROJ)/base/phyLayer/Signal_.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/messages/MacPkt_m.h
$O/fusionAppl.o: fusionAppl.cc \
	$(MIXIM_PROJ)/base/modules/Blackboard.h \
	$(MIXIM_PROJ)/base/modules/BaseUtility.h \
	$(MIXIM_PROJ)/base/modules/BaseLayer.h \
	$(MIXIM_PROJ)/base/modules/BatteryAccess.h \
	fusionAppl.h \
	myData.h \
	$(MIXIM_PROJ)/modules/power/battery/BatteryState.h \
	$(MIXIM_PROJ)/modules/nic/NicController.h \
	$(MIXIM_PROJ)/base/modules/BaseBattery.h \
	sentMsgNotify.h \
	$(MIXIM_PROJ)/modules/power/battery/SimpleBattery.h \
	$(MIXIM_PROJ)/modules/nic/NicStateNotify.h \
	$(MIXIM_PROJ)/modules/nic/IControllable.h \
	$(MIXIM_PROJ)/base/utils/SimpleAddress.h \
	$(MIXIM_PROJ)/base/utils/FWMath.h \
	myAppl.h \
	myBattery.h \
	wsnPkt_m.h \
	$(MIXIM_PROJ)/base/utils/ImNotifiable.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	$(MIXIM_PROJ)/base/utils/FindModule.h \
	$(MIXIM_PROJ)/base/utils/Coord.h \
	$(MIXIM_PROJ)/modules/power/battery/DeviceEntry.h \
	$(MIXIM_PROJ)/base/utils/HostState.h \
	$(MIXIM_PROJ)/base/modules/BaseApplLayer.h \
	$(MIXIM_PROJ)/modules/nic/NicCommandNotify.h \
	$(MIXIM_PROJ)/base/modules/BaseModule.h \
	$(MIXIM_PROJ)/base/utils/PassedMessage.h \
	$(MIXIM_PROJ)/base/utils/NetwControlInfo.h
$O/wsnPkt_m.o: wsnPkt_m.cc \
	wsnPkt_m.h \
	$(MIXIM_PROJ)/base/messages/ApplPkt_m.h \
	myData.h

