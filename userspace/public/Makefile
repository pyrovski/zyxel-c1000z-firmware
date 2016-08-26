#***********************************************************************
#
#  Copyright (c) 2009  Broadcom Corporation
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

#
# You probably don't need to modify anything in this Makefile.
#



#
# This is the list of directories to traverse.  
# Note that when you type make from the top, it does not actually
# go through this Makefile.  See the Makefile one level above here.
# But when you type make clean from the top, it does go through this Makefile.
#
PUBLIC_DIRS = include libs apps


#
# We need to declare these directory names as phony targets,
# otherwise make will think the directories themselves are
# the targets and will never execute the cmds.
# See GNU Make, 4.6 
#
.PHONY : $(PUBLIC_DIRS)
all    : $(PUBLIC_DIRS)


#
# Generic rule for making the dirs
#
$(PUBLIC_DIRS):
	$(MAKE) -C $@


#
# Cleaning rules
# See example in GNU Make, 4.6
#
clean: 
	for dir in $(PUBLIC_DIRS); do \
		$(MAKE) -C $$dir clean; \
	done



