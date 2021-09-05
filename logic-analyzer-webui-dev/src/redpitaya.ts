import * as $ from "jquery";
import pako from './libs/pako.js'
import * as Model from '../src/models/model'

class RedPitaya {
  decoders: Model.Decoder[]
  requestedOptions: Model.DecoderOption[]
  decoderChannels: Model.DecoderChannel[]
  acquirerOptions: Model.AcquirerRequestedOptions
  logicSession: Model.LogicSession
  measuredData: Model.MeasuredData
  webSocket?: WebSocket;
  app_id: string
  app_url: string
  socket_url: string

  channelMap: Record<string, string> = {}

  constructor (app_id: string, app_url: string, socket_url: string,
    decoders: Model.Decoder[], requestedOptions: Model.DecoderOption[],
    decoderChannels: Model.DecoderChannel[], acquirerOptions: Model.AcquirerRequestedOptions,
    logicSession: Model.LogicSession, measuredData: Model.MeasuredData) {
    this.app_id = app_id
    this.app_url = app_url
    this.socket_url = socket_url
    this.webSocket = undefined
    this.acquirerOptions = acquirerOptions
    this.decoders = decoders
    this.requestedOptions = requestedOptions
    this.decoderChannels = decoderChannels
    this.logicSession = logicSession
    this.measuredData = measuredData
  }

  test () {
    console.log('Successfully accessed the redpitaya instance.')
    console.log('app_id: ' + this.app_id)
    console.log('app_url: ' + this.app_url)
    console.log('socket_url: ' + this.socket_url)
  }

  start () {
    var self = this

    var xhr = new XMLHttpRequest();
    xhr.open('GET', this.app_url);
    xhr.onload = function() {
        if (xhr.status === 200) {
            console.log("Application should start now.")
            self.connectWebSocket();
        }
        else {
            console.error(`Unknown error: Could not start the application.`)
        }
    };
    xhr.send();
  }

  connectWebSocket () {
    var myself = this
    // Create WebSocket
    if (window.WebSocket) {
      this.webSocket = new WebSocket(this.socket_url)
      this.webSocket.binaryType = 'arraybuffer'
    } else {
      console.log('Browser does not support WebSocket')
    }
    // Define WebSocket event listeners
    if (this.webSocket) {
      this.webSocket.onopen = function () {
        console.log('Socket opened')
        var toSend = JSON.stringify(
          {
            parameters: {
              WEBSOCKET_OPENED: {
                value: 'true'
              }
            },
            signals: {}
          }
        )
        console.log('sending startup:')
        console.log(toSend)
        myself.webSocket?.send(toSend)
      }
      this.webSocket.onclose = function () {
        console.log('Socket closed')
      }
      this.webSocket.onerror = function (ev) {
        console.log('Websocket error: ', ev)
      }
      this.webSocket.onmessage = function (ev) {
        try {
          var data = new Uint8Array(ev.data)
          var inflate = pako.inflate(data)
          var text = new TextDecoder().decode(new Uint8Array(inflate))
          console.log('received:')
          console.log(text)
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
            if (receive.parameters.WEBSOCKET_OPENED) {
              var toSend = JSON.stringify(
                {
                  parameters: {
                    WEBSOCKET_OPENED: {
                      value: 'true'
                    }
                  },
                  signals: {}
                }
              )
              console.log('sending startup:')
              console.log(toSend)
              myself.webSocket?.send(toSend)
            }
            if (receive.parameters.ACQ_REQUESTED_OPTIONS) {
              var acqReqOptions = JSON.parse(receive.parameters.ACQ_REQUESTED_OPTIONS.value)
              console.log('received requested options')
              Object.assign(myself.acquirerOptions, acqReqOptions)
              // myself.acquirerOptions = acqReqOptions;
              console.log(myself.acquirerOptions.samplerates_Hz)
          }
          }
        } catch (e) {
          console.log(e)
        } finally {
          console.log('')
        }
      }
    }
  }

  sendSelectedDecoder(selectedDecoder: Model.Decoder) {
    var parameters: any = {}
    parameters["CHOSEN_DECODER"] = { value: JSON.stringify(selectedDecoder, null, 4) }
    this.webSocket?.send(JSON.stringify({ parameters: parameters }))
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
    this.webSocket?.send(JSON.stringify({ parameters: parameters }))
  }

  sendAcquirerOptions(chosenAcquirerOptions: Model.AcquirerChosenOptions) {
    console.log("Sending acquirer options");
    var innerJson = JSON.stringify(chosenAcquirerOptions);
    var parameters: any = {};
    parameters.ACQ_CHOSEN_OPTIONS = { value: innerJson };
    this.webSocket?.send(JSON.stringify({ parameters: parameters }));
  }

  sendDecoderChannel(acquirerChannel: string, decoderChannel: string)
  {
        this.channelMap[acquirerChannel] = decoderChannel;
        var tupleList: Model.DecoderChannelTuple[] = []
        for(var acqChannel in this.channelMap) {
            tupleList.push({acqChannel: acqChannel, srdChannel: this.channelMap[acqChannel]});
        }

        var srdChannelMap = JSON.stringify(tupleList);
        var parameters: any = {};
        parameters.SRD_CHANNEL_MAP = {value: srdChannelMap};
        this.webSocket?.send(JSON.stringify({parameters: parameters}));
  }

  startAnalyzing() {
      console.log("Starting to capture.");

      var logicSession: Model.LogicSession = { measurementState: Model.MeasurementState.Starting };
      var logicSessionJSON = JSON.stringify(logicSession);
      var parameters: any = {};
      parameters.LOGIC_SESSION = { value: logicSessionJSON };
      this.webSocket?.send(JSON.stringify({ parameters: parameters }));
  }

  /*receiveData (arg1, arg2) {
    // Holst daten
    // return data;
  }

  dispatch_received_data (parameters, signals) {
    if (parameters) {
      if (parameters.WRITE_POINTER) {
        $('#write_pointer').val(parameters.WRITE_POINTER.value)
      }
    }
    if (signals) {
      if (signals.VOLTAGE) {
        $('#output_data').text(signals.VOLTAGE.value)
      }
    }
  }*/
}

// Required to import the RedPitaya prototype in other files
export default RedPitaya
