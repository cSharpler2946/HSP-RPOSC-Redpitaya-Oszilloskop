<template>
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
</template>

<script>
import $ from 'jquery';
import 'jquery-ui-dist/jquery-ui';
window.$ = window.jQuery = require('jquery');
window.$ = $.extend(require('jquery-ui'));
require('jquery-ui-touch-punch');

export default {
  name: 'ChartScroller',
  data () {
    return {
     
      chartZoomValue: {
        left: 0,
        right: 100
      },
    }
  },
  props: {
    
  },
  methods: {

    updateChart: function (left, right){
        console.log(left);
        console.log(right);
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

      this.updateChart(left, right);

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

      this.updateChart(left, right);

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
  },
  mounted () {
    this.setupRangeSlider();

    var id = this.channelId;
      var self = this;
      $(function() {
        $("#range-"+id).draggable({ 
          axis: "x",
          containment: "parent",
          snap: '.thumb',
          snapTolerance: 1,
          snapMode: "both",
          addClasses: false,
          start: function(e, ui){
            this.classList.add('active');
          },
          drag: function(e, ui){
            var left = (100 * parseFloat($(this).position().left / parseFloat($(this).parent().width())));
            var right = (self.chartZoomValue.right - self.chartZoomValue.left) + left;

            self.chartZoomValue.left = left;
            self.chartZoomValue.right = right;

            self.updateRangeSlider();
          },
          stop: function(e, ui){
            this.classList.remove('active');
            self.updateRangeSlider();
          }
        });
      });

      var draggableRange = document.getElementById(`range-${this.channelId}`)
      draggableRange.addEventListener('mouseover', function () {
        draggableRange.classList.add('hover');
      });
      draggableRange.addEventListener('mouseout', function () {
        draggableRange.classList.remove('hover');
      });

    },
  components: {
  }
}
</script>

<style lang="scss" scoped>
@import './../styles/_variables';

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

</style>
