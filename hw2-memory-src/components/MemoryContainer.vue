<template>
  <div>
    <small> 0 KB </small>
    <div class="oshw2_memory_container">
        <transition-group name="fade2">
          <memory-container-element v-for="i in MemoryData.length" :key="MemoryData[i-1].jobId" :eleAttrs="{el:MemoryData[i-1],global_data:memContainerProp,id:i}"></memory-container-element>
        </transition-group>
    </div>
    <small> {{memContainerProp.maxMemory}} KB</small>
  </div>
</template>

<script>
import MemoryContainerElement from './MemoryContainerElement.vue'
export default {
  name: 'MemoryContainer',
  props: {
    memContainerProp:Object,
    memWatch:Boolean,
    memInfoArg:Array,
  },
  created(){
    console.log("PRINT DONE")
  },
  components:{
    MemoryContainerElement,  
  },
  watch:{
      memWatch:function(){
          this.MemoryData=this.memInfoArg
      }
  },
  data:function(){
      return{
        MemoryData:[]
      }
  }
  
}
</script>

<style scoped>
    .oshw2_memory_container{
        width:60%;
        border:#222222 2px solid;
        margin-left:0px;
        display: inline-block;
        height:80px;
        margin-top:0px;
        padding-top:2px;
        padding-bottom:2px;
        padding-left:2px;
        padding-right:2px;
        text-align: left;
        position: relative;
    }
    .fade2-leave-to{
        opacity: 0;
        transform:translateY(10px);
    }
    .fade2-enter{
        opacity: 0;
        transform:translateX(10px);
    }
    .fade2-enter-active, .fade2-leave-active{
        transition: all 1s;
    }
</style>
