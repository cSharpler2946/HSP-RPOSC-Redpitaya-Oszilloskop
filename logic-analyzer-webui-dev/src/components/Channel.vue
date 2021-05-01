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
            <apexchart width="100%" height="100px" type="line" :options="chartOptions" :series="series"></apexchart>

            <div class="multi-range-slider-wrapper">
                <div class="multi-range-slider">

                    <input type="range" v-model="chartZoomValue.left" class="slider-input-left"
                    v-bind:id="'slider-input-left-' + channelId" min="0" max="100" @input="onChartZoomLeft">

                    <input type="range" v-model="chartZoomValue.right" class="slider-input-right"
                    v-bind:id="'slider-input-right-' + channelId" min="0" max="100" @input="onChartZoomRight">

                    <div v-bind:id="'slider-' + channelId" class="slider">
                        <div class="track"></div>
                        <div v-bind:id="'range-'+channelId" class="range"></div>
                        <div class="thumb left"></div>
                        <div class="thumb right"></div>
                    </div>
                </div>
            </div>

            <!-- <div id="draggable2" class="draggable ui-widget-content">
              <p>I can be dragged only horizontally</p>
            </div> -->

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
import apexchart from 'vue3-apexcharts';
import $ from 'jquery';
import 'jquery-ui-dist/jquery-ui';

window.$ = window.jQuery = require('jquery');
window.$ = $.extend(require('jquery-ui'));
require('jquery-ui-touch-punch');

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
      chartZoomValue: {
        left: 0,
        right: 100
      },
      chartData: {
        xmin: 0,
        xmax: 1,
        seriesLength: 0,
        type: 'numeric',
        labels: {
          show: false
        }
      },
      chartOptions: {
        chart: {
          id: 'signal-channel-' + this.channelId,
          toolbar: {
            enabled: true,
            tools: {
              download: true,
              selection: true,
              zoom: true,
              zoomin: true,
              zoomout: true,
              pan: false,
              reset: true
            }
          },
          background: '#fff',
          offsetX: 0,
          offsetY: 0,
          parentHeightOffset: 0,
          sparkline: {
            enabled: false
          },
          events: {
            zoomed: (chartContext, { xaxis, yaxis }) => {
              this.onApexchartZoom(xaxis.min, xaxis.max)
            }
          }
        },
        fill: {
          colors: [
            '#09DE10'
          ],
          type: 'gradient',
          gradient: {
            shade: 'dark',
            type: 'horizontal',
            shadeIntensity: 1,
            inverseColors: false,
            opacityFrom: 1,
            opacityTo: 1,
            stops: [0, 100, 100, 100, 100, 100, 100],
            gradientToColors: [
              '#1df299'
            ]
          }
        },
        xaxis: {
          type: 'numeric',
          labels: {
            show: false
          }
        },
        yaxis: {
          type: 'numeric',
          labels: {
            show: false
          }
        },
        stroke: {
          curve: 'stepline'
        },
        grid: {
          show: false
        }
      },
      series: [{
        name: 'series-1',
        data: []
      }]
    }
  },
  props: {
    channelId: {
      type: Number,
      required: true
    },
    channelName: String,
    decoderChannels: []
  },
  methods: {
    editChannelName: function (channel) {
      this.editedChannel = channel
    },
    openChannelSettings: function () {
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
    onChartZoomLeft: function (e) {
      var right = parseInt(this.chartZoomValue.right)
      var left = parseInt(this.chartZoomValue.left)

      if (left >= right - 1) {
        left = right - 1
        this.chartZoomValue.left = left
      } else if (left < 0) {
        left = 0
        this.chartZoomValue.left = left
      }

      this.chartData.xmin = this.chartData.seriesLength * (left / 100) + 1
      this.chartOptions = {
        ...this.chartOptions,
        ...{
          xaxis: {
            type: this.chartData.type,
            labels: this.chartData.labels,
            min: this.chartData.xmin,
            max: this.chartData.xmax
          }
        }
      }

      var thumbLeft = document.querySelector(`#slider-${this.channelId} > .thumb.left`)
      var range = document.querySelector(`#slider-${this.channelId} > .range`)
      thumbLeft.style.left = left + '%'
      range.style.left = left + '%'
    },
    onChartZoomRight: function (e) {
      var right = parseInt(this.chartZoomValue.right)
      var left = parseInt(this.chartZoomValue.left)

      if (right <= left + 1) {
        right = left + 1
        this.chartZoomValue.right = right
      } else if (right > 100) {
        right = 100
        this.chartZoomValue.right = right
      }

      this.chartData.xmax = this.chartData.seriesLength * (right / 100)
      this.chartOptions = {
        ...this.chartOptions,
        ...{
          xaxis: {
            type: this.chartData.type,
            labels: this.chartData.labels,
            min: this.chartData.xmin,
            max: this.chartData.xmax
          }
        }
      }

      var thumbRight = document.querySelector(`#slider-${this.channelId} > .thumb.right`)
      var range = document.querySelector(`#slider-${this.channelId} > .range`)
      thumbRight.style.right = (100 - right) + '%'
      range.style.right = (100 - right) + '%'
    },
    onApexchartZoom: function (xmin, xmax) {
      this.chartData.xmin = xmin
      this.chartData.xmax = xmax

      // Calculate the percentage of the zoom values
      if (xmin === null || xmin === undefined || xmin < 0) {
        this.chartZoomValue.left = 0
      } else {
        this.chartZoomValue.left = ((xmin - 1) / this.chartData.seriesLength) * 100
      }

      if (xmax === null || xmax === undefined || xmax > 100) {
        this.chartZoomValue.right = 100
      } else {
        this.chartZoomValue.right = (xmax / this.chartData.seriesLength) * 100
      }

      // Update the values of the range-slider inputs if the user uses the apexcharts default zoom functions
      var inputLeft = document.getElementById(`slider-input-left-${this.channelId}`)
      var inputRight = document.getElementById(`slider-input-right-${this.channelId}`)
      inputLeft.value = this.chartZoomValue.left
      inputRight.value = this.chartZoomValue.right

      // Update the range slider
      this.updateRangeSlider()
    },
    setupRangeSlider: function () {
      var inputLeft = document.getElementById(`slider-input-left-${this.channelId}`)
      var inputRight = document.getElementById(`slider-input-right-${this.channelId}`)

      var thumbLeft = document.querySelector(`#slider-${this.channelId} > .thumb.left`)
      var thumbRight = document.querySelector(`#slider-${this.channelId} > .thumb.right`)

      inputLeft.addEventListener('mouseover', function () {
        thumbLeft.classList.add('hover')
      })
      inputLeft.addEventListener('mouseout', function () {
        thumbLeft.classList.remove('hover')
      })
      inputLeft.addEventListener('mousedown', function () {
        thumbLeft.classList.add('active')
      })
      inputLeft.addEventListener('mouseup', function () {
        thumbLeft.classList.remove('active')
      })

      inputRight.addEventListener('mouseover', function () {
        thumbRight.classList.add('hover')
      })
      inputRight.addEventListener('mouseout', function () {
        thumbRight.classList.remove('hover')
      })
      inputRight.addEventListener('mousedown', function () {
        thumbRight.classList.add('active')
      })
      inputRight.addEventListener('mouseup', function () {
        thumbRight.classList.remove('active')
      })
    },
    updateRangeSlider: function () {
      this.onChartZoomLeft()
      this.onChartZoomRight()
    },
    setupChartData: function () {
      this.chartData.seriesLength = this.series[0].data.length
    }
  },
  mounted () {
    if (this.channelId === 1) {
      this.series[0].data = [1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1]
    }

    this.setupChartData()
    if (parseInt(this.chartData.seriesLength) > 1) {
      this.setupRangeSlider()
    } else {
      var inputLeft = document.getElementById(`slider-input-left-${this.channelId}`)
      var inputRight = document.getElementById(`slider-input-right-${this.channelId}`)
      var slider = document.getElementById(`slider-${this.channelId}`)

      inputLeft.style.display = 'none'
      inputRight.style.display = 'none'
      slider.style.display = 'none'
    }

    var id = this.channelId;
    var self = this;
      $( function() {
        $("#range-"+id).draggable({ 
          axis: "x",
          containment: "parent",
          drag: function(e, ui){
            // console.log("position: " + ui.position.left);
            // console.log("offset: " + ui.offset.left);
            // console.log("original: " + ui.originalPosition.left);
            var bla = ui.originalPosition.left - ui.position.left;
            console.log("drag: " + bla);

            var inputLeft = document.getElementById(`slider-input-left-${id}`);
            var inputRight = document.getElementById(`slider-input-right-${id}`);
            inputLeft.value = ui.position.left;
            // inputRight.value = this.chartZoomValue.right
            self.updateRangeSlider()
          }
          });
      });
  },
  components: {
    apexchart
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

// Range-slider

.multi-range-slider-wrapper {
	position: relative;
	width: 100%;
	max-width: 100%;
}

.slider {
	position: relative;
	z-index: 1;
	height: 20px;
	margin: 0 15px;
}
.slider > .track {
	position: absolute;
	z-index: 1;
	left: 0;
	right: 0;
	top: 0;
	bottom: 0;
	border-radius: 5px;
	background-color: $sliderTrackColor;
    opacity: 50%;
}
.slider > .range {
	position: absolute;
	z-index: 2;
	left: 0;
	right: 0;
	top: 0;
	bottom: 0;
	border-radius: 5px;
	background-color: $sliderRangeColor;
}
.slider > .thumb {
	position: absolute;
	z-index: 3;
	width: 20px;
	height: 20px;
	background-color: $sliderThumbColor;
	border-radius: 5px;
	box-shadow: 0 0 0 0 rgba(98,0,238,.1);
	transition: box-shadow .3s ease-in-out;
}
.slider > .thumb.left {
	left: 0;
	transform: translate(0px, 0px);
}
.slider > .thumb.right {
	right: 0;
	transform: translate(0px, 0px);
}
.slider > .thumb.hover {
	box-shadow: 0 0 0 10px $sliderThumbHoverColor;
}
.slider > .thumb.active {
	box-shadow: 0 0 0 15px $sliderThumbHoverActiveColor;
}

input[type=range] {
  position: absolute;
  pointer-events: none;
  -webkit-appearance: none;
  z-index: 2;
  height: 20px;
  width: 98%;
  opacity: 0;
  left: 1%;
}

input[type=range]::-webkit-slider-thumb {
	pointer-events: all;
	width: 20px;
	height: 20px;
	border-radius: 0;
	border: 0 none;
	-webkit-appearance: none;
}

input[type=range]::-moz-range-thumb {
  pointer-events: all;
	width: 20px;
	height: 20px;
	border-radius: 0;
	border: 0 none;
	-webkit-appearance: none;
}

// - Range-slider

</style>
