<template>
    <div class="channel-wrapper row">

        <div class="channel-id-box col-md-1 col-2">
            <div class="channel-id">
            {{ channelId }}
            </div>
        </div>

        <div class="channel-box col-md-2 col-10">
            <div class="row" style="padding:0;">

            <div @dblclick="edit = true" class="col-9 channel">
                <div v-show="edit == false">
                    <label> {{ channelName }}</label>
                </div>
                <input class="form-control" v-show="edit == true" v-model="channelName" :maxlength="maxCharacters" v-on:blur="edit=false; $emit('update')" @keyup.enter="edit=false; $emit('update')">
            </div>

            <div class="col-3 channel-settings">
                <i class="fas fa-cog channel-settings-btn float-end" data-bs-toggle="modal" v-bind:data-bs-target="'#channel-settings-modal-' + channelId"></i>
            </div>
            </div>
        </div>

        <div class="signal-box col-md-9 col-12">

        </div>
    </div>

<!-- 
    This modal should be generated each time you click on the correspondig button.
    If you hard-code this, each time you reopen the settings, the selected element will be the one selected last.
    However, the last selected element is not equal to the last applied element (the value of the decodeChannel data object).
 -->
<div class="modal fade" v-bind:id="'channel-settings-modal-' + channelId" tabindex="-1" aria-labelledby="channelSettings" aria-hidden="true"> 
  <div class="modal-dialog modal-dialog-centered">
    <div class="modal-content">

      <div class="modal-header">
        <h5 class="modal-title" id="exampleModalLabel">Settings for: <b>{{channelName}}</b></h5>
        <button type="button" class="btn-close" data-bs-dismiss="modal" aria-label="Close"></button>
      </div>

      <div class="modal-body">
        <div class="row">
            <label class="col-sm-4 col-form-label">Decoder Channel:</label>
            <div class="col-sm-8">
                <select v-model="decoderChannel" class="form-select" aria-label="Select decoder channel">
                    <option selected>none</option>
                    <option value="1">One</option>
                    <option value="2">Two</option>
                    <option value="3">Three</option>
                </select>
            </div>
        </div>
      </div>

      <div class="modal-footer">
        <button type="button" class="btn btn-danger" data-bs-dismiss="modal">Cancel</button>
        <button @click="editChannelSettings(decoderChannel)" type="button" class="btn btn-success" data-bs-dismiss="modal">Ok</button>
      </div>
    </div>
  </div>
</div>

</template>

<script>
export default {
    name: "Channel",
    data(){
        return {
            editedChannel: null,
            maxCharacters: 20,
            decoderChannel: null,
        }
    },
    props: {
        channelId: {
            type: Number,
            required: true
        },
        channelName: String,
        edit: {
            type: Boolean,
            default: false,
        },
    },
    methods: {
        editChannelName: function(channel){
            this.editedChannel = channel;
        },
        editChannelSettings: function(decoderChannel){
            // The parameter decoderChannel contains the value of the selected element.
            console.log(decoderChannel);
        },
    }
}
</script>

<style lang="scss" scoped>
@import './../styles/_variables';

.channel-wrapper{
    border-bottom: 1px solid gray;
}

.channel-id-box{
    border-right: 1px solid gray;
    text-align: center;
    font-weight: bold;
     @media (max-width: 768px) {
        border-bottom: 1px solid gray;
    }
}

.channel-box{
    text-align: left;
    border-right: 1px solid $primaryColor;
    @media (max-width: 768px) {
        border-right: none;
        border-bottom: 1px solid gray;
    }
}

.channel-settings-btn{
    color: $primaryColor;
    cursor: pointer;
    margin-top: 2px;
    margin-left: 10px;
}

.signal-box{
    background-color: $signalBackgroundColor;
    min-height: 40px;
}

</style>