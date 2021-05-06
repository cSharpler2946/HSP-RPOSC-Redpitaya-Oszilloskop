<template>
    <div>
        <div style="margin-bottom: 15px;"></div>

        <div class="parameter-wrapper">
            <label class="form-label">
                Sample Rate
            </label>
            <select class="form-select" v-model="samplerate_Hz">
                <option v-for="possibleSampleRate in requestedOptions.samplerates_Hz" v-bind:key="possibleSampleRate">
                    {{ possibleSampleRate }}
                </option>
            </select>
            <br/>
            <label class="form-label">
                Samples to measure
            </label>
            <input type="number" class="form-control form-control-sm" v-model="samplecount"/>
            <br/>
            <label class="form-label">
                Time to measure
            </label>
            <input type="number" class="form-control form-control-sm" v-model="sampletime_us"/>
            <br/>
            <label class="form-label">
                Gain
            </label>
            <div class="row" v-for="channel in requestedOptions['availableChannels']" v-bind:key="channel">
                <label v-bind:for="'gain-'+channel" class="col col-form-label">{{ channel }}</label>
                <div class="col-sm-10">
                    <select v-bind:id="'gain-'+channel" class="form-select" v-model="gainPerChannel[channel]">
                    <option v-for="possibleGain in requestedOptions['gains']" v-bind:key="possibleGain">
                        {{ possibleGain }}
                    </option>
                </select>
                </div>
            </div>
            <br/>
            <label class="form-label">
                Probe Attenuation
            </label>
            <div class="row" v-for="channel in requestedOptions['availableChannels']" v-bind:key="channel">
                <label v-bind:for="'attenuation-'+channel" class="col col-form-label">{{ channel }}</label>
                <div class="col-sm-10">
                    <select v-bind:id="'gain-'+channel" class="form-select" v-model="probeAttenuationPerChannel[channel]">
                        <option>1x</option>
                        <option>10x</option>
                    </select>
                </div>
            </div>
        </div>
    </div>
</template>

<script lang="ts">

import { AcquirerRequestedOptions, AcquirerChosenOptions } from '../models/model'
import { PropType } from 'vue';
import { Options, Vue } from 'vue-class-component'

@Options({
  name: 'AcquirerParameters',
  props: {
    requestedOptions: {
        type: Object as PropType<AcquirerRequestedOptions>,
        required: true
    }
  },
  watch: {
    requestedOptions: {
      handler: function (currentOptions, old) {
        console.log('current requested acq options:')
        console.log(currentOptions)
        this.setDefaultOptions(currentOptions);
      },
      deep: true
    },
    chosenOptions: {
      handler: function (currentOptions, old) {
        this.$emit('chosenAcquirerOptionsChanged', currentOptions)
      },
      deep: true
    },
  }
})

export default class AcquirerParameters extends Vue {

    samplerate_Hz: number = 0;
    _samplecount: number = 0;
    _sampletime_us: number = 0;
    gainPerChannel: Record<string, string> = {};
    probeAttenuationPerChannel: Record<string, string> = {};

    get samplecount(): number {
        return this._samplecount;
    }

    set samplecount(newValue: number) {
        this._samplecount = newValue;
        this._sampletime_us = newValue / this.samplerate_Hz;
    }

    get sampletime_us(): number {
        return this._sampletime_us;
    }

    set sampletime_us(newValue: number) {
        this._sampletime_us = newValue;
        this._samplecount = newValue * this.samplerate_Hz;
    }

    get chosenOptions(): AcquirerChosenOptions {
        return {
            samplerate_Hz: this.samplerate_Hz,
            samplecount: this.samplecount,
            sampletime_us: this.sampletime_us,
            gainPerChannel: this.gainPerChannel,
            probeAttenuationPerChannel: this.probeAttenuationPerChannel
        }
    }

    setDefaultOptions(requestedOptions: AcquirerRequestedOptions): void {
        this.samplerate_Hz = requestedOptions.samplerates_Hz[0];
        this.samplecount = requestedOptions.maxSampleCount;
        var _this = this;
        requestedOptions.availableChannels.forEach(function(channelName) {
            _this.gainPerChannel[channelName] = requestedOptions.gains[0];
            _this.probeAttenuationPerChannel[channelName] = "10x";
        });
    }
}
</script>

<style lang="scss" scoped>

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
