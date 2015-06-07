package org.ucsd.db.Main;

import java.io.IOException;
import java.sql.SQLException;
import java.text.ParseException;

import org.ucsd.db.bsmagen.TestUtil;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ1;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ1Aggregation;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ2;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ2Aggregation;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ3;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ3Aggregation;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ4;
import org.ucsd.db.bsmagen1M.DeltaIvm1MQ4Aggregation;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ1;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ1Aggregation;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ2;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ2Aggregation;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ3;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ3Aggregation;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ4;
import org.ucsd.db.bsmagen1M.FastJoinDeltaManager1MQ4Aggregation;
import org.ucsd.db.bsmagen1M.H21MQ1;
import org.ucsd.db.bsmagen1M.H21MQ1Aggregation;
import org.ucsd.db.bsmagen1M.H21MQ2;
import org.ucsd.db.bsmagen1M.H21MQ2Aggregation;
import org.ucsd.db.bsmagen1M.H21MQ3;
import org.ucsd.db.bsmagen1M.H21MQ3Aggregation;
import org.ucsd.db.bsmagen1M.H21MQ4;
import org.ucsd.db.bsmagen1M.H21MQ4Aggregation;
import org.ucsd.db.bsmagen1M.Postgres1MQ1;
import org.ucsd.db.bsmagen1M.Postgres1MQ1Aggregation;
import org.ucsd.db.bsmagen1M.Postgres1MQ2;
import org.ucsd.db.bsmagen1M.Postgres1MQ2Aggregation;
import org.ucsd.db.bsmagen1M.Postgres1MQ3;
import org.ucsd.db.bsmagen1M.Postgres1MQ3Aggregation;
import org.ucsd.db.bsmagen1M.Postgres1MQ4;
import org.ucsd.db.bsmagen1M.Postgres1MQ4Aggregation;
import org.ucsd.db.bsmagen1M.diffs.FastViewFanout;
import org.ucsd.db.bsmagen1M.diffs.GIvmJoinLength;

public class Main1 {
    public static void main(String[] args) throws InstantiationException, IllegalAccessException, ClassNotFoundException, ParseException, SQLException, InterruptedException, IOException {
        int op = Integer.parseInt(args[0]);
        if((op & 1) != 0)
            runFastJoinManager(Integer.parseInt(args[1]));
        if((op & 2) != 0)
            runDeltaManager(Integer.parseInt(args[1]));
        if((op & 4) != 0) {
            TestUtil.loadH2Database("./bsmagen1m");
            runH2(Integer.parseInt(args[1]));
            TestUtil.checkH2Database("./bsmagen1m");
        }
        if((op & 8) != 0)
            runPostgres(Integer.parseInt(args[1]));
        if((op & 16) != 0)
            runVariables();
    }

    private static void runVariables() throws ParseException, InstantiationException, IllegalAccessException, ClassNotFoundException, IOException, SQLException, InterruptedException {
        FastViewFanout fanout = new FastViewFanout();
        fanout.testFanoutSpace();
//        fanout.testFanoutPv();
//        gc();
//        fanout.testFanoutScan();
//        fanout.testFanout1();
//        GIvmJoinLength joinLength = new GIvmJoinLength();
//        joinLength.gIvmJoinLengthSpaceTest();
//        joinLength.gIvmJoinLengthTest();
//        joinLength.gIvmJoinLengthPvTest();
//        joinLength.gIvmJoinLengthScanTest();
//        GIvmJoinLengthRetweet retweet = new GIvmJoinLengthRetweet();
//        retweet.gIvmJoinLengthTest();
//        retweet.gIvmJoinLengthPvTest();
//        retweet.gIvmJoinLengthScanTest();
    }

    private static void runH2(int run) throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException, InterruptedException {
        if(run == 1) {
            H21MQ1 h2q1 = new H21MQ1();
            h2q1.insertProjectedTupleTest();
            h2q1.insertJoinTuples();
            h2q1.insertProjectedTupleThroughputTest();
            h2q1.insertJoinTuplesThroughput();
        }
        if(run == 2) {
            H21MQ2 h2q2 = new H21MQ2();
            h2q2.insertProjectedTupleTest();
            h2q2.insertJoinTuples();
            h2q2.insertProjectedTupleThroughputTest();
            h2q2.insertJoinTuplesThroughput();
        }
        if(run == 3) {
            H21MQ3 h2q3 = new H21MQ3();
            h2q3.insertProjectedTupleTest();
            h2q3.insertJoinTuples1To1();
            h2q3.insertProjectedTupleThroughputTest();
            h2q3.insertJoinTuplesThroughput1To1();
        }
        if(run == 4) {
            H21MQ4 h2q4 = new H21MQ4();
            h2q4.insertProjectedTupleTest();
//            h2q4.insertJoinTuplesThroughput1To1();
            h2q4.insertProjectedTupleThroughputTest();
//            h2q4.insertJoinTuples1To1();
        }
        if(run == 5) {
            H21MQ1Aggregation q = new H21MQ1Aggregation();
            q.insertProjectedTupleTest();
            q.insertJoinTuples();
            q.insertProjectedTupleThroughputTest();
            q.insertJoinTuplesThroughput();
        }
        if(run == 6) {
            H21MQ2Aggregation q = new H21MQ2Aggregation();
            q.insertProjectedTupleTest();
            q.insertJoinTuples();
            q.insertProjectedTupleThroughputTest();
            q.insertJoinTuplesThroughput();
        }
        if(run == 7) {
            H21MQ3Aggregation q = new H21MQ3Aggregation();
            q.insertProjectedTupleTest();
            q.insertJoinTuples1To1();
            q.insertProjectedTupleThroughputTest();
            q.insertJoinTuplesThroughput1To1();
        }
        if(run == 8) {
            H21MQ4Aggregation q = new H21MQ4Aggregation();
            //q.insertProjectedTupleTest();
            //q.insertJoinTuples1To1();
            q.insertProjectedTupleThroughputTest();
//            q.insertJoinTuplesThroughput1To1();
        }
        if(run == 9) {
            org.ucsd.db.bsmagen1M.mixedworkload.H21MQ1 h1 = new org.ucsd.db.bsmagen1M.mixedworkload.H21MQ1();
            h1.insertMixedWorkLoadWithoutViewMaintenance();
//            h1.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            h1.insertMixedWorkLoadWithViewMaintenance();
            h1.insertMixedWorkLoadWithViewMaintenanceThroughput();
            
//            org.ucsd.db.bsmagen1M.mixedworkload.H21MQ2 h2 = new org.ucsd.db.bsmagen1M.mixedworkload.H21MQ2();
//            h2.insertMixedWorkLoadWithoutViewMaintenance();
//            h2.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            h2.insertMixedWorkLoadWithViewMaintenance();
//            h2.insertMixedWorkLoadWithViewMaintenanceThroughput();
            
            org.ucsd.db.bsmagen1M.mixedworkload.H21MQ3 h3 = new org.ucsd.db.bsmagen1M.mixedworkload.H21MQ3();
//            h3.insertMixedWorkLoadWithoutViewMaintenance();
//            h3.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            h3.insertMixedWorkLoadWithViewMaintenance();
//            h3.insertMixedWorkLoadWithViewMaintenanceThroughput();
        }
        if(run == 10) {
            org.ucsd.db.bsmagen1M.mixedworkload.H21MQ1Aggregation h1 = new org.ucsd.db.bsmagen1M.mixedworkload.H21MQ1Aggregation();
//            h1.insertMixedWorkLoadWithoutViewMaintenance();
//            h1.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            h1.insertMixedWorkLoadWithViewMaintenance();
//            h1.insertMixedWorkLoadWithViewMaintenanceThroughput();
            System.out.println("Executing H21MQ2Aggregation");
            org.ucsd.db.bsmagen1M.mixedworkload.H21MQ2Aggregation h2 = new org.ucsd.db.bsmagen1M.mixedworkload.H21MQ2Aggregation();
            h2.insertMixedWorkLoadWithoutViewMaintenance();
//            h2.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
            h2.insertMixedWorkLoadWithViewMaintenance();
            h2.insertMixedWorkLoadWithViewMaintenanceThroughput();
            
            org.ucsd.db.bsmagen1M.mixedworkload.H21MQ3Aggregation h3 = new org.ucsd.db.bsmagen1M.mixedworkload.H21MQ3Aggregation();
//            h3.insertMixedWorkLoadWithoutViewMaintenance();
//            h3.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
            h3.insertMixedWorkLoadWithViewMaintenance();
            h3.insertMixedWorkLoadWithViewMaintenanceThroughput();
        }
    }
    
    private static void runPostgres(int run) throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException, InterruptedException, IOException {
        if(run == 1) {
            Postgres1MQ1 q1 = new Postgres1MQ1();
            q1.insertProjectedTupleTest();
            q1.insertJoinTuples();
            q1.insertProjectedTupleThroughputTest();
            q1.insertJoinTuplesThroughput();
        }
        if(run == 11) {
            Postgres1MQ1 q1 = new Postgres1MQ1();
            q1.deleteProjectedTupleTest();
            q1.deleteJoinTuples();
            q1.deleteProjectedTupleThroughputTest();
            q1.deleteJoinTuplesThroughput();
        }
        if(run == 2) {
            Postgres1MQ2 q2 = new Postgres1MQ2();
            q2.insertProjectedTupleTest();
            q2.insertJoinTuples();
            q2.insertProjectedTupleThroughputTest();
            q2.insertJoinTuplesThroughput();
        }
        if(run == 21) {
            Postgres1MQ2 q2 = new Postgres1MQ2();
            q2.deleteProjectedTupleTest();
            q2.deleteJoinTuples();
            q2.deleteProjectedTupleThroughputTest();
            q2.deleteJoinTuplesThroughput();
        }
        if(run == 3) {
            Postgres1MQ3 q3 = new Postgres1MQ3();
            q3.insertProjectedTupleTest();
            q3.insertJoinTuples1To1();
            q3.insertProjectedTupleThroughputTest();
            q3.insertJoinTuplesThroughput1To1();
        }
        if(run == 31) {
            Postgres1MQ3 q3 = new Postgres1MQ3();
            q3.deleteProjectedTupleTest();
            q3.deleteJoinTuples1To1();
            q3.deleteProjectedTupleThroughputTest();
            q3.deleteJoinTuplesThroughput1To1();
        }
        if(run == 4) {
            Postgres1MQ4 q4 = new Postgres1MQ4();
            q4.insertProjectedTupleTest();
//            q4.insertJoinTuples1To1();
            q4.insertProjectedTupleThroughputTest();
//            q4.insertJoinTuplesThroughput1To1();
        }
        if(run == 41) {
            Postgres1MQ4 q4 = new Postgres1MQ4();
            q4.deleteProjectedTupleTest();
//            q4.insertJoinTuples1To1();
            q4.deleteProjectedTupleThroughputTest();
//            q4.insertJoinTuplesThroughput1To1();
        }
        if(run == 5) {
            Postgres1MQ1Aggregation q4 = new Postgres1MQ1Aggregation();
            q4.insertProjectedTupleTest();
            q4.insertJoinTuples();
            q4.insertProjectedTupleThroughputTest();
            q4.insertJoinTuplesThroughput();
        }
        if(run == 51) {
            Postgres1MQ1Aggregation q4 = new Postgres1MQ1Aggregation();
            q4.deleteProjectedTupleTest();
            q4.deleteJoinTuples();
            q4.deleteProjectedTupleThroughputTest();
            q4.deleteJoinTuplesThroughput();
        }
        if(run == 6) {
            Postgres1MQ2Aggregation q4 = new Postgres1MQ2Aggregation();
            q4.insertProjectedTupleTest();
            q4.insertJoinTuples();
            q4.insertProjectedTupleThroughputTest();
            q4.insertJoinTuplesThroughput();
        }
        if(run == 61) {
            Postgres1MQ2Aggregation q4 = new Postgres1MQ2Aggregation();
            q4.deleteProjectedTupleTest();
            q4.deleteJoinTuples();
            q4.deleteProjectedTupleThroughputTest();
            q4.deleteJoinTuplesThroughput();
        }
        if(run == 7) {
            Postgres1MQ3Aggregation q4 = new Postgres1MQ3Aggregation();
            q4.insertProjectedTupleTest();
            q4.insertJoinTuples1To1();
            q4.insertProjectedTupleThroughputTest();
            q4.insertJoinTuplesThroughput1To1();
        }
        if(run == 71) {
            Postgres1MQ3Aggregation q4 = new Postgres1MQ3Aggregation();
//            q4.deleteProjectedTupleTest();
//            q4.deleteJoinTuples1To1();
            q4.deleteProjectedTupleThroughputTest();
//            q4.deleteJoinTuplesThroughput1To1();
        }
        if(run == 8) {
            Postgres1MQ4Aggregation q4 = new Postgres1MQ4Aggregation();
            q4.insertProjectedTupleTest();
//            q4.insertJoinTuples1To1();
            q4.insertProjectedTupleThroughputTest();
//            q4.insertJoinTuplesThroughput1To1();
        }
        if(run == 81) {
            Postgres1MQ4Aggregation q4 = new Postgres1MQ4Aggregation();
            q4.deleteProjectedTupleTest();
//            q4.insertJoinTuples1To1();
//            q4.deleteProjectedTupleThroughputTest();
//            q4.insertJoinTuplesThroughput1To1();
        }
        if(run == 9) {
//            org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ1 p1 = new org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ1();
//            p1.insertMixedWorkLoadWithoutViewMaintenance();
//            p1.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            p1.insertMixedWorkLoadWithViewMaintenance();
//            p1.insertMixedWorkLoadWithViewMaintenanceThroughput();
            
//            org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ2 p2 = new org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ2();
//            p2.insertMixedWorkLoadWithoutViewMaintenance();
//            p2.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            p2.insertMixedWorkLoadWithViewMaintenance();
//            p2.insertMixedWorkLoadWithViewMaintenanceThroughput();
            
            org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ3 p3 = new org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ3();
            p3.insertMixedWorkLoadWithoutViewMaintenance();
//            p3.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            p3.insertMixedWorkLoadWithViewMaintenance();
//            p3.insertMixedWorkLoadWithViewMaintenanceThroughput();
        }
        if(run == 10) {
//            org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ1Aggregation p1 = new org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ1Aggregation();
//            p1.insertMixedWorkLoadWithoutViewMaintenance();
//            p1.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            p1.insertMixedWorkLoadWithViewMaintenance();
//            p1.insertMixedWorkLoadWithViewMaintenanceThroughput();
            
            org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ2Aggregation p2 = new org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ2Aggregation();
            p2.insertMixedWorkLoadWithoutViewMaintenance();
            p2.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
            p2.insertMixedWorkLoadWithViewMaintenance();
            p2.insertMixedWorkLoadWithViewMaintenanceThroughput();
            
//            org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ3Aggregation p3 = new org.ucsd.db.bsmagen1M.mixedworkload.Postgres1MQ3Aggregation();
//            p3.insertMixedWorkLoadWithoutViewMaintenance();
//            p3.insertMixedWorkLoadWithoutViewMaintenanceThroughput();
//            p3.insertMixedWorkLoadWithViewMaintenance();
//            p3.insertMixedWorkLoadWithViewMaintenanceThroughput();
        }
    }
    private static void runDeltaManager(int run) throws InstantiationException, IllegalAccessException, ClassNotFoundException, SQLException, InterruptedException, ParseException {
        if(run == 1) {
            DeltaIvm1MQ1.setUp();
            DeltaIvm1MQ1 q1 = new DeltaIvm1MQ1();
            gc();
            q1.insertProjectedTuplesTest();
            gc();
            q1.insertJoinTuples();
            gc();
            q1.deleteProjectedTuplesTest();
            gc();
            q1.deleteJoinTuplesTest();
            gc();
            DeltaIvm1MQ1.destruct();
        }
        if(run == 2) {
            DeltaIvm1MQ2.setUp();
            DeltaIvm1MQ2 q2 = new DeltaIvm1MQ2();
            gc();
            q2.insertProjectedTuplesTest();
            gc();
            q2.insertJoinTuplesTest();
            gc();
            q2.deleteProjectedTuplesTest();
            gc();
            q2.deleteJoinTuplesTest();
            gc();
            DeltaIvm1MQ2.destruct();
        }
        if(run == 3) {
            DeltaIvm1MQ3.setUp();
            DeltaIvm1MQ3 q3 = new DeltaIvm1MQ3();
            gc();
            q3.insertProjectedTuplesTest();
            gc();
            q3.insertJoinTuples1To1Test();
            gc();
            q3.deleteProjectedTuplesTest();
            gc();
            q3.deleteJoinTuples1To1Test();
            gc();
            DeltaIvm1MQ3.destruct();
        }
        if(run == 4) {
            DeltaIvm1MQ4.setUp();
            DeltaIvm1MQ4 q4 = new DeltaIvm1MQ4();
            gc();
            q4.insertProjectedTuplesTest();
            gc();
            q4.insertJoinedTuple1To1Test();
            gc();
            q4.deleteProjectedTuplesTest();
            gc();
            q4.deleteJoinedTuple1To1Test();
            gc();
            DeltaIvm1MQ4.destruct();
        }
        if(run == 5) {
            DeltaIvm1MQ1Aggregation.setUp();
            DeltaIvm1MQ1Aggregation q1 = new DeltaIvm1MQ1Aggregation();
            gc();
            q1.insertProjectedTuplesTest();
            gc();
            q1.insertJoinTuples();
            gc();
            q1.deleteProjectedTuplesTest();
            gc();
            q1.deleteJoinTuplesTest();
            gc();
            DeltaIvm1MQ1.destruct();
        }
        if(run == 6) {
            DeltaIvm1MQ2Aggregation.setUp();
            DeltaIvm1MQ2Aggregation q1 = new DeltaIvm1MQ2Aggregation();
            gc();
            q1.insertProjectedTuplesTest();
            gc();
            q1.insertJoinTuplesTest();
            gc();
            q1.deleteProjectedTuplesTest();
            gc();
            q1.deleteJoinTuplesTest();
            gc();
            DeltaIvm1MQ1.destruct();
        }
        if(run == 7) {
            DeltaIvm1MQ3Aggregation.setUp();
            DeltaIvm1MQ3Aggregation q1 = new DeltaIvm1MQ3Aggregation();
            gc();
            q1.insertProjectedTuplesTest();
            gc();
            q1.insertJoinTuples1To1Test();
            gc();
            q1.deleteProjectedTuplesTest();
            gc();
            q1.deleteJoinTuples1To1Test();
            gc();
            DeltaIvm1MQ1.destruct();
        }
        if(run == 8) {
            DeltaIvm1MQ4Aggregation.setUp();
            DeltaIvm1MQ4Aggregation q1 = new DeltaIvm1MQ4Aggregation();
            gc();
            q1.insertProjectedTuplesTest();
            gc();
            q1.insertJoinedTuple1To1Test();
            gc();
            q1.deleteProjectedTuplesTest();
            gc();
            q1.deleteJoinedTuple1To1Test();
            gc();
            DeltaIvm1MQ1.destruct();
        }
        if(run == 9) {
            org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ1 q1 = new org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ1();
            q1.setUp();
            q1.insertMixedWorkLoadWithoutViewMaintenance();
            q1.insertMixedWorkLoadWithViewMaintenance();
            q1.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ2 q2 = new org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ2();
            q2.setUp();
            //q2.insertMixedWorkLoadWithoutViewMaintenance();
            q2.insertMixedWorkLoadWithViewMaintenance();
            q2.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ3 q3 = new org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ3();
            q3.setUp();
            q3.insertMixedWorkLoadWithoutViewMaintenance();
            q3.insertMixedWorkLoadWithViewMaintenance();
            q3.destruct();
        }
        if(run == 10) {
            org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ1Aggregation q1 
                = new org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ1Aggregation();
            q1.setUp();
            //q1.insertMixedWorkLoadWithoutViewMaintenance();
            q1.insertMixedWorkLoadWithViewMaintenance();
            q1.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ2Aggregation q2 
                = new org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ2Aggregation();
            q2.setUp();
            //q2.insertMixedWorkLoadWithoutViewMaintenance();
            q2.insertMixedWorkLoadWithViewMaintenance();
            q2.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ3Aggregation q3 
                = new org.ucsd.db.bsmagen1M.mixedworkload.DeltaIvm1MQ3Aggregation();
            q3.setUp();
            //q3.insertMixedWorkLoadWithoutViewMaintenance();
            q3.insertMixedWorkLoadWithViewMaintenance();
            q3.destruct();
        }
    }
    private static void runFastJoinManager(int test) throws InstantiationException, IllegalAccessException, ClassNotFoundException, ParseException, InterruptedException, IOException, SQLException {
        if(test == 1) {
            FastJoinDeltaManager1MQ1.setUp();
            FastJoinDeltaManager1MQ1 q1 = new FastJoinDeltaManager1MQ1();
            gc();
            q1.insertProjectedTupleTest();
            gc();
            q1.insertJoinedTupleTest();
            gc();
            q1.deleteProjectedTupleTest();
            gc();
            q1.deleteJoinedTupleTest();
            FastJoinDeltaManager1MQ1.destruct();
            gc();
        }
        if(test == 2) {
            FastJoinDeltaManager1MQ2.setUp();
            FastJoinDeltaManager1MQ2 q2 = new FastJoinDeltaManager1MQ2();
            gc();
            q2.insertProjectedTupleTest();
            gc();
            q2.insertJoinedTupleTest();
            gc();
            q2.deleteProjectedTupleTest();
            gc();
            q2.deleteJoinedTupleTest();
            FastJoinDeltaManager1MQ2.destruct();
            gc();
        }
        if(test == 3) {
            FastJoinDeltaManager1MQ3.setUp();
            FastJoinDeltaManager1MQ3 q3 = new FastJoinDeltaManager1MQ3();
            gc();
            q3.insertProjectedTupleTest();
            gc();
            q3.insertJoinedTuple1ToMTest();
            gc();
            q3.deleteProjectedTupleTest();
            gc();
            q3.deleteJoinedTuple1ToMTest();
            FastJoinDeltaManager1MQ3.destruct();
            gc();
        }
        if(test == 4) {
            FastJoinDeltaManager1MQ4.setUp();
            FastJoinDeltaManager1MQ4 q4 = new FastJoinDeltaManager1MQ4();
            gc();
            q4.insertProjectedTupleTest();
            gc();
//            q4.insertJoinedTuple1To1Test();
            gc();
            q4.deleteProjectedTupleTest();
            gc();
//            q4.deleteJoinedTuple1To1Test();
            FastJoinDeltaManager1MQ4.destruct();
            gc();
        }
        if(test == 5) {
            FastJoinDeltaManager1MQ1Aggregation.setUp();
            FastJoinDeltaManager1MQ1Aggregation q1 = new FastJoinDeltaManager1MQ1Aggregation();
            gc();
            q1.insertProjectedTupleTest();
            gc();
            q1.insertJoinedTupleTest();
            gc();
            q1.deleteProjectedTupleTest();
            gc();
            q1.deleteJoinedTupleTest();
            FastJoinDeltaManager1MQ1.destruct();
            gc();
        }
        if(test == 6) {
            FastJoinDeltaManager1MQ2Aggregation.setUp();
            FastJoinDeltaManager1MQ2Aggregation q1 = new FastJoinDeltaManager1MQ2Aggregation();
            gc();
            q1.insertProjectedTupleTest();
            gc();
            q1.insertJoinedTupleTest();
            gc();
            q1.deleteProjectedTupleTest();
            gc();
            q1.deleteJoinedTupleTest();
            FastJoinDeltaManager1MQ1.destruct();
            gc();
        }
        if(test == 7) {
            FastJoinDeltaManager1MQ3Aggregation.setUp();
            FastJoinDeltaManager1MQ3Aggregation q1 = new FastJoinDeltaManager1MQ3Aggregation();
            gc();
            q1.insertProjectedAggregatedTupleTest();
            gc();
            q1.insertJoinedTuple1To1Test();
            gc();
            q1.deleteProjectedAggregatedTupleTest();
            gc();
            q1.deleteJoinedTuple1To1Test();
            gc();
            FastJoinDeltaManager1MQ1.destruct();
            gc();
        }
        if(test == 8) {
            FastJoinDeltaManager1MQ4Aggregation.setUp();
            FastJoinDeltaManager1MQ4Aggregation q1 = new FastJoinDeltaManager1MQ4Aggregation();
            gc();
            q1.insertProjectedTupleTest();
            gc();
            //q1.insertJoinedTuple1To1Test();
            gc();
            q1.deleteProjectedTupleTest();
            gc();
            //q1.deleteJoinedTuple1To1Test();
            FastJoinDeltaManager1MQ1.destruct();
            gc();
        }
        if(test == 9) {
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1 
                f1 = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1();
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1.setUp();
            f1.insertMixedWorkLoadPdPv();
            f1.insertMixedWorkLoadPv();
            f1.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ2 
                f2 = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ2();
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ2.setUp();
            f2.insertMixedWorkLoadPdPv();
            f2.insertMixedWorkLoadPv();
            f2.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3 
                f3 = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3();
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3.setUp();
            f3.insertMixedWorkLoadPdPv();
            f3.insertMixedWorkLoadPv();
            f3.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3_200K 
            f3_200K = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3_200K();
            f3_200K.insertMixedWorkLoadPdPv();
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1 f = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1();
//            f.setUp();
//            f.insertMixedWorkLoadPdPvFile();
        }
        if(test == 10) {
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1Aggregation 
                fagg1 = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1Aggregation();
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1Aggregation.setUp();
            fagg1.insertMixedWorkLoadPdPv();
            fagg1.insertMixedWorkLoadPv();
            fagg1.destruct();
            
//            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1Aggregation f = 
//                            new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ1Aggregation();
//            f.setUp();
//            f.insertMixedWorkLoadPdPvFile();
            
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ2Aggregation 
                fagg2 = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ2Aggregation();
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ2Aggregation.setUp();
            fagg2.insertMixedWorkLoadPdPv();
            fagg2.insertMixedWorkLoadPv();
            fagg2.destruct();
            
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3Aggregation 
                fagg3 = new org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3Aggregation();
            org.ucsd.db.bsmagen1M.mixedworkload.FastJoinDeltaManager1MQ3Aggregation.setUp();
            fagg3.insertMixedWorkLoadPdPv();
            fagg3.insertMixedWorkLoadPv();
            fagg3.destruct();
        }
    }

    private static void gc() throws InterruptedException {
        System.gc();
        Thread.sleep(1000);        
    }
}
