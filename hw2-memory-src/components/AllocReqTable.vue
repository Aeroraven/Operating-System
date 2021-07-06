<template>
  <div class="oshw2_alloc_req_table">
    
    <div class='oshw2_alloc_req_table_tbl'>
        <div class='oshw2_alloc_req_table_tbl_head oshw2_alloc_req_table_e'>请求作业ID</div>
        <div class='oshw2_alloc_req_table_tbl_head oshw2_alloc_req_table_e'>请求操作</div>
        <div class='oshw2_alloc_req_table_tbl_head oshw2_alloc_req_table_e'>请求大小</div>
        
        <div class="oshw2_alloc_req_table_tbl_slide">
            <transition-group name="fade2">
                <div class='oshw2_alloc_req_table_tbl_tr' v-for="i in reqList.length" :key="reqList[i-1].uniqId" style="font-family:微软雅黑">
                    <div class='oshw2_alloc_req_table_e2 oshw2_alloc_req_table_e'>{{reqList[i-1].jobId}}</div>
                    <div class='oshw2_alloc_req_table_e2 oshw2_alloc_req_table_e'>{{reqList[i-1].reqType?"申请":"释放"}}</div>
                    <div class='oshw2_alloc_req_table_e2 oshw2_alloc_req_table_e'>{{reqList[i-1].reqType?reqList[i-1].reqSize+" KB":"---"}}</div>
                    <br/>
                </div>
            </transition-group>
        </div>

    </div>
    
  </div>
</template>

<script>
export default {
  name: 'AllocReqTable',
  props: {
    allocReqTableProp:Array
  },
  data:function(){
      return{
          reqList:[]
      }
  },
  created:function(){
      this.reqList=this.allocReqTableProp;
  }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    .oshw2_alloc_req_table{
        margin-top:20px;
        text-align: center;
    }
    .oshw2_alloc_req_table_tbl{
        display: inline-block;
    }
    td{
        padding-top:13px;
        padding-bottom:13px;
    }
    .oshw2_alloc_req_table_tbl_head{
        color:#ffffff;
        background-color:#333333;
        padding-top:10px;
        padding-bottom:10px;
        margin-bottom:1px;
    }
    .oshw2_alloc_req_table_e2{
        color:#222222;
        background-color:#CCCCCC;
        padding-top:10px;
        padding-bottom:15px;
        margin-top:3px;
        
        height:12px;
    }
    .oshw2_alloc_req_table_e{
        display: inline-block;
        margin-left:3px;
        width: 25%;
    }
    .oshw2_alloc_req_table{
        width:100%;
        table-layout: fixed;
    }
    .oshw2_alloc_req_table_tbl{
        width:90%
    }
    .oshw2_alloc_req_table_tbl_slide{
        margin-top:0px;
        height:200px;
        overflow-y:overlay;
        overflow-x:hidden;
        position: relative;
    }
    .fade2-leave-to{
        opacity: 0;
        transform: translateX(30px);
    }
    .fade2-leave-active{
        position: absolute;
        display: none;
        transition: all 1s;

    }
    .oshw2_alloc_req_table_tbl_tr{
       transition: all 1s;
       display: inline-block;
       width:100%;
    }
</style>
