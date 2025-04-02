import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, text_sensor
# from esphome.const import CONF_ID
from .. import CONF_C1001_ID, C1001Component, c1001_ns

AUTO_LOAD = [ 'text_sensor' ]

# c1001_ns = cg.esphome_ns.namespace("c1001")

# C1001 = c1001_ns.class_("C1001", cg.PollingComponent, uart.UARTDevice)
C1001TextSensor = c1001_ns.class_("C1001TextSensor", text_sensor.TextSensor, cg.Component)

# CONF_C1001 = "C1001"
CONF_THE_TEXT = "c1001_text_sensor"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_C1001_ID): cv.use_id(C1001Component),
    cv.Optional(CONF_THE_TEXT): text_sensor.text_sensor_schema(C1001TextSensor),
    })

async def to_code(config):
    var = await cg.get_variable(config[CONF_C1001_ID])

    if conf := config.get(CONF_THE_TEXT):
      bs = await text_sensor.new_text_sensor(conf)
      await cg.register_component(bs, config)
      cg.add(bs.set_parent(var))