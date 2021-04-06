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
        </div>
    </form>
</template>

<script>
export default {
  name: 'AcquirerParameters',
  props: {
    requestedOptions: Object
  },
  data () {
    return {
      chosenOptions: 'Hello!'
    }
  },
  methods: {
    log (message) {
      console.log(message)
    }
  },
  watch: {
    requestedOptions: {
      handler: function (currentOptions, old) {
        console.log('current requested acq options:')
        console.log(currentOptions)
      },
      deep: true
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
