<template>
    <form method="post">
        <div class="parameter-wrapper">
        <label>Protocol decoder:</label>
        <select class="form-select" aria-label="Choose a protocol." v-model="selectedDecoder" v-on:change="$emit('selectedDecoderChanged', selectedDecoder)">
            <option :value="null" disabled hidden>Select decoder</option>
            <option v-for="decoder in decoders" v-bind:key="decoder" v-bind:value="decoder">
                {{ decoder["name"] }}
            </option>
        </select>
        </div>

        <hr/>


        <div style="margin-bottom: 15px;"></div>

        <div class="parameter-wrapper" v-for="option in requestedOptions" v-bind:key="option" v-bind:value="option">
            <label class="form-label">
                {{ option["desc"] }}
            </label>
            <input type="text" class="form-control form-control-sm" v-if="!('values' in option)" v-model.lazy="chosenOptions[option.id]" v-on:change="$emit('chosenOptionsChanged', chosenOptions)">
            <select class="form-select" v-if="'values' in option" v-model="chosenOptions[option.id]" v-on:change="$emit('chosenOptionsChanged', chosenOptions)">
                <option v-for="possibleValue in option['values']" v-bind:key="possibleValue">
                    {{ possibleValue }}
                </option>
            </select>
            <br/>
        </div>

        <!--div class="parameter-wrapper">
            <label for="sampleRange" class="form-label">Baud Rate:</label>
            <input type="range" class="form-range" id="sampleRange">

            <label for="sampleParam" class="col-sm-2 col-form-label">Sample Parameter:</label>
            <input type="text" class="form-control form-control-sm" placeholder="Sample param..." id="sampleParam">
        
            <div style="margin-bottom: 15px;"></div>

            <label>Data Polarity:</label>
            <div class="radio-buttons" style="float: left;">
                <div class="form-check form-check-inline">
                    <input class="form-check-input" type="radio" name="inlineRadioOptions" id="inlineRadio1" value="option1">
                    <label class="form-check-label" for="inlineRadio1">1</label>
                </div>
                <div class="form-check form-check-inline">
                    <input class="form-check-input" type="radio" name="inlineRadioOptions" id="inlineRadio2" value="option2">
                    <label class="form-check-label" for="inlineRadio2">2</label>
                </div>
            </div>
        </div>
    
        <br/>
        <hr/>

        <div class="parameter-wrapper">
            <div class="row">

            <label>Start-/Stop-Bits:</label>
            <div class="col">
                <input type="text" class="form-control form-control-sm" placeholder="start bit">
            </div>
            <div class="col">
                <input type="text" class="form-control form-control-sm" placeholder="stop bit">
            </div>
            </div>
        </div-->

    </form>
</template>

<script>
export default {
    name: "Parameters",
    props: {
        decoders: Array,
        requestedOptions: Array
    },
    data () {
        return {
            selectedDecoder: null,
            chosenOptions: {}
        }
    },
    watch: {
        'chosenOptions': {
            handler: function (currentOptions, old) {
                console.log("current options:");
                console.log(currentOptions);
            },
            deep: true
        },
        'requestedOptions': {
            handler: function (currentRequestedOptions) {
                console.log("setting default options");
                var _this = this;
                this.chosenOptions = {};
                currentRequestedOptions.forEach(function(requestedOption) {
                    _this.chosenOptions[requestedOption.id] = requestedOption.default;
                })
            },
            deep: true
        }
    }
}
</script>

<style scoped>

label {
    text-align: left;
    width: 100%;
    padding-bottom: 0;
    font-size: 0.9em;
}

.parameter-wrapper {
    padding-left: 15px;
    padding-right: 15px;
}

</style>
