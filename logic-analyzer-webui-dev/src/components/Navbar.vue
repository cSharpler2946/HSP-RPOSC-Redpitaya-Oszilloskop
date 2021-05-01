<template>
<nav class="navbar navbar-expand-lg navbar-light">
  <div class="container-fluid">
    <a class="navbar-brand" href="#">

        <img src="../assets/OTH-logo-extended.svg"/>

    </a>
    <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
      <span class="navbar-toggler-icon"></span>
    </button>

    <div class="collapse navbar-collapse" id="navbarSupportedContent">
      <ul class="navbar-nav me-auto mb-2 mb-lg-0">
        <li class="nav-item">
          <router-link :to="{name: 'LogicAnalyzer'}">Home</router-link>
        </li>
        <li class="nav-item">
          <router-link :to="{name: 'Documentation'}">Documentation</router-link>
        </li>
      </ul>
    </div>
  </div>
</nav>
</template>

<script>
export default {
  name: 'Navbar',
  props: {
  }
}
</script>

<style lang="scss" scoped>
@import './../styles/_variables';
@import './../styles/_mixins';

.navbar{
  border-bottom: 1px solid $primaryColor;
  background-color: white;
}

.navbar-brand > img{
  height: 40px;
}

nav a{
  @include link();
}

.nav-item{
  margin-left: 15px;
}

nav a.router-link-exact-active{
  color: $linkColorActive;
}

</style>
