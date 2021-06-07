<template>
    <div v-bind:id="id">
    </div>
</template>

<script>
import uPlot from 'uplot';
import 'uplot/dist/uPlot.min.css';

export default {
  name: 'UplotRangerGrip',
  data () {
    return {
      uRanger: null,
      testData: null,
      zoomFactor: 10,
      initX: {
        min: 0,
        max: 16000
      },
      lftWid: {
        left: null,
        widgth: null
      },
      minMax: {
        min: null,
        max: null
      },
      x0: null,
      lft0: null,
      wid0: null,
      rangerOpts: {
					width: 600,
					height: 100,
        plugins: [
          this.wheelZoomPlugin({factor: 0.75})
        ],
				cursor: {
					x: false,
					y: false,
					points: {
						show: false,
					},
					drag: {
						setScale: false,
						setSelect: true,
						x: true,
						y: false,
					},
				},
				legend: {
					show: false
				},
				scales: {
					x: {
						time: false,
					},
				},
				series: [
					{},
					{
						stroke: "#0eb5b5",
					}
				],
				hooks: {
					ready: [
						uRanger => {
							let left = Math.round(uRanger.valToPos(this.initX.min, 'x'));
							let width = Math.round(uRanger.valToPos(this.initX.max, 'x')) - left;
							let height = uRanger.bbox.height / devicePixelRatio;
							uRanger.setSelect({left, width, height}, false);

							const sel = uRanger.root.querySelector(".u-select");

							this.on("mousedown", sel, e => {
								this.bindMove(e, e => this.update(this.lft0 + (e.clientX - this.x0), this.wid0));
							});

							this.on("mousedown", this.placeDiv(sel, "u-grip-l"), e => {
								this.bindMove(e, e => this.update(this.lft0 + (e.clientX - this.x0), this.wid0 - (e.clientX - this.x0)));
							});

							this.on("mousedown", this.placeDiv(sel, "u-grip-r"), e => {
								this.bindMove(e, e => this.update(this.lft0, this.wid0 + (e.clientX - this.x0)));
							});
						}
					],
					setSelect: [
						uRanger => {
							this.zoom(uRanger.select.left, uRanger.select.width);
						}
					],
				}
			},
    }
  },
  props: ['uPlotCharts', 'id'],
  methods: {
    on: function(ev, el, fn) {
				el.addEventListener(ev, fn);
    },
    off: function(ev, el, fn) {
				el.removeEventListener(ev, fn);
    },
    debounce: function(fn) {
				let raf;

				return (...args) => {
					if (raf)
						return;

					raf = requestAnimationFrame(() => {
						fn(...args);
						raf = null;
					});
				};
    },
    placeDiv: function(par, cls) {
				let el = document.createElement("div");
				el.classList.add(cls);
				par.appendChild(el);
				return el;
    },
    update: function(newLft, newWid){
        let newRgt = newLft + newWid;
				let maxRgt = this.uRanger.bbox.width / devicePixelRatio;

				if (newLft >= 0 && newRgt <= maxRgt) {
					this.select(newLft, newWid);
					this.zoom(newLft, newWid);
				}
    },
    select: function(newLft, newWid) {
				this.lftWid.left = newLft;
				this.lftWid.width = newWid;
				this.uRanger.setSelect(this.lftWid, false);
    },
    zoom: function(newLft, newWid) {
				this.minMax.min = this.uRanger.posToVal(newLft, 'x');
				this.minMax.max = this.uRanger.posToVal(newLft + newWid, 'x');
        
        for(var i = 0; i < this.uPlotCharts.length; i++){
          this.uPlotCharts[i].setScale('x', this.minMax);
        }
    },
    bindMove: function(e, onMove) {
				this.x0 = e.clientX;
				this.lft0 = this.uRanger.select.left;
				this.wid0 = this.uRanger.select.width;
				const _onMove = this.debounce(onMove);

				this.on("mousemove", document, _onMove);
				this.on("mouseup", document, e => {
					this.off("mousemove", document, _onMove);
				});
				e.stopPropagation();
    },
    wheelZoomPlugin: function(opts) {
				let factor = opts.factor || 0.75;

				let xMin, xMax, xRange;

				function clamp(nRange, nMin, nMax, fRange, fMin, fMax) {
					if (nRange > fRange) {
						nMin = fMin;
						nMax = fMax;
					}
					else if (nMin < fMin) {
						nMin = fMin;
						nMax = fMin + nRange;
					}
					else if (nMax > fMax) {
						nMax = fMax;
						nMin = fMax - nRange;
					}

					return [nMin, nMax];
				}

				return {
					hooks: {
						ready: u => {
							xMin = u.scales.x.min;
							xMax = u.scales.x.max;

							xRange = xMax - xMin;

							let over = u.over;
							let rect = over.getBoundingClientRect();

							// wheel drag pan
							over.addEventListener("mousedown", e => {
								if (e.button == 1) {
								//	plot.style.cursor = "move";
									e.preventDefault();

									let left0 = e.clientX;
								//	let top0 = e.clientY;

									let scXMin0 = u.scales.x.min;
									let scXMax0 = u.scales.x.max;

									let xUnitsPerPx = u.posToVal(1, 'x') - u.posToVal(0, 'x');

									function onmove(e) {
										e.preventDefault();

										let left1 = e.clientX;
									//	let top1 = e.clientY;

										let dx = xUnitsPerPx * (left1 - left0);

										u.setScale('x', {
											min: scXMin0 - dx,
											max: scXMax0 - dx,
										});
									}

									function onup(e) {
										document.removeEventListener("mousemove", onmove);
										document.removeEventListener("mouseup", onup);
									}

									document.addEventListener("mousemove", onmove);
									document.addEventListener("mouseup", onup);
								}
							});

							// wheel scroll zoom
							over.addEventListener("wheel", e => {
								e.preventDefault();

								let {left, top} = u.cursor;

								let leftPct = left/rect.width;
								let xVal = u.posToVal(left, "x");
								let oxRange = u.scales.x.max - u.scales.x.min;

								let nxRange = e.deltaY < 0 ? oxRange * factor : oxRange / factor;
								let nxMin = xVal - leftPct * nxRange;
								let nxMax = nxMin + nxRange;
								[nxMin, nxMax] = clamp(nxRange, nxMin, nxMax, xRange, xMin, xMax);

								u.batch(() => {
									u.setScale("x", {
										min: nxMin,
										max: nxMax,
									});
                  
                  for(var i = 0; i < this.uPlotCharts.length; i++){
                    this.uPlotCharts[i].setScale('x', {min: nxMin, max: nxMax});
                  }
								});
							});
						}
					}
				};
			},
    getSize: function(){
        return {
           width: document.getElementById(this.id).offsetWidth,
           height: 100
        }
    },
  },
  beforeMount (){
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

    this.initX.max = this.testData[0].length;
  },
  mounted () {
    this.$nextTick(function () {
      this.uRanger = new uPlot(this.rangerOpts, this.testData, document.getElementById(this.id));
      this.uRanger.setSize(this.getSize());

      window.addEventListener("resize", e => {
        this.uRanger.setSize(this.getSize());
      });
    })
  },
  components: {
    uPlot
  }
}
</script>

<style lang="scss">
@import './../styles/_variables';

.u-select {
  top: 2px;
}

.u-grip-l {
  position: absolute;
  left: -5px;
  width: 10px;
  height: 100%;
  background: $uplotRangerGripLeftColor;
  cursor: ew-resize;
}

.u-grip-r {
  position: absolute;
  right: -5px;
  width: 10px;
  height: 100%;
  background: $uplotRangerGripRightColor;
  cursor: ew-resize;
}

</style>
