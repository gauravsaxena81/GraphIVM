package ddbt.gen;

import java.text.DateFormat;
import java.text.ParseException;
import java.text.SimpleDateFormat;
import java.util.concurrent.TimeUnit;

import scala.Tuple2;
import scala.collection.immutable.$colon$colon;
import scala.collection.immutable.List;
import scala.collection.immutable.List$;
import scala.concurrent.duration.Duration;
import scala.concurrent.duration.FiniteDuration;
import akka.actor.ActorRef;
import akka.actor.ActorSystem;
import akka.actor.Inbox;
import akka.actor.Props;
import ddbt.gen.Dbtoaster;
import ddbt.lib.Messages.EndOfStream$;
import ddbt.lib.Messages.TupleEvent;

public class Tpchq1 {
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

    public static void main(String[] args) throws ParseException {
       ActorSystem system = ActorSystem.create("mySystem");
       final Inbox inbox = Inbox.create(system);
       DateFormat df = new SimpleDateFormat("yyyy-MM-DD");
       final ActorRef q = system.actorOf(Props.create(Dbtoaster.class), "Query");

       // Send events
       Tuple2 result;
       System.out.println("Insert a tuple into lineitem.");
       q.tell(new TupleEvent(0, TUPLE_INSERT, "lineitem", tuple(582,17,9,4,36,38453.76,0.06,0.01,"NO"
           ,df.parse("1997-12-09"),df.parse("1997-11-27"),df.parse("1997-12-26"),"TAKE BACK RETURN","SHIP","lar requests. quickly")), null);

       // Retrieve result
       inbox.send(q, EndOfStream$.MODULE$);
       result = (Tuple2) (inbox.receive(DEFAULT_TIMEOUT));
       System.out.println("Final Result: " + result._2().toString());
       
       system.shutdown();
   }
}
