<template>
  <div class="oshw2_cust_button" @click="$emit('cust_click')" @hover="$emit('cust_hover')" :class="'oshw2_cust_button_s'+selectedProp">
    <span v-html="caption"></span>
  </div>
</template>

<script>

export default {
  name: 'CustButton',
  props: {
    caption:String,
    selectedProp:Boolean
  },
}
</script>

<style scoped>
    .oshw2_cust_button_sfalse{
        background-color:#222222;
        color:#ffffff;
        border:#222222 1px solid
    }
    .oshw2_cust_button_sfalse:hover{
      transition:background-color 1s;
      background-color:#666666;
    }

    .oshw2_cust_button_strue{
        background-color:#ffffff;
        color:#333333;
        border:#333333 1px solid
    }
    .oshw2_cust_button_strue:hover{
        background-color:#EEEEEE;

    }
    .oshw2_cust_button{
        text-align: center;
        display: inline-block;
        /*border:dodgerblue 1px solid;*/
        border-radius: 4px;
        margin-top:5px;
        margin-left:3px;
        margin-right:3px;
        margin-bottom:5px;
        padding-top:10px;
        padding-bottom:10px;
        padding-left:25px;
        padding-right:25px;
        cursor:pointer;
        transition:all 0.5s;
    }
    
</style>
