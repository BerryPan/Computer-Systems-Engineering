import java.util.LinkedList;
import java.util.List;
import java.util.PriorityQueue;
import java.util.Queue;


public class PriorityRR implements Algorithm {
    private List<Task> queue;
    public PriorityRR(List<Task> q){
        queue = q;
    }

    @Override
    public void schedule() {
        Queue<ComparableBig> pQueue = new PriorityQueue<>();
        while(!queue.isEmpty()){
            Task tmp = pickNextTask();
            pQueue.add(new ComparableBig(tmp.getPriority(), tmp.getName(),tmp.getBurst()));
            queue.remove(0);
        }
        System.out.println("Priority with RR Scheduling\n");

        while (pQueue.peek() != null) {
            ComparableBig tmp = pQueue.poll();
            if(pQueue.peek() != null && pQueue.peek().getId() == tmp.getId()){
                Queue<Task> RRQueue = new LinkedList<>();
                Integer splice = 10;
                RRQueue.offer(new Task(tmp.getName(),tmp.getId(),tmp.getTime()));
                while (pQueue.peek() != null && pQueue.peek().getId() == tmp.getId()){
                    tmp = pQueue.poll();
                    RRQueue.offer(new Task(tmp.getName(),tmp.getId(), tmp.getTime()));
                }
                while(!RRQueue.isEmpty()){
                    Task Tmp = RRQueue.poll();
                    System.out.println("Will run Name: " + Tmp.getName());
                    System.out.println("Priority: " + Tmp.getPriority());
                    System.out.println("Burst: "+Tmp.getBurst() + "\n");

                    if(Tmp.getBurst()>10){
                        Tmp.setBurst(Tmp.getBurst()-splice);
                        RRQueue.offer(Tmp);
                    }
                    else{
                        System.out.println("Task "+ Tmp.getName() +" finished.\n");
                    }
                }
            }
            else {
                System.out.println("Will run Name: " + tmp.getName());
                System.out.println("Priority: " + tmp.getId());
                System.out.println("Burst: "+tmp.getTime() + "\n");
                System.out.println("Task " + tmp.getName() + " finished\n");
            }

        }

    }

    @Override
    public Task pickNextTask(){
        return queue.get(0);
    }
}
