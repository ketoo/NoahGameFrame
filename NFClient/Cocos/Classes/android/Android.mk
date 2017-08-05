LOCAL_PATH := $(call my-dir)/..

include $(CLEAR_VARS)

$(call import-add-path,$(LOCAL_PATH)/../../..)

LOCAL_MODULE := Classes_static

LOCAL_MODULE_FILENAME := libClasses

LOCAL_SRC_FILES := AppDelegate.cpp \
HelloWorldScene.cpp \
NF/Logic/NFCChatLogic.cpp \
NF/Logic/NFCLogicBase.cpp \
NF/Logic/NFCLogicPlugin.cpp \
NF/Logic/NFCLoginLogic.cpp \
NF/Logic/NFCNetLogic.cpp \
NF/Logic/NFCPlayerLogic.cpp \
NF/Logic/NFCPropertyLogic.cpp \
NF/Logic/NFCRecordLogic.cpp \
NF/NFCClientPlugin.cpp \
NF/NFCRoot.cpp \
NF/Scene/NFCGameScene.cpp \
NF/Scene/NFCSceneManager.cpp \
NF/Scene/NFCScenePlugin.cpp \
NF/UI/NFCUIChatPanel.cpp \
NF/UI/NFCUIDialog.cpp \
NF/UI/NFCUILogin.cpp \
NF/UI/NFCUIManager.cpp \
NF/UI/NFCUIPlugin.cpp \
NF/UI/NFCUISelectRole.cpp \
NF/UI/NFCUISelectServer.cpp \

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
$(LOCAL_PATH)/NF \
$(LOCAL_PATH)/Scene \
$(LOCAL_PATH)/UI \

LOCAL_WHOLE_STATIC_LIBRARIES := cocos2dx_static nf_static protobuf_static event_static

include $(BUILD_STATIC_LIBRARY)

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH) \
$(LOCAL_PATH)/NF \
$(LOCAL_PATH)/Scene \
$(LOCAL_PATH)/UI \

LOCAL_CFLAGS += -DPROTOBUF_SRC
LOCAL_CFLAGS += -DLIBEVENT_SRC
LOCAL_CFLAGS += -DNF_STATIC_LIB

$(call import-module,nf)
$(call import-module,protobuf)
$(call import-module,libevent)
$(call import-module,.)

