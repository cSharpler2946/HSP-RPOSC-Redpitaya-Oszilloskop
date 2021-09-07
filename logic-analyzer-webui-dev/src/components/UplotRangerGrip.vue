<template>
  <div class="chart-navigation">
    <div class="zoom-navigation">
      <button ref="zoomInBtn" class="btn btn-small btn-primary">+</button>
      <button ref="zoomOutBtn" class="btn btn-small btn-primary">-</button>
    </div>
    
    <div id="scroll-navigation">
        <canvas id="scroll-navigation-canvas"></canvas>
    </div>
  </div>
</template>

<script>
import uPlot from "uplot";
import "uplot/dist/uPlot.min.css";
import { sendMessage } from './worker-api';

export default {
  name: "UplotRangerGrip",
  data() {
    return {
      scale: {
        min: null,
        max: null,
      },
      maxValue: null,
      scrolling: {
        factor: 0,
        interval: null,
      },
      zooming: {
        factor: 10,
        interval: null,
      }
    };
  },
  props: ["uPlotCharts", "id"],
  methods: {
    createScrollNavigation: function(){
      let canvas = document.getElementById("scroll-navigation-canvas");
      let ctx = canvas.getContext("2d");
      
      canvas.height = 20;
      canvas.width = document.getElementById("scroll-navigation").offsetWidth;

      const handleWidth = 30;
      const handleFill = "#1fb592";
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

          // IDEA: Make exponential function to calculate scrolling factor
          this.scrolling.factor = (handle.x - centerX);
          if (this.scrolling.factor > 100){
            this.scrolling.factor = 100;
          }
          if (this.scrolling.factor < -100){
            this.scrolling.factor = -100;
          }

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
      let dx = this.scrolling.factor;

      for (let i = 0; i < this.uPlotCharts.length; i++) {

        let min = this.uPlotCharts[i].scales.x.min;
        let max = this.uPlotCharts[i].scales.x.max;

        if (dx < 0 && min <= 0) return;
        if (dx > 0 && max >= this.maxValue) return;

        this.scale.min = this.clampValue(min + dx);
        this.scale.max = this.clampValue(max + dx);

        this.uPlotCharts[i].setScale("x", this.scale);
      }
    },
    onZoomChart: function(zoomIn){

      const zoomFactor = this.zooming.factor;

      for (let i = 0; i < this.uPlotCharts.length; i++) {

        let min = this.uPlotCharts[i].scales.x.min;
        let max = this.uPlotCharts[i].scales.x.max;

        if(zoomIn) {

          // clamp minimal zoom range
          if(max - min <= 10) {
            return;
          }

          this.scale.min = this.clampValue(min + zoomFactor);
          this.scale.max = this.clampValue(max - zoomFactor);
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
    initializeZoomEventListeners: function(){
      const self = this;
      this.$refs['zoomInBtn'].addEventListener('mousedown', function (e) {
      if(self.zooming.interval)
        return;

      // self.onZoomChart(true);
      self.zooming.interval = setInterval(function () {
        self.onZoomChart(true);
        self.zooming.factor += 10;
      }, 100)
    });

    this.$refs['zoomInBtn'].addEventListener('mouseup', function (e) {
      if(!self.zooming.interval)
        return;

      clearInterval(self.zooming.interval);
      self.zooming.interval = null;
      self.zooming.factor = 10;
    });

    this.$refs['zoomOutBtn'].addEventListener('mousedown', function (e) {
      if(self.zooming.interval)
        return;

      // self.onZoomChart(false);
      self.zooming.interval = setInterval(function () {
        self.onZoomChart(false);
        self.zooming.factor += 10;
      }, 100)
    });

    this.$refs['zoomOutBtn'].addEventListener('mouseup', function (e) {
      if(!self.zooming.interval)
        return;

      clearInterval(self.zooming.interval);
      self.zooming.interval = null;
      self.zooming.factor = 10;
    });

    },
    postMessage() {
      sendMessage('Hello world!');
    }

  },
  mounted() {

    this.createScrollNavigation();

    this.setMaxValue();

    this.initializeZoomEventListeners();
   
    for (let i = 0; i < this.uPlotCharts.length; i++) {
        this.scale.min = 0;
        this.scale.max = 60;

        this.uPlotCharts[i].setScale("x", this.scale);
        console.log(this.uPlotCharts[i].scales);
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

.zoom-navigation > button{
  margin-right: 10px;
  width: 25px;
  height: 25px;
  border-radius: 50%;
  font-size: 14pt;
  display: flex;
  align-items: center;
  justify-content: center;
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


// .joystick-base {
//     margin: 20px 0 20px 0;
//     position: relative;
//     height: 116px;
//     width: 116px;
//     border-radius: 100%;
//     border: 10px solid rgba(#08c, 0.1);
//     background: rgba(#08c, 0.05);
//     box-shadow: 0 0 15px rgba(#000, 0.5) inset,
//         0 0 5px rgba(#000, 0.2);
//     transition: border-color 0.3s;
//     cursor: pointer;
//     touch-action: none;
//     -webkit-tap-highlight-color: rgba(255, 255, 255, 0);
    
//     &:hover, &.active {
//         border-color: rgba(#08c, 0.2);
        
//         .joystick-shaft {
//             background: rgba(#08c, 0.35);
//         }
//     }
    
//     &.active {
//         background: rgba(#08c, 0.1);
//     }
// }

// .joystick-shaft {
//     position: absolute;
//     top: calc(50% - 32px);
//     left: calc(50% - 32px);
//     height: 64px;
//     width: 64px;
//     border-radius: 100%;
//     background: rgba(#08c, 0.25);
//     box-shadow: 0 0 5px rgba(#000, 0.7) inset;
//     transition: background 0.3s;
//     will-change: transform;
//     touch-action: none;
//     -webkit-tap-highlight-color: rgba(255, 255, 255, 0);
// }

</style>
