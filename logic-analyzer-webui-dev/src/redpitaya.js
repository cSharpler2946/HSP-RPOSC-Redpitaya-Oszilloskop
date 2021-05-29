import $ from './libs/jquery-3.5.1.min.js'
import pako from './libs/pako.js'

class RedPitaya {
  constructor (app_id, app_url, socket_url, decoders, requestedOptions, decoderChannels, acquirerOptions) {
    this.app_id = app_id
    this.app_url = app_url
    this.socket_url = socket_url

    this.webSocket = null

    this.decoders = decoders
    this.requestedOptions = requestedOptions
    this.decoderChannels = decoderChannels
    this.acquirerOptions = acquirerOptions
  }

  test () {
    console.log('Successfully accessed the redpitaya instance.')
    console.log('app_id: ' + this.app_id)
    console.log('app_url: ' + this.app_url)
    console.log('socket_url: ' + this.socket_url)
  }

  start () {
    var self = this

    $.get(this.app_url)
      .done(function (dresult) {
        if (dresult.status == 'OK') {
          self.connectWebSocket()
        } else if (dresult.status == 'ERROR') {
          console.error(dresult.reason ? dresult.reason : `Could not start the application ${self.app_id}.`)
        } else {
          console.error(`Unknown error: Could not start the application [${self.app_id}].`)
        }
      })
      .fail(function () {
        console.error(`Failed to make a request to ${self.app_url}`)
      })
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
        myself.webSocket.send(toSend)
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
            if (Object.keys(receive.signals).length !== 0) {

            }

            if (receive.signals.SRD_DECODER_LIST) {
              var decoders_json_repr = receive.signals.SRD_DECODER_LIST.value
              console.log('decoders in json:')
              decoders_json_repr.forEach(decoder_json => console.log(decoder_json))
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
              myself.webSocket.send(toSend)
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

  sendSelectedDecoder (selectedDecoder) {
    var util = require('util')
    var parameters = {}
    parameters.CHOSEN_DECODER = { value: JSON.stringify(selectedDecoder) }
    console.log('sending decoder')
    var chosenDecoderJson = JSON.stringify({ parameters: parameters })
    console.log(util.inspect(chosenDecoderJson, true, 20))
    this.webSocket.send(chosenDecoderJson)
    var self = this
    setTimeout(function () { console.log('sending decoder'); self.webSocket.send(JSON.stringify({ parameters: parameters })) }, 50)
    setTimeout(function () { console.log('sending decoder'); self.webSocket.send(JSON.stringify({ parameters: parameters })) }, 50)
  }

  sendChosenOptions (currentChosenOptions) {
    var signals = {}
    var option_list = []
    for (var option_id in currentChosenOptions) {
      option_list.push(
        {
          id: option_id,
          value: currentChosenOptions[option_id]
        }
      )
    }
    //var option_list_json = option_list.map(JSON.stringify)
    signals.SRD_CHOSEN_OPTIONS = { value: JSON.stringify(option_list) }
    console.log("chosen options to send:");
    console.log(JSON.stringify({ signals: signals }));
    this.webSocket.send(JSON.stringify({ signals: signals }))
  }

  sendAcquirerOptions(chosenAcquirerOptions) {
    console.log("Sending acquirer options");
    var innerJson = JSON.stringify(chosenAcquirerOptions);
    var parameters = {};
    parameters.ACQ_CHOSEN_OPTIONS = { value: innerJson };
    this.webSocket.send(JSON.stringify({ parameters: parameters }));
  }

  receiveData (arg1, arg2) {
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
  }
}

// Required to import the RedPitaya prototype in other files
export default RedPitaya
