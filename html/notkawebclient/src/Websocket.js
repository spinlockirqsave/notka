
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

function debug(message) {
    console.log(message);
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
            };

            websocket.onerror = function (evt) {
                debug('ERROR: ' + evt.data);
            };
        } catch (exception) {
            debug('ERROR: ' + exception);
        }
    }
};
