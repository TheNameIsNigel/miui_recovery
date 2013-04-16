LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

commands_recovery_local_path := $(LOCAL_PATH)

LOCAL_SRC_FILES := \
    bootloader.c \
    firmware.c \
    install.c \
    recovery.c \
    recovery_ui.c \
    roots.c \
    miui_intent.c \
	mount.c \
    nandroid.c \
    power.c \
    ../../system/core/toolbox/reboot.c \
    setprop.c \
    verifier.c

ADDITIONAL_RECOVERY_FILES := $(shell echo $$ADDITIONAL_RECOVERY_FILES)
LOCAL_SRC_FILES += $(ADDITIONAL_RECOVERY_FILES)

LOCAL_MODULE := recovery

LOCAL_FORCE_STATIC_EXECUTABLE := true

RECOVERY_API_VERSION := 3
MYDEFINE_CFLAGS :=  -D_GLIBCXX_DEBUG_PEDANTIC \
                  -DFT2_BUILD_LIBRARY=1 \
                  -DDARWIN_NO_CARBON \
				  -D_MIUI_NODEBUG=1

LOCAL_CFLAGS += -DRECOVERY_API_VERSION=$(RECOVERY_API_VERSION) 
LOCAL_CFLAGS += $(MYDEFINE_CFLAGS)

LOCAL_STATIC_LIBRARIES :=

LOCAL_CFLAGS += -DUSE_EXT4
LOCAL_C_INCLUDES += system/extras/ext4_utils
LOCAL_STATIC_LIBRARIES += libext4_utils libz

# This binary is in the recovery ramdisk, which is otherwise a copy of root.
# It gets copied there in config/Makefile.  LOCAL_MODULE_TAGS suppresses
# a (redundant) copy of the binary in /system/bin for user builds.
# TODO: Build the ramdisk image in a more principled way.

LOCAL_MODULE_TAGS := eng

#LOCAL_C_INCLUDES += $(LOCAL_PATH)/include
#LOCAL_LDLIBS += $(LOCAL_PATH)/lib

LOCAL_STATIC_LIBRARIES += libext4_utils libz
LOCAL_STATIC_LIBRARIES += libminzip libunz libmincrypt

LOCAL_STATIC_LIBRARIES += libedify libcrecovery libflashutils libmmcutils libbmlutils libmtdutils
LOCAL_STATIC_LIBRARIES += libmkyaffs2image libunyaffs liberase_image libdump_image libflash_image
LOCAL_STATIC_LIBRARIES += libcrypto_static

ifeq ($(BOARD_USES_BML_OVER_MTD),true)
LOCAL_STATIC_LIBRARIES += libbml_over_mtd
endif

LOCAL_STATIC_LIBRARIES += libmiui libminui libminizip libpixelflinger_static libpng libcutils
LOCAL_STATIC_LIBRARIES += libstdc++ libc libm

LOCAL_C_INCLUDES += system/extras/ext4_utils

include $(BUILD_EXECUTABLE)

RECOVERY_LINKS := flash_image dump_image mkyaffs2image unyaffs erase_image nandroid reboot setprop minizip

# nc is provided by external/netcat
RECOVERY_SYMLINKS := $(addprefix $(TARGET_RECOVERY_ROOT_OUT)/sbin/,$(RECOVERY_LINKS))
$(RECOVERY_SYMLINKS): RECOVERY_BINARY := $(LOCAL_MODULE)
$(RECOVERY_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "Symlink: $@ -> $(RECOVERY_BINARY)"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf $(RECOVERY_BINARY) $@

ALL_DEFAULT_INSTALLED_MODULES += $(RECOVERY_SYMLINKS)

BUSYBOX_PATH := $(LOCAL_PATH)/busybox
# Now let's do recovery symlinks
BUSYBOX_LINKS := $(shell cat $(LOCAL_PATH)/busybox/busybox-minimal.links) 
exclude := tune2fs mke2fs
RECOVERY_BUSYBOX_SYMLINKS := $(addprefix $(TARGET_ROOT_OUT)/sbin/,$(filter-out $(exclude),$(notdir $(BUSYBOX_LINKS))))
$(RECOVERY_BUSYBOX_SYMLINKS): BUSYBOX_BINARY := busybox
$(RECOVERY_BUSYBOX_SYMLINKS): $(LOCAL_INSTALLED_MODULE)
	@echo "Symlink: $@ -> $(BUSYBOX_BINARY)"
	@mkdir -p $(dir $@)
	@rm -rf $@
	$(hide) ln -sf $(BUSYBOX_BINARY) $@

ALL_DEFAULT_INSTALLED_MODULES += $(RECOVERY_BUSYBOX_SYMLINKS)

RECOVERY_BUSYBOX := $(TARGET_ROOT_OUT)/bin/busybox
$(RECOVERY_BUSYBOX): 
	@cp $(BUSYBOX_PATH)/busybox $(TARGET_ROOT_OUT)/sbin/ -f
	#@cp $(BUSYBOX_PATH)/adbd $(TARGET_ROOT_OUT)/sbin/ 

ALL_DEFAULT_INSTALLED_MODULES += $(RECOVERY_BUSYBOX) 

include $(CLEAR_VARS)

LOCAL_SRC_FILES := verifier_test.c verifier.c

LOCAL_MODULE := verifier_test

LOCAL_FORCE_STATIC_EXECUTABLE := true

LOCAL_MODULE_TAGS := tests

LOCAL_STATIC_LIBRARIES := libmincrypt libcutils libstdc++ libc

include $(BUILD_EXECUTABLE)
include $(commands_recovery_local_path)/bmlutils/Android.mk
include $(commands_recovery_local_path)/flashutils/Android.mk
include $(commands_recovery_local_path)/libcrecovery/Android.mk
include $(commands_recovery_local_path)/miui/Android.mk
include $(commands_recovery_local_path)/minui/Android.mk
include $(commands_recovery_local_path)/minelf/Android.mk
include $(commands_recovery_local_path)/minzip/Android.mk
include $(commands_recovery_local_path)/mtdutils/Android.mk
include $(commands_recovery_local_path)/mmcutils/Android.mk
include $(commands_recovery_local_path)/tools/Android.mk
include $(commands_recovery_local_path)/edify/Android.mk
include $(commands_recovery_local_path)/updater/Android.mk
include $(commands_recovery_local_path)/applypatch/Android.mk
include $(commands_recovery_local_path)/utilities/Android.mk
commands_recovery_local_path :=
