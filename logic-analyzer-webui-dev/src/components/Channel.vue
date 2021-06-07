<template>
    <div class="channel-wrapper row">

        <div class="channel-box col-md-3 col-12">
            <div class="row" style="padding:0;">

                <div class="channel-id-box col-2 d-flex align-items-center">
                    {{ channelId }}
                </div>

                <div @dblclick="edit = true" class="col-8 channel d-flex align-items-center">
                    <div v-show="edit == false">
                        <label> {{ channelName }}</label>
                        <div class="selected-decoder-channel"><strong>selected:</strong> {{ decoderChannel ?? '-' }}</div>
                    </div>
                    <input class="form-control" v-show="edit == true" v-model="channelName" :maxlength="maxCharacters"
                    v-on:blur="edit=false; $emit('update')" @keyup.enter="edit=false; $emit('update')">
                </div>

                <div class="col-2 channel-settings d-flex align-items-center">
                    <font-awesome-icon icon="cog" @click="openChannelSettings" class="fas fa-cog channel-settings-btn"
                    data-bs-toggle="modal" v-bind:data-bs-target="'#channel-settings-modal-' + channelId" />
                </div>
            </div>
        </div>

        <div class="signal-box col-md-9 col-12">
          
          <UplotChart :options="chartOptions" :data="testData" :id="channelId" @uplot="returnUplotToParent"></UplotChart>

        </div>
    </div>

<div class="modal fade" v-bind:id="'channel-settings-modal-' + channelId" tabindex="-1" aria-labelledby="channelSettings" aria-hidden="true">
  <div class="modal-dialog modal-dialog-centered">
    <div class="modal-content">

      <div class="modal-header">
        <h5 class="modal-title" id="exampleModalLabel">Settings for: <b>{{channelName}}</b></h5>
        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
      </div>

      <div class="modal-body">
        <div class="row">
            <label class="col-sm-4 col-form-label">Decoder Channel:</label>
            <div class="col-sm-8">
                <select v-model="selected" v-bind:id="'select-decoder-' + channelId" class="form-select" aria-label="Select decoder channel">
                    <option :value="null" disabled hidden>Select decoder channel</option>
                    <option v-for="decoderChannel in decoderChannels" v-bind:key="decoderChannel" v-bind:value="decoderChannel.id">
                        {{ decoderChannel["name"] }}
                    </option>
                </select>
            </div>
        </div>
      </div>

      <div class="modal-footer">
        <button type="button" class="btn btn-danger" data-bs-dismiss="modal">Cancel</button>
        <button @click="editChannelSettings(selected)" type="button" class="btn btn-success" data-bs-dismiss="modal">Ok</button>
      </div>
    </div>
  </div>
</div>

</template>

<script>
import UplotChart from './UplotChart';

const measureData = require("./../../testing/test-data.json");

export default {
  name: 'Channel',
  data () {
    return {
      editedChannel: null,
      maxCharacters: 20,
      channelSettingComponents: [],
      decoderChannel: null,
      selected: null,
      edit: false,
      chartOptions: {
          height: 100,
          strokeColor: "#0eb5b5",
          type: UplotChart.types.stepped,
          axes: [
						{
							stroke: "#333",
							grid: {
								width: 1 / devicePixelRatio,
								stroke: "lightgray",
							},
							ticks: {
								width: 1 / devicePixelRatio,
								stroke: "lightgray",
							}
						},
						{
							stroke: "#333",
							grid: {
								width: 1 / devicePixelRatio,
								stroke: "lightgray",
							},
							ticks: {
								width: 1 / devicePixelRatio,
								stroke: "lightgray",
							}
						},
					],

      },
      testData: null,
    }
  },
  props: {
    channelId: {
      type: Number,
      required: true
    },
    channelName: String,
    decoderChannels: [],
    channelData: [],
  },
  methods: {
    editChannelName: function (channel) {
      this.editedChannel = channel
    },
    openChannelSettings: function () {
      console.log(this.channelData);

      // When opening the modal, set the selected element to the current selected decoderChannel value.
      // Otherwise, the user might think that he's selected another channel, but in reality he just clicked "cancel" and not "ok".
      var selectDecoderDOM = document.getElementById('select-decoder-' + this.channelId)
      var options = selectDecoderDOM.options

      if (this.decoderChannel === null || this.decoderChannel === undefined) {
        selectDecoderDOM.selectedIndex = 0
        return
      }

      for (var opt, j = 0; opt = options[j]; j++) {
        if (opt.value == this.decoderChannel) {
          selectDecoderDOM.selectedIndex = j
          break
        }
      }
    },
    editChannelSettings: function (selected) {
      this.decoderChannel = selected
      var eventParams = {
        channelName: this.channelName,
        decoderChannel: this.decoderChannel
      }
      this.$emit('decoder-channel-changed', eventParams)
    },
    returnUplotToParent: function(e){
        this.$emit("uplot", e);
    }
  },
  beforeMount () {

    // FOR TESTING
    if(this.channelId == '1'){
      this.testData = [
        Array.from(Array(16000).keys()),
        measureData.data
      ]   
    }
    else if(this.channelId == '2'){
      this.testData = [
        Array.from(Array(16000).keys()),
        new Array(16000).fill(0).map(function(val, i){
          if(i % 2 != 0){
            return 1;
          }
          else{
            return 0;
          }
        })
      ]
    }
    else if(this.channelId == '3'){
      this.testData = [
        Array.from(Array(16000).keys()),
        new Array(16000).fill(0)
      ]
    }
    // - FOR TESTING
  },
  mounted(){
    if(this.channelId == '3'){
      var canvas = document.getElementsByTagName('canvas')[2];
      var c = canvas.getContext('2d');
      c.fillStyle = "black";
      c.fillRect(10, 10, 100, 100);

    }
  },
  components: {
    UplotChart
  }
}
</script>

<style lang="scss" scoped>
@import './../styles/_variables';

.channel-wrapper{
    border-bottom: 1px solid gray;
}

.channel-id-box{
    border-right: 1px solid gray;
    text-align: center;
    font-weight: bold;
}

.channel-box{
    text-align: left;
    border-right: 1px solid $primaryColor;
    @media (max-width: 768px) {
        border-right: none;
        border-bottom: 1px solid gray;
    }
}

.channel-box .row{
    height: 100%;
}

.channel-settings-btn{
    color: $primaryColor;
    cursor: pointer;
}

.signal-box{
    background-color: $signalBackgroundColor;
    min-height: 100px;
    padding: 10px;
}

.selected-decoder-channel{
  font-size: 0.7em;
}

</style>
