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
            <input class="form-control form-control-sm" v-model.lazy="samplecount_formatted"/>
            <br/>
            <label class="form-label">
                Time to measure
            </label>
            <input class="form-control form-control-sm" v-model.lazy="sampletime_formatted"/>
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
            <br/>
            <label class="form-label" style="color:red" v-show="samplecountInvalid">
                Error: The Samples to measure/time to sample is invalid.<br/>
                The maximum sample count is {{ maxSamplecount }}.<br/>
                At the current sample rate this translates to a maximum sample time of {{ maxSampletime_formatted }}.<br/>
            </label>
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
  beforeCreate () {
    var samples = Math.createUnit("Samples", {prefixes: "binary_short"});
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
        if(!this.samplecountInvalid) {
            this.$emit('chosenAcquirerOptionsChanged', currentOptions);
        }
      },
      deep: true
    },
  }
})

export default class AcquirerParameters extends Vue {

    samplerate_Hz: number = 0;
    samplecount: number = 0;
    maxSamplecount: number = 0;
    gainPerChannel: Record<string, string> = {};
    probeAttenuationPerChannel: Record<string, string> = {};

    possibleSampleRatesWithUnit: Math.Unit[] = [];

    set samplerate_formatted(newSamplerate: Math.Unit) {
        this.samplerate_Hz = newSamplerate.toNumber("Hz");
    }

    get samplerate_formatted(): Math.Unit {
        return Math.unit(this.samplerate_Hz, "Hz");
    }

    set samplecount_formatted(newSamplecount: string) {
        try {
            this.samplecount = Math.unit(newSamplecount).toNumber("Samples");
        }
        catch (e) {
            this.samplecount = Math.unit(parseFloat(newSamplecount), "Samples").toNumber("Samples");
        }
    }

    get samplecount_formatted(): string {
        return Math.unit(this.samplecount, "Samples").format({});
    }

    set sampletime_formatted(newSampletime: string) {
        this.sampletime_us = Math.unit(newSampletime).toNumber("us");
    }

    get sampletime_formatted(): string {
        return Math.unit(this.sampletime_us, "us").format({});
    }

    get sampletime_us(): number {
        return this.samplecount_to_sampletime_us(this.samplecount);
    }

    set sampletime_us(newValue: number) {
        this.samplecount = Math.round(newValue * this.samplerate_Hz / 1e6);
    }

    samplecount_to_sampletime_us(samplecount: number): number {
        return 1e6 * samplecount / this.samplerate_Hz;
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

    get samplecountInvalid(): boolean {
        return this.samplecount <= 0
            || this.samplecount > this.maxSamplecount;
    }

    get maxSampletime_us(): number {
        return this.samplecount_to_sampletime_us(this.maxSamplecount);
    }

    get maxSampletime_formatted(): string {
        return Math.unit(this.maxSampletime_us, "us").format({});
    }

    setDefaultOptions(requestedOptions: AcquirerRequestedOptions): void {
        var _this = this;
        requestedOptions.samplerates_Hz.forEach(function(samplerate) {
            _this.possibleSampleRatesWithUnit.push(Math.unit(samplerate, "Hz"));
        });
        this.samplerate_Hz = requestedOptions.samplerates_Hz[0];
        this.samplecount = this.maxSamplecount = requestedOptions.maxSampleCount;
        
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
