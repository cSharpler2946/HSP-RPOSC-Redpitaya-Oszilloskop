<template>
  <Navbar title="Logic Analyzer" brand="logo.png"/>



<div class="container-fluid">

  <div class="row">

<div class="signals-header col-md-9 d-md-none d-block">
          <div class="align-self-center">
            <h3>{{heading}}</h3>
          </div>
        </div>

    <div id="left-panel" class="col-md-9 order-md-1 order-2">
      <div id="logic-analyzer-header" class="row">

        <div class="channels-header col-md-3">
            <button id="startAnalyzing" type="button" class="btn">
              <span>Start Analyzing </span>
              <font-awesome-icon icon="play" style="vertical-align: middle; margin-left: 10px;" />
            </button>
        </div>

        <div class="signals-header col-md-9 d-md-block d-none">
          <div class="align-self-center">
            <h3>{{heading}}</h3>
          </div>
        </div>
        
      </div>

      <Channel v-for="n in 10" :key="n" :channelId="n" :channelName="`Channel ${n}`" :decoderChannels="decoderChannels"
        v-on:decoder-channel-changed="onDecoderChannelChanged"/>

      <!-- final border in case the right-panels height is bigger than the left-panels height -->
    </div>

    <!-- Button for colapsing the entire right panel on mobile devices. -->
    <div class="col d-md-none">
        <button @click="toggleRightPanel"
        id="toggleRightPanelBtn" class="btn float-end" type="button" data-bs-toggle="collapse" data-bs-target="#right-panel" 
        aria-expanded="true" aria-controls="right-panel" aria-label="Toggle right panel.">
          {{ rightPanelToggler.text }} <font-awesome-icon icon="caret-down" :style="toggleRightPanelIcon" />
        </button>
    </div>

    <div id="right-panel" class="collapse show col-md-3 order-md-2 order-1">
      <nav>
        <div class="nav nav-tabs" id="nav-tab" role="tablist">
          <a class="nav-link active col" id="nav-parameter-tab" data-bs-toggle="tab" href="#nav-parameter" role="tab" aria-controls="nav-parameter" aria-selected="true">
            Decoder
          </a>
          <a class="nav-link col" id="nav-decoded-data-tab" data-bs-toggle="tab" href="#nav-decoded-data" role="tab" aria-controls="nav-decoded-data" aria-selected="false">
            Decoded Data
          </a>
          <a class="nav-link col" id="nav-acquirer-parameters-tab" data-bs-toggle="tab" href="#nav-acquirer-parameters" role="tab" aria-controls="nav-acquirer-parameters" aria-selected="false">
            Acquirer
          </a>
        </div>
      </nav>
      <div class="tab-content" id="parameters-tabContent">
        <div class="tab-pane fade show active" id="nav-parameter" role="tabpanel" aria-labelledby="nav-parameter-tab">
          <Parameters :decoders="decoders" v-on:selectedDecoderChanged="onSelectedDecoderChanged"
            :requestedOptions="requestedOptions"
            v-on:chosenOptionsChanged="onChosenOptionsChanged"/>
        </div>
        <div class="tab-pane fade" id="nav-decoded-data" role="tabpanel" aria-labelledby="nav-decoded-data-tab">
          <DecodedData/>
        </div>
        <div class="tab-pane fade" id="nav-acquirer-parameters" role="tabpanel" aria-labelledby="nav-acquirer-parameters-tab">
          <AcquirerParameters :requestedOptions="acquirerRequestedOptions"/>
        </div>
      </div>
    </div>
  </div>
</div>

<Footer/>

</template>

<script>
import Navbar from './components/Navbar.vue'
import Footer from './components/Footer.vue'
import Channel from './components/Channel.vue'
import Parameters from './components/Parameters.vue'
import DecodedData from './components/DecodedData.vue'
import AcquirerParameters from './components/AcquirerParameters.vue'
import RedPitaya from './redpitaya.js'
import RedPitayaStub from './redpitaya_stub.js'

export default {
  name: 'App',
  data () {
    return {
      app_id: 'RPOSC-LogicAnalyser',
      app_port: 9002,
      redpitaya: null,
      heading: 'Logic Analyzer',
      rightPanelToggler:{
        collapsed: true,
        text: "Hide Parameters",
        iconTransform: 0.5,
      },
      rightPanelText: "Hide Parameters",
      decoders: [
          {
              "id": "Hello!",
              "name": "SampleDecoder1"
          }
      ],
      requestedOptions: [],
      decoderChannels: [],
      selectedDecoderChannels: [],
      acquirerRequestedOptions: {}
    }
  },
  // props: {
  //   rightPanelCollapsed: {
  //     type: Boolean,
  //     default: false
  //   }
  // },
  computed: {
    // Make calculations/computations in here
    get_app_url(){
      return `/bazaar?start=${this.app_id}?${location.search.substr(1)}`;
    },
    get_socket_url(){
      return `ws://${window.location.hostname}:${this.app_port}`;
    },
    toggleRightPanelIcon() {
      return { transform: 'rotate(' + this.rightPanelToggler.iconTransform + 'turn)' };
    },
  },
  methods:{
    getData: function(){
      var data = this.redpitaya.receiveData(arg1, arg2);
      this.decodedData = data;
    },
    toggleRightPanel(){
      if(this.rightPanelToggler.collapsed){
        this.rightPanelToggler.text = "Show Parameters";
        this.rightPanelToggler.iconTransform = 0;
      }
      else{
        this.rightPanelToggler.text = "Hide Parameters";
        this.rightPanelToggler.iconTransform = 0.5;
      }

      this.rightPanelToggler.collapsed = !this.rightPanelToggler.collapsed;
    },
    onSelectedDecoderChanged: function(newDecoder) {
        this.redpitaya.sendSelectedDecoder(newDecoder);
    },
    onChosenOptionsChanged: function(currentChosenOptions) {
        this.redpitaya.sendChosenOptions(currentChosenOptions);
    },
    onDecoderChannelChanged: function(eventParams) {
        console.log("Decoder channel changed:");
        console.log(eventParams.channelName);
        console.log(eventParams.decoderChannel);
    }
  },
  mounted () {
    // Build up WebSocket-Connection with RedPitaya in here.
    this.redpitaya = new RedPitaya(this.app_id, this.get_app_url, this.get_socket_url, this.decoders, this.requestedOptions, this.decoderChannels);
    //this.redpitaya = new RedPitayaStub(this.decoders, this.requestedOptions, this.decoderChannels, this.acquirerRequestedOptions);
    this.redpitaya.start();
  },
  components: {
    Navbar,
    Footer,
    Channel,
    Parameters,
    DecodedData,
    AcquirerParameters,
  }
}
</script>

<style lang="scss">
@import './styles/_variables';
@import './styles/_mixins';

#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  color: $color;
}

#logic-analyzer-header{
  border-bottom: 1px solid $primaryColor;
}

.channels-header{
  padding: 4px;
  border-right: 1px solid $primaryColor;
  @media (max-width: 768px) {
        border: none;
  }
}

.signals-header{
  background-color: $primaryColor;
  display: flex;
  justify-content: center;
  color: $colorInverted;
}

#left-panel {
  background-color: $panelBackgroundColor;
  min-height: 100%;
}

#startAnalyzing{
  width: 100%;
  background-color: $primaryColor;
  color: $colorInverted;
  &:hover{
    background-color: $primaryColorHover;
  }
}

@media(min-width: 768px){
  #right-panel:not(.show){
      display: block;
  }
}

#right-panel {
  border-left: 1px solid $primaryColor;
  border-bottom: 1px solid $primaryColor;
  background-color: $panelBackgroundColor;
  padding: 0;

  @media (max-width: 768px) {
        border: none;
        border-bottom: 1px solid $primaryColor;
        border-top: 1px solid $primaryColor;
        padding-bottom: 25px;
  }
}

.navbar-light .navbar-nav .nav-link {
  @include link();
}

.nav-tabs .nav-link{
  @include link();
}

.nav-tabs .nav-item.show .nav-link, .nav-tabs .nav-link.active{
  @include link();
}

</style>
