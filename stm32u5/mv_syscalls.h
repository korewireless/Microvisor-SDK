#ifndef MV_API_H
#define MV_API_H

#include <stdint.h>


/**
 *  32-bit result code from non-secure code.
 */
enum MvStatus {
    MV_STATUS_OKAY                 = 0x0, //< Success
    MV_STATUS_TIMENOTSET           = 0x1, //< The internal RTC has not yet been set, because the server hasn't yet provided time synchronization.
    MV_STATUS_PARAMETERFAULT       = 0x2, //< A pointer parameter references unreadable/unwritable memory.
    MV_STATUS_INVALIDBUFFERSIZE    = 0x3, //< Indicates the buffer size provided is not sufficient for the function called.
    MV_STATUS_PERIPHERALACCESSFAULT = 0x4, //< An unsupported peripheral address, or non-word-aligned address was passed.
    MV_STATUS_BUFFERALREADYINUSE   = 0x5, //< The supplied buffer is already in use by another object.
    MV_STATUS_INVALIDINTERRUPT     = 0x6, //< The specified interrupt is not usable by the application, or is already in use by another object.
    MV_STATUS_INVALIDHANDLE        = 0x7, //< The specified handle is not valid, or is a handle to a different type of object.
    MV_STATUS_TOOMANYNOTIFICATIONBUFFERS = 0x8, //< The internal limit on number of simultaneous notification buffers has been reached.
    MV_STATUS_INVALIDVECTOR        = 0x9, //< An invalid interrupt vector address specified for a fast interrupt.
    MV_STATUS_UNAVAILABLE          = 0xa, //< Functionality unavailable due to current state.
    MV_STATUS_UNSUPPORTEDSTRUCTUREVERSION = 0xb, //< A versioned structure was encountered that Microvisor does not support.
    MV_STATUS_TOOMANYCHANNELS      = 0xc, //< The internal limit on the number of simultaneous network channels has been reached.
    MV_STATUS_UNKNOWNCHANNELENDPOINT = 0xd, //< The endpoint specified when opening a channel is unknown.
    MV_STATUS_UNKNOWNCHANNELTYPE   = 0xe, //< The type specified when opening a channel is unknown.
    MV_STATUS_CHANNELCLOSED        = 0xf, //< The channel was unexpectedly closed.
    MV_STATUS_NETWORKNOTCONNECTED  = 0x10, //< The network operation could not be actioned because the device is offline.
    MV_STATUS_RATELIMITED          = 0x11, //< The operation has been called too often. Try again later.
    MV_STATUS_INVALIDBUFFERALIGNMENT = 0x12, //< Indicates the buffer alignment provided is not sufficient for the function called.
    MV_STATUS_LATEFAULT            = 0x13, //< Indicates the fault after an action was taken, e.g. error writing result of a successful operation.
    MV_STATUS_RESPONSENOTPRESENT   = 0x14, //< There is no HTTP response to read.
    MV_STATUS_HEADERINDEXINVALID   = 0x15, //< The HTTP header index is out of bounds.
    MV_STATUS_OFFSETINVALID        = 0x16, //< THe offset into the http response body exceeds its size.
    MV_STATUS_REQUESTALREADYSENT   = 0x17, //< Request has already been sent, can't do it again over the same channel.
    MV_STATUS_REQUESTUNSUCCESSFUL  = 0x18, //< Request has failed, data can't be read.
    MV_STATUS_LOGMESSAGETOOLONG    = 0x19, //< The log message exceeds the maximum allowed size.
    MV_STATUS_LOGGINGDISABLEDBYSERVER = 0x1a, //< The server has disabled logging.
    MV_STATUS__MAX                 = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvTemporaryTestSubStruct {
    /// First member
    uint64_t a;
    /// Second member
    uint32_t b;
};

struct MvTemporaryTestStruct {
    /// First member
    uint8_t a;
    /// Second member
    struct MvTemporaryTestSubStruct *b;
    /// Third member
    uint32_t c;
};

struct MvTemporaryTestVersionedStruct {
    uint32_t version; //< version field; describes which other fields are valid
    union {
        struct {
            /// First member
            uint16_t a;
            /// Second member
            struct MvTemporaryTestStruct b;
        } v1;
    };
};

/**
 *  Notification event type.
 */
enum MvEventType {
    MV_EVENTTYPE_NOEVENT           = 0x0, //< Sentinel value meaning an illegal or absent event.
    MV_EVENTTYPE_NETWORKSTATUSCHANGED = 0x1, //< Value defining a network state change event.
    MV_EVENTTYPE_CHANNELDATAREADABLE = 0x2, //< Event notifying of a channel-readable event.
    MV_EVENTTYPE_CHANNELDATAWRITESPACE = 0x3, //< Event notifying of a channel write space event.
    MV_EVENTTYPE_CHANNELNOTCONNECTED = 0x4, //< Event notifying of a channel becoming disconnected.
    MV_EVENTTYPE__MAX              = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvNotification {
    /// When this notification was issued. This is the same time basis as `mvGetMicroseconds`.
    uint64_t microseconds;
    /// The type of event that triggered the notification.
    enum MvEventType event_type;
    /// A user-supplied value which allows you to reference the source of the notification.
    uint32_t tag;
};

struct MvNotificationSetup {
    /// The IRQ number which will be pended by Microvisor when a new notification becomes available.
    uint32_t irq;
    /// A pointer to a buffer into which Microvisor will write notifications.
    struct MvNotification *buffer;
    /// The length of the notification buffer in bytes. Must be a multiple of 16 bytes and at least 32 bytes.
    uint32_t buffer_size;
};

/// An opaque handle to a notification object. This is a 32-bit value chosen randomly by Microvisor.  Zero is never a valid handle.

typedef struct MvNotificationHandleOpaque *MvNotificationHandle;

struct MvRequestNetworkParams {
    uint32_t version; //< version field; describes which other fields are valid
    union {
        struct {
            /// The handle of the notification center that will dispatch network-related notifications.
            MvNotificationHandle notification_handle;
            /// A user-supplied value which allows you to reference the source of the notification.
            uint32_t notification_tag;
        } v1;
    };
};

/// An opaque handle to a network object. This is a 32-bit value chosen randomly by Microvisor. Zero is never a valid handle.

typedef struct MvNetworkHandleOpaque *MvNetworkHandle;

/**
 *  Network state definitions.
 */
enum MvNetworkStatus {
    MV_NETWORKSTATUS_DELIBERATELYOFFLINE = 0x0, //< The device is not currently connected to the server.
    MV_NETWORKSTATUS_CONNECTED     = 0x1, //< The device is currently connecting to the server.
    MV_NETWORKSTATUS_CONNECTING    = 0x2, //< The device is currently connected to the server.
    MV_NETWORKSTATUS__MAX          = 0xffffffff, //< Ensure use of correct underlying size.
};

/**
 *  32-bit bitfield of current connectivity reasons.
 *  Reasons marked with `PREVENTING` will prevent connection attempts while the associated `MvNetworkReason` prevails.
 */
enum MvNetworkReason {
    MV_NETWORKREASON_NOTCONNECTING = 0x0, //< No pending connection attempts and connections not being prevented.
    MV_NETWORKREASON_USINGNETWORK  = 0x1, //< Application is using the network.
    MV_NETWORKREASON_NEVERUSEDNETWORKAPI = 0x2, //< Application has not yet used the networking API.
    MV_NETWORKREASON_MINIMUMCHECKINEXPIRED = 0x4, //< Minimum check-in period has been reached.
    MV_NETWORKREASON_RTCNOTSET     = 0x8, //< RTC is not yet set.
    MV_NETWORKREASON_KERNELERROR   = 0x10, //< A kernel error requires reporting.
    MV_NETWORKREASON_APPLICATIONERROR = 0x20, //< An application error requires reporting.
    MV_NETWORKREASON_COLDBOOTCONNECTION = 0x40, //< Connection needed on cold boot for updates.
    MV_NETWORKREASON_FETCHINGUPDATE = 0x80, //< An update is being fetched in the background.
    MV_NETWORKREASON_DEBUGGERATTACHED = 0x100, //< A debugging session is attached.
    MV_NETWORKREASON_NOAPPLICATIONCODE = 0x200, //< No application code is present.
    MV_NETWORKREASON_ENHANCECALM   = 0x800, //< \[PREVENTING] Taking a break from connection attempts.
    MV_NETWORKREASON__MAX          = 0xffffffff, //< Ensure use of correct underlying size.
};

/**
 *  The type of data carried by the channel.
 */
enum MvChannelType {
    MV_CHANNELTYPE_OPAQUEBYTES     = 0x0, //< The channel carries opaque bytes.
    MV_CHANNELTYPE_HTTP            = 0x2, //< The channel carries HTTP data.
    MV_CHANNELTYPE__MAX            = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvOpenChannelParams {
    uint32_t version; //< version field; describes which other fields are valid
    union {
        struct {
            /// The handle of the notification center that will dispatch channel-related notifications.
            MvNotificationHandle notification_handle;
            /// A user-supplied value which allows you to reference the source of the notification.
            uint32_t notification_tag;
            /// The handle of the network that will host the channel.
            MvNetworkHandle network_handle;
            /// Pointer to a buffer to receive channel data.
            uint8_t *receive_buffer;
            /// The length of the receive buffer in bytes.
            uint32_t receive_buffer_len;
            /// Pointer to a buffer to send channel data.
            uint8_t *send_buffer;
            /// The length of the send buffer in bytes.
            uint32_t send_buffer_len;
            /// The type of content carried by the channel.
            enum MvChannelType channel_type;
            /// Pointer to an endpoint identifier for cloud configuration.
            const uint8_t *endpoint;
            /// The length of `endpoint` in bytes.
            uint32_t endpoint_len;
        } v1;
    };
};

/// An opaque handle to a channel object. This is a 32-bit value assigned by Microvisor. Zero is never a valid handle.

typedef struct MvChannelHandleOpaque *MvChannelHandle;

/**
 *  Reasons why a channel has been closed.
 */
enum MvClosureReason {
    MV_CLOSUREREASON_NONE          = 0x0, //< The channel is not closed or the closure reason is not known.
    MV_CLOSUREREASON_CHANNELCLOSEDBYSERVER = 0x1, //< The channel was closed by the server.
    MV_CLOSUREREASON_CHANNELRESETBYSERVER = 0x2, //< The channel was reset by the server.
    MV_CLOSUREREASON_NETWORKDISCONNECTED = 0x3, //< The channel was closed due to a network disconnection.
    MV_CLOSUREREASON_CONNECTIONTERMINATED = 0x4, //< The connection to the server was terminated due to an error.
    MV_CLOSUREREASON__MAX          = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvHttpHeader {
    /// The length of the header in bytes.
    uint32_t length;
    /// Header content.
    const uint8_t *data;
};

struct MvHttpRequest {
    /// HTTP method (GET, HEAD, POST, PUT, DELETE, PATCH or OPTIONS).
    const uint8_t *method;
    /// The length of the method string in bytes.
    uint32_t method_len;
    /// URL to access, only "https://" is supported.
    const uint8_t *url;
    /// The length of the URL string in bytes.
    uint32_t url_len;
    /// Number of headers in `headers` array.
    uint32_t num_headers;
    /// Headers for request.
    const struct MvHttpHeader *headers;
    /// Request body.
    const uint8_t *body;
    /// The length of the body in bytes.
    uint32_t body_len;
    /// Request timeout in milliseconds. Microvisor supports timeouts from 5000 to 10000 milliseconds.
    uint32_t timeout_ms;
};

/**
 *  The outcome of an HTTP response.
 */
enum MvHttpResult {
    MV_HTTPRESULT_OK               = 0x0, //< The HTTP request succeeded.
    MV_HTTPRESULT_UNSUPPORTEDURISCHEME = 0x1, //< An unsupported URI scheme was used in the request.
    MV_HTTPRESULT_UNSUPPORTEDMETHOD = 0x2, //< An unsupported method was used in the request.
    MV_HTTPRESULT_INVALIDHEADERS   = 0x3, //< Invalid headers were provided in the request.
    MV_HTTPRESULT_INVALIDTIMEOUT   = 0x4, //< An invalid timeout was specified in the request.
    MV_HTTPRESULT_REQUESTFAILED    = 0x5, //< The HTTP request failed.
    MV_HTTPRESULT_RESPONSETOOLARGE = 0x6, //< The HTTP response returned by the server doesn't fit the receive buffer.
    MV_HTTPRESULT__MAX             = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvHttpResponseData {
    /// HTTP request result as reported by Microvisor server.
    enum MvHttpResult result;
    /// Status code reported by the endpoint.
    uint32_t status_code;
    /// Number of headers in the response.
    uint32_t num_headers;
    /// Length of response body in bytes.
    uint32_t body_length;
};

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  Does nothing, and returns `Status::Okay`.
 */
enum MvStatus mvNoOp(void);

/**
 *  Gets the current value of a free-running microsecond clock. This
 *  clock starts from zero when Microvisor starts and counts up
 *  from then on. It has no relation to (and may drift with respect
 *  to) wall time.
 *
 * Parameters:
 * @param[out]    ms              Pointer to receive the result.
 *
 * @retval MV_STATUS_PARAMETERFAULT `ms` is an illegal pointer.
 */
enum MvStatus mvGetMicroseconds(uint64_t *ms);

/**
 *  Gets the current frequency of the CPU clock.
 *
 * Parameters:
 * @param[out]    hz              Pointer to receive the result.
 *
 * @retval MV_STATUS_PARAMETERFAULT `hz` is an illegal pointer.
 */
enum MvStatus mvGetSysClk(uint32_t *hz);

/**
 *  Gets the current frequency of the AHB bus.
 *
 * Parameters:
 * @param[out]    hz              Pointer to receive the result.
 *
 * @retval MV_STATUS_PARAMETERFAULT `hz` is an illegal pointer.
 */
enum MvStatus mvGetHClk(uint32_t *hz);

/**
 *  Gets the current frequency of the APB1 bus.
 *
 * Parameters:
 * @param[out]    hz              Pointer to receive the result.
 *
 * @retval MV_STATUS_PARAMETERFAULT `hz` is an illegal pointer.
 */
enum MvStatus mvGetPClk1(uint32_t *hz);

/**
 *  Gets the current frequency of the APB2 bus/
 *
 * Parameters:
 * @param[out]    hz              Pointer to receive the result.
 *
 * @retval MV_STATUS_PARAMETERFAULT `hz` is an illegal pointer.
 */
enum MvStatus mvGetPClk2(uint32_t *hz);

/**
 *  Gets the current time in non-leap microseconds since the Unix
 *  epoch. This will return `MV_STATUS_TIMENOTSET` on restart
 *  until time is synchronized with the server. The value populated
 *  into `usec` may jump forward or backward in response to an updated
 *  time from the server.
 *
 * Parameters:
 * @param[out]    usec            Pointer to receive the result.
 *
 * @retval MV_STATUS_PARAMETERFAULT `sec` is an illegal pointer.
 * @retval MV_STATUS_TIMENOTSET No wall time is available.
 */
enum MvStatus mvGetWallTime(uint64_t *usec);

/**
 *  Test structure parameters`
 *
 * Parameters:
 * @param[in]     in              input structure.
 *
 * @retval MV_STATUS_PARAMETERFAULT `in` is an illegal pointer.
 * @retval MV_STATUS_UNSUPPORTEDSTRUCTUREVERSION Structure version is not supported.
 */
enum MvStatus mvTemporaryStructTest(const struct MvTemporaryTestVersionedStruct *in);

/**
 *  Returns the device's 34-byte unique ID code.
 *
 * Parameters:
 * @param[out]    buf             A pointer to a `uint8_t` buffer into which the ID will be written.
 * @param         len             The length of the buffer allocated to receive ID. Must be 34.
 *
 * @retval MV_STATUS_PARAMETERFAULT `buf` is not entirely in non-secure memory.
 * @retval MV_STATUS_INVALIDBUFFERSIZE `buf` is not the correct size for result.
 */
enum MvStatus mvGetDeviceId(uint8_t *buf, uint32_t len);

/**
 *  Read a peripheral register.
 *
 * Parameters:
 * @param         reg             The address of the non-secure register being accessed.
 * @param[out]    output          A pointer to a `uint32_t` into which the result will be written.
 *
 * @retval MV_STATUS_PERIPHERALACCESSFAULT An unsupported or non-word-aligned register address.
 */
enum MvStatus mvPeriphPeek32(uint32_t reg, uint32_t *output);

/**
 *  Write a peripheral register.
 *
 * Parameters:
 * @param         reg             The address of the non-secure register being accessed.
 * @param         mask            A mask to indicate the the bits that will be affected by the write.
 * @param         xorvalue        A value to exclusive-or with the register's current contents.
 *
 * @retval MV_STATUS_PERIPHERALACCESSFAULT An unsupported or non-word-aligned register address.
 */
enum MvStatus mvPeriphPoke32(uint32_t reg, uint32_t mask, uint32_t xorvalue);

/**
 *  Set up receipt of notifications via an interrupt.
 *
 *  The ability to receive notifications is represented by a handle which
 *  can be provided to other functions for their use.
 *
 *  Microvisor does not configure the supplied IRQ for you. As with other
 *  interrupts, the application must set up the IRQ itself, e.g., clearing it,
 *  setting its priority, enabling it, etc.
 *
 *  The supplied IRQ must be usable the application. It must not be a
 *  fast interrupt. It must not already be used by another notification
 *  object.
 *
 * Parameters:
 * @param[in]     notifications    Parameters used to configure the new notification object.
 * @param[out]    handle_out      On success, receives a handle to the notification object.
 *
 * @retval MV_STATUS_PARAMETERFAULT The buffer specified in `notifications` is not writeable.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The buffer is too small (under 32 bytes) or not a whole number of Notification structures.
 * @retval MV_STATUS_BUFFERALREADYINUSE The buffer is already used by another notification object.
 * @retval MV_STATUS_INVALIDINTERRUPT The IRQ specified is not usable by non-secure mode or is in use by a different notification object.
 * @retval MV_STATUS_TOOMANYNOTIFICATIONBUFFERS The limit of 32 simultaneously in use notification buffers has been reached.
 */
enum MvStatus mvSetupNotifications(const struct MvNotificationSetup *notifications, MvNotificationHandle *handle_out);

/**
 *  Cease receipt of notifications previously set up by `mvSetupNotifications`.
 *
 *  If the notification object is in use by a different object, this is not an error,
 *  but the notifications will no longer be delivered.
 *
 *  No interrupts are pended by Microvisor after this call returns. However, if the interrupt
 *  is already pended then it is not cleared - this is the job of the application.
 *
 * Parameters:
 * @param[in,out] handle_in_out    The handle to be destroyed. On success, Microvisor writes a zero through this pointer.
 *
 * @retval MV_STATUS_PARAMETERFAULT The pointer is not readable/writable.
 * @retval MV_STATUS_INVALIDHANDLE The handle is not valid, or does not identify a notification object.
 */
enum MvStatus mvCloseNotifications(MvNotificationHandle *handle_in_out);

/**
 *  Triggers a notification.
 *
 * Parameters:
 * @param         handle          The handle of the notification center that will dispatch notifications.
 * @param         type            The notification type.
 * @param         tag             The notification tag.
 *
 * @retval MV_STATUS_INVALIDHANDLE The handle is not valid, or does not identify a notification object.
 */
enum MvStatus mvTempTriggerNotification(MvNotificationHandle handle, uint32_t type, uint32_t tag);

/**
 *  Sets the interrupt to be served with low latency.
 *
 * Parameters:
 * @param         irqn            The IRQ number.
 *
 * @retval MV_STATUS_INVALIDINTERRUPT `irqn` is not available to non-secure code.
 * @retval MV_STATUS_INVALIDVECTOR VTOR or interrupt vector is not in non-secure memory.
 * @retval MV_STATUS_UNAVAILABLE Fast interrupts are unavailable. Code is being shut down after a hung interrupt or interrupt flood.
 */
enum MvStatus mvSetFastInterrupt(uint32_t irqn);

/**
 *  Makes an interrupt previously set as fast with `mvSetFastInterrupt`
 *  a normal interrupt again.
 *
 * Parameters:
 * @param         irqn            The IRQ number.
 *
 * @retval MV_STATUS_INVALIDINTERRUPT `irqn` is not available to non-secure code.
 * @retval MV_STATUS_UNAVAILABLE Fast interrupts are unavailable. Code is being shut down after a hung interrupt or interrupt flood.
 */
enum MvStatus mvClearFastInterrupt(uint32_t irqn);

/**
 *  Enables a fast interrupt.
 *
 * Parameters:
 * @param         irqn            The IRQ number.
 *
 * @retval MV_STATUS_INVALIDINTERRUPT `irqn` is not available to non-secure code.
 * @retval MV_STATUS_UNAVAILABLE Fast interrupts are unavailable. Code is being shut down after a hung interrupt or interrupt flood.
 */
enum MvStatus mvEnableFastInterrupt(uint32_t irqn);

/**
 *  Disables a previously enabled fast interrupt.
 *
 * Parameters:
 * @param         irqn            The IRQ number.
 *
 * @retval MV_STATUS_INVALIDINTERRUPT `irqn` is not available to non-secure code.
 * @retval MV_STATUS_UNAVAILABLE Fast interrupts are unavailable. Code is being shut down after a hung interrupt or interrupt flood.
 */
enum MvStatus mvDisableFastInterrupt(uint32_t irqn);

/**
 *  Disables all fast interrupts.
 */
enum MvStatus mvDisableAllFastInterrupts(void);

/**
 *  Undoes the effect of `mvDisableAllFastInterrupts`. Individual
 *  enabled/disabled states still hold.
 */
enum MvStatus mvEnableAllFastInterrupts(void);

/**
 *  Applications can call `mvRequestNetwork` to ask Microvisor to bring up and keep up
 *  networking. Microvisor creates a resource representing this request and returns
 *  a handle to it as an output of mvRequestNetwork.
 *
 * Parameters:
 * @param[in]     params          Settings used to configure notifications relating to the network.
 * @param[out]    handle          A pointer to where the network handle will be written.
 *
 * @retval MV_STATUS_PARAMETERFAULT `params` or `handle` does not reference memory accessible to the application.
 * @retval MV_STATUS_UNSUPPORTEDSTRUCTUREVERSION Structure version is not supported.
 */
enum MvStatus mvRequestNetwork(const struct MvRequestNetworkParams *params, MvNetworkHandle *handle);

/**
 *  Release the network request associated with the handle and the associated
 *  kernel resource, writing a zero to the passed handle on success.
 *
 * Parameters:
 * @param[in,out] handle          A pointer to the handle of the network to be released.
 *
 * @retval MV_STATUS_PARAMETERFAULT `handle` does not reference memory accessible to the application.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid network handle.
 * @retval MV_STATUS_UNSUPPORTEDSTRUCTUREVERSION Structure version is not supported.
 */
enum MvStatus mvReleaseNetwork(MvNetworkHandle *handle);

/**
 *  Get the current network status, i.e., whether the device is connected
 *  to the server or not.
 *
 * Parameters:
 * @param         handle          The network’s handle.
 * @param[out]    status          A pointer to where the network status value will be written.
 *
 * @retval MV_STATUS_PARAMETERFAULT `status` or `handle` does not reference memory accessible to the application.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid network handle.
 */
enum MvStatus mvGetNetworkStatus(MvNetworkHandle handle, enum MvNetworkStatus *status);

/**
 *  Obtain the current network connectivity reasons and number of
 *  outstanding network handles.
 *
 * Parameters:
 * @param[out]    reasons         A pointer to a bitfield of connectivity reasons. See `mvNetworkReason` for details.
 * @param[out]    request_ref_count    A pointer to a `uint32_t` in which the number of outstanding network handles will be written.
 *
 * @retval MV_STATUS_PARAMETERFAULT `reasons` or `request_ref_count` does not reference memory accessible to the application.
 */
enum MvStatus mvGetNetworkReasons(enum MvNetworkReason *reasons, uint32_t *request_ref_count);

/**
 *  Open a data transfer channel.
 *
 * Parameters:
 * @param[in]     params          A pointer to the parameters that will be used to configure the channel.
 * @param[out]    handle          A pointer to where the channel handle will be written.
 *
 * @retval MV_STATUS_PARAMETERFAULT `params` is an illegal pointer.
 * @retval MV_STATUS_PARAMETERFAULT `params` contains an illegal pointer.
 * @retval MV_STATUS_PARAMETERFAULT `handle` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE The value of `notification_handle` in `params` is not a valid notification center handle.
 * @retval MV_STATUS_INVALIDHANDLE The value of `network_handle` in `params` is not a valid network handle.
 * @retval MV_STATUS_INVALIDBUFFERALIGNMENT The value of `send_buffer` buffer in `params` is not suitably aligned.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The value of `send_buffer` buffer in `params` is mis-sized.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The value of `receive_buffer` buffer in `params` is mis-sized.
 * @retval MV_STATUS_UNKNOWNCHANNELENDPOINT The value of `endpoint` in `params` is not known to Microvisor.
 * @retval MV_STATUS_UNKNOWNCHANNELTYPE The value of `channel_type` in `params` is not known to Microvisor.
 * @retval MV_STATUS_NETWORKNOTCONNECTED Microvisor must be connected to the server first. See mvGetNetworkStatus.
 * @retval MV_STATUS_RATELIMITED The operation has been called too often (limit is 8). Wait one second then retry.
 * @retval MV_STATUS_UNAVAILABLE The channel could not be opened at this time.
 * @retval MV_STATUS_TOOMANYCHANNELS The limit of four simultaneously in use channels has been reached.
 */
enum MvStatus mvOpenChannel(const struct MvOpenChannelParams *params, MvChannelHandle *handle);

/**
 *  Read from a channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel from which to read.
 * @param[out]    read_pointer_out    A pointer to where a pointer to read data will be written.
 * @param[out]    length_out      A pointer to a `uint32_t` in which the number of bytes read will be written.
 *
 * @retval MV_STATUS_PARAMETERFAULT `length` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvReadChannel(MvChannelHandle handle, uint8_t **read_pointer_out, uint32_t *length_out);

/**
 *  Signal that reading from a channel is complete.
 *
 * Parameters:
 * @param         handle          The handle of the channel to signal that reading is complete.
 * @param         consumed        The number of bytes consumed.
 *
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvReadChannelComplete(MvChannelHandle handle, uint32_t consumed);

/**
 *  Write to a channel in streaming mode. This will write
 *  as much of the data passed as it can to the channel,
 *  and report back the number of bytes that were written.
 *
 * Parameters:
 * @param         handle          The handle of the channel to write to.
 * @param[in]     data            A pointer to the data to write to the channel.
 * @param         len             The number of bytes to write.
 * @param[out]    written         A pointer to a `uint32_t` in which the number of bytes written will be stored.
 *
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid channel handle.
 * @retval MV_STATUS_PARAMETERFAULT Either `data` or `written` is not a valid pointer.
 * @retval MV_STATUS_LATEFAULT `written` is not a valid pointer, but data has been written.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvWriteChannelStream(MvChannelHandle handle, const uint8_t *data, uint32_t len, uint32_t *written);

/**
 *  Write to a channel. This will only write to the channel
 *  if there are enough bytes available to consume all of
 *  the data passed. Otherwise it will write none of the
 *  data. The function may be called with a zero length
 *  value in order to determine the number of bytes available
 *  that can be written to on the channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel to write to.
 * @param[in]     data            A pointer to the data to write to the channel.
 * @param         len             The number of bytes to write.
 * @param[out]    available       A pointer to a `uint32_t` in which the number of bytes
available in the channel's write buffer will be stored.

 *
 * @retval MV_STATUS_INVALIDBUFFERSIZE No data was consumed because it does not all fit into the channel's send buffer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid channel handle.
 * @retval MV_STATUS_PARAMETERFAULT Either `data` or `available` is not a valid pointer.
 * @retval MV_STATUS_LATEFAULT `available` is not a valid pointer, but data has been written.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvWriteChannel(MvChannelHandle handle, const uint8_t *data, uint32_t len, uint32_t *available);

/**
 *  Close a channel.
 *
 * Parameters:
 * @param[in,out] handle          A pointer to the channel handle.
 *
 * @retval MV_STATUS_PARAMETERFAULT The supplied pointer is not readable or writable.
 * @retval MV_STATUS_INVALIDHANDLE The specified handle is not a valid channel handle.
 */
enum MvStatus mvCloseChannel(MvChannelHandle *handle);

/**
 *  Discover why a channel was closed.
 *
 * Parameters:
 * @param         handle          The channel which has been terminated
 * @param[out]    reason_out      Pointer to where the channel closure reason will be written.
 *
 * @retval MV_STATUS_PARAMETERFAULT The supplied pointer is not readable or writable.
 * @retval MV_STATUS_INVALIDHANDLE The specified handle is not a valid channel handle.
 */
enum MvStatus mvGetChannelClosureReason(MvChannelHandle handle, enum MvClosureReason *reason_out);

/**
 *  Send HTTP request to a channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 * @param[in]     request         A pointer to the struct describing the request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `request` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `request`'s nested pointers are illegal. The stream is not usable after that and should be closed.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid HTTP channel handle.
 * @retval MV_STATUS_INVALIDBUFFERSIZE Request doesn't fit the send buffer.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_REQUESTALREADYSENT The request has already been sent either successfully or not.
 */
enum MvStatus mvSendHttpRequest(MvChannelHandle handle, const struct MvHttpRequest *request);

/**
 *  Read the status of an HTTP response on a channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel on which the response was received.
 * @param[out]    response_data    The result of the HTTP request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `response_data` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid HTTP channel handle.
 * @retval MV_STATUS_RESPONSENOTPRESENT No HTTP response is present.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvReadHttpResponseData(MvChannelHandle handle, struct MvHttpResponseData *response_data);

/**
 *  Read header data from an HTTP response.
 *
 * Parameters:
 * @param         handle          The handle of the channel on which the response was received.
 * @param         header_index    The index of the header to read.
 * @param[out]    buf             The buffer into which to copy the header data.
 * @param         size            The size of `buf` in bytes.
 *
 * @retval MV_STATUS_PARAMETERFAULT `buf` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid HTTP channel handle.
 * @retval MV_STATUS_RESPONSENOTPRESENT No HTTP response is present.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_HEADERINDEXINVALID `header_index` is out of bounds.
 */
enum MvStatus mvReadHttpResponseHeader(MvChannelHandle handle, uint32_t header_index, uint8_t *buf, uint32_t size);

/**
 *  Read body data from an HTTP response.
 *
 * Parameters:
 * @param         handle          The handle of the channel on which the response was received.
 * @param         offset          The byte offset from the start of the body to read from.
 * @param[out]    buf             The buffer into which to copy the body data.
 * @param         size            The size of `buf` in bytes.
 *
 * @retval MV_STATUS_PARAMETERFAULT `buf` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid HTTP channel handle.
 * @retval MV_STATUS_RESPONSENOTPRESENT No HTTP response is present.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_OFFSETINVALID `offset` exceeds the size of the returned body.
 */
enum MvStatus mvReadHttpResponseBody(MvChannelHandle handle, uint32_t offset, uint8_t *buf, uint32_t size);

/**
 *  Enable logging to the server.
 *
 * Parameters:
 * @param[out]    buffer          The buffer to use for log  messages.
 * @param         length_bytes    The size of `buffer` in bytes.
 *
 * @retval MV_STATUS_PARAMETERFAULT `buffer` is an illegal pointer.
 * @retval MV_STATUS_INVALIDBUFFERALIGNMENT The value of `buffer` is not suitably aligned.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The value of `buffer` is mis-sized.
 */
enum MvStatus mvServerLoggingInit(uint8_t *buffer, uint32_t length_bytes);

/**
 *  Log a message to the server.
 *
 * Parameters:
 * @param[in]     message         The log message to send to the server.
 * @param         length_bytes    The size of `message` in bytes.
 *
 * @retval MV_STATUS_UNAVAILABLE Logging has not been enabled via `mvServerLoggingInit`.
 * @retval MV_STATUS_PARAMETERFAULT `message` is an illegal pointer.
 * @retval MV_STATUS_INVALIDBUFFERSIZE Not enough space in the buffer for the message.
 * @retval MV_STATUS_LOGMESSAGETOOLONG `message` exceeds the maximum allowed size.
 * @retval MV_STATUS_LOGGINGDISABLEDBYSERVER Logging has been disabled by the server.
 */
enum MvStatus mvServerLog(const uint8_t *message, uint16_t length_bytes);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MV_API_H
