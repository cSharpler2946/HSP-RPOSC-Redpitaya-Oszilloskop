import $ from './libs/jquery-3.5.1.min.js';
import pako from './libs/pako.js';

class RedPitayaStub {
    constructor(decoders) {
        this.decoders = decoders;
    }

    start(){
        var myself = this; //What is this shit? Why is this necessary?
        if (window.WebSocket) {
            this.webSocket = new WebSocket("ws://localhost:9200");
            this.webSocket.binaryType = "arraybuffer";
        } else {
            console.log('Browser does not support WebSocket');
        }

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
                console.log("Myself:")
                console.log(myself);
                try {
                    var data = new Uint8Array(ev.data);
                    var inflate = pako.inflate(data);
                    var text = new TextDecoder().decode(new Uint8Array(inflate));
                    var receive = JSON.parse(text);
                    if(receive.signals) {
                        if(receive.signals["SRD_DECODER_LIST"]) {
                            var new_decoder_list = JSON.parse(receive.signals["SRD_DECODER_LIST"]);
                            myself.decoders.splice(0);
                            myself.decoders.push(...new_decoder_list);
                        }
                    }
                    console.log("received:")
                    console.log(receive)
                } catch (e) {
                    console.log(e);
                } finally {
                    console.log("");
                }
            };
        }
    }
}

export default RedPitayaStub;