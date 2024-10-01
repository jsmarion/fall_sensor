import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, binary_sensor
from esphome.const import CONF_ID

AUTO_LOAD = [ 'binary_sensor' ]

c1001_ns = cg.esphome_ns.namespace("c1001")

C1001 = c1001_ns.class_("C1001", cg.PollingComponent, uart.UARTDevice)
C1001BinarySensor = c1001_ns.class_("C1001BinarySensor", binary_sensor.BinarySensor, cg.Component)


CONF_THE_BINARY = "c1001_binary_sensor"

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_C1001): cv.use_id(C1001),
    cv.Optional(CONF_THE_BINARY): binary_sensor.binary_sensor_schema(C1001BinarySensor),
    })

# async def to_code(config):
#     var = await cg.get_variable(config[CONF_C1001])

#     bs = await binary_sensor.new_binary_sensor(config[CONF_THE_BINARY])
#     cg.add(bs.set_parent(var))

async def to_code(config):
    var = await cg.get_variable(config[CONF_C1001])

    if conf := config.get(CONF_THE_BINARY):
      bs = await binary_sensor.new_binary_sensor(conf)
      await cg.register_component(bs, config)
      cg.add(bs.set_parent(var))