import $ from './libs/jquery-3.5.1.min.js';
import pako from './libs/pako.js';

class RedPitayaStub {
    constructor(decoders, requestedOptions, decoderChannels, acquirerOptions) {
        this.decoders = decoders;
        this.requestedOptions = requestedOptions;
        this.decoderChannels = decoderChannels;
        this.acquirerOptions = acquirerOptions;
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
                            var decoders_json_repr = receive.signals["SRD_DECODER_LIST"].value;
                            var new_decoder_list = decoders_json_repr.map(JSON.parse);
                            myself.decoders.splice(0);
                            myself.decoders.push(...new_decoder_list);
                        }
                        if(receive.parameters["ACQ_REQUESTED_OPTIONS"]) {
                            
                        }
                        if(receive.signals["SRD_REQUESTED_OPTIONS"]) {
                            var requested_options_json_repr = receive.signals["SRD_REQUESTED_OPTIONS"].value;
                            var requested_options_list = requested_options_json_repr.map(JSON.parse);
                            myself.requestedOptions.splice(0);
                            myself.requestedOptions.push(...requested_options_list);
                        }
                        if(receive.signals["SRD_CHANNELS"]) {
                            var decoderChannels_json_repr = receive.signals["SRD_CHANNELS"].value;
                            var decoderChannels_list = decoderChannels_json_repr.map(JSON.parse);
                            myself.decoderChannels.splice(0);
                            myself.decoderChannels.push(...decoderChannels_list);
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

    sendSelectedDecoder(selectedDecoder) {
        var parameters = {};
        parameters["CHOSEN_DECODER"] = { value: JSON.stringify(selectedDecoder, null, 4) };
        this.webSocket.send(JSON.stringify({ parameters: parameters }));
    }

    sendChosenOptions(currentChosenOptions) {
        var signals = {};
        var option_list = [];
        for(var option_id in currentChosenOptions) {
            option_list.push(
                {
                    id: option_id,
                    value: currentChosenOptions[option_id]
                }
            );
        }
        var option_list_json = option_list.map(JSON.stringify);
        signals["SRD_CHOSEN_OPTIONS"] = { value: option_list_json };
        this.webSocket.send(JSON.stringify({ signals: signals }));
    }
}

export default RedPitayaStub;