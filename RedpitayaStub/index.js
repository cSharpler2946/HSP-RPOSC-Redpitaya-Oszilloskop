var WebSocketServer = require('websocket').server;
var http = require('http');
var pako = require('pako');
const util = require("util");

var server = http.createServer(function(request, response) {
    console.log((new Date()) + ' Received request for ' + request.url);
    response.writeHead(404);
    response.end();
});
server.listen(9200, function() {
    console.log((new Date()) + ' Server is listening on port 9200');
});

wsServer = new WebSocketServer({
    httpServer: server,
    // You should not use autoAcceptConnections for production
    // applications, as it defeats all standard cross-origin protection
    // facilities built into the protocol and the browser.  You should
    // *always* verify the connection's origin and decide whether or not
    // to accept it.
    autoAcceptConnections: false
});

function originIsAllowed(origin) {
  // put logic here to detect whether the specified origin is allowed.
  return true;
}

var connection = null;
var dataToSend = {
    signals: {},
    parameters: {}
};
var state = "";

var receivedData = {
    signals: {},
    parameters: {}
}

var chosenDecoderNew = false;

const decoders =
[
    {
        "id": "I2C",
        "name": "I2C",
        "longname": "I squared C",
        "desc": "Synchronous open drain protocol with one controller and many peripherals."
    },
    {
        "id": "UART",
        "name": "UART",
        "longname": "UART RS232",
        "desc": "Asynchronous point-to-point protocol. Very old."
    }
];

const decoder_options =
{
    "I2C":
    [
        {
            "id": "address_format",
            "desc": "Displayed slave address format",
            "default": "shifted",
            "values": ["shifted", "unshifted"]
        }
    ],
    "UART":
    [
        {'id': 'baudrate', 'desc': 'Baud rate', 'default': 115200},
        {'id': 'data_bits', 'desc': 'Data bits', 'default': 8,
            'values': [5, 6, 7, 8, 9]},
        {'id': 'parity', 'desc': 'Parity', 'default': 'none',
            'values': ['none', 'odd', 'even', 'zero', 'one', 'ignore']},
        {'id': 'stop_bits', 'desc': 'Stop bits', 'default': 1.0,
            'values': [0.0, 0.5, 1.0, 1.5]},
        {'id': 'bit_order', 'desc': 'Bit order', 'default': 'lsb-first',
            'values': ['lsb-first', 'msb-first']},
        {'id': 'format', 'desc': 'Data format', 'default': 'hex',
            'values': ['ascii', 'dec', 'hex', 'oct', 'bin']},
        {'id': 'invert_rx', 'desc': 'Invert RX', 'default': 'no',
            'values': ['yes', 'no']},
        {'id': 'invert_tx', 'desc': 'Invert TX', 'default': 'no',
            'values': ['yes', 'no']},
        {'id': 'sample_point', 'desc': 'Sample point (%)', 'default': 50},
        {'id': 'rx_packet_delim', 'desc': 'RX packet delimiter (decimal)',
            'default': -1},
        {'id': 'tx_packet_delim', 'desc': 'TX packet delimiter (decimal)',
            'default': -1},
        {'id': 'rx_packet_len', 'desc': 'RX packet length', 'default': -1},
        {'id': 'tx_packet_len', 'desc': 'TX packet length', 'default': -1},
    ]
}

const decoder_channels = {
    "I2C": [
        {'id': 'scl', 'name': 'SCL', 'desc': 'Serial clock line', 'isOptional': false},
        {'id': 'sda', 'name': 'SDA', 'desc': 'Serial data line', 'isOptional': false},
    ],
    "UART": [
        {'id': 'rx', 'name': 'RX', 'desc': 'UART receive line', 'isOptional': true},
        {'id': 'tx', 'name': 'TX', 'desc': 'UART transmit line', 'isOptional': true},
    ]
}

wsServer.on('request', function(request) {
    if (!originIsAllowed(request.origin)) {
      // Make sure we only accept requests from an allowed origin
      request.reject();
      console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
      return;
    }
    
    connection = request.accept(null, request.origin);
    state = "Initial";
    console.log((new Date()) + ' Connection accepted.');
    stm_doStep();
    connection.on('message', function(message) {
        if (message.type === 'utf8') {
            var receivedDecoded = JSON.parse(message.utf8Data);
            Object.assign(receivedData["parameters"], receivedDecoded["parameters"]);
            Object.assign(receivedData["signals"], receivedDecoded["signals"]);
            if(receivedDecoded["parameters"]["CHOSEN_DECODER"]) {
                chosenDecoderNew = true;
            }
            else {
                chosenDecoderNew = false;
            }
            console.log("Received message. New receivedData:");
            console.log(util.inspect(receivedData, true, 10));
            stm_doStep();
        }
        else if (message.type === 'binary') {
            console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
        }
    });
    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
    });
});

function allOptionsValid() {
    return true;
}

function stm_doStep() {
    switch(state) {
        case "Initial":
            console.log("Sending decoder list.");
            var decoders_json_repr = decoders.map(JSON.stringify);
            dataToSend.signals["SRD_DECODER_LIST"] = {value: decoders_json_repr};
            send_data();
            state = "SelectingDecoder";
            break;
        case "SelectingDecoder":
            if(receivedData.parameters["CHOSEN_DECODER"]) {
                var chosenDecoder = JSON.parse(receivedData.parameters["CHOSEN_DECODER"].value);
                var chosen_decoder_options = decoder_options[chosenDecoder["id"]];
                var chosen_decoder_options_json_repr = chosen_decoder_options.map(JSON.stringify);
                var chosen_decoder_channels = decoder_channels[chosenDecoder["id"]];
                var chosen_decoder_channels_json_repr = chosen_decoder_channels.map(JSON.stringify);
                dataToSend.parameters = {};
                dataToSend.signals = {};
                dataToSend.signals["SRD_REQUESTED_OPTIONS"] = { value: chosen_decoder_options_json_repr };
                dataToSend.signals["SRD_CHANNELS"] = { value: chosen_decoder_channels_json_repr };
                send_data();
                state = "UserSetsOptions";
            }
            break;
        case "UserSetsOptions":
            if(allOptionsValid()) {
                dataToSend.parameters = {};
                dataToSend.signals = {};
                dataToSend.parameters["ALL_OPTIONS_VALID"] = { value: JSON.stringify(true) };
                send_data();
            }
            if(chosenDecoderNew) {
                var chosenDecoder = JSON.parse(receivedData.parameters["CHOSEN_DECODER"].value);
                var chosen_decoder_options = decoder_options[chosenDecoder["id"]];
                var chosen_decoder_options_json_repr = chosen_decoder_options.map(JSON.stringify);
                var chosen_decoder_channels = decoder_channels[chosenDecoder["id"]];
                var chosen_decoder_channels_json_repr = chosen_decoder_channels.map(JSON.stringify);
                dataToSend.parameters = {};
                dataToSend.signals = {};
                dataToSend.signals["SRD_REQUESTED_OPTIONS"] = {value: chosen_decoder_options_json_repr};
                dataToSend.signals["SRD_CHANNELS"] = { value: chosen_decoder_channels_json_repr };
                send_data();
            }
    }
}

function send_data() {
    console.log(util.inspect(dataToSend, true, 10));
    var compressed = pako.deflate(JSON.stringify(dataToSend));
    connection.sendBytes(Buffer.from(compressed));
}