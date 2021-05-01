<template>
    <form method="post">
        <div style="margin-bottom: 15px;"></div>

        <div class="parameter-wrapper">
            <label class="form-label">
                Sample Rate
            </label>
            <select class="form-select" v-model="samplerate">
                <option v-for="possibleSampleRate in requestedOptions['samplerates']" v-bind:key="possibleSampleRate">
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
            <input type="number" class="form-control form-control-sm" v-model="sampletime"/>
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
                <label v-bind:for="'gain-'+channel" class="col col-form-label">{{ channel }}</label>
                <div class="col-sm-10">
                    <select v-bind:id="'gain-'+channel" class="form-select" v-model="probeAttenuationPerChannel[channel]">
                        <option>1x</option>
                        <option>10x</option>
                    </select>
                </div>
            </div>
        </div>
    </form>
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
      },
      deep: true
    },
    chosenOptions: {
      handler: function (currentOptions, old) {
        console.log('current chosen acq options:')
        console.log(currentOptions)
        this.$emit('chosenAcquirerOptionsChanged', currentOptions)
      },
      deep: true
    }
  }
})

export default class AcquirerParameters extends Vue {
    samplerate: string = "";
    samplecount: string = "";
    sampletime: string = "";
    gainPerChannel: Record<string, string> = {};
    probeAttenuationPerChannel: Record<string, string> = {};

    get chosenOptions(): AcquirerChosenOptions {
        return {
            samplerate: this.samplerate,
            samplecount: this.samplecount,
            sampletime: this.sampletime,
            gainPerChannel: this.gainPerChannel,
            probeAttenuationPerChannel: this.probeAttenuationPerChannel
        }
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
