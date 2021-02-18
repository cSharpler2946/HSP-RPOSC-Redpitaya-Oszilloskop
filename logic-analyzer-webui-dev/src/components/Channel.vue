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
                <i class="fas fa-cog channel-settings-btn float-end"></i>
            </div>
            </div>
        </div>
        

        <div class="signal-box col-md-9 col-12">

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