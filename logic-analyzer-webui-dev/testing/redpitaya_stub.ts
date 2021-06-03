import * as $ from "jquery";
import pako from '../src/libs/pako.js'
import * as Model from '../src/models/model'

const testData = require("./test-data.json");

class RedPitayaStub {
    decoders: Model.Decoder[]
    requestedOptions: Model.DecoderOption[]
    decoderChannels: Model.DecoderChannel[]
    acquirerOptions: Model.AcquirerRequestedOptions
    webSocket: WebSocket

    constructor(decoders: Model.Decoder[], requestedOptions: Model.DecoderOption[],
        decoderChannels: Model.DecoderChannel[], acquirerOptions: Model.AcquirerRequestedOptions) {
        this.decoders = decoders
        this.requestedOptions = requestedOptions
        this.decoderChannels = decoderChannels
        this.acquirerOptions = acquirerOptions
        this.webSocket = new WebSocket('ws://localhost:9200')
        this.webSocket.binaryType = 'arraybuffer'
        this.start()
    }

    start() {
        var myself = this

        console.log("start().");
        var xhr = new XMLHttpRequest();
        xhr.open('GET', 'localhost');
        xhr.onload = function() {
            if (xhr.status === 200) {
                console.log("start is good.")
                myself.connectWebSocket();
            }
            else {
                console.log("start is not good.")
                console.error(`Unknown error: Could not start the application.`)
            }
        };
        xhr.send();
        console.log("end start().");
    }

    connectWebSocket() {
        var myself = this
        if (this.webSocket) {
            this.webSocket.onopen = function () {
                console.log('Socket opened')
            }
            this.webSocket.onclose = function () {
                console.log('Socket closed')
            }
            this.webSocket.onerror = function (ev) {
                console.log('Websocket error: ', ev)
            }
            this.webSocket.onmessage = function (ev) {
                console.log('Myself:')
                console.log(myself)
                try {
                    var data = new Uint8Array(ev.data)
                    var inflate = pako.inflate(data)
                    var text = new TextDecoder().decode(new Uint8Array(inflate))
                    var receive = JSON.parse(text)
                    if (receive.signals) {
                        if (receive.signals.SRD_DECODER_LIST) {
                            var decoders_json_repr = receive.signals.SRD_DECODER_LIST.value
                            var new_decoder_list = decoders_json_repr.map(JSON.parse)
                            myself.decoders.splice(0)
                            myself.decoders.push(...new_decoder_list)
                        }

                        if (receive.signals.SRD_REQUESTED_OPTIONS) {
                            var requested_options_json_repr = receive.signals.SRD_REQUESTED_OPTIONS.value
                            var requested_options_list = requested_options_json_repr.map(JSON.parse)
                            myself.requestedOptions.splice(0)
                            myself.requestedOptions.push(...requested_options_list)
                        }
                        if (receive.signals.SRD_CHANNELS) {
                            var decoderChannels_json_repr = receive.signals.SRD_CHANNELS.value
                            var decoderChannels_list = decoderChannels_json_repr.map(JSON.parse)
                            myself.decoderChannels.splice(0)
                            myself.decoderChannels.push(...decoderChannels_list)
                        }
                    }
                    if (receive.parameters) {
                        if (receive.parameters.ACQ_REQUESTED_OPTIONS) {
                            var acqReqOptions = JSON.parse(receive.parameters.ACQ_REQUESTED_OPTIONS.value)
                            console.log('received requested options')
                            Object.assign(myself.acquirerOptions, acqReqOptions)
                            // myself.acquirerOptions = acqReqOptions;
                            console.log(myself.acquirerOptions.samplerates_Hz)
                        }
                    }

                    console.log('received:')
                    console.log(receive)
                } catch (e) {
                    console.log(e)
                } finally {
                    console.log('------------------------------------------')
                }
            }
        }
    }

    sendSelectedDecoder(selectedDecoder: Model.Decoder) {
        var parameters: any = {}
        parameters["CHOSEN_DECODER"] = { value: JSON.stringify(selectedDecoder, null, 4) }
        this.webSocket.send(JSON.stringify({ parameters: parameters }))
        console.log('sending decoder')
        var self = this
        /*setTimeout(function () { console.log('sending decoder'); self.webSocket.send(JSON.stringify({ parameters: parameters })) }, 50)
        setTimeout(function () { console.log('sending decoder'); self.webSocket.send(JSON.stringify({ parameters: parameters })) }, 50)*/
    }

    sendChosenOptions(currentChosenOptions: { [id: string]: string }) {
        var parameters: any = {}
        var option_list = []
        for (var option_id in currentChosenOptions) {
            option_list.push(
                {
                    id: option_id,
                    value: currentChosenOptions[option_id]
                }
            )
        }
        //var option_list_json = option_list.map(option => JSON.stringify(option))
        parameters.SRD_CHOSEN_OPTIONS = { value: JSON.stringify(option_list) }
        this.webSocket.send( JSON.stringify({ parameters: parameters }))
    }

    sendAcquirerOptions(chosenAcquirerOptions: Model.AcquirerChosenOptions) {
        console.log("Sending acquirer options");
        var innerJson = JSON.stringify(chosenAcquirerOptions);
        var parameters: any = {};
        parameters.ACQ_CHOSEN_OPTIONS = { value: innerJson };
        this.webSocket.send(JSON.stringify({ parameters: parameters }));
    }

    receiveData() {
        console.log("data received.");

        testData.data = testData.data.slice(0, 32)
        testData.name = testData.acqChannel;
        return testData;
    }
}

export default RedPitayaStub
