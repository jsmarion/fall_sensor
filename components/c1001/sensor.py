# import esphome.codegen as cg
# import esphome.config_validation as cv
# from esphome.components import uart, sensor, switch
# from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY

# MULTI_CONF = True
# DEPENDENCIES = [ 'uart' ]
# AUTO_LOAD = [ 'switch' ]

# c1001_ns = cg.esphome_ns.namespace("c1001")

# C1001 = c1001_ns.class_("c1001", cg.PollingComponent, uart.UARTDevice)
# C1001FallLEDSwitch = c1001_ns.class_("FallLEDSwitch", switch.Switch, cg.Component)

# CONF_UART = "uart"
# CONF_THE_SWITCH = "fall_led_switch"

# # CONFIG_SCHEMA = cv.COMPONENT_SCHEMA.extend({
# #     cv.GenerateID(): cv.declare_id(C1001),
# #     cv.Required(CONF_UART): cv.use_id(uart.UARTComponent),
# #     cv.Optional(CONF_THE_SWITCH): switch.SWITCH_SCHEMA.extend({cv.GenerateID(): cv.declare_id(C1001FallLEDSwitch)}),
# # })

# CONFIG_SCHEMA = cv.Schema({
#     cv.GenerateID(): cv.declare_id(C1001),
#     # cv.Required(CONF_UART): cv.use_id(uart.UARTComponent),
#     # cv.Optional(CONF_THE_SWITCH): switch.SWITCH_SCHEMA.extend({cv.GenerateID(): cv.declare_id(C1001FallLEDSwitch)}),
# }).extend(uart.UART_DEVICE_SCHEMA)

# # CONFIG_SCHEMA = (
# #     sensor.sensor_schema(
# #         C1001,
# #         unit_of_measurement=UNIT_EMPTY,
# #         icon=ICON_EMPTY,
# #         accuracy_decimals=1,
# #     )
# #     .extend(cv.polling_component_schema("5s"))
# #     .extend(uart.UART_DEVICE_SCHEMA)
# # )

# async def to_code(config):
#     var = cg.new_Pvariable(config[CONF_ID])
#     await cg.register_component(var, config)

#     await uart.register_uart_device(var, config)

#     # uart = await cg.get_variable(config[CONF_UART])
#     # cg.add(var.set_uart(uart))
#     # if CONF_THE_SWITCH in config:
#     #     sw = await switch.new_switch(config[CONF_THE_SWITCH])
#     #     cg.add(sw.set_parent(var))

# # async def to_code(config):
# #     var = await sensor.new_sensor(config)
# #     await cg.register_component(var, config)
# #     await uart.register_uart_device(var, config)
