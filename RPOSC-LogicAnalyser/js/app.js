/*
 * Red Pitaya Template Application
 *
 *
 * (c) Red Pitaya  http://www.redpitaya.com
 */


(function(APP, $, undefined) {

    // App configuration
    APP.config = {};
    APP.config.app_id = 'RPOSC-LogicAnalyser';
    APP.config.app_url = '/bazaar?start=' + APP.config.app_id + '?' + location.search.substr(1);
    APP.config.socket_url = 'ws://' + window.location.hostname + ':9002';

    // WebSocket
    APP.ws = null;




    // Starts template application on server
    APP.startApp = function() {

        $.get(APP.config.app_url)
            .done(function(dresult) {
                if (dresult.status == 'OK') {
                    APP.connectWebSocket();
                } else if (dresult.status == 'ERROR') {
                    console.log(dresult.reason ? dresult.reason : 'Could not start the application (ERR1)');
                    APP.startApp();
                } else {
                    console.log('Could not start the application (ERR2)');
                    APP.startApp();
                }
            })
            .fail(function() {
                console.log('Could not start the application (ERR3)');
                APP.startApp();
            });
    };




    APP.connectWebSocket = function() {

        //Create WebSocket
        if (window.WebSocket) {
            APP.ws = new WebSocket(APP.config.socket_url);
            APP.ws.binaryType = "arraybuffer";
        } else if (window.MozWebSocket) {
            APP.ws = new MozWebSocket(APP.config.socket_url);
            APP.ws.binaryType = "arraybuffer";
        } else {
            console.log('Browser does not support WebSocket');
        }


        // Define WebSocket event listeners
        if (APP.ws) {

            APP.ws.onopen = function() {
                console.log('Socket opened');
            };

            APP.ws.onclose = function() {
                console.log('Socket closed');
            };

            APP.ws.onerror = function(ev) {
                console.log('Websocket error: ', ev);
            };

            APP.ws.onmessage = function(ev) {
                try {
                    var data = new Uint8Array(ev.data);
                    var inflate = pako.inflate(data);
                    //var text = String.fromCharCode.apply(null, new Uint8Array(inflate));
                    var text = new TextDecoder().decode(new Uint8Array(inflate));
                    var receive = JSON.parse(text);

                    APP.dispatch_received_data(receive.parameters, receive.signals);

                } catch (e) {
                    console.log(e);
                } finally {}
            };
        }
    };

    APP.dispatch_received_data = function(parameters, signals) {
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

    };

}(window.APP = window.APP || {}, jQuery));



// Page onload event handler
$(function() {
    // Start application
    APP.startApp();

    APP.led_state = false;
    /*$('#led_state').click(function() {
        console.log('Button clicked');
        if(APP.led_state == true){
            $('#led_on').hide();
            $('#led_off').show();
            APP.led_state = false;
        }
        else{
            $('#led_off').hide();
            $('#led_on').show();
            APP.led_state = true;
        }

        var local = {};
        local['LED_STATE'] = { value: APP.led_state };
        APP.ws.send(JSON.stringify({ parameters: local }));

    })*/
    $("#trigger_acq").click(function() {
        console.log("Button clicked");
        $("#output_data").text("");
        var out_parameters = {};
        out_parameters["START_ACQUISITION"] = { value: true };
        APP.ws.send(JSON.stringify({ parameters: out_parameters }));
    })
});
