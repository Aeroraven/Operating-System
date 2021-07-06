<template>
  <div class="oshw2_main_container">
    <!--<dynamic-mem-alloc :dynamicMemAllocProp="mainContainerProp.dynamicMemAlloc"></dynamic-mem-alloc>-->
      <component :is="activeControl" :dynamicMemAllocProp="mainContainerProp.dynamicMemAlloc" :reqMemAllocProp="mainContainerProp.reqMemAlloc"></component>
  </div>
</template>

<script>
import DynamicMemAlloc from './DynamicMemAlloc.vue'
import ReqMemAlloc from './ReqMemAlloc.vue'

export default {
  name: 'MainContainer',
  props: {
      mainContainerProp:Object
  },
  components:{
      DynamicMemAlloc,
      ReqMemAlloc
  },
  computed:{
      activeControl:function(){
          console.log(this.controlList[this.mainContainerProp.activatedPage]);
          return this.controlList[this.mainContainerProp.activatedPage];
      }
  },
  data:function(){
      return{
          controlList:['DynamicMemAlloc','ReqMemAlloc']
      }
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    .oshw2_main_container{
        margin-top:100px;
        text-align: center;
    }
</style>
