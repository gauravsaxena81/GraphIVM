package ddbt.gen;

import ddbt.gen.*;
import ddbt.lib.Messages.*;
import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Props;
import akka.actor.Inbox;
import akka.pattern.Patterns;
import akka.util.Timeout;
import scala.PartialFunction;
import scala.Tuple2;
import scala.collection.immutable.List;
import scala.collection.immutable.List$;
import scala.collection.immutable.$colon$colon;
import scala.concurrent.Await;
import scala.concurrent.Future;
import scala.concurrent.duration.Duration;
import scala.concurrent.duration.FiniteDuration;

import java.util.concurrent.TimeUnit;

public class RstLoadTest {
    public static final int TUPLE_DELETE = 0x00;
    public static final int TUPLE_INSERT = 0x01;
    private static final FiniteDuration DEFAULT_TIMEOUT = Duration.create(1L << 23, TimeUnit.SECONDS);
    private static final List EMPTY_LIST = List$.MODULE$.empty();
    public static List<Object> tuple(Object ... ts) {
       List<Object> result = EMPTY_LIST;
       for(int i = ts.length; i > 0; i--) {
          result = new $colon$colon<Object>(ts[i - 1], result);
       }
       return result;
    }

    public static void main(String[] args) throws Exception {
       ActorSystem system = ActorSystem.create("mySystem");
       final Inbox inbox = Inbox.create(system);

       final ActorRef q = system.actorOf(Props.create(Dbtoaster.class), "Query");

       // Send events
       Tuple2 result;
       
       final int MAX = 50000;
       int t = 0;
       long X = 0;
       final Timeout timeout = new Timeout(DEFAULT_TIMEOUT);
       for(int i = 0; i < MAX; i++) {
           q.tell(new TupleEvent(0, TUPLE_INSERT, "R", tuple(1L, X + i)), null);
           q.tell(new TupleEvent(1, TUPLE_INSERT, "S", tuple(X + i, MAX + X + i)), null);
       }
       long tm = System.nanoTime();
       for(int i = 0; i < MAX; i++) {
           q.tell(new TupleEvent(2, TUPLE_INSERT, "T", tuple(MAX + X + i, 1L)), null);
           inbox.send(q, new GetSnapshot(new $colon$colon(1, EMPTY_LIST)));
           result = (Tuple2) (inbox.receive(DEFAULT_TIMEOUT));
           //System.out.println("Result after step: " + i + ":" + result._2().toString());
       }
       System.out.println((System.nanoTime() - tm) / 1000000);

       // Retrieve result
       inbox.send(q, EndOfStream$.MODULE$);
       result = (Tuple2) (inbox.receive(DEFAULT_TIMEOUT));
       System.out.println("Final Result: " + result._2().toString());
       
       system.shutdown();
   }
}
