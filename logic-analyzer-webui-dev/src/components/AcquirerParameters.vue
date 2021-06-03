<template>
    <div>
        <div style="margin-bottom: 15px;"></div>

        <div class="parameter-wrapper">
            <label class="form-label">
                Sample Rate
            </label>
            <select class="form-select" v-model="samplerate_formatted">
                <option v-for="possibleSampleRate in possibleSampleRatesWithUnit" v-bind:key="possibleSampleRate" v-bind:value="possibleSampleRate">
                    {{ possibleSampleRate.format({ precision: 4 }) }}
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

import { AcquirerRequestedOptions, AcquirerChosenOptions } from '../models/model';
import { PropType } from 'vue';
import { Options, Vue } from 'vue-class-component';
import * as Math from 'mathjs';

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
    samplecount: number = 0;
    gainPerChannel: Record<string, string> = {};
    probeAttenuationPerChannel: Record<string, string> = {};

    possibleSampleRatesWithUnit: Math.Unit[] = [];

    set samplerate_formatted(newSamplerate: Math.Unit) {
        this.samplerate_Hz = newSamplerate.toNumber("Hz");
    }

    get samplerate_formatted(): Math.Unit {
        return Math.unit(this.samplerate_Hz, "Hz");
    }

    set samplecount_formatted(newSamplecount: Math.Unit) {
        this.samplecount = newSamplecount.toNumber("")
    }

    get sampletime_us(): number {
        return 1e6 * this.samplecount / this.samplerate_Hz;
    }

    set sampletime_us(newValue: number) {
        this.samplecount = newValue * this.samplerate_Hz / 1e6;
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
        var _this = this;
        requestedOptions.samplerates_Hz.forEach(function(samplerate) {
            _this.possibleSampleRatesWithUnit.push(Math.unit(samplerate, "Hz"));
        });
        this.samplerate_Hz = requestedOptions.samplerates_Hz[0];
        this.samplecount = requestedOptions.maxSampleCount;
        
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
