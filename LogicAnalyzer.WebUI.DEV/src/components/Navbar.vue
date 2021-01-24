<template>
<nav class="navbar navbar-expand-lg navbar-light">
  <div class="container-fluid">
    <a class="navbar-brand" href="#">
    
        <img src="../assets/logo.png"/>

    </a>
    <button class="navbar-toggler" type="button" data-bs-toggle="collapse" data-bs-target="#navbarSupportedContent" aria-controls="navbarSupportedContent" aria-expanded="false" aria-label="Toggle navigation">
      <span class="navbar-toggler-icon"></span>
    </button>
    
    <div class="collapse navbar-collapse" id="navbarSupportedContent">
      <ul class="navbar-nav me-auto mb-2 mb-lg-0">
        <li class="nav-item">
          <a class="nav-link active" aria-current="page" href="#">Home</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="#">Documentation</a>
        </li>
        <li class="nav-item">
          <a class="nav-link" href="#">About</a>
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
    title: String,
    brand: String
  }
}
</script>

<style scoped>

.navbar{
    border-bottom: 1px solid rgb(15, 235, 161);
    background-color: white;
}

.nav-item > .nav-link{
    color: rgb(4, 179, 120);
}

.nav-item > .nav-link:hover{
    color: rgb(15, 235, 161);
}

.nav-item > .active{
    color: rgb(4, 179, 120) !important;
    text-decoration: underline;
}

.nav-item > .active:hover{
    color: rgb(15, 235, 161) !important;
}

.navbar-brand > img{
    height: 40px;
}


</style>