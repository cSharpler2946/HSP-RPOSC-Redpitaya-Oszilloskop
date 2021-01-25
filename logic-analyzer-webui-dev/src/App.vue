<template>
  <Navbar title="Logic Analyzer" brand="logo.png"/>

<div class="container-fluid">

  <div class="row">
    <div id="left-panel" class="col-md-9">
      <div id="logic-analyzer-header" class="row">
        <div class="channels-header col-md-3">
            <button id="startAnalyzing" type="button" class="btn">
              <span>Start Analyzing</span>
              <i class="fas fa-play" style="vertical-align: middle; margin-left: 10px;"></i>
            </button>
        </div>
        <div class="signals-header col-md-9">
          <div class="align-self-center">
            <h3>Logic Analyzer</h3>
          </div>
        </div>
      </div>

      <Channel v-for="n in 10" :key="n" :channelId="n" :channelName="`Channel ${n}`" />

      <!-- final border in case the right-panels height is bigger than the left-panels height -->
    </div>

    <div id="right-panel" class="col-md-3">
      <nav>
        <div class="nav nav-tabs" id="nav-tab" role="tablist">
          <a class="nav-link active col" id="nav-parameter-tab" data-bs-toggle="tab" href="#nav-parameter" role="tab" aria-controls="nav-parameter" aria-selected="true">
            Prameters
          </a>
          <a class="nav-link col" id="nav-decoded-data-tab" data-bs-toggle="tab" href="#nav-decoded-data" role="tab" aria-controls="nav-decoded-data" aria-selected="false">
            Decoded Data
          </a>
        </div>
      </nav>
      <div class="tab-content" id="nav-tabContent">
        <div class="tab-pane fade show active" id="nav-parameter" role="tabpanel" aria-labelledby="nav-parameter-tab">
          <Parameters/>
        </div>
        <div class="tab-pane fade" id="nav-decoded-data" role="tabpanel" aria-labelledby="nav-decoded-data-tab">
          <DecodedData/>
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
import RedPitaya from './redpitaya.js'

export default {
  name: 'App',
  data () {
    return {
      app_id: 'TestApp',
      app_url: 'bla',
      socket_url: 'ws://' + window.location.hostname + ':9002',
      redpitaya: null
    }
  },
  computed: {
    // Make calculations/computations in here
  },
  mounted () {
    // TODO: Build up WebSocket-Connection with RedPitaya in here.
    this.redpitaya = new RedPitaya(this.app_id, this.app_url, this.socket_url);
    this.redpitaya.test();
  },
  components: {
    Navbar,
    Footer,
    Channel,
    Parameters,
    DecodedData
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

#right-panel {
  border-left: 1px solid $primaryColor;
  background-color: $panelBackgroundColor;
  padding: 0;
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
