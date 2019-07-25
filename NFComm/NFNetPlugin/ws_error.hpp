#pragma once
#include<system_error>

namespace websocket
{
    enum class error;
    enum class condition;
}

namespace std {
    template<>
    struct is_error_code_enum<websocket::error>
    {
        static bool const value = true;
    };
    template<>
    struct is_error_condition_enum<websocket::condition>
    {
        static bool const value = true;
    };
} // std

namespace websocket
{
    enum class error
    {
        buffer_overflow = 800000,
        read_message_too_big,//The socket message exceeded the locally configured limit
        write_message_too_big,//The socket message exceeded the locally configured limit
        read_timeout, //socket read time out
        send_queue_too_big, // send message queue size exceeded config.h configured limit
        ws_bad_http_header,// websocket parse http header failed
        ws_bad_http_version,// The WebSocket handshake was not HTTP/1.1
        ws_bad_method,//The WebSocket handshake method was not GET
        ws_bad_http_status_code,//The WebSocket handshake http status code was not 101
        ws_no_host,//The WebSocket handshake Host field is missing
        ws_no_connection,//The WebSocket handshake Connection field is missing
        ws_no_connection_upgrade,//The WebSocket handshake Connection field is missing the upgrade token
        ws_no_upgrade,//The WebSocket handshake Upgrade field is missing
        ws_no_upgrade_websocket,//The WebSocket handshake Upgrade field is missing the websocket token
        ws_no_sec_key,//The WebSocket handshake Sec-WebSocket-Key field is missing
        ws_bad_sec_key,//The WebSocket handshake Sec-WebSocket-Key field is invalid
        ws_no_sec_version,//The WebSocket handshake Sec-WebSocket-Version field is missing
        ws_bad_sec_version,//The WebSocket handshake Sec-WebSocket-Version field is invalid
        ws_no_sec_accept,//The WebSocket handshake Sec-WebSocket-Accept field is missing
        ws_bad_sec_accept,//The WebSocket handshake Sec-WebSocket-Accept field is invalid
        ws_bad_opcode,//The WebSocket frame contained an illegal opcode
        ws_bad_data_frame,//The WebSocket data frame was unexpected
        ws_bad_continuation,//The WebSocket continuation frame was unexpected
        ws_bad_reserved_bits,//The WebSocket frame contained illegal reserved bits
        ws_bad_control_fragment,//The WebSocket control frame was fragmented
        ws_bad_control_size,//The WebSocket control frame size was invalid
        ws_bad_unmasked_frame,//The WebSocket frame was unmasked
        ws_bad_masked_frame,//The WebSocket frame was masked
        ws_bad_size,//The WebSocket frame size was not canonical
        bad_frame_payload,//The WebSocket frame payload was not valid utf8
        ws_closed,//The WebSocket receive close frame
        ws_complete_frame,//The WebSocket receive a complete frame
    };

    /// Error conditions corresponding to sets of error codes.
    enum class condition
    {
        /** The WebSocket handshake failed

            This condition indicates that the WebSocket handshake failed. If
            the corresponding HTTP response indicates the keep-alive behavior,
            then the handshake may be reattempted.
        */
        ws_handshake_failed = 1,

        /** A WebSocket protocol violation occurred

            This condition indicates that the remote peer on the WebSocket
            connection sent data which violated the protocol.
        */
        ws_protocol_violation
    };

    namespace detail
    {
        class error_codes : public std::error_category
        {
        public:
            const char* name() const noexcept override
            {
                return "websocket";
            }

            std::string message(int ev) const override
            {
                switch (static_cast<error>(ev))
                {
                default:
                case error::buffer_overflow: return "The websocket handshake buffer exceeded configured limit";
                case error::read_message_too_big:  return "The socket read message  exceeded the locally configured limit";
                case error::write_message_too_big: return "The socket write message  exceeded the locally configured limit";
                case error::read_timeout:  return "Socket read timeout";
                case error::send_queue_too_big: return "The socket send message queue size exceeded configured(config.hpp) limit";
                case error::ws_bad_http_header: return "Websocket parse http header failed";
                case error::ws_bad_http_version: return "The WebSocket handshake was not HTTP/1.1";
                case error::ws_bad_method: return "The WebSocket handshake method was not GET";
                case error::ws_bad_http_status_code: return "The WebSocket handshake http status code was not 101";
                case error::ws_no_host: return "The WebSocket handshake Host field is missing";
                case error::ws_no_connection: return "The WebSocket handshake Connection field is missing";
                case error::ws_no_connection_upgrade:  return "The WebSocket handshake Connection field is missing the upgrade token";
                case error::ws_no_upgrade: return "The WebSocket handshake Upgrade field is missing";
                case error::ws_no_upgrade_websocket:   return "The WebSocket handshake Upgrade field is missing the websocket token";
                case error::ws_no_sec_key: return "The WebSocket handshake Sec-WebSocket-Key field is missing";
                case error::ws_bad_sec_key: return "The WebSocket handshake Sec-WebSocket-Key field is invalid";
                case error::ws_no_sec_version: return "The WebSocket handshake Sec-WebSocket-Version field is missing";
                case error::ws_bad_sec_version: return "The WebSocket handshake Sec-WebSocket-Version field is invalid";
                case error::ws_no_sec_accept: return "The WebSocket handshake Sec-WebSocket-Accept field is missing";
                case error::ws_bad_sec_accept: return "The WebSocket handshake Sec-WebSocket-Accept field is invalid";

                case error::ws_bad_opcode: return "The WebSocket frame contained an illegal opcode";
                case error::ws_bad_data_frame: return "The WebSocket data frame was unexpected";
                case error::ws_bad_continuation: return "The WebSocket continuation frame was unexpected";
                case error::ws_bad_reserved_bits: return "The WebSocket frame contained illegal reserved bits";
                case error::ws_bad_control_fragment: return "The WebSocket control frame was fragmented";
                case error::ws_bad_control_size: return "The WebSocket control frame size was invalid";
                case error::ws_bad_unmasked_frame: return "The WebSocket frame was unmasked";
                case error::ws_bad_masked_frame: return "The WebSocket frame was masked";
                case error::ws_bad_size: return "The WebSocket frame size was not canonical";
                case error::bad_frame_payload: return "The WebSocket frame payload was not valid utf8";
                case error::ws_closed: return "The WebSocket receive close frame";
                }
            }

            std::error_condition default_error_condition(int ev) const noexcept override
            {
                switch (static_cast<error>(ev))
                {
                default:
                case error::buffer_overflow:
                case error::read_message_too_big:
                case error::write_message_too_big:
                case error::read_timeout:
                case error::send_queue_too_big:
                    return { ev, *this };
                case error::ws_bad_http_version:
                case error::ws_bad_method:
                case error::ws_no_host:
                case error::ws_no_connection:
                case error::ws_no_connection_upgrade:
                case error::ws_no_upgrade:
                case error::ws_no_upgrade_websocket:
                case error::ws_no_sec_key:
                case error::ws_bad_sec_key:
                case error::ws_no_sec_version:
                case error::ws_bad_sec_version:
                case error::ws_no_sec_accept:
                case error::ws_bad_sec_accept:
                    return condition::ws_handshake_failed;
                case error::ws_bad_opcode:
                case error::ws_bad_data_frame:
                case error::ws_bad_continuation:
                case error::ws_bad_reserved_bits:
                case error::ws_bad_control_fragment:
                case error::ws_bad_control_size:
                case error::ws_bad_unmasked_frame:
                case error::ws_bad_masked_frame:
                case error::ws_bad_size:
                case error::bad_frame_payload:
                    return condition::ws_protocol_violation;
                }
            }
        };

        class error_conditions : public std::error_category
        {
        public:
            const char* name() const noexcept override
            {
                return "websocket";
            }

            std::string message(int cv) const override
            {
                switch (static_cast<condition>(cv))
                {
                default:
                case condition::ws_handshake_failed: return "The WebSocket handshake failed";
                case condition::ws_protocol_violation: return "A WebSocket protocol violation occurred";
                }
            }
        };
    }

    inline std::error_code
        make_error_code(error e)
    {
        static detail::error_codes const cat{};
        return std::error_code{ static_cast<
            std::underlying_type<error>::type>(e), cat };
    }

    inline std::error_condition
        make_error_condition(condition c)
    {
        static detail::error_conditions const cat{};
        return std::error_condition{ static_cast<
            std::underlying_type<condition>::type>(c), cat };
    }
}