import {LinkListNode} from './linklist_node'
//双向链表
export class BidirectionalLinkedList{
    constructor(){
        this.size = 0
        this.head = new LinkListNode({tag:'head'})
        this.cur = null
    }
    //迭代执行
    iterativeScan(func){
        this.cur=this.head
        while(this.cur!=null){
            func(this.cur.data)
            this.cur=this.cur.next
        }
    }
    //定位结点
    find(condFunc){
        this.cur=this.head
        while(this.cur!=null){
            if(condFunc(this.cur.data)){
                break
            }
            this.cur=this.cur.next
        }
        
    }
    findLastSatisfy(condFunc){
        let x= null
        this.cur=this.head.next
        while(this.cur!=null){
            if(condFunc(this.cur.data)){
                x=this.cur
            }
            this.cur=this.cur.next
        }
        this.cur=x
        if(x===null)
            this.cur= this.head
    }
    findLast(){
        this.cur=this.head
        while(this.cur.next!=null){
            this.cur=this.cur.next
        }
    }
    //统计最值
    findMax(condFunc){
        let ret=0
        this.cur=this.head
        while(this.cur!=null){
            ret=Math.max(condFunc(this.cur),ret)
            this.cur=this.cur.next
        }
        return ret
    }
    //最佳适应算法查找
    bestFitFind(minValue,condFunc){
        let x=null
        let curMin=9999999
        this.cur=this.head.next
        while(this.cur!=null){
            if(condFunc(this.cur.data)>=minValue&&condFunc(this.cur.data)<curMin){
                x=this.cur
                curMin=condFunc(this.cur.data)
            }
            this.cur=this.cur.next
        }
        this.cur=x
    }
    //删除节点
    remove(){
        if(this.cur===null){
            //throw "[BiLinkList] Cannot remove a node of null"
        }else if(this.cur.data.tag==='head'){
            //throw "[BiLinkList] Cannot remove the head node"
        }else{
            if(this.cur.prev!=null){
                this.cur.prev.next = this.cur.next
            }
            if(this.cur.next!=null){
                this.cur.next.prev = this.cur.prev
            }
        }
    }
    //在节点后追加
    appendAfter(data){
        if(this.cur===null){
            //throw "[BiLinkList] Cannot remove a node of null"
        }else{
            let temp=this.cur.next
            this.cur.next = new LinkListNode(data)
            this.cur.next.prev=this.cur
            this.cur.next.next = temp
            if(temp!=null){
                this.cur.next.next.prev=this.cur.next
            }
            console.log("INSERTED NODE"+this.cur.data.tag)
            console.log(data)
            console.log(this.cur.next.data)
        }
    }
    //修改数据
    alter(data){
        this.cur.data=data
    }
}