import esphome.codegen as cg
from esphome.components import button
import esphome.config_validation as cv
from esphome.const import (
    CONF_RESTART,
    DEVICE_CLASS_RESTART,
    ENTITY_CATEGORY_DIAGNOSTIC,
    ICON_RESTART,
)
from .. import CONF_C1001_ID, C1001Component, c1001_ns

RestartButton = c1001_ns.class_("RestartButton", button.Button)


CONFIG_SCHEMA = {
    cv.GenerateID(CONF_C1001_ID): cv.use_id(C1001Component),
    cv.Optional(CONF_RESTART): button.button_schema(
        RestartButton,
        device_class=DEVICE_CLASS_RESTART,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        icon=ICON_RESTART,
    ),
}


async def to_code(config):
    c1001_component = await cg.get_variable(config[CONF_C1001_ID])
    if restart_config := config.get(CONF_RESTART):
        b = await button.new_button(restart_config)
        await cg.register_parented(b, config[CONF_C1001_ID])
        cg.add(c1001_component.set_restart_button(b))
