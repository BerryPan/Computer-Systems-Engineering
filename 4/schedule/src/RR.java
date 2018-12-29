import java.util.LinkedList;
import java.util.List;
import java.util.Queue;

public class RR implements Algorithm{
    private List<Task> queue;
    public RR(List<Task> q){
        queue = q;
    }

    @Override
    public void schedule() {
        Queue<Task> RRQueue = new LinkedList<>();
        Integer splice = 10;
        System.out.println("RR Scheduling\n");
        while(!queue.isEmpty()){
            RRQueue.offer(pickNextTask());
            queue.remove(0);
        }
        while(!RRQueue.isEmpty()){
            Task tmp = RRQueue.poll();
            System.out.println("Will run Name: " + tmp.getName());
            System.out.println("Tid: " + tmp.getTid());
            System.out.println("Priority: " + tmp.getPriority());
            System.out.println("Burst: "+tmp.getBurst() + "\n");

            if(tmp.getBurst()>10){
                tmp.setBurst(tmp.getBurst()-splice);
                RRQueue.offer(tmp);
            }
            else{
                System.out.println("Task "+ tmp.getName() +" finished.\n");
            }

        }
    }

    @Override
    public Task pickNextTask(){
        return queue.get(0);
    }


}
