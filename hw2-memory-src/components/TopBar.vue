<template>
   <div class="oshw2_topbar"  :style="overridingBarStyle">
      <span v-html="title"></span> | <small><small>1950641 黄智文</small></small>
      <div class="oshw2_extended_topbar">
          <h3 style="color:#ffffff;margin-top:50px">动态分区分配模拟</h3>
          <div class="oshw2_dynamic_mem_alloc_app" >
            <form>
                <label>内存大小:</label>
                <div class="oshw2_dynamic_mem_alloc_appic">
                    <input class="oshw2_dynamic_mem_alloc_appi"  v-model.number="inputMemSize"><br/>
                </div><br/>
                <label>默认作业:</label>
                <div class="oshw2_dynamic_mem_alloc_appic">
                    <input name="new_req_type" id="nrt1" v-model.number="inputReqType" type="radio" value="1"><label for="nrt1">使用默认作业列表</label>&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;&nbsp;
                    <input name="new_req_type" id="nrt2" v-model.number="inputReqType" type="radio" value="0"><label for="nrt2">手动添加</label>
                </div><br/><br/>
            </form>
            </div>
             <cust-button id="oshw2_et_1" caption="开始动态分区分配模拟" selectedProp="false" :style="overridingBtnStyle" @cust_click="collapseMenu();"></cust-button>
            <!--<cust-button id="oshw2_et_2" caption="请求分区分配模拟" selectedProp="false" :style="overridingBtnStyle" @cust_click="collapseMenu();$emit('tpselectb');"></cust-button>-->
        </div>
    </div>
</template>

<script>
import CustButton from './CustButton.vue'

export default {
    name: 'TopBar',
    components:{
        CustButton,
    },
    props: {
        title:String
    },
    data:function(){
        return{
            overridingBtnStyle:{
                border:"#ffffff 1px solid",
                'marginTop':'50px',
                height:''
            },
            overridingBarStyle:{
                height:"100%",
            },
            inputMemSize:640,
            inputReqType:1,
        }
    },
    methods:{
        collapseMenu:function(){
            this.overridingBarStyle.height="30px";
            this.$emit('tpselecta',{
                default:this.inputReqType,
                memSize:this.inputMemSize
            });
            
        }
    }
}
</script>

<style scoped>
    #oshw2_et_1{
        left:50%;
        margin-left:-120px;
        position:absolute;
    }
    #oshw2_et_2{
        top:30%;
        right:20%;
        position:absolute;
    }
    .oshw2_extended_topbar{
        margin-top:50px;
    }
    .oshw2_dynamic_mem_alloc_appic{
        width:50%;
        display: inline-block;
    }
    .oshw2_dynamic_mem_alloc_app{
        margin-top:80px;
    }
    .oshw2_dynamic_mem_alloc_appi{
        height:30px;
        margin-top:0px;
        margin-bottom: 10px;
        border:#333333 2px solid;
        border-radius: 2px;
        font-size:16px;
    }
    .oshw2_extended_topbar{
        text-align: center;
        position:relative;
        height:100%;
        width:100%
    }
    .oshw2_topbar{
        background-color:#222222;
        text-align:left;
        position: fixed;
        left:0;
        top:0;
        right:0;
        height:100%;
        padding-left:10px;
        padding-top:10px;
        padding-bottom:10px;
        color:#ffffff;
        font-family:Verdana, Geneva, Tahoma, sans-serif;
        font-size:20px;
        z-index:9999999;
        overflow:hidden;
        transition: all 1s;
    }
</style>
