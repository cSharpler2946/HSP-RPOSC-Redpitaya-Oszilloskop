<template>
    <div class="channel-wrapper row">

        <div class="channel-id-box col-md-1">
            <div class="channel-id align-self-center">
            {{ channelId }}
            </div>
        </div>

        <div class="channel-box col-md-2">
            <div class="channel align-self-center">
                {{ channelName }}
                <i class="fas fa-cog channel-settings-btn"></i>
            </div>
        </div>

        <div class="signal-box col-md-9">


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

<style scoped>

.channel-wrapper{
    border-top: 1px solid gray;
    height: 40px;
}

.channel-id-box{
    border-right: 1px solid gray;
    text-align: center;
    font-weight: bold;
    display: flex;
    justify-content: center;
}

.channel-box{
    text-align: left;
    border-right: 1px solid rgb(15, 235, 161);
    display: flex;
    justify-content: flex-end;
}

.channel-settings-btn{
    color: rgb(4, 179, 120);
    cursor: pointer;
    margin-top: 2px;
    margin-left: 10px;
}

.signal-box{
    background-color: white;
}

</style>