<template>
  <div class="chart-navigation">
    <div class="zoom-navigation">
      <button ref="zoomInBtn" class="zoomBtn">+</button>
      <button ref="zoomOutBtn" class="zoomBtn">-</button>
    </div>
    
    <div id="scroll-navigation">
        <canvas id="scroll-navigation-canvas"></canvas>
    </div>
  </div>
</template>

<script>
import uPlot from "uplot";
import "uplot/dist/uPlot.min.css";

export default {
  name: "UplotRangerGrip",
  data() {
    return {
      scale: {
        min: null,
        max: null,
      },
      scrolling: {
        factor: 0,
        interval: null,
        handle: {
          color: "#1fb592",
          width: 30
        }
      },
      zooming: {
        factor: 0,
        growthRate: 1,
        maxGrowthRate: 10,
        interval: null,
        minRange: 10,
      }
    };
  },
  props: ["uPlotCharts", "id", "maxValue"],
  watch: {
    // TODO: watch "maxValue" and call setScale for each chart whenever maxValue changes
    maxValue: function(newVal, oldVal){
      console.log('Prop changed: ', newVal, ' | was: ', oldVal)
    }
  },
  methods: {
    createScrollNavigation: function(){
      let canvas = document.getElementById("scroll-navigation-canvas");
      let ctx = canvas.getContext("2d");
      
      canvas.height = 20;
      canvas.width = document.getElementById("scroll-navigation").offsetWidth;

      const handleWidth = this.scrolling.handle.width;
      const handleFill = this.scrolling.handle.color;
      let handle = null;
      let dragEnabled = false;
      let offsetX;
      let offsetY;
      let startX;
      let centerX;

      const clear = () => {
        ctx.clearRect(0, 0, canvas.width, canvas.height)
      }

      const updateOffset = () => {
        var boundingBorders = canvas.getBoundingClientRect();
        offsetX = boundingBorders.left;
        offsetY = boundingBorders.top;
      }

      const onWindowResize = () => {
        canvas.width = document.getElementById("scroll-navigation").offsetWidth;
        updateOffset();
        resetHandle();
      }

      const drawHandle = (x) => {
        let positionX;

        if (!x) {
          positionX = canvas.width / 2;
          centerX = positionX;
        }
        else{
          positionX = x;
        }

        const positionY = canvas.height / 2;
        const radius = canvas.height / 2;

        ctx.fillStyle = handleFill;

        ctx.beginPath();
        ctx.arc(positionX - handleWidth / 2, positionY, radius, (Math.PI/180) * 90, (Math.PI/180) * 270, false);
        ctx.fill();
        ctx.stroke();
        ctx.beginPath();
        ctx.arc(positionX + handleWidth / 2, positionY, radius, (Math.PI/180) * 90, (Math.PI/180) * 270, true);
        ctx.fill();
        ctx.stroke();
        ctx.beginPath();
        ctx.rect(positionX - handleWidth / 2 - 1, 0, handleWidth + 2, canvas.height);
        ctx.fill();
        ctx.stroke();

        handle = {x: positionX, y: 0, width: handleWidth + radius * 2, height: canvas.height, isDragging: false};
      }

      const resetHandle = () => {
        clear();
        drawHandle();

        if (this.scrolling.interval){
          clearInterval(this.scrolling.interval);
          this.scrolling.interval = null;
        }

        this.scrolling.factor = 0;
      }

      const clampHandleValue = () => {
        if(handle.x < handle.width / 2){
          handle.x = handle.width / 2;
        }
        else if (handle.x > (canvas.width - handle.width / 2)){
          handle.x = canvas.width - handle.width / 2;
        }
      }

      const isColliding = (mousePosition) => {        
        // test if mouse is inside rect
        let x = handle.x - handle.width/2 - 1;

        if(mousePosition.x > x && mousePosition.x < x + handle.width &&
           mousePosition.y > handle.y && mousePosition.y < handle.y + handle.height){
            return true;
        }

        return false;
      }

      const onMousedown = (clientX, clientY) => {
        updateOffset();

        var mouseX = parseInt(clientX-offsetX);
        var mouseY = parseInt(clientY-offsetY);

        dragEnabled = false;

        if (isColliding({x: mouseX, y: mouseY})){
          dragEnabled = true;
          handle.isDragging = true;
        }

        // save the current mouse position
        startX = mouseX;
      }

      const onMouseup = () => {
        dragEnabled = false;
        handle.isDragging = false;

        resetHandle();
      }

      const onMousemove = clientX => {
        if (dragEnabled) {
          var mouseX = parseInt(clientX-offsetX);

          // calculate the distance the mouse has moved since the last mousemove
          var dx=mouseX-startX;
          handle.x += dx;

          clampHandleValue();
          clear();
          drawHandle(handle.x);

          // this should never happen and the check is just in case.
          // if this value would be '0', it would result in errors in the lines below
          if((handle.x - centerX) === 0){
            return;
          }

          let scrollWidth = this.scale.max - this.scale.min;
          let sign = handle.x - centerX > 0 ? 1 : -1;

          // calc how much percentage from center to left/right the handler was dragged.
          let scrollInputValue = (handle.x - centerX) > 0 ? 
          ((handle.x + handle.width / 2 - centerX) / centerX) : 
          (((centerX - handle.x + handle.width / 2) / centerX) * -1);

          // calc and set the scroll value
          let scrollValue = sign * Math.pow(scrollWidth, Math.abs(scrollInputValue)) / 10;
          this.scrolling.factor = ~~(scrollValue * 100) / 100;

          // call the actual scroll method
          if(!this.scrolling.interval){
            const self = this;
            this.scrolling.interval = setInterval(function() {
              self.onScrollChart()
              } , 100)
          }

          // save the current mouse position
          startX = mouseX;
        }
      }

      const registerEventlisteners = () => {
        canvas.addEventListener('mousedown', function(e){
          e.preventDefault();
          e.stopPropagation();
          onMousedown(e.clientX, e.clientY);
        });
        window.addEventListener('mouseup', function(e){
          e.preventDefault();
          e.stopPropagation();
          onMouseup();
        });
        window.addEventListener('mousemove', function(e){
          e.preventDefault();
          e.stopPropagation();
          onMousemove(e.clientX);
        });
        canvas.addEventListener('touchstart', function(e){
          e.preventDefault();
          e.stopPropagation();
          onMousedown(e.changedTouches[0].clientX, e.changedTouches[0].clientY);
        });
        window.addEventListener('touchend', function(e){
          e.preventDefault();
          e.stopPropagation();
          onMouseup();
        });
        window.addEventListener('touchmove', function(e){
          e.preventDefault();
          e.stopPropagation();
          onMousemove(e.changedTouches[0].clientX);
        });

        window.addEventListener("resize", e => {
          onWindowResize();
        });
      }
      
      resetHandle();
      registerEventlisteners();
    },
    onScrollChart: function(){
      for (let i = 0; i < this.uPlotCharts.length; i++) {

        if (this.scrolling.factor < 0 && this.scale.min <= 0) 
          return;

        if (this.scrolling.factor > 0 && this.scale.max >= this.maxValue) 
          return;

        this.scale.min = this.clampValue(this.scale.min + this.scrolling.factor);
        this.scale.max = this.clampValue(this.scale.max + this.scrolling.factor);

        this.uPlotCharts[i].setScale("x", this.scale);
      }
    },
    onZoomChart: function(zoomIn){

      const zoomFactor = this.zooming.factor;

      for (let i = 0; i < this.uPlotCharts.length; i++) {

        let min = this.scale.min;
        let max = this.scale.max;

        // calc growthRate
        this.zooming.growthRate = (max - min) / 100;
        this.zooming.growthRate = this.zooming.growthRate > this.zooming.maxGrowthRate ? 
                                  this.zooming.maxGrowthRate : 
                                  this.zooming.growthRate;

        if(zoomIn) {
          // clamp minimal zoom range
          if (((max - zoomFactor) - (min + zoomFactor)) + 1 <= this.zooming.minRange) {

            this.zooming.factor = 0;
            this.zooming.growthRate = 1;

            return;
          }
          else{
            this.scale.min = this.clampValue(min + zoomFactor);
            this.scale.max = this.clampValue(max - zoomFactor);
          }
        }
        else {
          this.scale.min = this.clampValue(min - zoomFactor);
          this.scale.max = this.clampValue(max + zoomFactor);
        }

        this.uPlotCharts[i].setScale("x", this.scale);  
      }
    },
    clampValue(a) {  
      if (a < 0) {
        return 0;
      }
      else if (a > this.maxValue){
        return this.maxValue;
      }
      else {
        return a;
      }
    },
    setMaxValue(){
      const self = this;

      for (let i = 0; i < this.uPlotCharts.length; i++) {
          this.uPlotCharts[i].data.forEach(arr => {
            if(self.maxValue < arr.length){
              self.maxValue = arr.length;
            }
          });
      }
    },
    onZoomStart(zoomIn){
      console.log("zooming in:", this.scale);

      if(this.zooming.interval)
        return;

      const self = this;
      this.zooming.interval = setInterval(function () {
        self.onZoomChart(zoomIn);
        self.zooming.factor += self.zooming.growthRate;
      }, 100)
    },
    onZoomEnd(){
      if(!this.zooming.interval)
          return;

      clearInterval(this.zooming.interval);
      this.zooming.interval = null;
    },
    initializeZoomEventListeners: function(){
      const self = this;

      // Mouse
      this.$refs['zoomInBtn'].addEventListener('mousedown', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomStart(true);
      });

      this.$refs['zoomInBtn'].addEventListener('mouseup', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomEnd();
      });

      this.$refs['zoomOutBtn'].addEventListener('mousedown', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomStart(false);
      });

      this.$refs['zoomOutBtn'].addEventListener('mouseup', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomEnd();
      });

      // Touch
      this.$refs['zoomInBtn'].addEventListener('touchstart', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomStart(true);
      });

      this.$refs['zoomInBtn'].addEventListener('touchend', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomEnd();
      });

      this.$refs['zoomOutBtn'].addEventListener('touchstart', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomStart(false);
      });

      this.$refs['zoomOutBtn'].addEventListener('touchend', function (e) {
        e.preventDefault();
        e.stopPropagation();

        self.onZoomEnd();
      });
    },
  },
  mounted() {

    this.createScrollNavigation();

    // this.setMaxValue();

    console.log("max value:", this.maxValue);

    this.initializeZoomEventListeners();

    this.scale.min = 0;
    this.scale.max = 60;
   
    for (let i = 0; i < this.uPlotCharts.length; i++) {
        this.uPlotCharts[i].setScale("x", this.scale);
    }
  },
  components: {
    uPlot,
  },
};
</script>

<style lang="scss">
@import "./../styles/_variables";

.chart-navigation{
  display: flex;
  flex-flow: column;
  margin: 0 15px;
}

.zoom-navigation {
  margin-bottom: 10px;
  display: flex;
  align-items: center;
  justify-content: center;
}

.zoom-navigation > button {
  margin: 0 5px;
  width: 25px;
  height: 25px;
  border-radius: 50%;
  font-size: 14pt;
  display: flex;
  align-items: center;
  justify-content: center;
  background-color: #1fb592;
  color: black;
  border: 1px solid black;
  line-height: 0;

  &:hover {
    box-shadow: 0 0 0 5px rgba(28, 247, 196, 0.3), 
  }
}

*, *:before, *:after {
    box-sizing: border-box;
}

#scroll-navigation{
  width: 100%;
}

#scroll-navigation-canvas {
  background-color: dimgray;
  border-radius: 10px;
}
</style>
