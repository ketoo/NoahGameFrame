LOCAL_PATH := $(call my-dir)/..
NF_PATH := $(call my-dir)/../../../..

include $(CLEAR_VARS)

LOCAL_MODULE := nf_static

LOCAL_MODULE_FILENAME := libnf

LOCAL_CFLAGS += -fexceptions

LOCAL_SRC_FILES := \
$(NF_PATH)/NFComm/NFCore/NFCObject.cpp \
$(NF_PATH)/NFComm/NFCore/NFCProperty.cpp \
$(NF_PATH)/NFComm/NFCore/NFCPropertyManager.cpp \
$(NF_PATH)/NFComm/NFCore/NFCRecord.cpp \
$(NF_PATH)/NFComm/NFCore/NFCRecordManager.cpp \
$(NF_PATH)/NFComm/NFPluginLoader/NFCPluginManager.cpp \
$(NF_PATH)/NFComm/NFPluginLoader/NFCDynLib.cpp \
$(NF_PATH)/NFComm/NFConfigPlugin/NFCClassModule.cpp \
$(NF_PATH)/NFComm/NFConfigPlugin/NFCElementModule.cpp \
$(NF_PATH)/NFComm/NFKernelPlugin/NFCEventModule.cpp \
$(NF_PATH)/NFComm/NFKernelPlugin/NFCKernelModule.cpp \
$(NF_PATH)/NFComm/NFKernelPlugin/NFCSceneAOIModule.cpp \
$(NF_PATH)/NFComm/NFKernelPlugin/NFCScheduleModule.cpp \
$(NF_PATH)/NFComm/NFKernelPlugin/NFKernelPlugin.cpp \
$(NF_PATH)/NFComm/NFLogPlugin/NFCLogModule.cpp \
$(NF_PATH)/NFComm/NFNetPlugin/NFCNetClientModule.cpp \
$(NF_PATH)/NFComm/NFNetPlugin/NFCNetModule.cpp \
$(NF_PATH)/NFComm/NFNetPlugin/NFCNet.cpp \
$(NF_PATH)/NFComm/NFMessageDefine/NFDefine.pb.cc \
$(NF_PATH)/NFComm/NFMessageDefine/NFFleetingDefine.pb.cc \
$(NF_PATH)/NFComm/NFMessageDefine/NFMsgBase.pb.cc \
$(NF_PATH)/NFComm/NFMessageDefine/NFMsgBaseEx.pb.cc \
$(NF_PATH)/NFComm/NFMessageDefine/NFMsgDefine.cpp \
$(NF_PATH)/NFComm/NFMessageDefine/NFMsgPreGame.pb.cc \
$(NF_PATH)/NFComm/NFMessageDefine/NFMsgShare.pb.cc \
$(NF_PATH)/NFComm/NFMessageDefine/NFMsgURl.pb.cc \

LOCAL_C_INCLUDES := $(NF_PATH) \
$(NF_PATH)/Dependencies \

LOCAL_WHOLE_STATIC_LIBRARIES := protobuf_static event_static


LOCAL_EXPORT_C_INCLUDES := $(NF_PATH) \
$(NF_PATH)/Dependencies \

LOCAL_CFLAGS += -DPROTOBUF_SRC
LOCAL_CFLAGS += -DLIBEVENT_SRC
LOCAL_CFLAGS += -DNF_STATIC_LIB

include $(BUILD_STATIC_LIBRARY)

$(call import-module,protobuf)
$(call import-module,libevent)