<template>
  <div class="oshw2_oprecord">
    <div class='oshw2_oprecord_tbl'>
        <div class="oshw2_oprecord_tbl_slide">
            <pre>
            {{recordInfo.log}}
            </pre>
        </div>
    </div>
  </div>
</template>

<script>
export default {
  name: 'OperationRecord',
  props: {
    recordInfo:Object
  },
  watch:{
      recordInfo:function(){
          console.log(this.recordInfo.log);
      }
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
<style scoped>
    .oshw2_oprecord{
        margin-top:20px;
        text-align: center;
    }
    .oshw2_oprecord_tbl{
        display: inline-block;
    }
    td{
        padding-top:13px;
        padding-bottom:13px;
    }
    .oshw2_oprecord_tbl_head{
        color:#ffffff;
        background-color:#333333;
        padding-top:10px;
        padding-bottom:10px;
        margin-bottom:1px;
    }
    .oshw2_oprecord_e2{
        color:#222222;
        background-color:#CCCCCC;
        padding-top:10px;
        padding-bottom:10px;
        margin-top:3px;
        height:12px;
    }
    .oshw2_oprecord_e{
        display: inline-block;
        margin-left:3px;
        width: 25%;
    }
    .oshw2_oprecord{
        width:100%;
        table-layout: fixed;
    }
    .oshw2_oprecord_tbl{
        width:60%
    }
    .oshw2_oprecord_tbl_slide{
        margin-top:0px;
        height:200px;
        overflow-y:overlay;
        overflow-x:hidden;
        position: relative;
        border:#333333 2px solid;
        padding:10px 10px 10px 10px;
        text-align: left;
        font-size:14px;
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
    .oshw2_oprecord_tbl_tr{
       transition: all 1s;
       display: inline-block;
       width:100%;
    }
</style>
