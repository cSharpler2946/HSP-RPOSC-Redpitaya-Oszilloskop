<template>
    <div class="card">
        <div class="card-body">
            <p class="card-text decoded-data">
                Lorem ipsum dolor sit amet, consetetur sadipscing elitr,
                sed diam nonumy eirmod tempor invidunt ut labore et dolore magna
                aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo
                dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus
                est Lorem ipsum dolor sit amet. Lorem ipsum dolor sit amet, consetetur
                sadipscing elitr, sed diam nonumy eirmod tempor invidunt ut labore et dolore
                magna aliquyam erat, sed diam voluptua. At vero eos et accusam et justo duo
                dolores et ea rebum. Stet clita kasd gubergren, no sea takimata sanctus est
                Lorem ipsum dolor sit amet.
            </p>
        </div>
    </div>
</template>

<script>
export default {
  name: 'DecodedData',
  props: {

  }
}
</script>

<style lange="scss" scoped>

.decoded-data{
    text-align: left;
    overflow-y: auto;
    max-height: 100%;
}

</style>
