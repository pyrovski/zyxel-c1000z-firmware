#***********************************************************************
#
#  Copyright (c) 2008  Broadcom Corporation
#  All Rights Reserved
#
# 
# 
# Unless you and Broadcom execute a separate written software license 
# agreement governing use of this software, this software is licensed 
# to you under the terms of the GNU General Public License version 2 
# (the "GPL"), available at http://www.broadcom.com/licenses/GPLv2.php, 
# with the following added to such license:
# 
#    As a special exception, the copyright holders of this software give 
#    you permission to link this software with independent modules, and 
#    to copy and distribute the resulting executable under terms of your 
#    choice, provided that you also meet, for each linked independent 
#    module, the terms and conditions of the license of that module. 
#    An independent module is a module which is not derived from this
#    software.  The special exception does not apply to any modifications 
#    of the software.  
# 
# Not withstanding the above, under no circumstances may you combine 
# this software in any way with any other Broadcom software provided 
# under a license other than the GPL, without Broadcom's express prior 
# written consent. 
#
#
#***********************************************************************/

VERSION_FILE=cms_version.h
COLORS_FILE=bcm_colors.h
ASSERT_FILE=bcm_assert.h
SWVERSION_FILE=bcm_swversion.h

all install: $(VERSION_FILE) $(COLORS_FILE) $(ASSERT_FILE) $(SWVERSION_FILE)

clean: force $(VERSION_FILE)

force:
	rm -f $(VERSION_FILE) $(COLORS_FILE) linux/$(COLORS_FILE)
	rm -f $(ASSERT_FILE) linux/$(ASSERT_FILE)
	rm -f $(SWVERSION_FILE)

modsw_clean: generic_modsw_base_clean


#
# Set our CommEngine directory (by splitting the pwd into two words
# at /userspace and taking the first word only).
# Then include the common defines under CommEngine.
# You do not need to modify this part.
#
CURR_DIR := $(shell pwd)
BUILD_DIR:=$(subst /userspace, /userspace,$(CURR_DIR))
BUILD_DIR:=$(word 1, $(BUILD_DIR))

include $(BUILD_DIR)/make.common



#
# We only need one rule in this Makefile, which is the one for dynamically
# generating the version.h file.
#
cms_version.h: $(BUILD_DIR)/version.make
	@echo "generating $(VERSION_FILE)"
	@echo "/* this is an automatically generated file.  Do not modify. */" > $(VERSION_FILE)
	@echo "/* modify version.make at the top. */" >> $(VERSION_FILE)
	@echo "#ifndef _CMS_VERSION_H_" >> $(VERSION_FILE)
	@echo "#define _CMS_VERSION_H_" >> $(VERSION_FILE)
	@echo -n "#define CMS_RELEASE_VERSION \"" >> $(VERSION_FILE)
	@echo -n $(BRCM_VERSION) >> $(VERSION_FILE)
	@echo -n "." >> $(VERSION_FILE)
	@echo -n $(BRCM_RELEASE) >> $(VERSION_FILE)
	@echo -n "L." >> $(VERSION_FILE)
	@echo -n $(BRCM_EXTRAVERSION) >> $(VERSION_FILE)
	@echo "\"" >> $(VERSION_FILE)
	@echo "#endif" >> $(VERSION_FILE)

bcm_colors.h:
	@echo "Making symlink to $(COLORS_FILE)"
	@ln -s ../../../kernel/linux/include/linux/bcm_colors.h .
	@(cd linux; ln -s ../bcm_colors.h .)

bcm_assert.h:
	@echo "Making symlink to $(ASSERT_FILE)"
	@ln -s ../../../kernel/linux/include/linux/bcm_assert.h .
	@(cd linux; ln -s ../bcm_assert.h .)
	
$(SWVERSION_FILE):
	@echo "Making symlink to $@"
	@ln -s ../../../kernel/linux/include/linux/bcm_swversion.h .
