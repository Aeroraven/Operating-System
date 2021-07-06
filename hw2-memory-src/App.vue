<template>
    <div id="app">
        <top-bar v-bind:title='projectTitle' @tpselecta="switchMode(arguments)"></top-bar>
        <component :is="getActiveControl" :mainContainerProp="mainContainerProp"/>
    </div>
</template>

<script>
//import HelloWorld from './components/HelloWorld.vue'
import TopBar from './components/TopBar.vue'
import MainContainer from './components/MainContainer.vue'
import ComPlaceholder from './components/ComPlaceholder.vue';

export default {
    name: 'App',
    methods:{
        switchMode:function(x){
            console.log("SwitchMode");
            console.log(x);
            this.mainContainerProp.activatedPage=0;
            this.mainContainerProp.dynamicMemAlloc.memContainer.maxMemory=x[0].memSize
            if(x[0].default==0){
                this.mainContainerProp.dynamicMemAlloc.memRequests=[]
            }
        }
    },
    computed:{
        getActiveControl(){
            return this.componentsList[this.mainContainerProp.activatedPage]
        }
    },
    data:function(){
        return{
            componentsList:[MainContainer,ComPlaceholder],
            projectTitle:'操作系统课程项目—内存管理',
            mainContainerProp:{
                activatedPage:1,
                reqMemAlloc:{
                    title:'请求分区分配模拟',
                },
                dynamicMemAlloc:{
                    title:'动态分区分配模拟',
                    executeBtn:{
                        caption:'执行',
                    },
                    ignoreBtn:{
                        caption:'跳过',
                    },
                    addBtn:{
                        caption:'增加',
                    },
                    memContainer:{
                        maxMemory:640,
                    },
                    memRequests:[
                        {jobId:1,reqType:1,reqSize:130*1},
                        {jobId:2,reqType:1,reqSize:60*1},
                        {jobId:3,reqType:1,reqSize:100*1},
                        {jobId:2,reqType:0,reqSize:60*1},
                        {jobId:4,reqType:1,reqSize:200*1},
                        {jobId:3,reqType:0,reqSize:100*1},
                        {jobId:1,reqType:0,reqSize:130*1},
                        {jobId:5,reqType:1,reqSize:140*1},
                        {jobId:6,reqType:1,reqSize:60*1},
                        {jobId:7,reqType:1,reqSize:50*1},
                        {jobId:6,reqType:0,reqSize:60*1},
                    ]
                },
            }
        }
    },
    components: {
        TopBar,
        MainContainer
    }
}
</script>

<style>
#app {
    font-family: Avenir, Helvetica, Arial, sans-serif;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-align: center;
    color: #2c3e50;
    margin-top: 0px;
    -moz-user-select: none;
    -webkit-user-select: none;
    user-select:none;
}
body{
    margin:0,0,0,0;
}
</style>
