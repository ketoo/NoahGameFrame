LOCAL_PATH := $(call my-dir)
EVENT_PATH := $(call my-dir)/../../../../Dependencies/libevent
include $(CLEAR_VARS)

LOCAL_MODULE := event_static   
 
LOCAL_MODULE_FILENAME := libevent   

LOCAL_SRC_FILES :=  \
	$(EVENT_PATH)/buffer.c \
	$(EVENT_PATH)/bufferevent.c \
	$(EVENT_PATH)/bufferevent_filter.c \
	$(EVENT_PATH)/bufferevent_pair.c \
	$(EVENT_PATH)/bufferevent_ratelim.c \
	$(EVENT_PATH)/bufferevent_sock.c\
	$(EVENT_PATH)/epoll.c \
	$(EVENT_PATH)/epoll_sub.c \
	$(EVENT_PATH)/evdns.c \
	$(EVENT_PATH)/event.c \
	$(EVENT_PATH)/event_tagging.c \
	$(EVENT_PATH)/evmap.c \
	$(EVENT_PATH)/evrpc.c \
	$(EVENT_PATH)/evthread.c \
	$(EVENT_PATH)/evthread_pthread.c \
	$(EVENT_PATH)/evutil.c \
	$(EVENT_PATH)/evutil_time.c\
	$(EVENT_PATH)/evutil_rand.c\
	$(EVENT_PATH)/http.c \
	$(EVENT_PATH)/listener.c \
	$(EVENT_PATH)/log.c \
	$(EVENT_PATH)/poll.c \
	$(EVENT_PATH)/select.c \
	$(EVENT_PATH)/signal.c \
	$(EVENT_PATH)/strlcpy.c 

LOCAL_C_INCLUDES := $(EVENT_PATH) \
                    $(EVENT_PATH)/include \
		    $(LOCAL_PATH) \

LOCAL_EXPORT_C_INCLUDES := $(EVENT_PATH)/include \
			   $(LOCAL_PATH)

include $(BUILD_STATIC_LIBRARY)