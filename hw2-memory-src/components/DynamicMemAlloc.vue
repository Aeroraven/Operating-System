<template>
  <div class="oshw2_dynamic_mem_alloc">
    <span class="oshw2_dynamic_mem_alloc_title" v-html="dynamicMemAllocProp.title"></span><br/><br/>
    <b>作业情况</b><br/><small><small>鼠标移动至对应块可查看详情</small></small>
    <div id="mem_container">
        <memory-container :memContainerProp="dynamicMemAllocProp.memContainer" :memInfoArg="memInfo" :memWatch="memSwitch"/>
    </div>
    <b>空闲分区链-链表内情况</b><br/><small><small>鼠标移动至对应块可查看详情</small></small>
    <div id="mem_container">
        <memory-container :memContainerProp="dynamicMemAllocProp.memContainer" :memInfoArg="idleBlockChains" :memWatch="memSwitch"/>
    </div>
    <cust-button :caption="dynamicMemAllocProp.executeBtn.caption" :selectedProp="false" @cust_click="executeClickHandler"></cust-button>
    <cust-button :caption="dynamicMemAllocProp.ignoreBtn.caption" :selectedProp="false" @cust_click="ignoreHandler"></cust-button>
    
    <span style="font-size:32px;display:inline-block;width:25px;">|</span>
    <cust-button caption="请求队列" :selectedProp="frameComponentActive==0" @cust_click="functionClickHandler(0)"></cust-button>
    <cust-button caption="操作记录" :selectedProp="frameComponentActive==1" @cust_click="functionClickHandler(1)"></cust-button>
    <cust-button caption="增加请求" :selectedProp="frameComponentActive==2" @cust_click="functionClickHandler(2)"></cust-button>
    <span style="font-size:32px;display:inline-block;width:25px;">|</span>
    <cust-select @cust_click="algoChangeHandler" style="z-index:99999;"></cust-select>
    <div class="oshw2_dynamic_mem_alloc_frame">
        <div v-if="frameComponentActive!=2">
            <component :is="switchFrame" :allocReqTableProp="dynamicWorkList" :recordInfo="{log:operationLog}"></component>
        </div>
        <div class="oshw2_dynamic_mem_alloc_app" v-if="frameComponentActive==2">
            <form>
                <label>作业编号:</label>
                <div class="oshw2_dynamic_mem_alloc_appic">
                    <input class="oshw2_dynamic_mem_alloc_appi" @keyup="inputChange" v-model.number="inputJobId"><br/>
                </div><br/>
                <label>内存大小:</label>
                <div class="oshw2_dynamic_mem_alloc_appic">
                    <input class="oshw2_dynamic_mem_alloc_appi"  @keyup="inputChange" v-model.number="inputMemSize"><br/>
                </div><br/>
                <label>请求方式:</label>
                <div class="oshw2_dynamic_mem_alloc_appic">
                    <input name="new_req_type" id="nrt1" v-model.number="inputReqType" type="radio" value="1"><label for="nrt1">申请</label>
                    <input name="new_req_type" id="nrt2" v-model.number="inputReqType" type="radio" value="0"><label for="nrt2">释放</label>
                </div><br/><br/>
            </form>
            <cust-button caption="增加指令" @cust_click="insertRequest(inputJobId,inputReqType,inputMemSize)"></cust-button>
    
        </div>
    </div>
  </div>
</template>

<script>
import MemoryContainer from './MemoryContainer.vue'
import CustButton from './CustButton.vue'
import AllocReqTable from'./AllocReqTable.vue'
import CustSelect from './CustSelect.vue'
import OperationRecord from './OperationRecord.vue'
import {BidirectionalLinkedList} from '@/data_structures/linklist.js'

export default {
    name: 'DynamicMemAlloc',
    props: {
        dynamicMemAllocProp:Object
    },
    components:{
        MemoryContainer,
        CustButton,
        AllocReqTable,
        CustSelect,
        OperationRecord
    },
    data:function(){
        return{
            frameComponentList:['AllocReqTable','OperationRecord'],
            frameComponentActive:0,
            
            usingAlgo:0,

            dynamicWorkList:[],
            workList:[],
            workList_cur:0,
            memInfo:[], //作业信息（仅用于UI显示，不是算法执行的依据）
            memSwitch:true,

            freeblockInfo:new BidirectionalLinkedList(), //空闲分区链

            inputJobId:0,
            inputMemSize:0,
            inputReqType:0,

            operationLog:"\n准备进行动态内存分配模拟\n",
            uniqIdx:0
        }
    },
    created:function(){
        this.workList=this.dynamicMemAllocProp.memRequests;
        var i;
        for(i=0;i<this.workList.length;i++){
            this.dynamicWorkList.push({
                jobId:this.workList[i].jobId,
                reqType:this.workList[i].reqType,
                reqSize:this.workList[i].reqSize,
                uniqId:i,
                })
            this.uniqIdx=i; 
        }
        this.fbiInit()
        this.memSwitch=!this.memSwitch;
    },
    mounted(){
        this.memSwitch=!this.memSwitch;
    },
    computed:{
        switchFrame:function(){
            return this.frameComponentList[this.frameComponentActive];
        },
        idleBlockChains(){
            let x = []
            let s = 1
            this.freeblockInfo.iterativeScan(
                function(data){
                    if(data.tag!='head'){
                        x.push({
                            begin:data.begin,
                            end:data.begin+data.length-1,
                            length:data.length,
                            orderId:s,
                            jobId:s
                        })
                        s++
                    }
                }
            )
            console.log(x)
            return x
        }
    },
    methods:{
        inputChange:function(){
            this.inputJobId = this.inputJobId.replace(/[^\d]/g, '')
            this.inputMemSize = this.inputMemSize.replace(/[^\d]/g, '')
        },
        insertRequest:function(_reqJob,_reqType,_reqSize){
            this.workList.push({
                    jobId:_reqJob,
                    reqType:_reqType,
                    reqSize:_reqSize,
                    uniqId:this.uniqIdx,
                    })
            this.dynamicWorkList.push({
                    jobId:_reqJob,
                    reqType:_reqType,
                    reqSize:_reqSize,
                    uniqId:this.uniqIdx,
                    })
            this.uniqIdx++;
            window.alert('请求添加成功！')
        },
        appendLog:function(fx){
            this.operationLog=this.operationLog+fx+'\n';
        },
        algoChangeHandler:function(x){
            this.usingAlgo=x[0]
            console.log("Algorithm Switched");
        },
        functionClickHandler:function(x){
            this.frameComponentActive=x;
        },
        executeClickHandler:function(){
            this.memSwitch=!this.memSwitch;
            if(this.usingAlgo==0){
                this.execAlgo(0);
            }else{
                this.execAlgo(1);
            }
            console.log("==============")
            this.fbiShow()
            console.log("==============")
        },
        //空闲分区链处理
        fbiShow(){
            this.freeblockInfo.iterativeScan(
                function(data){
                    console.log("ELEMENT-BEGIN="+data.begin+" LEN="+data.length)
                }
            )
        },
        fbiInit(){ //初始化空闲分区链
            this.freeblockInfo = new BidirectionalLinkedList()
            this.freeblockInfo.cur=this.freeblockInfo.head
            this.freeblockInfo.appendAfter({
                tag:'normal',
                begin:0,
                length:this.dynamicMemAllocProp.memContainer.maxMemory
            })
        },
        //首次适应插入
        fbiInsert(task){
            this.freeblockInfo.find(
                function(llnode){
                    if(llnode.length>=task.length)
                        return true
                    return false;
                }
            )
            if(this.freeblockInfo.cur.data.tag!='head'){
                let x=this.fbiAllocate(task.length)
                return {
                    stat:true,
                    begin:x.begin
                }
            }
            return {
                stat:false,
                begin:-1
            }
        },
        //最佳适应插入
        fbiInsertBestFit(task){
            this.freeblockInfo.bestFitFind(
                task.length,
                function(data){
                    return data.length
                }
            )
            if(this.freeblockInfo.cur!=null){
                let x=this.fbiAllocate(task.length)
                return {
                    stat:true,
                    begin:x.begin
                }
            }
            return {
                stat:false,
                begin:-1
            }
        },
        fbiAllocate(allocLen){//分配并且更新分区表
            this.freeblockInfo.cur.data.begin+=allocLen
            this.freeblockInfo.cur.data.length-=allocLen
            return{
                begin:this.freeblockInfo.cur.data.begin-allocLen
            }
        },
        fbiRelease(task){ //任务释放
            //情况1：可以与前一空闲分区合并
            let mergedBlock = null
            let complete = false
            this.freeblockInfo.findLastSatisfy(
                function(llnode){
                    if(llnode.begin+llnode.length===task.begin)
                        return true
                    return false;
                }
            )
            if(this.freeblockInfo.cur.data.tag!='head'){
                console.log("BRANCH ENTERED")
                if(this.freeblockInfo.cur.data.begin+this.freeblockInfo.cur.data.length===task.begin){
                    this.freeblockInfo.cur.data.length+=task.length
                    mergedBlock=this.freeblockInfo.cur
                    complete = true
                    console.log("REMOVED COND 1")
                }
            }
            
            //情况2：可以与后一空闲分区合并
            this.freeblockInfo.find(
                function(llnode){
                    if(task.begin+task.length===llnode.begin)
                        return true;
                    return false;
                }
            )
            if(this.freeblockInfo.cur!=null){
                if(mergedBlock===null){
                    this.freeblockInfo.cur.data.begin=task.begin
                    this.freeblockInfo.cur.data.length+=task.length
                    complete=true
                    console.log("REMOVED COND 2")
                }else{
                    //情况3：与前后空闲分区都可合并
                    mergedBlock.data.length+=this.freeblockInfo.cur.data.length
                    this.freeblockInfo.remove()
                    console.log("REMOVED COND 3")
                }
                complete=true
            }
            if(complete)
                return true;
            //情况4：都不能合并-创建新结点
            this.freeblockInfo.findLastSatisfy(
                function(llnode){
                    if(llnode.begin+llnode.length<=task.begin)
                        return true
                    return false;
                }
            )
            console.log("REMOVED COND 4")
            this.freeblockInfo.appendAfter({
                tag:'normal',
                begin:task.begin,
                length:task.length
            })
        },
        //空闲分区链处理结束
        firstfitSortCompare:function(x,y){
            return x.begin<y.begin;
        },
        ignoreHandler(){
            this.appendLog("忽略作业"+this.workList[this.workList_cur].jobId+
                "的"+(this.workList[this.workList_cur].reqType?"申请":"释放")+"请求-长度"+this.workList[this.workList_cur].reqSize+" KB");
            this.dynamicWorkList.splice(0,1);
            this.workList_cur++;
        },
        execAlgo:function(x){
            if(this.workList_cur==this.workList.length){
                return 0;
            }
            var thisLength=this.workList[this.workList_cur].reqSize;
            var i;
            this.appendLog("处理作业"+this.workList[this.workList_cur].jobId+
                "的"+(this.workList[this.workList_cur].reqType?"申请":"释放")+"请求-长度"+thisLength+" KB");
            //处理作业申请
            if(this.workList[this.workList_cur].reqType==1){

                let allocInfo = null
                if(x==0){
                    allocInfo=this.fbiInsert({length:this.workList[this.workList_cur].reqSize})
                }else{
                    allocInfo=this.fbiInsertBestFit({length:this.workList[this.workList_cur].reqSize})
                }
                if(allocInfo.stat){
                    this.dynamicWorkList.splice(0,1);
                    
                    let insertPlace=allocInfo.begin
                    let thisLength = this.workList[this.workList_cur].reqSize
                    this.memInfo.push({
                        begin:insertPlace,
                        end:insertPlace+thisLength-1,
                        jobId:this.workList[this.workList_cur].jobId,
                        orderId:0,
                        });
                    this.appendLog("作业"+this.workList[this.workList_cur].jobId+
                        "已经放入内存，起始地址:"+allocInfo.begin+" KB ,终止地址:"+(allocInfo.begin+this.workList[this.workList_cur].reqSize-1)+" KB");
                    this.workList_cur++;
                    this.memInfo.sort(this.firstfitSortCompare);
                    for(i=0;i<this.memInfo.length;i++){
                        this.memInfo[i].orderId=i;
                    }
                }else{
                    this.appendLog("作业"+this.workList[this.workList_cur].jobId+
                        "目前无法放入内存中，可用空间不足");
                    window.alert("作业"+this.workList[this.workList_cur].jobId+
                        "目前无法放入内存中，可用空间不足。点击跳过忽略该请求。"); 
                }
            //处理作业释放
            }else{ 
                var successFlag=false;
                let x = null
                for(i=0;i<this.memInfo.length;i++){
                    if(this.memInfo[i].jobId==this.workList[this.workList_cur].jobId){
                        x={
                            begin:this.memInfo[i].begin,
                            length:this.memInfo[i].end-this.memInfo[i].begin+1,
                        }
                        this.memInfo.splice(i,1);
                        this.dynamicWorkList.splice(0,1);
                        this.workList_cur++;
                        successFlag=true;
                        break;
                    }
                }
                if(!successFlag){
                    this.appendLog("作业"+this.workList[this.workList_cur].jobId+
                        "未在内存中，无法进行释放。");
                    window.alert("作业"+this.workList[this.workList_cur].jobId+
                        "未在内存中，无法进行释放。点击跳过忽略该请求。")
                }else{
                    this.fbiRelease(x)
                }
                for(i=0;i<this.memInfo.length;i++){
                    this.memInfo[i].orderId=i;
                }
            }
        }
    }
}
</script>

<!-- Add "scoped" attribute to limit CSS to this component only -->
<style scoped>
    .oshw2_dynamic_mem_alloc{
        text-align: center;
    }
    .oshw2_dynamic_mem_alloc_title{
        font-weight: bold;
        font-size:23px;
    }
    #mem_container{
        margin-bottom:70px;
    }
    .oshw2_dynamic_mem_alloc_frame{
        text-align: center;
    }
    .oshw2_dynamic_mem_alloc_fropt{
        border:#333333 1px solid;
        border-radius: 5px;
    }
    .oshw2_dynamic_mem_alloc_app{
        margin-top:30px;
    }
    .oshw2_dynamic_mem_alloc_appi{
        height:30px;
        margin-top:0px;
        margin-bottom: 10px;
        border:#333333 2px solid;
        border-radius: 2px;
        font-size:16px;
    }
    .oshw2_dynamic_mem_alloc_appic{
        width:30%;
        display: inline-block;
    }
</style>
