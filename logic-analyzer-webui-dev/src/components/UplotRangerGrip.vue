<template>
  <div class="chart-navigation">
    <div class="zoom-navigation">
      <button class="btn btn-small btn-primary">+</button>
      <button class="btn btn-small btn-primary">-</button>
    </div>
    
    <div id="scroll-navigation">
        <canvas id="scroll-navigation-canvas"></canvas>
    </div>
  </div>
</template>

<script>
import uPlot from "uplot";
import "uplot/dist/uPlot.min.css";

import Joystick from './Joystick'

export default {
  name: "UplotRangerGrip",
  data() {
    return {
    };
  },
  props: ["uPlotCharts", "id"],
  methods: {
    createScrollNavigation: function(){
      var canvas = document.getElementById("scroll-navigation-canvas");
      var ctx = canvas.getContext("2d");
      
      canvas.height = 20;
      canvas.width = document.getElementById("scroll-navigation").offsetWidth;

      const handleWidth = 30;
      const handleFill = "#1fb592";
      let handle = null;
      let dragEnabled = false;
      let offsetX;
      let offsetY;

      var startX;
      var startY;

      const clear = () => {
        ctx.clearRect(0, 0, canvas.width, canvas.height)
      }

      const updateOffset = () => {
        var boundingBorders = canvas.getBoundingClientRect();
        offsetX = boundingBorders.left;
        offsetY = boundingBorders.top;
      }

      const drawHandle = (x) => {
        const centerX = x ? x : canvas.width / 2;
        const centerY = canvas.height / 2;
        const radius = canvas.height / 2;

        ctx.fillStyle = handleFill;

        ctx.beginPath();
        ctx.arc(centerX - handleWidth / 2, centerY, radius, (Math.PI/180) * 90, (Math.PI/180) * 270, false);
        ctx.fill();
        ctx.stroke();
        ctx.beginPath();
        ctx.arc(centerX + handleWidth / 2, centerY, radius, (Math.PI/180) * 90, (Math.PI/180) * 270, true);
        ctx.fill();
        ctx.stroke();
        ctx.beginPath();
        ctx.rect(centerX - handleWidth / 2 - 1, 0, handleWidth + 2, canvas.height);
        ctx.fill();
        ctx.stroke();

        handle = {x: centerX, y: 0, width: handleWidth + radius * 2, height: canvas.height, isDragging: false};
      }

      const resetHandle = () => {
        clear();
        drawHandle();
      }

      const isColliding = (mousePosition) => {        
        // test if mouse is inside rect
        if(mousePosition.x > handle.x && mousePosition.x < handle.x + handle.width &&
           mousePosition.y > handle.y && mousePosition.y < handle.y + handle.height){
            console.log("is dragging rect");
            return true;
        }
        else {
          // test if mouse is inside arc
          var dx = handle.x - mousePosition.x;
          var dy = handle.y - mousePosition.y;
          var radius = handle.height / 2;

          if(dx*dx + dy*dy < radius*radius){
            console.log("is dragging arc");
            return true;
          }
        }

        return false;
      }

      const onMousedown = e => {
        e.preventDefault();
        e.stopPropagation();

        updateOffset();

        var mouseX = parseInt(e.clientX-offsetX);
        var mouseY = parseInt(e.clientY-offsetY);

        dragEnabled = false;

        if (isColliding({x: mouseX, y: mouseY})){
          dragEnabled = true;
          handle.isDragging = true;
        }

        // save the current mouse position
        startX = mouseX;
        startY = mouseY;
      }

      const onMouseup = e => {
        e.preventDefault();
        e.stopPropagation();

        dragEnabled = false;
        handle.isDragging = false;

        resetHandle();
      }

      const onMouseout = e => {
        e.preventDefault();
        e.stopPropagation();

        dragEnabled = false;
        handle.isDragging = false;

        resetHandle();
      }

      const onMousemove = e => {
        if (dragEnabled) {
          e.preventDefault();
          e.stopPropagation();

          var mouseX = parseInt(e.clientX-offsetX);
          var mouseY = parseInt(e.clientY-offsetY);

          // calculate the distance the mouse has moved
          // since the last mousemove
          var dx=mouseX-startX;
          var dy=mouseY-startY;

          handle.x += dx;
          clear();
          drawHandle(handle.x);

          // save the current mouse position
          startX = mouseX;
          startY = mouseY;
        }
      }

      const registerEventlisteners = () => {
        canvas.addEventListener('mousedown', function(e){
          onMousedown(e);
        });
        canvas.addEventListener('mousemove', function(e){
          onMousemove(e);
        });
        canvas.addEventListener('mouseup', function(e){
          onMouseup(e);
        });
        canvas.addEventListener('mouseout', function(e){
          onMouseout(e);
        });

        // TODO: register touch-events
      }
      
      resetHandle();
      registerEventlisteners();
    }

  },
  beforeMount() {   

  },
  mounted() {
    this.createScrollNavigation();
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
