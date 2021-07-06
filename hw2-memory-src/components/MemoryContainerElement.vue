<template>
  <div class="oshw2_dynamic_mem_alloc_child" :class="'oshw2_dynamic_mem_alloc_childx_'+eleAttrs.el.orderId%2" :style="styAttr" @mouseover="tacklingHover(true);" @mouseleave="tacklingHover(false);">
        <div class="oshw2_dynamic_mem_alloc_child_detailhw">
            <b>#</b><span v-html="eleAttrs.el.jobId"></span><br/>
        </div>
        <transition name="fade">
            <div class="oshw2_dynamic_mem_alloc_child_detail" :key="eleAttrs.id"  v-show="hoverStat">
                <div class="oshw2_dynamic_mem_alloc_child_detail">
                    <b>起始地址:</b><span v-html="eleAttrs.el.begin"></span> KB<br/>
                    <b>终止地址:</b><span v-html="eleAttrs.el.end"></span> KB<br/>
                    <b>内存长度:</b><span v-html="eleAttrs.el.end-eleAttrs.el.begin+1"></span> KB<br/>
                </div>
            </div>
        </transition>
    </div>
</template>

<script>

export default {
  name: 'MemoryContainerElement',
  props: {
      eleAttrs:Object
  },
  created:function(){
      this.updateWidth();
  },
  watch:{
      eleAttrs:function(){
          this.updateWidth();
      }
  },
  methods:{
      updateWidth:function(){
        this.styAttr.width=(this.eleAttrs.el.end-this.eleAttrs.el.begin+1)/(this.eleAttrs.global_data.maxMemory)*100+'%';
        this.styAttr.left=(this.eleAttrs.el.begin)/(this.eleAttrs.global_data.maxMemory)*100+'%';
      },
      tacklingHover:function(value){
          this.hoverStat=value;
      }
  },
  data:function(){
      return{
        styAttr:{
            width:'22px',
            height:'100%',
            left:'0px',
            top:'0px',
        },
        hoverStat:false,
      }
  },
  computed:{
      
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scope>
    .oshw2_dynamic_mem_alloc_childx_0{
        background-color:#777777;
    }
    .oshw2_dynamic_mem_alloc_childx_1{
        background-color:#555555;
    }
    .oshw2_dynamic_mem_alloc_child{
        
        display: inline-block;
        position: absolute;
        margin-right:10px;
        transition:all 1s;

    }
    .oshw2_dynamic_mem_alloc_child:hover{
        transition:background-color 1s;
        background-color:#999999;
    }
    .oshw2_dynamic_mem_alloc_child_detail{
        position:absolute;
        margin-top:30px;
        font-size: 12px;
        width:150px;
    }
    .fade-enter, .fade-leave-to{
        opacity: 0;
        transform:translateY(10px);
    }
    .fade-enter-active, .fade-leave-active{
        transition: all 1s;
    }

    .oshw2_dynamic_mem_alloc_child_detailhw{
        color:#ffffff;
        text-align: center;
        padding-top:12px;
    }
</style>
