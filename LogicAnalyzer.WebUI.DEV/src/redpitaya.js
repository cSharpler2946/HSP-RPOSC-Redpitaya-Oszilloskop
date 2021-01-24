import $ from './libs/jquery-3.5.1.min.js';
import pako from './libs/pako.js';

class RedPitaya {
    constructor(app_id, app_url, socket_url) {

        this.app_id = app_id;
        this.app_url = app_url;
        this.socket_url = socket_url;

        this.webSocket = null;
    }

    test(){
        console.log("Successfully accessed the redpitaya instance.");
    }

    start() {
        $.get(this.app_url)
                .done(function(dresult) {
                    if (dresult.status == 'OK') {
                        this.connectWebSocket();
                    } else if (dresult.status == 'ERROR') {
                        console.log(dresult.reason ? dresult.reason : 'Could not start the application (ERR1)');
                        this.start();
                    } else {
                        console.log('Could not start the application (ERR2)');
                        this.start();
                    }
                })
                .fail(function() {
                    console.log('Could not start the application (ERR3)');
                    this.start();
                });
    }

    connectWebSocket() {
        //Create WebSocket
        if (window.WebSocket) {
            this.webSocket = new WebSocket(this.socket_url);
            this.webSocket.binaryType = "arraybuffer";
        } else {
            console.log('Browser does not support WebSocket');
        }
        // Define WebSocket event listeners
        if (this.webSocket) {
            this.webSocket.onopen = function() {
                console.log('Socket opened');               
            };
            this.webSocket.onclose = function() {
                console.log('Socket closed');
            };
            this.webSocket.onerror = function(ev) {
                console.log('Websocket error: ', ev);         
            };
            this.webSocket.onmessage = function(ev) {
                try {
                    var data = new Uint8Array(ev.data);
                    var inflate = pako.inflate(data);
                    //var text = String.fromCharCode.apply(null, new Uint8Array(inflate));
                    var text = new TextDecoder().decode(new Uint8Array(inflate));
                    var receive = JSON.parse(text);
                    this.dispatch_received_data(receive.parameters, receive.signals);
                } catch (e) {
                    console.log(e);
                } finally {
                    console.log("");
                }
            };
        }
    }

    dispatch_received_data(parameters, signals) {
        if (parameters) {
            if(parameters["WRITE_POINTER"]) {
                $("#write_pointer").val(parameters["WRITE_POINTER"].value)
            }
        }
        if (signals) {
            if(signals["VOLTAGE"]) {
                $("#output_data").text(signals["VOLTAGE"].value);
            }
        }
    }
}

// Required to import the RedPitaya prototype in other files
export default RedPitaya;