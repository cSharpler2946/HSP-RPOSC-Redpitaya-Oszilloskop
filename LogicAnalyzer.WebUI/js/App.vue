<template>
    <div id="app">

    @{{ user.username }} - {{ fullName }}

    </div>
</template>


<script>
export default {
    name: 'App',
    data(){
        return{
            // Return the data that is used in the component
            followers: 0,
            user: {
                id: 1,
                username: '_MitchelRomney',
                fistName: 'Mitchell',
                lastName: 'Romney',
                email: 'michellromney@theearthissquare.com',
                isAdmin: true
            }
        }
    },
    computed:{
        fullName(){
            return `${this.user.firstNam} ${this.user.lastName}`;
        }
    }
}
</script>