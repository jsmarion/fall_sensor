import esphome.codegen as cg
from esphome.components import select
import esphome.config_validation as cv
from esphome.const import (
    ENTITY_CATEGORY_CONFIG,
    ICON_RULER,
)
from .. import CONF_C1001_ID, C1001Component, c1001_ns

CONF_FALL_SENSITIVITY = "fall_sensitivity"

FallSensitivitySelect = c1001_ns.class_("FallSensitivitySelect", select.Select)

CONFIG_SCHEMA = {
    cv.GenerateID(CONF_C1001_ID): cv.use_id(C1001Component),
    cv.Optional(CONF_FALL_SENSITIVITY): select.select_schema(
        FallSensitivitySelect,
        entity_category=ENTITY_CATEGORY_CONFIG,
        icon=ICON_RULER,
    ),
}


async def to_code(config):
    c1001_component = await cg.get_variable(config[CONF_C1001_ID])
    if fall_sensitivity_config := config.get(CONF_FALL_SENSITIVITY):
        s = await select.new_select(
            fall_sensitivity_config,
            options=[
                "0",
                "1",
                "2",
                "3",
            ],
        )
        await cg.register_parented(s, config[CONF_C1001_ID])
        cg.add(c1001_component.set_sensitivity_select(s))