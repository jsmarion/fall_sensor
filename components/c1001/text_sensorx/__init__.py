# import esphome.codegen as cg
# import esphome.config_validation as cv
# from esphome.components import uart, text_sensor
# from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY

# AUTO_LOAD = [ 'text_sensor' ]

# c1001_ns = cg.esphome_ns.namespace("c1001")

# C1001 = c1001_ns.class_("C1001", cg.PollingComponent, uart.UARTDevice)
# C1001TextSensor = c1001_ns.class_("C1001TextSensor", text_sensor.TextSensor, cg.Component)

# CONF_C1001 = "C1001"
# CONF_THE_TEXT = "c1001_text_sensor"

# CONFIG_SCHEMA = cv.Schema({
#     cv.GenerateID(CONF_C1001): cv.use_id(C1001),
#     cv.Optional(CONF_THE_TEXT): text_sensor.text_sensor_schema(text_sensor.TextSensor),
# })

# # async def to_code(config):
# #     var = await cg.get_variable(config[CONF_C1001])

# #     if CONF_THE_TEXT in config:
# #         sens = await text_sensor.new_text_sensor(config[CONF_THE_TEXT])
# #         # cg.add(sens.set_the_text("sensor text"))

# async def to_code(config):
#     var = await cg.get_variable(config[CONF_C1001])

#     if conf := config.get(CONF_THE_TEXT):
#       ts = await text_sensor.new_text_sensor(conf)
#       await cg.register_component(ts, config)
#       cg.add(ts.set_parent(var))




import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, text_sensor
from esphome.const import CONF_ID

AUTO_LOAD = [ 'text_sensor' ]

c1001_ns = cg.esphome_ns.namespace("c1001")

C1001 = c1001_ns.class_("C1001", cg.PollingComponent, uart.UARTDevice)
C1001TextSensor = c1001_ns.class_("C1001TextSensor", text_sensor.TextSensor, cg.Component)

CONF_C1001 = "C1001"
CONF_THE_TEXT = "c1001_text_sensor"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_C1001): cv.use_id(C1001),
    cv.Optional(CONF_THE_TEXT): text_sensor.text_sensor_schema(C1001TextSensor),
    })

# async def to_code(config):
#     var = await cg.get_variable(config[CONF_C1001])

#     bs = await binary_sensor.new_binary_sensor(config[CONF_THE_BINARY])
#     cg.add(bs.set_parent(var))

async def to_code(config):
    var = await cg.get_variable(config[CONF_C1001])

    if conf := config.get(CONF_THE_TEXT):
      bs = await text_sensor.new_text_sensor(conf)
      await cg.register_component(bs, config)
      cg.add(bs.set_parent(var))