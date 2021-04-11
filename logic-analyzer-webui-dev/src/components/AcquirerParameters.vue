<template>
    <form method="post">
        <div style="margin-bottom: 15px;"></div>

        <div class="parameter-wrapper">
            <label class="form-label">
                Sample Rate
            </label>
            <select class="form-select">
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
            <div class="row">
                <div class="col-auto">
                    <label class="form-label">
                        One
                    </label>
                </div>
                <div class="col-auto">
                    <label class="form-label">
                        Two
                    </label>
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
      },
      deep: true
    }
  }
})

export default class AcquirerParameters extends Vue {
    samplerate: string = "";
    samplecount: string = "";
    sampletime: string = "";
    gainPerChannel: { channel: string, gain: string }[] = [];
    probeAttenuationPerChannel: { channel: string, attenuation: string }[] = [];

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
