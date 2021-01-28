<template>
    <div class="channel-wrapper row">

        <div class="channel-id-box col-md-1 col-2">
            <div class="channel-id">
            {{ channelId }}
            </div>
        </div>

        <div class="channel-box col-md-2 col-10">
            <div class="col channel">
                {{ channelName }} 
                <i class="fas fa-cog channel-settings-btn"></i>
            </div>
        </div>
        <div class="signal-box col-md-9 col-12">

        </div>
    </div>
</template>

<script>
export default {
    name: "Channel",
    props: {
        channelId: {
            type: Number,
            required: true
        },
        channelName: String
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
}

.channel-box{
    text-align: left;
    border-right: 1px solid $primaryColor;
    @media (max-width: 768px) {
        border-right: none;
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