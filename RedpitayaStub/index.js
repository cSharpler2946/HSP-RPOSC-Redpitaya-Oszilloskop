var WebSocketServer = require('websocket').server;
var http = require('http');
var pako = require('pako');

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
var sharedData = {
    signals: {},
    parameters: {}
};
var state = "Initial";

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

wsServer.on('request', function(request) {
    if (!originIsAllowed(request.origin)) {
      // Make sure we only accept requests from an allowed origin
      request.reject();
      console.log((new Date()) + ' Connection from origin ' + request.origin + ' rejected.');
      return;
    }
    
    connection = request.accept(null, request.origin);
    console.log((new Date()) + ' Connection accepted.');
    stm_doStep();
    connection.on('message', function(message) {
        if (message.type === 'utf8') {
            console.log('Received Message: ' + message.utf8Data);
            //connection.sendUTF(message.utf8Data);
        }
        else if (message.type === 'binary') {
            console.log('Received Binary Message of ' + message.binaryData.length + ' bytes');
            //connection.sendBytes(message.binaryData);
        }
    });
    connection.on('close', function(reasonCode, description) {
        console.log((new Date()) + ' Peer ' + connection.remoteAddress + ' disconnected.');
    });
});

function stm_doStep() {
    switch(state) {
        case "Initial":
            console.log("Sending decoder list.");
            sharedData.signals["SRD_DECODER_LIST"] = JSON.stringify(decoders, null, 4);
            sync_data();
            break;
    }
}

function sync_data() {
    console.log(sharedData)
    var compressed = pako.deflate(JSON.stringify(sharedData));
    connection.sendBytes(Buffer.from(compressed));
}