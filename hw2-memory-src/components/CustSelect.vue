<template>
    <div class="oshw2_cust_select" @mouseover="onDplOver($event)" @mouseout="onDplOut($event)">
        <span style="position:relative;"><a>▼</a>{{dplLable}}<i></i></span>
        <ul v-dpl>
            <li v-for="(item, index) in dataList" :key="index" @click="onLiClick(index, $event);$emit('cust_click',activeIndex)">{{item[labelProperty]}}</li>
        </ul>
    </div>
</template>

<script>
export default {
    name: "CustSelect",
    data(){
        return {
            activeIndex:0
        }
    },
    props:{
        dataList:{
            type:Array,
            default(){
                return [
                    {name: "首次适应算法"},
                    {name: "最佳适应算法"}
                ]
            }
        },
        labelProperty:{
            type:String,
            default(){ return "name" }
        }
    },
    directives:{
        dpl:{
            bind(el){
                el.style.display = "none";
            }
        }
    },
    methods:{
        onDplOver(event){
            let ul = event.currentTarget.childNodes[1];
            ul.style.display = "block";
        },
        onDplOut(event){
            let ul = event.currentTarget.childNodes[1];
            ul.style.display = "none";
        },
        onLiClick(index){
            let path = event.path || (event.composedPath && event.composedPath()) //兼容火狐和safari
            path[1].style.display = "none";
            this.activeIndex = index;
            this.$emit("change", {
                index:index,
                value:this.dataList[index]
            })
        }
    },
    computed:{
        dplLable(){
            return this.dataList[this.activeIndex][this.labelProperty]
        }
    }
}
</script>


<style scoped >
    a{
        position:absolute;
        right:7px;
        bottom:10px;
    }
    li{
        list-style: none;
        height: 30px;
        line-height: 30px;
        font-size: 14px;
        border-bottom: solid 1px #f1f1f1;
        background: #ffffff;
        transition: all 1s;
    }
    li:last-child{
        border-bottom: none;
    }
    li:hover{
        background: #333333;
        color:#f6f6f6;
        transition: all 1s;
    }
    span{
        display: block;
        padding-top:10px;
        padding-bottom:10px;
        padding-left:25px;
        padding-right:25px;
        background: #ffffff;
        border:#333333 1px solid;
        text-align: center;
        color: #333333;
        border-radius: 4px;
        
    }
    i{
        margin-left: 6px;
        display: inline-block;
    }
    ul{
        position: absolute;
        top: 40px;
        left: 0;
        width: 100%;
        margin: 0;
        padding: 0;
        border: solid 1px #f1f1f1;
        border-radius: 4px;
        overflow: hidden;
        
    }
    .oshw2_cust_select{
        display: inline-block;
        min-width: 100px;
        position: relative;
        
        
    }
</style>