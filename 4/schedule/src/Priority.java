import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;


public class Priority implements Algorithm {
    private List<Task> queue;
    public Priority(List<Task> q){
        queue = q;
    }
    @Override
    public void schedule(){
        Queue<ComparableBig> pQueue = new PriorityQueue<>();
        while(!queue.isEmpty()){
            Task tmp = pickNextTask();
            pQueue.add(new ComparableBig(tmp.getPriority(), tmp.getName(),tmp.getBurst()));
            queue.remove(0);
        }
        System.out.println("Priority Scheduling");

        while (pQueue.peek() != null) {
            ComparableBig tmp = pQueue.peek();
            System.out.println(tmp.getName()+"\t"+tmp.getId()+"\t"+tmp.getTime());
            pQueue.remove();
        }
    }
    @Override
    public Task pickNextTask(){
        return this.queue.get(0);
    }
}
