LOCAL_PATH := $(call my-dir)
PROTOBUF_PATH := $(call my-dir)/../../../../Dependencies/protobuf
  
include $(CLEAR_VARS)  
 
LOCAL_MODULE := protobuf_static   
 
LOCAL_MODULE_FILENAME := libprotobuf   
 
LOCAL_SRC_FILES :=   $(PROTOBUF_PATH)/src/google/protobuf/io/coded_stream.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/stubs/common.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/descriptor.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/descriptor.pb.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/descriptor_database.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/dynamic_message.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/extension_set.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/extension_set_heavy.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/generated_message_reflection.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/generated_message_util.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/io/gzip_stream.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/compiler/importer.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/message.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/message_lite.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/stubs/once.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/compiler/parser.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/io/printer.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/reflection_ops.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/repeated_field.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/service.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/stubs/structurally_valid.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/stubs/strutil.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/stubs/substitute.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/text_format.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/io/tokenizer.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/unknown_field_set.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/wire_format.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/wire_format_lite.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/io/zero_copy_stream.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/io/zero_copy_stream_impl.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/io/zero_copy_stream_impl_lite.cc \
                     $(PROTOBUF_PATH)/src/google/protobuf/stubs/stringprintf.cc 
 
LOCAL_EXPORT_C_INCLUDES :=
LOCAL_EXPORT_LDLIBS := 
 
LOCAL_C_INCLUDES := $(PROTOBUF_PATH) \
                    $(PROTOBUF_PATH)/src \
                    $(LOCAL_PATH) 

LOCAL_EXPORT_C_INCLUDES := $(PROTOBUF_PATH)/src \
			   $(LOCAL_PATH)
 
LOCAL_LDLIBS := \
                -llog \
                -lz

include $(BUILD_STATIC_LIBRARY)