#ifndef VANILLA_SERVER_H
#define VANILLA_SERVER_H

#if defined(__cplusplus)
extern "C" {
#endif

#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>

#define VANILLA_SUCCESS              0
#define VANILLA_ERROR               -1
#define VANILLA_READY               -2
#define VANILLA_INFO                -3
#define VANILLA_UNKNOWN_COMMAND     -4
#define VANILLA_INVALID_ARGUMENT    -5

enum VanillaGamepadButtons
{
    VANILLA_BTN_A,
    VANILLA_BTN_B,
    VANILLA_BTN_X,
    VANILLA_BTN_Y,
    VANILLA_BTN_L,
    VANILLA_BTN_R,
    VANILLA_BTN_ZL,
    VANILLA_BTN_ZR,
    VANILLA_BTN_MINUS,
    VANILLA_BTN_PLUS,
    VANILLA_BTN_HOME,
    VANILLA_BTN_L3,
    VANILLA_BTN_R3,
    VANILLA_BTN_LEFT,
    VANILLA_BTN_RIGHT,
    VANILLA_BTN_DOWN,
    VANILLA_BTN_UP,
    VANILLA_AXIS_L_X,
    VANILLA_AXIS_L_Y,
    VANILLA_AXIS_R_X,
    VANILLA_AXIS_R_Y,
    VANILLA_AXIS_L_LEFT,
    VANILLA_AXIS_L_UP,
    VANILLA_AXIS_L_RIGHT,
    VANILLA_AXIS_L_DOWN,
    VANILLA_AXIS_R_LEFT,
    VANILLA_AXIS_R_UP,
    VANILLA_AXIS_R_RIGHT,
    VANILLA_AXIS_R_DOWN,
    VANILLA_AXIS_VOLUME,
    VANILLA_SENSOR_ACCEL_X,
    VANILLA_SENSOR_ACCEL_Y,
    VANILLA_SENSOR_ACCEL_Z,
    VANILLA_SENSOR_GYRO_PITCH,
    VANILLA_SENSOR_GYRO_YAW,
    VANILLA_SENSOR_GYRO_ROLL,
    VANILLA_BTN_COUNT
};

enum VanillaEvent
{
    VANILLA_EVENT_VIDEO,
    VANILLA_EVENT_AUDIO,
    VANILLA_EVENT_VIBRATE
};

enum VanillaRegion
{
    VANILLA_REGION_JAPAN         = 0,
    VANILLA_REGION_AMERICA       = 1,
    VANILLA_REGION_EUROPE        = 2,
    VANILLA_REGION_CHINA         = 3,
    VANILLA_REGION_SOUTH_KOREA   = 4,
    VANILLA_REGION_TAIWAN        = 5,
    VANILLA_REGION_AUSTRALIA     = 6,
};

enum VanillaBatteryStatus {
    VANILLA_BATTERY_STATUS_CHARGING = 0,
    VANILLA_BATTERY_STATUS_UNKNOWN  = 1,
    VANILLA_BATTERY_STATUS_VERY_LOW = 2,
    VANILLA_BATTERY_STATUS_LOW      = 3,
    VANILLA_BATTERY_STATUS_MEDIUM   = 4,
    VANILLA_BATTERY_STATUS_HIGH     = 5,
    VANILLA_BATTERY_STATUS_FULL     = 6
};

/**
 * Event handler used by caller to receive events
 */
typedef void (*vanilla_event_handler_t)(void *context, int event_type, const char *data, size_t data_size);

/**
 * Attempt to sync with the console
 *
 * This will block until the task is over or vanilla_stop() is called from another thread.
 */
int vanilla_sync_with_console(const char *wireless_interface, uint16_t code);

/**
 * Attempt gameplay connection with console
 *
 * This will block until the task is over or vanilla_stop() is called from another thread.
 */
int vanilla_connect_to_console(const char *wireless_interface, vanilla_event_handler_t event_handler, void *context);

/**
 * Determine if we have a configuration file that we can connect with.
 */
int vanilla_has_config();

/**
 * Attempt to stop the current action
 *
 * This can be called from another thread to safely exit a blocking call to vanilla_sync_with_console() or vanilla_connect_to_console().
 */
void vanilla_stop();

/**
 * Set button/axis state
 *
 * This can be called from another thread to change the button state while vanilla_connect_to_console() is running.
 *
 * For buttons, anything non-zero will be considered a press.
 * For axes, the range is signed 16-bit (-32,768 to 32,767).
 * For accelerometers, cast a float value in m/s^2.
 * For gyroscopes, cast a float value in radians per second.
 */
void vanilla_set_button(int button, int32_t value);

/**
 * Set touch screen coordinates to `x` and `y`
 *
 * This can be called from another thread to change the button state while vanilla_connect_to_console() is running.
 *
 * `x` and `y` are expected to be in gamepad screen coordinates (0x0 to 853x479).
 * If either `x` or `y` are -1, this point will be disabled.
 */
void vanilla_set_touch(int x, int y);

/**
 * Logging function
 */
void vanilla_log(const char *format, ...);
void vanilla_log_no_newline(const char *format, ...);
void vanilla_log_no_newline_va(const char *format, va_list args);

/**
 * Install custom logger
 */
void vanilla_install_logger(void (*logger)(const char *, va_list args));

/**
 * Request an IDR (instant decoder refresh) video frame from the console
 */
void vanilla_request_idr();

/**
 * Retrieve SPS/PPS data for H.264 encoding
 * 
 * If `data` is null, `*size` will be set to the number of bytes required.
 * If `data` is not null, bytes will be copied up to `*size` or the total number of bytes.
 */
void vanilla_retrieve_sps_pps_data(void *data, size_t *size);

/**
 * Sets the region Vanilla should present itself to the console
 * 
 * The gamepad is region locked and the console will complain if the gamepad doesn't match.
 * 
 * Set to a member of the VanillaRegion enum.
 */
void vanilla_set_region(int region);

/**
 * Sets the gamepad battery status that Vanilla should send to the console
 */
void vanilla_set_battery_status(int battery_status);

#if defined(__cplusplus)
}
#endif

#endif // VANILLA_SERVER_H