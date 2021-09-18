<template>
  <div id="dataAnnotations"></div>
</template>

<script>
import uPlot from 'uplot';
import 'uplot/dist/uPlot.min.css';

import * as distr from '../libs/distr.js';
import * as quadtree from '../libs/quadtree.js';

const testAnnotations = require("./../../testing/test-annotations.json");
const { round, min, max, ceil } = Math;
const rampWidth = 10;

function pointWithin(px, py, rlft, rtop, rrgt, rbtm) {
    return px >= rlft && px <= rrgt && py >= rtop && py <= rbtm;
}

const chartOptions = {
  fontColor: "black",
  series: [
    {
      label: "annotations",
    },
    {
      label: "rtx",
      fill: "rgba(51, 187, 85, 0.7)",
      stroke: "darkgreen",
      width: 2,
    },
    {
      label: "rtx-data",
      fill: "rgba(51, 187, 85, 0.7)",
      stroke: "darkgreen",
      width: 2,
    },
  ],
  size: [0.9, 100],
  fill: (seriesIdx, dataIdx, value) => "rgba(0, 255, 0, 0.2)",
  stroke: (seriesIdx, dataIdx, value) => "rgba(0, 200, 0)",
};

export default {
  name: "DataAnnotations",
  data() {
    return {
      uplot: null,
      groupedAnnotationData: null,
      zoomInText: "Zoom in to see annotations"
    };
  },
  props: {
    annotationData: { annotations: Array },
  },
  watch: {
    annotationData: {
      handler: function () {
        this.renderAnnotations();
      },
      deep: true,
    },
  },
  methods: {
    groupBy: function (xs, key) {
      return xs.reduce(function (rv, x) {
        (rv[x[key]] = rv[x[key]] || []).push(x);
        return rv;
      }, {});
    },
    dataAnnotaionPlugin: function (opts) {
      var self = this;
      const { fontColor, count, fill, stroke } = opts;

      const pxRatio = devicePixelRatio;

      const laneWidth = 0.9;
      const laneDistr = distr.SPACE_BETWEEN;

      const font = round(14 * pxRatio) + "px Arial";

      function walk(yIdx, count, dim, draw) {
        distr.distr(count, laneWidth, laneDistr, yIdx, (i, offPct, dimPct) => {
          let laneOffPx = dim * offPct;
          let laneWidPx = dim * dimPct;

          draw(i, laneOffPx, laneWidPx);
        });
      }

      const fillPaths = new Map();
      const strokePaths = new Map();

      function drawBoxes(ctx) {
        fillPaths.forEach((fillPath, fillStyle) => {
          ctx.fillStyle = fillStyle;
          ctx.fill(fillPath);
        });

        strokePaths.forEach((strokePath, strokeStyle) => {
          ctx.strokeStyle = strokeStyle;
          ctx.stroke(strokePath);
        });

        fillPaths.clear();
        strokePaths.clear();
      }

      function putBox(
        ctx,
        rect,
        xOff,
        yOff,
        lft,
        top,
        wid,
        hgt,
        strokeWidth,
        iy,
        ix,
        value
      ) {
        let fillStyle = fill(iy + 1, ix, value);
        let fillPath = fillPaths.get(fillStyle);

        if (fillPath == null)
          fillPaths.set(fillStyle, (fillPath = new Path2D()));

        fillPath.moveTo(lft, top + hgt / 2);
        fillPath.lineTo(lft + rampWidth, top); // /
        fillPath.lineTo(lft + wid - rampWidth, top); // -
        fillPath.lineTo(lft + wid, top + hgt / 2); // \
        fillPath.lineTo(lft - rampWidth + wid, top + hgt); // /
        fillPath.lineTo(lft + rampWidth, top + hgt); // -
        fillPath.closePath();

        if (strokeWidth) {
          let strokeStyle = stroke(iy + 1, ix, value);
          let strokePath = strokePaths.get(strokeStyle);

          if (strokePath == null)
            strokePaths.set(strokeStyle, (strokePath = new Path2D()));

          strokePath.moveTo(lft + strokeWidth / 2, top + hgt / 2);
          strokePath.lineTo(
            lft + rampWidth + strokeWidth / 2,
            top + strokeWidth / 2
          ); // /
          strokePath.lineTo(
            lft + wid - rampWidth - strokeWidth / 2,
            top + strokeWidth / 2
          ); // -
          strokePath.lineTo(
            lft - strokeWidth / 2 + wid,
            top + hgt / 2 - strokeWidth / 2
          ); // \
          strokePath.lineTo(
            lft - rampWidth + strokeWidth / 2 + wid,
            top + hgt - strokeWidth / 2
          ); // /
          strokePath.lineTo(
            lft + rampWidth + strokeWidth / 2,
            top + hgt - strokeWidth / 2
          ); // -
          strokePath.closePath();
        }

        qt.add({
          x: round(lft - xOff),
          y: round(top - yOff),
          w: wid,
          h: hgt,
          sidx: iy + 1,
          didx: ix,
        });
      }

      function drawPaths(u, sidx, idx0, idx1) {
        uPlot.orient(
          u,
          sidx,
          (
            series,
            dataX,
            dataY,
            scaleX,
            scaleY,
            valToPosX,
            valToPosY,
            xOff,
            yOff,
            xDim,
            yDim,
            moveTo,
            lineTo,
            rect
          ) => {
            if (!self.renderAnnotationsEnabled(scaleX.min, scaleX.max)) {
              return;
            }

            let strokeWidth = round((series.width || 0) * pxRatio);

            u.ctx.save();
            rect(u.ctx, u.bbox.left, u.bbox.top, u.bbox.width, u.bbox.height);
            u.ctx.clip();

            walk(sidx - 1, count, yDim, (iy, y0, hgt) => {
              // draw spans

              for (let ix = 0; ix < dataY.length; ix++) {
                if (dataY[ix] != null) {
                  let lft = round(valToPosX(dataX[ix], scaleX, xDim, xOff));

                  let nextIx = ix;
                  while (
                    dataY[++nextIx] === undefined &&
                    nextIx < dataY.length
                  ) {}

                  // to now (not to end of chart)
                  let rgt =
                    nextIx == dataY.length
                      ? xOff + xDim + strokeWidth
                      : round(valToPosX(dataX[nextIx], scaleX, xDim, xOff));

                  putBox(
                    u.ctx,
                    rect,
                    xOff,
                    yOff,
                    lft,
                    round(yOff + y0),
                    rgt - lft,
                    round(hgt),
                    strokeWidth,
                    iy,
                    ix,
                    dataY[ix]
                  );

                  ix = nextIx - 1;
                }
              }
            });

            u.ctx.lineWidth = strokeWidth;
            drawBoxes(u.ctx);

            u.ctx.restore();
          }
        );

        return null;
      }

      // draws text in annotations
      function drawPoints(u, sidx, i0, i1) {
        u.ctx.save();
        u.ctx.rect(u.bbox.left, u.bbox.top, u.bbox.width, u.bbox.height);
        u.ctx.clip();

        u.ctx.font = font; // Font style
        u.ctx.fillStyle = fontColor; // Font color
        u.ctx.textAlign = "left";
        u.ctx.textBaseline = "middle";

        uPlot.orient(
          u,
          sidx,
          (
            series,
            dataX,
            dataY,
            scaleX,
            scaleY,
            valToPosX,
            valToPosY,
            xOff,
            yOff,
            xDim,
            yDim,
            moveTo,
            lineTo,
            rect
          ) => {
            if (!self.renderAnnotationsEnabled(scaleX.min, scaleX.max)) {
                // Render zoomInText to indicate, that the user needs to zoom in to see any data annotations
                u.ctx.textAlign = "center";
                u.ctx.fillText("Zoom in to see annotations", u.ctx.canvas.offsetWidth / 2, u.ctx.canvas.offsetHeight / 2);
                return;
            }

            let strokeWidth = round((series.width || 0) * pxRatio);
            let textOffset = rampWidth;

            let y = round(yOff + yMids[sidx - 1]);

            for (let ix = 0; ix < dataY.length; ix++) {
              if (dataY[ix] != null) {
                let x = valToPosX(dataX[ix], scaleX, xDim, xOff) + textOffset;
                u.ctx.fillText(dataY[ix], x, y);
              }
            }
          }
        );

        u.ctx.restore();

        return false;
      }

      let qt;

      let hoverMarks = Array(count)
        .fill(null)
        .map((_) => {
          let mark = document.createElement("div");
          mark.classList.add("bar-mark");
          return mark;
        });

      let hovered = Array(count).fill(null);

        let yMids = Array(count).fill(0);
        let ySplits = Array(count).fill(0);

      return {
        hooks: {
          init: (u) => {
            let over = u.over;

            over.style.overflow = "hidden";

            hoverMarks.forEach((m) => {
              over.appendChild(m);
            });
          },
          drawClear: (u) => {
            qt = qt || new quadtree.Quadtree(0, 0, u.bbox.width, u.bbox.height);

            qt.clear();

            // force-clear the path cache to cause drawBars() to rebuild new quadtree
            u.series.forEach((s) => {
              s._paths = null;
            });
          },
          setCursor: (u) => {
            let cx = round(u.cursor.left * pxRatio);

            for (let i = 0; i < count; i++) {
              let found = null;

              if (cx >= 0) {
                let cy = yMids[i];

                qt.get(cx, cy, 1, 1, (o) => {
                  if (pointWithin(cx, cy, o.x, o.y, o.x + o.w, o.y + o.h))
                    found = o;
                });
              }

              let h = hoverMarks[i];

              if (found) {
                if (found != hovered[i]) {
                  hovered[i] = found;

                  h.style.display = null;
                  h.style.left = ~(found.x / pxRatio) + "px";
                  h.style.top = ~(found.y / pxRatio) + "px";
                  h.style.width = ~(found.w / pxRatio) + "px";
                  h.style.height = ~(found.h / pxRatio) + "px";
                }
              } else if (hovered[i] != null) {
                h.style.display = "none";
                hovered[i] = null;
              }
            }
          },
        },
        opts: (u, opts) => {
          uPlot.assign(opts, {
            cursor: {
              //	x: false,
              y: false,
              points: { show: false },
            },
            scales: {
              x: {
                range(u, min, max) {
                  return [min, max];
                },
              },
              y: {
                range: [0, 1],
              },
            },
          });

          uPlot.assign(opts.axes[1], {
            splits: (u, axisIdx) => {
              walk(null, count, u.bbox.height, (iy, y0, hgt) => {
                // vertical midpoints of each series' timeline (stored relative to .u-over)
                yMids[iy] = round(y0 + hgt / 2);
                ySplits[iy] = u.posToVal(yMids[iy] / pxRatio, "y");
              });

              return ySplits;
            },
            values: () =>
              Array(count)
                .fill(null)
                .map((v, i) => u.series[i + 1].label),
            gap: 15,
            size: 70,
            grid: { show: false },
            ticks: { show: false },
            side: 3,
          });

          opts.series.forEach((s, i) => {
            if (i > 0) {
              uPlot.assign(s, {
                paths: drawPaths,
                points: {
                  show: drawPoints,
                },
              });
            }
          });
        },
      };
    },
    makeChart: function (o, d) {
      const opts = {
        width: this.getSize().width,
        height: this.getSize().height,
        drawOrder: ["series", "axes"],
        scales: {
          x: {
            time: false,
          },
        },
        axes: [
          {
            show: true,
          },
          {},
        ],
        legend: {
          live: false,
          markers: {
            width: 0,
          },
        },
        series: o.series,
        plugins: [
          this.dataAnnotaionPlugin({
            count: d.length - 1,
            ...o,
          }),
        ],
      };

      return new uPlot(opts, d, document.getElementById("dataAnnotations"));
    },
    unsetSameFutureValues: function (series) {
      series.forEach((values, i) => {
        if (i == 0) return;

        let prevVal = values[0];

        for (let i = 1; i < values.length; i++) {
          let value = values[i];

          if (value === null) prevVal = null;
          else {
            if (value === prevVal) values[i] = undefined;
            else if (value != null) prevVal = value;
          }
        }
      });
    },
    getSize: function () {
      return {
        width: document.getElementById("dataAnnotations").offsetWidth,
        height: Object.keys(this.groupedAnnotationData).length * 30 + 50,
      };
    },
    renderAnnotationsEnabled: function (min, max) {
      if (max - min > 80) {
        return false;
      } else {
        return true;
      }
    },
    getUplotAnnotations(){
      console.debug("annotation data: ", this.annotationData);
      this.groupedAnnotationData = this.groupBy(this.annotationData.annotations, "annotationClass");

      let annotationDataArr = [];
      for (let annotationGroupName in this.groupedAnnotationData) {

        let texts = [];
        let indices = [];

        let annotationGroup = this.groupedAnnotationData[annotationGroupName];
        for (let i = 0; i < annotationGroup.length; i++) {
          texts.push(annotationGroup[i].annotationText);
          indices.push(annotationGroup[i].start);

          if(annotationGroup[i + 1] && annotationGroup[i].end != annotationGroup[i + 1].start){
            // push null, if the end of an annotation was reached to mark the ending of it.
            // otherwise, the annotation would be drawn until another annotation is hit.
            texts.push(null);
            indices.push(annotationGroup[i].end);
          }
          else if (!annotationGroup[i + 1]){
            // push null at the end of the annotationGroup to mark the "finish line"
            texts.push(null);
            indices.push(annotationGroup[i].end);
          }
        }

        annotationDataArr.push([indices, texts]);
      }
      
      let annotationData = uPlot.join(annotationDataArr, annotationDataArr.map(f => [1,1]));
      return annotationData;
    },
    renderAnnotations() {  
      const annotations = this.getUplotAnnotations();

      var chartSeriesOptions = [];
      chartSeriesOptions.push({ label: "annotations" });
      for (var annotationClass in this.groupedAnnotationData) {
        chartSeriesOptions.push({
          label: annotationClass,
          fill: "rgba(51, 187, 85, 0.7)",
          stroke: "darkgreen",
          width: 2,
        });
      }

      console.log("chartSeriesOptions:", chartSeriesOptions);

      var options = chartOptions;
      options.series = chartSeriesOptions;
      this.uplot = this.makeChart(options, annotations);
    },
  },
  updated(){
      console.log("Annotations got updated");
        this.uplot.redraw();
  },
  mounted() {
    this.renderAnnotations();

    window.addEventListener("resize", (e) => {
      this.uplot.setSize(this.getSize());
    });

    this.$emit("uplot", this.uplot);
  },
  components: {
    uPlot,
  },
};
</script>

<style lang="scss">
@import "./../styles/_variables";

.u-legend .u-marker {
  border-radius: 3px;
  border-width: 1px !important;
}

.u-select {
  background: rgba(0, 0, 0, 0.25);
  pointer-events: all;
  cursor: move;
}
</style>
