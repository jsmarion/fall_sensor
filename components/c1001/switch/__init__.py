import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, switch
# from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY

AUTO_LOAD = [ 'switch' ]

c1001_ns = cg.esphome_ns.namespace("c1001")

C1001 = c1001_ns.class_("C1001Component", cg.PollingComponent, uart.UARTDevice)
C1001FallLEDSwitch = c1001_ns.class_("C1001FallLEDSwitch", switch.Switch, cg.Component)
C1001WorkModeSwitch = c1001_ns.class_("C1001WorkModeSwitch", switch.Switch, cg.Component)

CONF_C1001 = "C1001"
CONF_LED_SWITCH = "fall_led_switch"
CONF_MODE_SWITCH = "work_mode_switch"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_C1001): cv.use_id(C1001),
    # cv.Optional(CONF_LED_SWITCH): switch.SWITCH_SCHEMA.extend({cv.GenerateID(): cv.declare_id(C1001FallLEDSwitch)}),
    # cv.Optional(CONF_MODE_SWITCH): switch.SWITCH_SCHEMA.extend({cv.GenerateID(): cv.declare_id(C1001WorkModeSwitch)}),
    cv.Optional(CONF_LED_SWITCH): switch.switch_schema(C1001FallLEDSwitch),
    cv.Optional(CONF_MODE_SWITCH): switch.switch_schema(C1001WorkModeSwitch),
})

async def to_code(config):
    var = await cg.get_variable(config[CONF_C1001])

    if conf := config.get(CONF_LED_SWITCH):
      lsw = await switch.new_switch(config[CONF_LED_SWITCH])
      await cg.register_component(lsw, config)
      cg.add(lsw.set_parent(var))

    if conf := config.get(CONF_MODE_SWITCH):
      msw = await switch.new_switch(config[CONF_MODE_SWITCH])
      await cg.register_component(msw, config)
      cg.add(msw.set_parent(var))
