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
            <input v-model="chartZoomValue" type="range" class="form-range" v-bind:id="'chart-scrollbar-' + channelId" min="0" max="100" step="1" @input="onChartScroll"/>
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
import apexchart from "vue3-apexcharts";

export default {
    name: "Channel",
    data(){
        return {
            editedChannel: null,
            maxCharacters: 20,
            channelSettingComponents: [],
            decoderChannel: null,
            selected: null,
            edit: false,
            chartZoomValue: 0,
            chartData: {
                xmin: 0,
                xmax: 0,
            },
            chartOptions: {
                chart: {
                    id: "signal-channel-" + this.channelId,
                    toolbar: {
                        enabled: true
                    },
                    background: '#fff',
                    offsetX: 0,
                    offsetY: 0,
                    parentHeightOffset: 0,
                    sparkline: {
                        enabled: false,
                    },
                    events: {
                        zoomed: (chartContext, { xaxis, yaxis}) => {
                            this.test(xaxis.min, xaxis.max);
                        }
                    },
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
                xaxis:{
                    type: 'numeric',
                    labels: {
                        show: false,
                    },
                    min: 1,
                    max: 10,
                },
                yaxis:{
                    type: 'numeric',
                    labels: {
                        show: false,
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
                data: [],
            }],
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
        editChannelName: function(channel){
            this.editedChannel = channel;
        },
        openChannelSettings: function(){

            // When opening the modal, set the selected element to the current selected decoderChannel value. 
            // Otherwise, the user might think that he's selected another channel, but in reality he just clicked "cancel" and not "ok".
            var selectDecoderDOM = document.getElementById("select-decoder-" + this.channelId);
            var options = selectDecoderDOM.options;

            if(this.decoderChannel === null || this.decoderChannel === undefined){
                selectDecoderDOM.selectedIndex = 0;
                return;
            }
            
            for (var opt, j = 0; opt = options[j]; j++) {
                if (opt.value == this.decoderChannel) {
                    selectDecoderDOM.selectedIndex = j;
                    break;
                }
            }
        },
        editChannelSettings: function(selected){
            this.decoderChannel = selected;
            var eventParams = {
                "channelName": this.channelName,
                "decoderChannel": this.decoderChannel
            }
            this.$emit("decoder-channel-changed", eventParams);
        },
        onChartScroll: function(e){
            if(this.chartData === null || this.chartData === undefined){
                return;
            }

            this.chartData.xmax = this.chartData.xmax + 0.1;
            this.chartData.xmin = this.chartData.xmin + 0.1;

            this.chartOptions = {...this.chartOptions, ...{
                xaxis: {
                    max: this.chartData.xmax,
                    min: this.chartData.xmin,
                }
            }}
        },
        test: function(xmin, xmax){
            this.chartData.xmin = xmin;
            this.chartData.xmax = xmax;
        }
    },
    mounted() {
        if(this.channelId === 1){
            this.series[0].data = [1, 0, 1, 1, 0, 1, 1, 0, 1, 0, 1, 1, 0, 0, 0, 1, 0, 1, 1];
        }
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

</style>