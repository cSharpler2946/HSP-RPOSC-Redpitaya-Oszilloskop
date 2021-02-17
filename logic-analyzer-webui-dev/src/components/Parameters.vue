<template>
    <form method="post">
        <select class="form-select" aria-label="Choose a protocol.">
            <!--option selected>-- Choose protocol --</option>
            <option value="1">I2C</option>
            <option value="2">CAN</option-->
            <option v-for="decoder in decoders" v-bind:key="decoder">
                {{ decoder["name"] }}
            </option>
        </select>

        <div style="margin-bottom: 15px;"></div>

        <div class="parameter-wrapper">
            <label for="sampleRange" class="form-label">Baud Rate:</label>
            <input type="range" class="form-range" id="sampleRange">

            <label for="sampleParam" class="col-sm-2 col-form-label">Sample Parameter:</label>
            <input type="text" class="form-control form-control-sm" placeholder="Sample param..." id="sampleParam">
        
            <div style="margin-bottom: 15px;"></div>

            <label>Data Polarity:</label>
            <div class="radio-buttons" style="float: left;">
                <div class="form-check form-check-inline">
                    <input class="form-check-input" type="radio" name="inlineRadioOptions" id="inlineRadio1" value="option1">
                    <label class="form-check-label" for="inlineRadio1">1</label>
                </div>
                <div class="form-check form-check-inline">
                    <input class="form-check-input" type="radio" name="inlineRadioOptions" id="inlineRadio2" value="option2">
                    <label class="form-check-label" for="inlineRadio2">2</label>
                </div>
            </div>
        </div>
    
        <br/>
        <hr/>

        <div class="parameter-wrapper">
            <div class="row">

            <label>Start-/Stop-Bits:</label>
            <div class="col">
                <input type="text" class="form-control form-control-sm" placeholder="start bit">
            </div>
            <div class="col">
                <input type="text" class="form-control form-control-sm" placeholder="stop bit">
            </div>
            </div>
        </div>

    </form>
</template>

<script>
export default {
    name: "Parameters",
    props: {
        decoders: Array
    }
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