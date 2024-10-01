import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    # CONF_ID,
    # DEVICE_CLASS_ILLUMINANCE,
    UNIT_SECOND,
    UNIT_CENTIMETER,
    ENTITY_CATEGORY_CONFIG,
    ICON_TIMELAPSE,
    DEVICE_CLASS_DISTANCE,
    STATE_CLASS_MEASUREMENT
)
from .. import CONF_C1001_ID, C1001Component, c1001_ns

DwellTimeNumber = c1001_ns.class_("DwellTimeNumber", number.Number)
UnmannedTimeNumber = c1001_ns.class_("UnmannedTimeNumber", number.Number)
FallTimeNumber = c1001_ns.class_("FallTimeNumber", number.Number)
InstallHeightNumber = c1001_ns.class_("InstallHeightNumber", number.Number)

CONF_DWELL_TIME_LIMIT = "dwell_time_limit"
CONF_UNMANNED_TIME_LIMIT = "unmanned_time_limit"
CONF_FALL_TIME_LIMIT = "fall_time_limit"
CONF_INSTALL_HEIGHT = "install_height"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(CONF_C1001_ID): cv.use_id(C1001Component),
        cv.Optional(CONF_DWELL_TIME_LIMIT): number.number_schema(
            DwellTimeNumber,
            unit_of_measurement=UNIT_SECOND,
            # device_class=DEVICE_CLASS_ILLUMINANCE,
            entity_category=ENTITY_CATEGORY_CONFIG,
            icon=ICON_TIMELAPSE,
        ),
        cv.Optional(CONF_UNMANNED_TIME_LIMIT): number.number_schema(
            UnmannedTimeNumber,
            unit_of_measurement=UNIT_SECOND,
            # device_class=DEVICE_CLASS_ILLUMINANCE,
            entity_category=ENTITY_CATEGORY_CONFIG,
            icon=ICON_TIMELAPSE,
        ),
        cv.Optional(CONF_FALL_TIME_LIMIT): number.number_schema(
            FallTimeNumber,
            unit_of_measurement=UNIT_SECOND,
            # device_class=DEVICE_CLASS_ILLUMINANCE,
            entity_category=ENTITY_CATEGORY_CONFIG,
            icon=ICON_TIMELAPSE,
        ),
        cv.Optional(CONF_INSTALL_HEIGHT): number.number_schema(
            InstallHeightNumber,
            device_class=DEVICE_CLASS_DISTANCE,
            unit_of_measurement=UNIT_CENTIMETER,
            # accuracy_decimals=0,
            entity_category=ENTITY_CATEGORY_CONFIG,
            # state_class=STATE_CLASS_MEASUREMENT,
        ),
    }
)

async def to_code(config):
    c1001_component = await cg.get_variable(config[CONF_C1001_ID])

    if dwell_limit_config := config.get(CONF_DWELL_TIME_LIMIT):
        n = await number.new_number(dwell_limit_config, min_value=60, max_value=300, step=1)
        await cg.register_parented(n, config[CONF_C1001_ID])
        cg.add(c1001_component.set_dwell_time_number(n))

    if unmanned_time_limit_config := config.get(CONF_UNMANNED_TIME_LIMIT):
        n = await number.new_number(unmanned_time_limit_config, min_value=1, max_value=60, step=1)
        await cg.register_parented(n, config[CONF_C1001_ID])
        cg.add(c1001_component.set_unmanned_time_number(n))

    if fall_time_limit_config := config.get(CONF_FALL_TIME_LIMIT):
        n = await number.new_number(fall_time_limit_config, min_value=1, max_value=60, step=1)
        await cg.register_parented(n, config[CONF_C1001_ID])
        cg.add(c1001_component.set_fall_time_number(n))

    if install_height_config := config.get(CONF_INSTALL_HEIGHT):
        n = await number.new_number(install_height_config, min_value=1, max_value=60, step=1)
        await cg.register_parented(n, config[CONF_C1001_ID])
        cg.add(c1001_component.set_install_height_number(n))
