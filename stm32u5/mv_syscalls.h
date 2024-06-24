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
    MV_STATUS_LATEFAULT            = 0x13, //< Indicates the fault after an action was taken, e.g., error writing result of a successful operation.
    MV_STATUS_RESPONSENOTPRESENT   = 0x14, //< There is no HTTP response to read.
    MV_STATUS_INDEXINVALID         = 0x15, //< The index of a requested item is invalid.
    MV_STATUS_OFFSETINVALID        = 0x16, //< The offset into the http response body exceeds its size.
    MV_STATUS_REQUESTALREADYSENT   = 0x17, //< Request has already been sent, can't do it again over the same channel.
    MV_STATUS_REQUESTUNSUCCESSFUL  = 0x18, //< Request has failed, data can't be read.
    MV_STATUS_LOGMESSAGETOOLONG    = 0x19, //< The log message exceeds the maximum allowed size.
    MV_STATUS_LOGGINGDISABLEDBYSERVER = 0x1a, //< The server has disabled logging.
    MV_STATUS_TOOMANYCONFIGKEYS    = 0x1b, //< Too many config keys are being requested simultaneously.
    MV_STATUS_UNKNOWNCONFIGSTORE   = 0x1c, //< The config store type is unknown.
    MV_STATUS_UNKNOWNCONFIGSCOPE   = 0x1d, //< The config item scope is unknown.
    MV_STATUS_INVALIDCONFIGKEY     = 0x1e, //< The config key is invalid. Only printable Ascii is allowed.
    MV_STATUS_INVALIDAUTHENTICATION = 0x1f, //< The authentication supplied is invalid.
    MV_STATUS_WRONGDATAREQUESTED   = 0x20, //< The data requested is not what the queried object has.
    MV_STATUS_INTERNALERROR        = 0x21, //< Unexpected internal error.
    MV_STATUS_TOOMANYELEMENTS      = 0x22, //< Too many elements. Meaning of the element depends on request type.
    MV_STATUS_REQUIREDELEMENTMISSING = 0x23, //< A required element is missing from a request.
    MV_STATUS_MICROVISORBUSY       = 0x24, //< Could not perform the operation, because Microvisor is busy.
    MV_STATUS_ADDRESSOUTOFRANGE    = 0x25, //< Supplied address is out of range.
    MV_STATUS_INPUTTOOLONG         = 0x26, //< Supplied input buffer is too long for intended use.
    MV_STATUS__MAX                 = 0xffffffff, //< Ensure use of correct underlying size.
};

/**
 *  Notification event type.
 */
enum MvEventType {
    MV_EVENTTYPE_NOEVENT           = 0x0, //< Sentinel value meaning an illegal or absent event.
    MV_EVENTTYPE_NETWORKSTATUSCHANGED = 0x1, //< Network state change event, "Network" event.
    MV_EVENTTYPE_CHANNELDATAREADABLE = 0x2, //< Event notifying of a channel-readable event.
    MV_EVENTTYPE_CHANNELDATAWRITESPACE = 0x3, //< Event notifying of a channel write space event.
    MV_EVENTTYPE_CHANNELNOTCONNECTED = 0x4, //< Event notifying of a channel becoming disconnected.
    MV_EVENTTYPE_UPDATEDOWNLOADED  = 0x5, //< Network state change event, "Update" event.
    MV_EVENTTYPE__MAX              = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvNotification {
    /// When this notification was issued. This is the same time basis as `mvGetMicroseconds()`.
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

/// An opaque handle to a notification object. This is a 32-bit value chosen randomly by Microvisor. Zero is never a valid handle.

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
    MV_NETWORKSTATUS_DISCONNECTEDWITHLINKUP = 0x3, //< The device is disconnected from the server, but network link is up.
    MV_NETWORKSTATUS__MAX          = 0xffffffff, //< Ensure use of correct underlying size.
};

/**
 *  32-bit bitfield of current connectivity reasons.
 *  Reasons marked with `PREVENTING` will prevent connection attempts while the associated `MvNetworkReason` prevails.
 */
enum MvNetworkReason {
    MV_NETWORKREASON_NOTCONNECTING = 0x0, //< No pending connection attempts and connections not being prevented.
    MV_NETWORKREASON_USINGNETWORK  = 0x1, //< The application is using the network.
    MV_NETWORKREASON_NEVERUSEDNETWORKAPI = 0x2, //< The application has not yet used the networking API.
    MV_NETWORKREASON_MINIMUMCHECKINEXPIRED = 0x4, //< The minimum check-in period has been reached.
    MV_NETWORKREASON_RTCNOTSET     = 0x8, //< The RTC is not yet set.
    MV_NETWORKREASON_KERNELERROR   = 0x10, //< A kernel error requires reporting.
    MV_NETWORKREASON_APPLICATIONERROR = 0x20, //< An application error requires reporting.
    MV_NETWORKREASON_COLDBOOTCONNECTION = 0x40, //< A connection is needed on cold boot for updates.
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
    MV_CHANNELTYPE_MQTT            = 0x1, //< The channel carries MQTT data.
    MV_CHANNELTYPE_HTTP            = 0x2, //< The channel carries HTTP data.
    MV_CHANNELTYPE_CONFIGFETCH     = 0x3, //< The channel carries configuration and secrets data.
    MV_CHANNELTYPE__MAX            = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvSizedString {
    /// String data. Need not be nul-terminated.
    const uint8_t *data;
    /// The length of the string in bytes.
    uint32_t length;
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
            /// Endpoint identifier for cloud configuration.
            struct MvSizedString endpoint;
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
    MV_CLOSUREREASON_NOTYETOPEN    = 0x5, //< `mvOpenChannel()` was not called or failed on the channel.
    MV_CLOSUREREASON_CLOSEDBYAPPLICATION = 0x6, //< `mvCloseChannel()` has been called.
    MV_CLOSUREREASON_UNEXPECTEDLYTERMINATED = 0x7, //< The channel was terminated, but the reason is unclear.
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
    struct MvSizedString method;
    /// URL to access, only "https://" is supported.
    struct MvSizedString url;
    /// Number of headers in `headers` array.
    uint32_t num_headers;
    /// Headers for request.
    const struct MvHttpHeader *headers;
    /// Request body.
    struct MvSizedString body;
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

/**
 *  The scope for a configuration item. This determines visibility across an entire account or a single device.
 */
enum MvConfigKeyFetchScope {
    MV_CONFIGKEYFETCHSCOPE_ACCOUNT = 0x0, //< The requested key is scoped to all devices on the associated account.
    MV_CONFIGKEYFETCHSCOPE_DEVICE  = 0x1, //< The requested key is scoped solely to the subject device.
    MV_CONFIGKEYFETCHSCOPE__MAX    = 0xffffffff, //< Ensure use of correct underlying size.
};

/**
 *  The store for a configuration item. This determines whether a given key is write-only or read/write from the perspective of the Microvisor web facing REST API.
 */
enum MvConfigKeyFetchStore {
    MV_CONFIGKEYFETCHSTORE_SECRET  = 0x0, //< The requested key is in the web API write-only store.
    MV_CONFIGKEYFETCHSTORE_CONFIG  = 0x1, //< The requested key is in the web API readable/writable store.
    MV_CONFIGKEYFETCHSTORE__MAX    = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvConfigKeyToFetch {
    /// The scope for the requested item.
    enum MvConfigKeyFetchScope scope;
    /// The store in which the requested item is located: per device or per account.
    enum MvConfigKeyFetchStore store;
    /// The key name for the requested item.
    struct MvSizedString key;
};

struct MvConfigKeyFetchParams {
    /// The number of fetch requests in the `keys_to_fetch` array.
    uint32_t num_items;
    /// An array of fetch requests.
    const struct MvConfigKeyToFetch *keys_to_fetch;
};

/**
 *  Response result for the overall config fetch request.
 */
enum MvConfigFetchResult {
    MV_CONFIGFETCHRESULT_OK        = 0x0, //< The fetch operation succeeded.
    MV_CONFIGFETCHRESULT_RESPONSETOOLARGE = 0x1, //< The resulting response was too large.
    MV_CONFIGFETCHRESULT__MAX      = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvConfigResponseData {
    /// The result of the overall fetch request.
    enum MvConfigFetchResult result;
    /// The number of items returned by the fetch request. The order of the returned items matches the item request order.
    uint32_t num_items;
};

/**
 *  Response result for an individual key fetch request.
 */
enum MvConfigKeyFetchResult {
    MV_CONFIGKEYFETCHRESULT_OK     = 0x0, //< The requested item was found.
    MV_CONFIGKEYFETCHRESULT_KEYNOTFOUND = 0x1, //< The requested item was not found.
    MV_CONFIGKEYFETCHRESULT_SERVERERROR = 0x2, //< The request resulted in a server error.
    MV_CONFIGKEYFETCHRESULT__MAX   = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvSizedStringBuffer {
    /// A buffer into which Microvisor will write string data.
    uint8_t *data;
    /// The size of the buffer in bytes.
    uint32_t size;
    /// The number of bytes actually written.
    uint32_t *length;
};

struct MvConfigResponseReadItemParams {
    /// The index of the fetched item to read.
    uint32_t item_index;
    /// A pointer to the memory to copy the result of the fetch into.
    enum MvConfigKeyFetchResult *result;
    /// A buffer into which to copy the fetched item.
    struct MvSizedStringBuffer buf;
};

/**
 *  Indicates what data is readable for an MQTT channel.
 */
enum MvMqttReadableDataType {
    MV_MQTTREADABLEDATATYPE_NONE   = 0x0, //< No unconsumed data is available at this time.
    MV_MQTTREADABLEDATATYPE_MESSAGERECEIVED = 0x1, //< A message is ready for consumption.
    MV_MQTTREADABLEDATATYPE_MESSAGELOST = 0x2, //< A message was lost; details are available.
    MV_MQTTREADABLEDATATYPE_CONNECTRESPONSE = 0x3, //< A response to a connect request is available.
    MV_MQTTREADABLEDATATYPE_SUBSCRIBERESPONSE = 0x4, //< A response to a subscribe request is available.
    MV_MQTTREADABLEDATATYPE_UNSUBSCRIBERESPONSE = 0x5, //< A response to an unsubscribe request is available.
    MV_MQTTREADABLEDATATYPE_PUBLISHRESPONSE = 0x6, //< A response to a publish request is available.
    MV_MQTTREADABLEDATATYPE_DISCONNECTRESPONSE = 0x7, //< A response to a disconnect operation is available.
    MV_MQTTREADABLEDATATYPE__MAX   = 0xffffffff, //< Ensure use of correct underlying size.
};

/**
 *  MQTT version to speak with remote broker.
 */
enum MvMqttProtocolVersion {
    MV_MQTTPROTOCOLVERSION_V3_1_1  = 0x0, //< MQTT version 3.1.1.
    MV_MQTTPROTOCOLVERSION_V5      = 0x1, //< MQTT version 5.
    MV_MQTTPROTOCOLVERSION__MAX    = 0xffffffff, //< Ensure use of correct underlying size.
};

/**
 *  MQTT authentication method (v5 only).
 */
enum MvMqttAuthenticationMethod {
    MV_MQTTAUTHENTICATIONMETHOD_NONE = 0x0, //< No authentication used.
    MV_MQTTAUTHENTICATIONMETHOD_USERNAMEPASSWORD = 0x1, //< Standard authentication with username and password.
    MV_MQTTAUTHENTICATIONMETHOD__MAX = 0xff, //< Ensure use of correct underlying size.
};

struct MvUsernamePassword {
    /// A username for authentication.
    struct MvSizedString username;
    /// A password for authentication.
    struct MvSizedString password;
};

struct MvMqttAuthentication {
    /// The required authentication method.
    enum MvMqttAuthenticationMethod method;
    /// Authentication credentials in the form of a username/password pair.
    struct MvUsernamePassword username_password;
};

struct MvTlsCertificateChain {
    /// The number of certificates in the `certs` array.
    uint32_t num_certs;
    /// An array of the certificate(s) in the chain.
    const struct MvSizedString *certs;
};

struct MvOwnTlsCertificateChain {
    /// A certificate chain.
    struct MvTlsCertificateChain chain;
    /// A certificate key.
    struct MvSizedString key;
};

struct MvTlsCredentials {
    /// The server's Certificate Authority certificate.
    struct MvTlsCertificateChain cacert;
    /// The client certificate and key for TLS client authentication.
    struct MvOwnTlsCertificateChain clientcert;
};

struct MvMqttWill {
    /// The broker's Will topic.
    struct MvSizedString topic;
    /// The Will payload.
    struct MvSizedString payload;
    /// The Will MQTT Quality of Service level.
    uint32_t qos;
    /// A flag indicating whether the message should be the Will topic's retained message.
    uint8_t retain;
};

struct MvMqttConnectRequest {
    /// The MQTT protocol version used by the target broker.
    enum MvMqttProtocolVersion protocol_version;
    /// The target MQTT broker's hostname or IP address.
    struct MvSizedString host;
    /// The target MQTT broker's port number.
    uint16_t port;
    /// An MQTT client identifier (utf-8).
    struct MvSizedString clientid;
    /// MQTT Authentication credentials.
    struct MvMqttAuthentication authentication;
    /// TLS credentials.
    const struct MvTlsCredentials *tls_credentials;
    /// The connection keepalive interval in seconds.
    uint32_t keepalive;
    /// A flag indicating whether the session should be cleaned upon termination.
    uint8_t clean_start;
    /// An optional Will message for the connection.
    const struct MvMqttWill *will;
};

/**
 *  Result state of a given MQTT operation.
 */
enum MvMqttRequestState {
    MV_MQTTREQUESTSTATE_REQUESTCOMPLETED = 0x0, //< The request completed successfully.
    MV_MQTTREQUESTSTATE_INVALIDPARAMETERS = 0x1, //< Invalid parameters provided in the request.
    MV_MQTTREQUESTSTATE_ALREADYCONNECTED = 0x2, //< An MQTT connection was already attempted on this channel. Close the channel and open a fresh one.
    MV_MQTTREQUESTSTATE_NOTCONNECTED = 0x3, //< The connection was not open or failed at the point the request was made.
    MV_MQTTREQUESTSTATE_NXDOMAIN   = 0x4, //< Broker DNS resolution failed (MqttConnectResponse only).
    MV_MQTTREQUESTSTATE_UNKNOWNCA  = 0x5, //< An unknown Certificate Authority was specified (MqttConnectResponse only).
    MV_MQTTREQUESTSTATE_CERTIFICATEEXPIRED = 0x6, //< A certificate has expired (MqttConnectResponse only).
    MV_MQTTREQUESTSTATE_SOCKETERROR = 0x7, //< Other socket error (MqttConnectResponse only).
    MV_MQTTREQUESTSTATE_CONNECTIONCIRCUITBREAKER = 0x8, //< Microvisor server dropped the connection because incoming messages were rate limited.
    MV_MQTTREQUESTSTATE__MAX       = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvMqttConnectResponse {
    /// The status of the connection request.
    enum MvMqttRequestState request_state;
    /// An MQTT reason code as per specification.
    uint32_t reason_code;
};

struct MvMqttSubscription {
    /// The topic to subscribe to.
    struct MvSizedString topic;
    /// The desired MQTT Quality of Service level.
    uint32_t desired_qos;
    /// The request's desired no-local setting (v5 only).
    uint32_t nl;
    /// The request's desired retain-as-published setting (v5 only).
    uint32_t rap;
    /// The request's desired retain handling (v5 only).
    uint32_t rh;
};

struct MvMqttSubscribeRequest {
    /// A Correlation ID that will be returned in the response to this request. Requests may be fulfilled in a non-deterministic order.
    uint32_t correlation_id;
    /// An array of subscriptions.
    const struct MvMqttSubscription *subscriptions;
    /// The number of subscriptions in the `subscriptions` array.
    uint32_t num_subscriptions;
};

struct MvMqttSubscribeResponse {
    /// The result of the overall subscribe request.
    enum MvMqttRequestState *request_state;
    /// The operation's Correlation ID.
    uint32_t *correlation_id;
    /// A pointer to a results buffer which must be large enough to accept the `num_subscriptions` value of the subscribe request.
    uint32_t *reason_codes;
    /// The size of supplied codes buffer in bytes.
    uint32_t reason_codes_size;
    /// The number of reason codes written by Microvisor into the `reason_codes` array.
    uint32_t *reason_codes_len;
};

struct MvMqttUnsubscribeRequest {
    /// A Correlation ID that will be returned in the response to this request. Requests may be fulfilled in a non-deterministic order.
    uint32_t correlation_id;
    /// An array of topic names.
    const struct MvSizedString *topics;
    /// The number of topics in the `topics` array.
    uint32_t num_topics;
};

struct MvMqttUnsubscribeResponse {
    /// The result of the overall unsubscribe request.
    enum MvMqttRequestState *request_state;
    /// The operation's Correlation ID.
    uint32_t *correlation_id;
    /// A pointer to a results buffer which must be large enough to accept the `num_topics` value of the unsubscribe request.
    uint32_t *reason_codes;
    /// The size of supplied codes buffer in bytes.
    uint32_t reason_codes_size;
    /// The number of reason codes written by Microvisor into the `reason_codes` array.
    uint32_t *reason_codes_len;
};

struct MvMqttPublishRequest {
    /// A Correlation ID that will be returned in the response to this request. Requests may be fulfilled in a non-deterministic order.
    uint32_t correlation_id;
    /// The topic to publish the message to.
    struct MvSizedString topic;
    /// The message payload.
    struct MvSizedString payload;
    /// The MQTT Quality of Service level the broker is requested to deliver the message with.
    uint32_t desired_qos;
    /// A flag indicating whether the message should be the named topic's retained message.
    uint32_t retain;
};

struct MvMqttPublishResponse {
    /// The result of publish request.
    enum MvMqttRequestState request_state;
    /// The operation's Correlation ID.
    uint32_t correlation_id;
    /// The reason code returned by the broker. Will always be `0` for MQTT V3_1_1.
    uint32_t reason_code;
};

struct MvMqttMessage {
    /// The operation's Correlation ID.
    uint32_t *correlation_id;
    /// The topic via which the message was delivered.
    struct MvSizedStringBuffer topic;
    /// The message payload.
    struct MvSizedStringBuffer payload;
    /// The MQTT Quality of Service level the message was delivered with. If it is greater than `0`, you should acknowledge the message to avoid redelivery.
    uint32_t *qos;
    /// A flag indicating whether the message is the topic's retained message.
    uint8_t *retain;
};

/**
 *  Reason code for lost messages.
 */
enum MvMqttLostMessageReason {
    MV_MQTTLOSTMESSAGEREASON_DEVICERECEIVEBUFFERTOOSMALL = 0x0, //< The buffer allocated by the application was insufficient to receive the message. Consider increasing the size of the channel's RX buffer.
    MV_MQTTLOSTMESSAGEREASON__MAX  = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvMqttLostMessageInfo {
    /// The reason the message was lost.
    enum MvMqttLostMessageReason *reason;
    /// The lost message's topic.
    struct MvSizedStringBuffer topic;
    /// The length of the lost message in bytes.
    uint32_t *message_len;
};

struct MvMqttDisconnectResponse {
    /// The result of the disconnect operation.
    enum MvMqttRequestState request_state;
    /// The disconnect code received from the broker.
    uint32_t disconnect_code;
};

/**
 *  Power saving mode. Currently refer to STM32U5 modes.
 */
enum MvPowerSavingMode {
    MV_POWERSAVINGMODE_SLEEP       = 0x0, //< Sleep mode.
    MV_POWERSAVINGMODE_STOP0       = 0x1, //< Stop 0.
    MV_POWERSAVINGMODE_STOP1       = 0x2, //< Stop 1.
    MV_POWERSAVINGMODE_STOP2       = 0x3, //< Stop 2.
    MV_POWERSAVINGMODE_STOP3       = 0x4, //< Stop 3.
    MV_POWERSAVINGMODE__MAX        = 0xff, //< Ensure use of correct underlying size.
};

/**
 *  System-wide notification source.
 */
enum MvSystemNotificationSource {
    MV_SYSTEMNOTIFICATIONSOURCE_NETWORK = 0x0, //< Events (just `NetworkStatusChanged` now) related to being connected to Microvisor cloud
    MV_SYSTEMNOTIFICATIONSOURCE_UPDATE = 0x1, //< Events (just `UpdateDownloaded` now) related to application and kernel updates
    MV_SYSTEMNOTIFICATIONSOURCE__MAX = 0xffffffff, //< Ensure use of correct underlying size.
};

struct MvOpenSystemNotificationParams {
    /// The handle of the notification center that will dispatch notifications.
    MvNotificationHandle notification_handle;
    /// A value which allows you to reference the source of the notification.
    uint32_t notification_tag;
    /// What to be notified about.
    enum MvSystemNotificationSource notification_source;
};

/// An opaque handle to a system event object. This is a 32-bit value assigned by Microvisor. Zero is never a valid handle.

typedef struct MvSystemEventHandleOpaque *MvSystemEventHandle;

/**
 *  Deep sleep mode. Currently refer to STM32U5 modes.
 */
enum MvDeepSleepMode {
    MV_DEEPSLEEPMODE_STANDBY       = 0x0, //< Standby mode.
    MV_DEEPSLEEPMODE_SHUTDOWN      = 0x1, //< Shutdown mode.
    MV_DEEPSLEEPMODE__MAX          = 0xff, //< Ensure use of correct underlying size.
};

/**
 *  Wake-up or application restart reason
 */
enum MvWakeReason {
    MV_WAKEREASON_COLDBOOT         = 0x0, //< Cold boot or wake-up from Shutdown mode.
    MV_WAKEREASON_REMOTEMICROVISORRESTART = 0x1, //< Microvisor restart has been requested by server.
    MV_WAKEREASON_REMOTEAPPLICATIONRESTART = 0x2, //< Application restart has been requested by server.
    MV_WAKEREASON_DEBUGGERRESTART  = 0x3, //< Application has been restarted by debugger.
    MV_WAKEREASON_MICROVISORCRASH  = 0x4, //< Microvisor kernel crash.
    MV_WAKEREASON_MICROVISORWATCHDOG = 0x5, //< Microvisor watchdog failure.
    MV_WAKEREASON_MICROVISOROUTOFMEMORY = 0x6, //< Microvisor out of memory error.
    MV_WAKEREASON_MICROVISORERROR  = 0x7, //< Unspecified Microvisor error.
    MV_WAKEREASON_APPLICATIONCRASH = 0x8, //< Application crash.
    MV_WAKEREASON_APPLICATIONUPDATE = 0x9, //< Application has been updated.
    MV_WAKEREASON_MICROVISORUPDATE = 0xa, //< Microvisor has been updated.
    MV_WAKEREASON_OPTIONBYTES      = 0xb, //< Device option bytes have been updated.
    MV_WAKEREASON_DEEPSLEEPCHECKIN = 0xc, //< Device woken up from deep sleep due to check-in period expiration.
    MV_WAKEREASON_DEEPSLEEPAPPLICATION = 0xd, //< Device woken up from deep sleep due to a wake-up reason owned by application.
    MV_WAKEREASON_DEEPSLEEPMODEM   = 0xe, //< Device woken up from deep sleep due to cellular modem interrupt.
    MV_WAKEREASON_DEEPSLEEPAPPLICATIONRTC = 0xf, //< Device woken up from deep sleep due to application RTC wakeup.
    MV_WAKEREASON_DEEPSLEEPOTHER   = 0x10, //< Device woken up from deep sleep, the reason is unclear.
    MV_WAKEREASON__MAX             = 0xff, //< Ensure use of correct underlying size.
};

/**
 *  Restart mode.
 */
enum MvRestartMode {
    MV_RESTARTMODE_AUTOAPPLYUPDATE = 0x0, //< Perform either an application or device restart, depending on what updates are pending.
    MV_RESTARTMODE__MAX            = 0xffffffff, //< Ensure use of correct underlying size.
};

/// An opaque handle to an external flash object. This is a 32-bit value assigned by Microvisor. Zero is never a valid handle.

typedef struct MvExternalFlashHandleOpaque *MvExternalFlashHandle;

struct MvExternalFlashInfo {
    uint32_t version; //< version field; describes which other fields are valid
    union {
        struct {
            /// SPI flash chip ID.
            uint32_t chip_id;
            /// Number of bytes available to the application in bytes.
            uint32_t size;
            /// Number of open handles to the external flash.
            uint32_t num_handles;
        } v1;
    };
};

/**
 *  How wifi configuration is applied.
 */
enum MvWifiConfigMode {
    MV_WIFICONFIGMODE_APPLYNOW     = 0x0, //< Configuration overwrites existing settings and is used for all subsequent reconnection attempts.
    MV_WIFICONFIGMODE_CLEARNOW     = 0x1, //< Configuration is immediately cleared, reconnection disabled.
    MV_WIFICONFIGMODE__MAX         = 0xff, //< Ensure use of correct underlying size.
};

struct MvWifiConfig {
    uint32_t version; //< version field; describes which other fields are valid
    union {
        struct {
            /// How wifi configuration is applied.
            enum MvWifiConfigMode config_mode;
            /// Wifi SSID.
            struct MvSizedString wifi_ssid;
            /// Wifi password.
            struct MvSizedString wifi_password;
        } v1;
    };
};

struct MvWifiConfigOut {
    uint32_t version; //< version field; describes which other fields are valid
    union {
        struct {
            /// A buffer where the SSID will be written.
            struct MvSizedStringBuffer ssid;
            /// Pointer to byte where has_encryption flag will be written.
            uint8_t *has_encryption;
        } v1;
    };
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
 *  Write a peripheral register in secure space. The result of this
 *  operation is
 *      `*reg = (*reg & ~mask) ^ xorvalue`
 *  which means that
 *  a) bits of the register where `mask` is 0 are being xor-ed with
 *     respective bits of `xorvalue`
 *  b) bits of the register where `mask` is ` are being set to the
 *     value of respective bits of `xorvalue`
 *  This mechanism lets you set, reset or toggle multiple bits in one
 *  operation. E.g. `mvPeriphPoke(0x46020ca8, 0x00000003, 0x10000001)`
 *  toggles bit 28, resets bit 1 and sets bit 0 of the register at the
 *  address 0x46020ca8.
 *  Microvisor will ignore requests to write bits that application is not
 *  allowed to change while changing the bits it can on the same register.
 *
 * Parameters:
 * @param         reg             The address of the non-secure register being accessed.
 * @param         mask            A mask to indicate the operation over bits (xor for `0` assign for `1`)
 * @param         xorvalue        The argument to operation defined by `mask`.
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
enum MvStatus mvTempTriggerNotification(MvNotificationHandle handle, enum MvEventType type, uint32_t tag);

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
 *  Send an HTTP request to a channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 * @param[in]     request         A pointer to the struct describing the request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `request` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `request`'s nested pointers are illegal. The stream is not usable after that and should be closed.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid HTTP channel handle.
 * @retval MV_STATUS_INVALIDBUFFERSIZE Request doesn't fit the send buffer.
 * @retval MV_STATUS_TOOMANYELEMENTS Too many headers (>32) are attached to request.
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
 *  This function does not tell you what the length of the header is.
 *  Observe that HTTP headers may only contain printable Ascii characters,
 *  so you can using a terminator character outside this (like `NUL` is used
 *  in C) to learn the length after the call is complete. Ensure that if
 *  the header length is precisely `size` bytes the termination is still
 *  present.
 *
 * Parameters:
 * @param         handle          The handle of the channel on which the response was received.
 * @param         header_index    The index of the header to read.
 * @param[out]    buf             The buffer into which to copy the header data; up to `size` bytes.
 * @param         size            The size of `buf` in bytes.
 *
 * @retval MV_STATUS_PARAMETERFAULT `buf` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid HTTP channel handle.
 * @retval MV_STATUS_RESPONSENOTPRESENT No HTTP response is present.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_INDEXINVALID `header_index` is out of bounds.
 */
enum MvStatus mvReadHttpResponseHeader(MvChannelHandle handle, uint32_t header_index, uint8_t *buf, uint32_t size);

/**
 *  Read body data from an HTTP response.
 *
 *  To read the whole body, first learn its length using `mvReadHttpResponseData`.
 *  Then call this function repeatedly for successive offsets. The final
 *  read will only write the first bytes of `buf`; the remainder are left alone.
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

/**
 *  Send a configuration fetch request to a channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 * @param[in]     request         A pointer to the structure describing the request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `request` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `request`'s nested pointers are illegal. The channel is not usable after that and should be closed.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid fetch channel handle.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The request doesn't fit into the send buffer.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_REQUESTALREADYSENT The request has already been issued, successfully or not.
 */
enum MvStatus mvSendConfigFetchRequest(MvChannelHandle handle, const struct MvConfigKeyFetchParams *request);

/**
 *  Read the status of a configuration fetch response on a channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel on which the response was received.
 * @param[out]    response_data    A pointer to the fetch request's response.
 *
 * @retval MV_STATUS_PARAMETERFAULT `response_data` is an illegal pointer.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvReadConfigFetchResponseData(MvChannelHandle handle, struct MvConfigResponseData *response_data);

/**
 *  Read configuration item data from a fetch response.
 *
 * Parameters:
 * @param         handle          The handle of the channel on which the response was received.
 * @param[in]     params          A pointer to the parameters that will be used to access the configuration item.
 *
 * @retval MV_STATUS_PARAMETERFAULT `params` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid fetch channel handle.
 * @retval MV_STATUS_RESPONSENOTPRESENT No configuration response is present.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_INDEXINVALID `MvMqttConfigResponseReadItemParams`'s `item_index` value is out of bounds.
 */
enum MvStatus mvReadConfigResponseItem(MvChannelHandle handle, const struct MvConfigResponseReadItemParams *params);

/**
 *  Determine what type of MQTT message a ChannelDataReadable event has been raised for.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to obtain the data type.
 * @param[out]    data_type       A pointer to memory into which Microvisor will write the data type.
 *
 * @retval MV_STATUS_PARAMETERFAULT `data_type` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvMqttGetNextReadableDataType(MvChannelHandle handle, enum MvMqttReadableDataType *data_type);

/**
 *  Initiate an MQTT connection request. Only one connect is permitted per channel life cycle. To connect again create a new channel.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 * @param[in]     request         A pointer to the structure describing the request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `request` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `request`'s nested pointers are illegal. The channel is not usable after that and should be closed.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_REQUESTALREADYSENT A connection attempt has already been made on this channel.
 * @retval MV_STATUS_INVALIDAUTHENTICATION Both `username_password` and `cert_key` were supplied which is invalid.
 * @retval MV_STATUS_TOOMANYELEMENTS Too many (>8) certificates were included in either the CA or device certificate chains.
 * @retval MV_STATUS_REQUIREDELEMENTMISSING Either host name, client ID, client key (if a client certificate is specified),
or topic or payload of Will message (if specified) are missing.

 */
enum MvStatus mvMqttRequestConnect(MvChannelHandle handle, const struct MvMqttConnectRequest *request);

/**
 *  Read the MQTT connection response.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the response was received.
 * @param[out]    response_data    A pointer to the connection request's response.
 *
 * @retval MV_STATUS_PARAMETERFAULT `response_data` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT One of `response_data`'s' nested pointers is illegal.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvMqttReadConnectResponse(MvChannelHandle handle, struct MvMqttConnectResponse *response_data);

/**
 *  Initiate an MQTT subscription request. Only one subscribe request will be serviced at a time. Subscribing to multiple topics should be done with a single subscription request.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 * @param[in]     request         A pointer to the structure describing the request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `request` or some of nested pointers are illegal.
 * @retval MV_STATUS_LATEFAULT Some of `request`'s' nested pointers are illegal, and some data has already been sent. Channel cannot be used further.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_RATELIMITED There are too many subscribe requests in flight.
 * @retval MV_STATUS_TOOMANYELEMENTS Too many (>8) topics to subscribe to were included in the request.
 */
enum MvStatus mvMqttRequestSubscribe(MvChannelHandle handle, const struct MvMqttSubscribeRequest *request);

/**
 *  Read the MQTT subscription response.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the response was received.
 * @param[in]     response_data    A pointer to the subscription request's response.
 *
 * @retval MV_STATUS_PARAMETERFAULT `response_data` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT One of `response_data`'s' nested pointers is illegal.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The provided buffer for response codes is too small.
 */
enum MvStatus mvMqttReadSubscribeResponse(MvChannelHandle handle, const struct MvMqttSubscribeResponse *response_data);

/**
 *  Initiate an MQTT unsubscribe request. Only one unsubscribe request will be serviced at a time. Unsubscribing to multiple topics should be done with a single unsubscribe request.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 * @param[in]     request         A pointer to the structure describing the request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `request` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `request`'s' nested pointers are illegal, and some data has already been sent. Channel cannot be used further.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_RATELIMITED There are too many subscribe requests in flight.
 * @retval MV_STATUS_TOOMANYELEMENTS Too many (>8) topics to unsubscribe were included in the request.
 */
enum MvStatus mvMqttRequestUnsubscribe(MvChannelHandle handle, const struct MvMqttUnsubscribeRequest *request);

/**
 *  Read the MQTT unsubscribe response.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the response was received.
 * @param[in]     response_data    A pointer to the unsubscribe request's response.
 *
 * @retval MV_STATUS_PARAMETERFAULT `response_data` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT One of `response_data`'s' nested pointers is illegal.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The provided buffer for response codes is too small.
 */
enum MvStatus mvMqttReadUnsubscribeResponse(MvChannelHandle handle, const struct MvMqttUnsubscribeResponse *response_data);

/**
 *  Initiate an MQTT publish request.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 * @param[in]     request         A pointer to the structure describing the request.
 *
 * @retval MV_STATUS_PARAMETERFAULT `request` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `request`'s' nested pointers are illegal, and some data has already been sent. Channel cannot be used further.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_RATELIMITED There are too many subscribe requests in flight.
 */
enum MvStatus mvMqttRequestPublish(MvChannelHandle handle, const struct MvMqttPublishRequest *request);

/**
 *  Read the MQTT publish response.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the response was received.
 * @param[out]    response_data    A pointer to the publish request's response.
 *
 * @retval MV_STATUS_PARAMETERFAULT `response_data` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT One of `response_data`'s' nested pointers is illegal.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT request channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvMqttReadPublishResponse(MvChannelHandle handle, struct MvMqttPublishResponse *response_data);

/**
 *  Obtain a received MQTT message.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the message was received.
 * @param[in]     message_data    A pointer to the message structure Microvisor will populate.
 *
 * @retval MV_STATUS_PARAMETERFAULT `message_data` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `message_data`'s nested pointers are illegal.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_INVALIDBUFFERSIZE Response data doesn't fit into one of the provided buffers.
 */
enum MvStatus mvMqttReceiveMessage(MvChannelHandle handle, const struct MvMqttMessage *message_data);

/**
 *  Obtain information about a lost MQTT message.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the message was received.
 * @param[in]     lost_message_info    A pointer to the lost message structure Microvisor will populate.
 *
 * @retval MV_STATUS_PARAMETERFAULT `lost_message_info` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `lost_message_info`'s nested pointers are illegal.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_INVALIDBUFFERSIZE Response data doesn't fit into one of the provided buffers.
 */
enum MvStatus mvMqttReceiveLostMessageInfo(MvChannelHandle handle, const struct MvMqttLostMessageInfo *lost_message_info);

/**
 *  Acknowledge receipt of an MQTT message that was received with QoS 1 or 2.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the message was received.
 * @param         correlation_id    The Correlation ID of the message that will be acknowledged.
 *
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT request channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_RATELIMITED Too many acknowledge message operations in flight.
 */
enum MvStatus mvMqttAcknowledgeMessage(MvChannelHandle handle, uint32_t correlation_id);

/**
 *  Request an explicit disconnection from the MQTT broker.
 *
 * Parameters:
 * @param         handle          The handle of the channel over which to send the request.
 *
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 * @retval MV_STATUS_RATELIMITED Too many disconnect requests in flight.
 */
enum MvStatus mvMqttRequestDisconnect(MvChannelHandle handle);

/**
 *  Read the MQTT disconnect response.
 *
 * Parameters:
 * @param         handle          The handle of the channel via which the response was received.
 * @param[out]    response_data    A pointer to the disconnect request's response.
 *
 * @retval MV_STATUS_PARAMETERFAULT `response_data` is an illegal pointer.
 * @retval MV_STATUS_LATEFAULT Some of `response_data`'s nested pointers are illegal.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid MQTT channel handle.
 * @retval MV_STATUS_CHANNELCLOSED The specified channel is already closed.
 */
enum MvStatus mvMqttReadDisconnectResponse(MvChannelHandle handle, struct MvMqttDisconnectResponse *response_data);

/**
 *  Indicate to kernel that application testing has completed.
 *
 *  This system call only makes sense from application testing code.
 *  If called elsewhere, `Unavailable` is returned.
 *
 *  The application testing code is stopped by this call.
 *
 *  If the application testing code indicates success (with `status` == 0)
 *  the final application is installed from external flash (if present).
 *
 *  If the application testing code indicates failure (with `status` != 0)
 *  no further application is run.  The testing code will be re-run
 *  on reset.
 *
 * Parameters:
 * @param         status          Result of the application testing.  Zero means success, other values mean failure and are opaque to the kernel.
 *
 * @retval MV_STATUS_UNAVAILABLE Called outside application testing mode.
 */
enum MvStatus mvTestingComplete(uint32_t status);

/**
 *  Enable logging to the provisioning console during application test.
 *
 * Parameters:
 * @param[out]    buffer          The buffer to use for log  messages.
 * @param         length_bytes    The size of `buffer` in bytes.
 *
 * @retval MV_STATUS_PARAMETERFAULT `buffer` is an illegal pointer.
 * @retval MV_STATUS_INVALIDBUFFERALIGNMENT The value of `buffer` is not suitably aligned.
 * @retval MV_STATUS_INVALIDBUFFERSIZE The value of `buffer` is mis-sized.
 * @retval MV_STATUS_UNAVAILABLE Called outside application testing mode.
 */
enum MvStatus mvTestLoggingInit(uint8_t *buffer, uint32_t length_bytes);

/**
 *  Log a message to the provisioning console during application test.
 *
 * Parameters:
 * @param[in]     message         The log message to send to the server.
 * @param         length_bytes    The size of `message` in bytes.
 *
 * @retval MV_STATUS_UNAVAILABLE Logging has not been enabled via `mvTestLoggingInit`.
 * @retval MV_STATUS_PARAMETERFAULT `message` is an illegal pointer.
 * @retval MV_STATUS_INVALIDBUFFERSIZE Not enough space in the buffer for the message.
 * @retval MV_STATUS_LOGMESSAGETOOLONG `message` exceeds the maximum allowed size.
 */
enum MvStatus mvTestLog(const uint8_t *message, uint16_t length_bytes);

/**
 *  Enter a power saving mode
 *
 * Parameters:
 * @param         mode            The mode to enter.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT Unknown power saving mode.
 * @retval MV_STATUS_MICROVISORBUSY Cannot enter power-saving mode as network is still active.
 */
enum MvStatus mvPowerSave(enum MvPowerSavingMode mode);

/**
 *  Disable System LED to save power or enable it back
 *
 * Parameters:
 * @param         enable          `0` to disable, `1` to enable, other values are reserved.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 */
enum MvStatus mvSystemLedEnable(uint32_t enable);

/**
 *  Opens a stream of system-wide notifications to a notification channel
 *
 * Parameters:
 * @param[in]     params          Notification parameters
 * @param[out]    handle          A pointer to where the system event handle will be written.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT Invalid pointer to application memory.
 * @retval MV_STATUS_INVALIDHANDLE Invalid notification handle or event source.
 */
enum MvStatus mvOpenSystemNotification(const struct MvOpenSystemNotificationParams *params, MvSystemEventHandle *handle);

/**
 *  Closes system-wide notification stream
 *
 * Parameters:
 * @param[in,out] handle          A pointer to system notification handle.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_INVALIDHANDLE Invalid handle.
 */
enum MvStatus mvCloseSystemNotification(MvSystemEventHandle *handle);

/**
 *  Enter deep sleep
 *
 * Parameters:
 * @param         mode            Specific mode to enter.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT Unknown power saving mode.
 * @retval MV_STATUS_MICROVISORBUSY Cannot enter power-saving mode as network is still active.
 */
enum MvStatus mvDeepSleep(enum MvDeepSleepMode mode);

/**
 *  Get wake-up or application restart
 *
 * Parameters:
 * @param[out]    mode            Wake reason.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT Invalid pointer to ApplicationWakeReason.
 */
enum MvStatus mvGetWakeReason(enum MvWakeReason *mode);

/**
 *  Restart application and/or the device
 *
 * Parameters:
 * @param         mode            Type of restart to perform.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT Unknown restart mode.
 * @retval MV_STATUS_MICROVISORBUSY Cannot perform the chosen restart, because it will interrupt Microvisor's operation.
 */
enum MvStatus mvRestart(enum MvRestartMode mode);

/**
 *  Open external flash.
 *
 * Parameters:
 * @param[out]    handle          A pointer to where the external flash handle will be written.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT `handle_out` is an illegal pointer.
 * @retval MV_STATUS_TOOMANYCHANNELS Too many open handles. Current limit is 32.
 */
enum MvStatus mvExternalFlashOpen(MvExternalFlashHandle *handle);

/**
 *  Release external flash handle.
 *
 * Parameters:
 * @param[in,out] handle          A pointer to external flash handle.
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is not a valid external flash handle.
 */
enum MvStatus mvExternalFlashClose(MvExternalFlashHandle *handle);

/**
 *  Read data from external flash.
 *
 * Parameters:
 * @param         handle          External flash handle to use.
 * @param         address         Address to read from
 * @param         length          Number of bytes to read
 * @param[out]    buffer          Buffer to read the data to. Should be at least `length` bytes long
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT `buffer` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is invalid.
 * @retval MV_STATUS_ADDRESSOUTOFRANGE Address and/or length are out of range. Call `mvExternalFlashGetInfo` to get available flash size
 */
enum MvStatus mvExternalFlashReadBlocking(MvExternalFlashHandle handle, uint32_t address, uint32_t length, uint8_t *buffer);

/**
 *  Erase external flash. Both address and length should be aligned to 4096 byte boundary.
 *
 * Parameters:
 * @param         handle          External flash handle to use.
 * @param         address         Address to read from
 * @param         length          Number of bytes to read
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT `buffer` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is invalid.
 * @retval MV_STATUS_ADDRESSOUTOFRANGE Address and/or length are out of range. Call `mvExternalFlashGetInfo` to get available flash size
 * @retval MV_STATUS_INVALIDBUFFERALIGNMENT Address and/or length is not aligned to 4096 byte boundary
 */
enum MvStatus mvExternalFlashEraseBlocking(MvExternalFlashHandle handle, uint32_t address, uint32_t length);

/**
 *  Write data to external flash.
 *
 * Parameters:
 * @param         handle          External flash handle to use.
 * @param         address         Address to write to
 * @param         length          Number of bytes to write
 * @param[in]     buffer          Buffer to write the data from. Should be at least `length` bytes long
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT `buffer` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is invalid.
 * @retval MV_STATUS_ADDRESSOUTOFRANGE Address and/or length are out of range. Call `mvExternalFlashGetInfo` to get available flash size
 */
enum MvStatus mvExternalFlashWriteBlocking(MvExternalFlashHandle handle, uint32_t address, uint32_t length, const uint8_t *buffer);

/**
 *  Read data from external flash.
 *
 * Parameters:
 * @param         handle          External flash handle to use.
 * @param[out]    info            Where to save flash info. `version` should be set to 1 by the application
 *
 * @retval MV_STATUS_UNAVAILABLE Calling from an interrupt.
 * @retval MV_STATUS_PARAMETERFAULT `info` is an illegal pointer.
 * @retval MV_STATUS_INVALIDHANDLE `handle` is invalid.
 * @retval MV_STATUS_UNSUPPORTEDSTRUCTUREVERSION `info` structure has a version set to an unsupported value (only 1 is presently supported)
 */
enum MvStatus mvExternalFlashGetInfo(MvExternalFlashHandle handle, struct MvExternalFlashInfo *info);

/**
 *  Sets wifi configuration.
 *
 * Parameters:
 * @param[in]     params          Structure containing wifi configuration and application mode.
 *
 */
enum MvStatus mvSetWifiConfig(const struct MvWifiConfig *params);

/**
 *  Gets wifi configuration.
 *
 * Parameters:
 * @param[in]     params          Structure containing current wifi configuration.
 *
 */
enum MvStatus mvGetWifiConfig(const struct MvWifiConfigOut *params);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // MV_API_H
