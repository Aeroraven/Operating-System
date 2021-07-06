<template>
  <div class="oshw2_req_mem_alloc">
    <span class="oshw2_req_mem_alloc_title" v-html="reqMemAllocProp.title"></span><br/><br/>
    
  </div>
</template>

<script>

export default {
    name: 'ReqMemAlloc',
    props: {
        reqMemAllocProp:Object
    },
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    .oshw2_req_mem_alloc{
        text-align: center;
    }
    .oshw2_req_mem_alloc_title{
        font-weight: bold;
        font-size:23px;
    }
    #mem_container{
        margin-bottom:70px;
    }
    .oshw2_req_mem_alloc_frame{
        text-align: center;
    }
    .oshw2_req_mem_alloc_fropt{
        border:#333333 1px solid;
        border-radius: 5px;
    }
    .oshw2_req_mem_alloc_app{
        margin-top:30px;
    }
    .oshw2_req_mem_alloc_appi{
        height:30px;
        margin-top:0px;
        margin-bottom: 10px;
        border:#333333 2px solid;
        border-radius: 2px;
        font-size:16px;
    }
    .oshw2_req_mem_alloc_appic{
        width:30%;
        display: inline-block;
    }
</style>
