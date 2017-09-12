var React = require('react');
var ReactDOM = require('react-dom');
var FL = require('./FormLogin.js');
var FN = require('./FormNotka.js');


var wsUri = "ws://localhost:1235";
var websocket = null;
var endiannes = 0;      // 0 - le, 1 - be

function swap32(val) {
    return ((val & 0xFF) << 24)
           | ((val & 0xFF00) << 8)
           | ((val >> 8) & 0xFF00)
           | ((val >> 24) & 0xFF);
}

function check_endianness() {
    var arrayBuffer = new ArrayBuffer(2);
    var uint8Array = new Uint8Array(arrayBuffer);
    var uint16array = new Uint16Array(arrayBuffer);
    uint8Array[0] = 0xAA; // set first byte
    uint8Array[1] = 0xBB; // set second byte

    if (uint16array[0] === 0xBBAA)
        endiannes = 0; // le
    else if (uint16array[0] === 0xAABB)
        endiannes = 1; // be
    else {
        endiannes = 2;
        throw new Error("Weird ediannes!");
    }
}

/**
 * Creates a new Uint8Array based on two different ArrayBuffers
 *
 * @private
 * @param {ArrayBuffers} buffer1 The first buffer.
 * @param {ArrayBuffers} buffer2 The second buffer.
 * @return {ArrayBuffers} The new ArrayBuffer created out of the two.
 */
var _appendBuffer = function(buffer1, buffer2) {
  var tmp = new Uint8Array(buffer1.byteLength + buffer2.byteLength);
  tmp.set(new Uint8Array(buffer1), 0);
  tmp.set(new Uint8Array(buffer2), buffer1.byteLength);
  return tmp.buffer;
};

function tx_MsgSYN() {
        var msg = new ArrayBuffer(8);
        var bufView = new Uint32Array(msg);     // by default js uses big endian
        bufView[0] = 1;
        bufView[1] = 0;
        if ( websocket != null ) {
                if (endiannes === 0) {          // but if this machine is le
                        console.log("swapping");
                        bufView[0] = swap32(bufView[0]);
                        bufView[1] = swap32(bufView[1]);
                }

                websocket.send(msg);
                console.log( "sent MsgSYN :", '"'+msg+'"' );
                debug(msg);
        }
}

function LoginOK() {
    return (
                <div>
                <h1>OK</h1>
                </div>
    );
}

function LoginFailedNoUser() {
    return (
                <div>
                <h1>Login failed</h1>
                <h2>Register?</h2>
                </div>
    );
}

function LoginFailedWrongPass() {
    return (
                <div>
                <h1>Login failed</h1>
                <h2>Wrong password...</h2>
                </div>
    );
}

var WsState = {
    LOGIN:      0,
    LOGIN_PASS: 1,
    LOGGED_IN:  2,
    LOGGIN_FAIL: 3
}

var ws_state = WsState.LOGIN;

function rx_msg_login_ack(data) {
    var raw_msg = new Uint8Array(data);
    var error_code = raw_msg[8];
    if (error_code === 0) {
            // Login successful.
            const element = <LoginOK />;
            ReactDOM.render(element, document.getElementById('root'));

            ReactDOM.render(<FN.FormNotka />, document.getElementById('Notka-text'));
    } else if (error_code === 1) {
            // No such user.
            const element = <LoginFailedNoUser />;
            ReactDOM.render(element, document.getElementById('root'));
    } else {
            // Wrong password.
            var status = FL.FormLogin.getLoginState();
            if (status === WsState.LOGIN) {
                // Now give password.
                FL.FormLogin.updateLoginState(WsState.LOGIN_PASS);
                //const element = React.createElement(FL.FormLogin);
                ReactDOM.render(<FL.FormLogin />, document.getElementById('root'));
            } else {
                    const element = <LoginFailedWrongPass />
                    ReactDOM.render(element, document.getElementById('root'));
            }
    }
}

function rx_msg_notka(data) {
    var raw_msg = new Uint8Array(data);
    var raw_notka = raw_msg.subarray(8);
    var dec = new TextDecoder();
    var text = dec.decode(raw_notka);
    FN.FormNotka.setNotka(text);
    ReactDOM.render(<FN.FormNotka />, document.getElementById('Notka-text'));
}

function debug(message) {
    console.log(message);
}

var MsgTXId = {
        IdMsgUnknown            : -1,
        IdMsgHandshakeAck       : 1,
        IdMsgLoginAck           : 2,
        IdMsgNotka              : 3
}

module.exports = {
    initWebSocket: function () {
        try {
            if (websocket && websocket.readyState === 1)
                websocket.close();

            websocket = new WebSocket(wsUri);
            websocket.binaryType = 'arraybuffer';

            websocket.onopen = function (evt) {
                debug("CONNECTED");
                check_endianness();
                tx_MsgSYN();
            };

            websocket.onclose = function (evt) {
                debug("DISCONNECTED");
            };

            websocket.onmessage = function (evt) {
                debug( evt.data );
                var raw_msg = new Uint32Array(evt.data, 0, 2);
                if (endiannes === 0) {          // but if this machine is le
                        console.log("swapping");
                        raw_msg[0] = swap32(raw_msg[0]);
                        raw_msg[1] = swap32(raw_msg[1]);
                }
                var payload_id = raw_msg[0];
                var payload_len = raw_msg[1];
                debug("Payload id: " + payload_id);
                debug("Payload len: " + payload_len);

                switch (payload_id)
                {
                        case MsgTXId.IdMsgLoginAck: {
                                rx_msg_login_ack(evt.data);
                                break;
                        }
                        case MsgTXId.IdMsgNotka: {
                                rx_msg_notka(evt.data);
                                break;
                        }
                        default: {
                                break;
                        }
                }
            };

            websocket.onerror = function (evt) {
                debug('ERROR: ' + evt.data);
            };
        } catch (exception) {
            debug('ERROR: ' + exception);
        }
    },
    tx_msg_login: function(login, pass) {
            var msg = new ArrayBuffer(8);             // 4 bytes - id, 4 - len
            var bufView32 = new Uint32Array(msg);     // by default js uses big endian
            bufView32[0] = 2;                         // MsgRX::MsgLogin (rx from server point of view)
            bufView32[1] = 64;                        // payload length, login 32 bytes + pass 32 bytes
            if (endiannes === 0) {                    // but if this machine is le
                    console.log("swapping");
                    bufView32[0] = swap32(bufView32[0]);
                    bufView32[1] = swap32(bufView32[1]);
            }

            var login_bin = new ArrayBuffer(32);
            var bufView8 = new Uint8Array(login_bin);
            for (var i=0, strLen=login.length; i<strLen && i<32; i++) {
                    bufView8[i] = login.charCodeAt(i);
            }

            var pass_bin = new ArrayBuffer(32);
            bufView8 = new Uint8Array(pass_bin);
            for (i=0, strLen=pass.length; i<strLen && i<32; i++) {
                    bufView8[i] = pass.charCodeAt(i);
            }

            msg = _appendBuffer(msg, login_bin);
            msg = _appendBuffer(msg, pass_bin);

            if (websocket != null)
            {
                    websocket.send(msg);
                    console.log("tx_msg_login: " + msg);
            }
    },
    ws_state: ws_state,
    WsState: WsState,
};
