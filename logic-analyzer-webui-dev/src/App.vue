<template>
  <Navbar/>
  <router-view/>
  <Footer/>
</template>

<script>
import Navbar from './components/Navbar.vue'
import Footer from './components/Footer.vue'

export default {
  name: 'App',
  data () {
    return {
    }
  },
  components: {
    Navbar,
    Footer
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
