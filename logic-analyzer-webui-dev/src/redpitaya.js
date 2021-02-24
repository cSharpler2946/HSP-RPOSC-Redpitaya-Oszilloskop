import $ from './libs/jquery-3.5.1.min.js';
import pako from './libs/pako.js';

class RedPitaya {
    constructor(app_id, app_url, socket_url, decoders, requestedOptions, decoderChannels) {

        this.app_id = app_id;
        this.app_url = app_url;
        this.socket_url = socket_url;

        this.webSocket = null;

        this.decoders = decoders;
        this.requestedOptions = requestedOptions;
        this.decoderChannels = decoderChannels;
    }

    test(){
        console.log("Successfully accessed the redpitaya instance.");
        console.log("app_id: " + this.app_id);
        console.log("app_url: " + this.app_url);
        console.log("socket_url: " + this.socket_url);
    }

    start(){
        var self = this;

        $.get(this.app_url)
            .done(function(dresult) {
                if (dresult.status == 'OK') {
                    self.connectWebSocket();
                } else if (dresult.status == 'ERROR') {
                    console.error(dresult.reason ? dresult.reason : `Could not start the application ${self.app_id}.` );
                } else {
                    console.error(`Unknown error: Could not start the application [${self.app_id}].`);
                }
            })
            .fail(function() {
                console.error(`Failed to make a request to ${self.app_url}`);
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
                    var text = new TextDecoder().decode(new Uint8Array(inflate));
                    var receive = JSON.parse(text);
                    if(receive.signals) {
                        if(Object.keys(receive.signals).length !== 0) {
                            console.log("received:")
                            console.log(receive.signals)
                        }
                        
                        if(receive.signals["SRD_DECODER_LIST"]) {
                            var decoders_json_repr = receive.signals["SRD_DECODER_LIST"].value;
                            var new_decoder_list = decoders_json_repr.map(JSON.parse);
                            myself.decoders.splice(0);
                            myself.decoders.push(...new_decoder_list);
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

    receiveData(arg1, arg2){
        // Holst daten
        // return data;
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