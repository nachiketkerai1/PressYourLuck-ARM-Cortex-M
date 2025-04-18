#
# This file is generated by ModusToolbox during the 'make getlibs' operation
# Any edits to this file will be lost the next time the library manager is run or
# the next time 'make getlibs' is run.
#
# List of local libraries


# Path to the current BSP
SEARCH_TARGET_ece353=bsps/TARGET_ece353

# The search paths for the included middleware
SEARCH_freertos=../mtb_shared/freertos/release-v10.5.002
SEARCH_retarget-io=../mtb_shared/retarget-io/release-v1.5.0
SEARCH_abstraction-rtos=../mtb_shared/abstraction-rtos/release-v1.8.2
SEARCH_cat1cm0p=../mtb_shared/cat1cm0p/release-v1.5.0
SEARCH_clib-support=../mtb_shared/clib-support/release-v1.5.0
SEARCH_cmsis=../mtb_shared/cmsis/release-v5.8.0
SEARCH_core-lib=../mtb_shared/core-lib/release-v1.4.2
SEARCH_core-make=../mtb_shared/core-make/release-v3.3.1
SEARCH_mtb-hal-cat1=../mtb_shared/mtb-hal-cat1/release-v2.6.1
SEARCH_mtb-pdl-cat1=../mtb_shared/mtb-pdl-cat1/release-v3.11.0
SEARCH_recipe-make-cat1a=../mtb_shared/recipe-make-cat1a/release-v2.2.1

# Search libraries added to build
SEARCH_MTB_MK+=$(SEARCH_freertos)
SEARCH_MTB_MK+=$(SEARCH_retarget-io)
SEARCH_MTB_MK+=$(SEARCH_abstraction-rtos)
SEARCH_MTB_MK+=$(SEARCH_cat1cm0p)
SEARCH_MTB_MK+=$(SEARCH_clib-support)
SEARCH_MTB_MK+=$(SEARCH_cmsis)
SEARCH_MTB_MK+=$(SEARCH_core-lib)
SEARCH_MTB_MK+=$(SEARCH_core-make)
SEARCH_MTB_MK+=$(SEARCH_mtb-hal-cat1)
SEARCH_MTB_MK+=$(SEARCH_mtb-pdl-cat1)
SEARCH_MTB_MK+=$(SEARCH_recipe-make-cat1a)

-include $(CY_INTERNAL_APP_PATH)/importedbsp.mk
COMPONENTS += MW_FREERTOS
COMPONENTS += MW_RETARGET_IO
COMPONENTS += MW_ABSTRACTION_RTOS
COMPONENTS += MW_CAT1CM0P
COMPONENTS += MW_CLIB_SUPPORT
COMPONENTS += MW_CMSIS
COMPONENTS += MW_CORE_LIB
COMPONENTS += MW_CORE_MAKE
COMPONENTS += MW_MTB_HAL_CAT1
COMPONENTS += MW_MTB_PDL_CAT1
COMPONENTS += MW_RECIPE_MAKE_CAT1A

# Register map file
DEVICE_CY8C6244AZI-S4D93_SVD=$(SEARCH_mtb-pdl-cat1)/devices/COMPONENT_CAT1A/svd/psoc6_04.svd


#
# generate make targets for the graphical editors that are specific to this project
#

modlibs:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name library-manager
.PHONY: modlibs

CY_HELP_modlibs=Launches the Library Manager 2.20 GUI
CY_HELP_modlibs_VERBOSE=Launches the Library Manager 2.20 GUI. Check the Library Manager 2.20 User Guide for more information.
mtb_help_tool_modlibs:
	@:
	$(info $(MTB__SPACE)modlibs             $(CY_HELP_modlibs))

mtb_help_tools_end: mtb_help_tool_modlibs
mtb_help_tool_modlibs: mtb_help_tools_start
.PHONY: mtb_help_tool_modlibs

library-manager:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name library-manager
.PHONY: library-manager

CY_HELP_library-manager=Launches the Library Manager 2.20 GUI
CY_HELP_library-manager_VERBOSE=Launches the Library Manager 2.20 GUI. Check the Library Manager 2.20 User Guide for more information.
mtb_help_tool_library-manager:
	@:
	$(info $(MTB__SPACE)library-manager     $(CY_HELP_library-manager))

mtb_help_tools_end: mtb_help_tool_library-manager
mtb_help_tool_library-manager: mtb_help_tools_start
.PHONY: mtb_help_tool_library-manager

qspi-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name qspi-configurator
.PHONY: qspi-configurator

CY_HELP_qspi-configurator=Launches the QSPI Configurator 4.30 GUI for the target's cyqspi file
CY_HELP_qspi-configurator_VERBOSE=Launches the QSPI Configurator 4.30 GUI. Check the QSPI Configurator 4.30 User Guide for more information.
mtb_help_tool_qspi-configurator:
	@:
	$(info $(MTB__SPACE)qspi-configurator   $(CY_HELP_qspi-configurator))

mtb_help_tools_end: mtb_help_tool_qspi-configurator
mtb_help_tool_qspi-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_qspi-configurator

config:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name device-configurator
.PHONY: config

CY_HELP_config=Launches the Device Configurator 4.20 GUI for the target's modus file
CY_HELP_config_VERBOSE=Launches the Device Configurator 4.20 GUI. Check the Device Configurator 4.20 User Guide for more information.
mtb_help_tool_config:
	@:
	$(info $(MTB__SPACE)config              $(CY_HELP_config))

mtb_help_tools_end: mtb_help_tool_config
mtb_help_tool_config: mtb_help_tools_start
.PHONY: mtb_help_tool_config

device-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name device-configurator
.PHONY: device-configurator

CY_HELP_device-configurator=Launches the Device Configurator 4.20 GUI for the target's modus file
CY_HELP_device-configurator_VERBOSE=Launches the Device Configurator 4.20 GUI. Check the Device Configurator 4.20 User Guide for more information.
mtb_help_tool_device-configurator:
	@:
	$(info $(MTB__SPACE)device-configurator $(CY_HELP_device-configurator))

mtb_help_tools_end: mtb_help_tool_device-configurator
mtb_help_tool_device-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_device-configurator

capsense-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name capsense-configurator
.PHONY: capsense-configurator

CY_HELP_capsense-configurator=Launches the CAPSENSE™ Configurator 6.20 GUI for the target's cycapsense file
CY_HELP_capsense-configurator_VERBOSE=Launches the CAPSENSE™ Configurator 6.20 GUI. Check the CAPSENSE™ Configurator 6.20 User Guide for more information.
mtb_help_tool_capsense-configurator:
	@:
	$(info $(MTB__SPACE)capsense-configurator $(CY_HELP_capsense-configurator))

mtb_help_tools_end: mtb_help_tool_capsense-configurator
mtb_help_tool_capsense-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_capsense-configurator

capsense-tuner:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name capsense-tuner
.PHONY: capsense-tuner

CY_HELP_capsense-tuner=Launches the CAPSENSE™ Tuner 6.20 GUI for the target's cycapsense file
CY_HELP_capsense-tuner_VERBOSE=Launches the CAPSENSE™ Tuner 6.20 GUI. Check the CAPSENSE™ Tuner 6.20 User Guide for more information.
mtb_help_tool_capsense-tuner:
	@:
	$(info $(MTB__SPACE)capsense-tuner      $(CY_HELP_capsense-tuner))

mtb_help_tools_end: mtb_help_tool_capsense-tuner
mtb_help_tool_capsense-tuner: mtb_help_tools_start
.PHONY: mtb_help_tool_capsense-tuner

seglcd-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name seglcd-configurator
.PHONY: seglcd-configurator

CY_HELP_seglcd-configurator=Launches the Segment LCD Configurator 1.60 GUI for the target's cyseglcd file
CY_HELP_seglcd-configurator_VERBOSE=Launches the Segment LCD Configurator 1.60 GUI. Check the Segment LCD Configurator 1.60 User Guide for more information.
mtb_help_tool_seglcd-configurator:
	@:
	$(info $(MTB__SPACE)seglcd-configurator $(CY_HELP_seglcd-configurator))

mtb_help_tools_end: mtb_help_tool_seglcd-configurator
mtb_help_tool_seglcd-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_seglcd-configurator

bsp-assistant:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name bsp-assistant
.PHONY: bsp-assistant

CY_HELP_bsp-assistant=Launches the BSP Assistant 1.20 GUI
CY_HELP_bsp-assistant_VERBOSE=Launches the BSP Assistant 1.20 GUI. Check the BSP Assistant 1.20 User Guide for more information.
mtb_help_tool_bsp-assistant:
	@:
	$(info $(MTB__SPACE)bsp-assistant       $(CY_HELP_bsp-assistant))

mtb_help_tools_end: mtb_help_tool_bsp-assistant
mtb_help_tool_bsp-assistant: mtb_help_tools_start
.PHONY: mtb_help_tool_bsp-assistant

smartio-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name smartio-configurator
.PHONY: smartio-configurator

CY_HELP_smartio-configurator=Launches the Smart I/O Configurator 4.20 GUI for the target's modus file
CY_HELP_smartio-configurator_VERBOSE=Launches the Smart I/O Configurator 4.20 GUI. Check the Smart I/O Configurator 4.20 User Guide for more information.
mtb_help_tool_smartio-configurator:
	@:
	$(info $(MTB__SPACE)smartio-configurator $(CY_HELP_smartio-configurator))

mtb_help_tools_end: mtb_help_tool_smartio-configurator
mtb_help_tool_smartio-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_smartio-configurator

config_lin:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name lin-configurator
.PHONY: config_lin

CY_HELP_config_lin=Launches the LIN Configurator 1.30 GUI for the target's mtblin file
CY_HELP_config_lin_VERBOSE=Launches the LIN Configurator 1.30 GUI. Check the LIN Configurator 1.30 User Guide for more information.
mtb_help_tool_config_lin:
	@:
	$(info $(MTB__SPACE)config_lin          $(CY_HELP_config_lin))

mtb_help_tools_end: mtb_help_tool_config_lin
mtb_help_tool_config_lin: mtb_help_tools_start
.PHONY: mtb_help_tool_config_lin

lin-configurator:
	$(CY_TOOL_mtblaunch_EXE_ABS) --project . --short-name lin-configurator
.PHONY: lin-configurator

CY_HELP_lin-configurator=Launches the LIN Configurator 1.30 GUI for the target's mtblin file
CY_HELP_lin-configurator_VERBOSE=Launches the LIN Configurator 1.30 GUI. Check the LIN Configurator 1.30 User Guide for more information.
mtb_help_tool_lin-configurator:
	@:
	$(info $(MTB__SPACE)lin-configurator    $(CY_HELP_lin-configurator))

mtb_help_tools_end: mtb_help_tool_lin-configurator
mtb_help_tool_lin-configurator: mtb_help_tools_start
.PHONY: mtb_help_tool_lin-configurator

.PHONY: mtb_help_tools_start mtb_help_tools_end
