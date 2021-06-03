<template>
    <div v-bind:id="get_id">

    </div>
</template>

<script>
import uPlot from 'uplot';
import 'uplot/dist/uPlot.min.css';

export default {
  name: 'UplotChart',
  types: {
    point: {
      lineInterpolation: null,
      drawStyle: 2,
    },
    line: {
      lineInterpolation: 0,
      drawStyle: 0,
    },
    spline: {
      lineInterpolation: 3,
      drawStyle: 0
    },
    stepped: {
      lineInterpolation: 1,
      drawStyle: 0
    },
    bar: {
      lineInterpolation: null,
      drawStyle: 1
    }
  },
  data () {
    return {
      uplot: null,
      lineInterpolations: {
        linear:     0,
        stepAfter:  1,
        stepBefore: 2,
        spline:     3,
      },
      drawStyles: {
          line:      0,
          bars:      1,
          points:    2,
          barsLeft:  3,
          barsRight: 4,
      },
      _bars80_100: null,
      _bars100Left: null,
      _bars100Right: null,
      _stepBefore: null,
      _stepAfter: null,
      _linear: null,
      _spline: null,
    }
  },
  computed: {
      get_id(){
        return ([1e7]+-1e3+-4e3+-8e3+-1e11).replace(/[018]/g, c =>
            (c ^ crypto.getRandomValues(new Uint8Array(1))[0] & 15 >> c / 4).toString(16)
        );
      }
  },
  props: ['options', 'data'],
  methods: {
    paths: function (u, seriesIdx, idx0, idx1, extendGap, buildClip){
          let s = u.series[seriesIdx];
          let style = s.drawStyle;
          let interp = s.lineInterpolation;

          let renderer = (
              style == this.drawStyles.line ? (
              interp == this.lineInterpolations.linear     ? this._linear :
              interp == this.lineInterpolations.stepAfter  ? this._stepAfter :
              interp == this.lineInterpolations.stepBefore ? this._stepBefore :
              interp == this.lineInterpolations.spline     ? this._spline :
              null
              ) :
              style == this.drawStyles.bars ? (
              this._bars80_100
              ) :
              style == this.drawStyles.barsLeft ? (
              this._bars100Left
              ) :
              style == this.drawStyles.barsRight ? (
              this._bars100Right
              ) :
              style == this.drawStyles.points ? (
              () => null
              ) : () => null
          );

          return renderer(u, seriesIdx, idx0, idx1, extendGap, buildClip);
    },
    makeChart: function(cfg){
      var paths = this.paths;

      let opts = {
					width: 600,
					height: 400,
					cursor: {
						points: {
							size:   (u, seriesIdx)       => u.series[seriesIdx].points.size * 1.5,
							width:  (u, seriesIdx, size) => size / 4,
							stroke: (u, seriesIdx)       => u.series[seriesIdx].points.stroke(u, seriesIdx) + '90',
							fill:   (u, seriesIdx)       => "#fff",
						},
						sync: {
							key: 0,
						}
					},
					scales: {
						x: {
							time: false,
						},
					},
					axes: [
						{
							stroke: "#c7d0d9",
							grid: {
								width: 1 / devicePixelRatio,
								stroke: "#2c3235",
							},
							ticks: {
								width: 1 / devicePixelRatio,
								stroke: "#2c3235",
							}
						},
						{
							stroke: "#c7d0d9",
							grid: {
								width: 1 / devicePixelRatio,
								stroke: "#2c3235",
							},
							ticks: {
								width: 1 / devicePixelRatio,
								stroke: "#2c3235",
							}
						},
					],
					series: [
						{
							label: "X",
						},
						Object.assign({
							label: "Y",
							width: 1 / devicePixelRatio,
							drawStyle: this.drawStyles.points,
							lineInterpolation: null,
							paths,
						}, {
							drawStyle:         cfg.drawStyle,
							lineInterpolation: cfg.lineInterpolation,
							stroke:            cfg.stroke,
							fill:              cfg.stroke + "1A",
						}),
					],
				};

        const finalOptions = { ...opts, ...this.options };

				return new uPlot(finalOptions, this.data, document.getElementById(this.get_id));
    },

  },
  mounted () {

    var width = document.getElementById(this.get_id).offsetWidth;
    this.options.width = width;

    const { linear, stepped, bars, spline } = uPlot.paths;

    // generate bar builder with 80% bar (20% gap) & 100px max bar width
    this._bars80_100   = bars({size: [0.8, 100]});
    this._bars100Left  = bars({size: [1], align:  1});
    this._bars100Right = bars({size: [1], align: -1});
    this._stepBefore   = stepped({align: -1});
    this._stepAfter    = stepped({align:  1});
    this._linear       = linear();
    this._spline       = spline();

    let cfg = {
      drawStyle: this.options.type.drawStyle,
      lineInterpolation: this.options.type.lineInterpolation,
      stroke: this.options.strokeColor,
    };
    
    this.uplot = this.makeChart(cfg);
  },
  components: {
    uPlot
  }
}
</script>

<style lang="scss">

.u-legend .u-marker{
  border-radius: 3px;
  border-width: 1px !important;
}

.uplot{
  width: 100%;
}

.u-wrap{
  width: 100% !important;
}

</style>
