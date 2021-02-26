<template>
    <form method="post">
        <label>{{requestedOptions}}</label>
    </form>
</template>

<script>
export default {
    name: "AcquirerParameters",
    props: {
        requestedOptions: "Hello!"
    },
    data () {
        return {
            chosenOptions: "Hello!"
        }
    },
}
</script>

<style scoped>

label {
    text-align: left;
    width: 100%;
    padding-bottom: 0;
    font-size: 0.9em;
}

.parameter-wrapper {
    padding-left: 15px;
    padding-right: 15px;
}

</style>