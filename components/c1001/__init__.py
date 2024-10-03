import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import uart, sensor, switch, text_sensor, binary_sensor, number
from esphome.const import CONF_ID, ICON_EMPTY, UNIT_EMPTY, UNIT_SECOND, DEVICE_CLASS_VOLTAGE, STATE_CLASS_MEASUREMENT, STATE_CLASS_NONE
from esphome.const import DEVICE_CLASS_DISTANCE, DEVICE_CLASS_SWITCH, UNIT_CENTIMETER, DEVICE_CLASS_DURATION, ENTITY_CATEGORY_DIAGNOSTIC

AUTO_LOAD = [ 'switch', 'text_sensor', 'binary_sensor', 'number', 'sensor', 'select', 'button' ]

DEPENDENCIES = [ 'uart' ]
MULTI_CONF = True

c1001_ns = cg.esphome_ns.namespace("c1001")
C1001Component = c1001_ns.class_("C1001Component", cg.PollingComponent, uart.UARTDevice)
# C1001FallLEDSwitch = c1001_ns.class_("C1001FallLEDSwitch", switch.Switch, cg.Component)

CONF_C1001_ID = "C1001"
CONF_UART = "uart"
CONF_MOTION = "voltage"
CONF_DISTANCE = "distance"
CONF_FALL_MOVEMENT_RANGE = "fall_movement_range"
CONF_FALL_MOVEMENT_SPEED = "fall_movement_speed"
CONF_FALL_PRESENCE = "fall_presence"
CONF_FALL_DWELL = "fall_dwell"
CONF_FALL_DWELL_TIME = "fall_dwell_time"
CONF_FALL_UNMANNED_TIME = "fall_unmanned_time"
CONF_FALL_INSTALL_HEIGHT = "fall_install_height"
CONF_FALL_FALL_TIME = "fall_fall_time"
CONF_FALL_SENSITIVITY = "fall_sensitivity"
CONF_FALL_EVENT = "fall_event"
CONF_FIRMWARE_VERSION = "firmware_version"
# CONF_THE_SWITCH = "fall_led_switch"
CONF_MODE = "mode_sensor"

CONFIG_SCHEMA = cv.Schema({
    # cv.Required(CONF_ID): cv.declare_id(C1001),
    cv.GenerateID(): cv.declare_id(C1001Component),
    cv.Required(CONF_UART): cv.use_id(uart.UARTComponent),
    cv.Optional(CONF_MOTION): sensor.sensor_schema(
            device_class=DEVICE_CLASS_VOLTAGE,
            unit_of_measurement=UNIT_EMPTY,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_DISTANCE): sensor.sensor_schema(
            device_class=DEVICE_CLASS_DISTANCE,
            unit_of_measurement=UNIT_CENTIMETER,
            accuracy_decimals=0,
            state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_FALL_MOVEMENT_RANGE): sensor.sensor_schema(
        device_class=DEVICE_CLASS_DISTANCE,
        unit_of_measurement=UNIT_EMPTY,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_FALL_MOVEMENT_SPEED): text_sensor.text_sensor_schema(text_sensor.TextSensor).extend(),
    cv.Optional(CONF_FALL_PRESENCE): binary_sensor.binary_sensor_schema(binary_sensor.BinarySensor).extend(),
    cv.Optional(CONF_FALL_DWELL): binary_sensor.binary_sensor_schema(binary_sensor.BinarySensor).extend(),
    cv.Optional(CONF_FALL_DWELL_TIME): sensor.sensor_schema(
        device_class=DEVICE_CLASS_DURATION,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        unit_of_measurement=UNIT_SECOND,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_FALL_UNMANNED_TIME): sensor.sensor_schema(
        device_class=DEVICE_CLASS_DURATION,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        unit_of_measurement=UNIT_SECOND,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_FALL_FALL_TIME): sensor.sensor_schema(
        device_class=DEVICE_CLASS_DURATION,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        unit_of_measurement=UNIT_SECOND,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_FALL_INSTALL_HEIGHT): sensor.sensor_schema(
        device_class=DEVICE_CLASS_DISTANCE,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        unit_of_measurement=UNIT_CENTIMETER,
        accuracy_decimals=0,
        state_class=STATE_CLASS_MEASUREMENT).extend(),
    cv.Optional(CONF_FALL_SENSITIVITY): sensor.sensor_schema(
        device_class=DEVICE_CLASS_VOLTAGE,
        entity_category=ENTITY_CATEGORY_DIAGNOSTIC,
        unit_of_measurement=UNIT_EMPTY,
        state_class=STATE_CLASS_NONE).extend(),
    cv.Optional(CONF_FALL_EVENT): binary_sensor.binary_sensor_schema(binary_sensor.BinarySensor).extend(),
   # cv.Optional(CONF_THE_SWITCH): switch.SWITCH_SCHEMA.extend({cv.GenerateID(): cv.declare_id(C1001FallLEDSwitch)}),
    cv.Optional(CONF_FIRMWARE_VERSION): text_sensor.text_sensor_schema(text_sensor.TextSensor).extend(),
    cv.Optional(CONF_MODE): text_sensor.text_sensor_schema(text_sensor.TextSensor).extend(),
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    await uart.register_uart_device(var, config)

    # uart = await cg.get_variable(config[CONF_UART])
    # cg.add(var.set_uart(uart))

    if CONF_MOTION in config:
        ms = await sensor.new_sensor(config[CONF_MOTION])
        cg.add(var.set_movement_sensor(ms))

    if CONF_DISTANCE in config:
        ms = await sensor.new_sensor(config[CONF_DISTANCE])
        cg.add(var.set_distance_sensor(ms))

    if CONF_FALL_MOVEMENT_RANGE in config:
        ms = await sensor.new_sensor(config[CONF_FALL_MOVEMENT_RANGE])
        cg.add(var.set_fall_movement_range_sensor(ms))

    if CONF_FALL_MOVEMENT_SPEED in config:
        ms = await text_sensor.new_text_sensor(config[CONF_FALL_MOVEMENT_SPEED])
        cg.add(var.set_fall_movement_speed_sensor(ms))

    if CONF_FALL_PRESENCE in config:
        ms = await binary_sensor.new_binary_sensor(config[CONF_FALL_PRESENCE])
        cg.add(var.set_fall_presence_sensor(ms))

    if CONF_FALL_DWELL in config:
        ms = await binary_sensor.new_binary_sensor(config[CONF_FALL_DWELL])
        cg.add(var.set_fall_dwell_sensor(ms))

    if CONF_FALL_DWELL_TIME in config:
        ms = await sensor.new_sensor(config[CONF_FALL_DWELL_TIME])
        cg.add(var.set_fall_dwell_time_sensor(ms))

    if CONF_FALL_UNMANNED_TIME in config:
        ms = await sensor.new_sensor(config[CONF_FALL_UNMANNED_TIME])
        cg.add(var.set_fall_unmanned_time_sensor(ms))

    if CONF_FALL_FALL_TIME in config:
        ms = await sensor.new_sensor(config[CONF_FALL_FALL_TIME])
        cg.add(var.set_fall_fall_time_sensor(ms))

    if CONF_FALL_INSTALL_HEIGHT in config:
        ms = await sensor.new_sensor(config[CONF_FALL_INSTALL_HEIGHT])
        cg.add(var.set_fall_install_height_sensor(ms))

    if CONF_FALL_SENSITIVITY in config:
        ms = await sensor.new_sensor(config[CONF_FALL_SENSITIVITY])
        cg.add(var.set_fall_sensitivity_sensor(ms))

    if CONF_FALL_EVENT in config:
        ms = await binary_sensor.new_binary_sensor(config[CONF_FALL_EVENT])
        cg.add(var.set_fall_event_sensor(ms))

    if CONF_FIRMWARE_VERSION in config:
        sens = await text_sensor.new_text_sensor(config[CONF_FIRMWARE_VERSION])
        cg.add(var.set_firmware_version_sensor(sens))

    if CONF_MODE in config:
        sens = await text_sensor.new_text_sensor(config[CONF_MODE])
        cg.add(var.set_mode_text_sensor(sens))

