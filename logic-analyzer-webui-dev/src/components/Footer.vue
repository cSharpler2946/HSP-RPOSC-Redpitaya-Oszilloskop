<template>
    <div class="container-fluid">
        <div id="footer" class="row">
            <hr/>
            <div class="col">
                &copy; OTH Regensburg
            </div>
        </div>
    </div>
</template>

<script>
export default {
  name: 'Footer',
  props: {
  }
}
</script>

<style lang="scss" scoped>
@import './../styles/_variables';

#footer{
    margin-top: 50px;
    text-align: left;
}

</style>
