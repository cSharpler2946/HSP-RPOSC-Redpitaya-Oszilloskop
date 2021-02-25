<template>
    <form method="post">
        <div class="parameter-wrapper" v-for="option in requestedOptions" v-bind:key="option" v-bind:value="option">
            <label class="form-label">
                {{ option["desc"] }}
            </label>
            <input type="text" class="form-control form-control-sm" v-if="!('values' in option)" v-model.lazy="chosenOptions[option.id]" v-on:change="$emit('chosenOptionsChanged', chosenOptions)">
            <select class="form-select" v-if="'values' in option" v-model="chosenOptions[option.id]" v-on:change="$emit('chosenOptionsChanged', chosenOptions)">
                <option v-for="possibleValue in option['values']" v-bind:key="possibleValue">
                    {{ possibleValue }}
                </option>
            </select>
            <br/>
        </div>
    </form>
</template>

<script>
export default {
    name: "AcquirerParameters",
    props: {
        requestedOptions: Array
    },
    data () {
        return {
            chosenOptions: {}
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