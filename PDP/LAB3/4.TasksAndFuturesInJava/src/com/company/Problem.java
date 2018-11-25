package com.company;

import java.util.Vector;
import java.util.concurrent.*;

public class Problem {

    public static final int N = 30;
    public static final int NRThreads = 4;
    public static final int MAXNR = 1000;


    public Vector<Vector< Integer >  > a;
    public Vector< Vector< Integer > > b;
    public Vector< Vector< Integer > > c;


    public void doAddition(int pos) {
        int totalRepo = N * N;

        for(int crt = pos; crt < totalRepo; crt += NRThreads) {
            int i = crt / N ;
            int j = crt % N ;
            c.get(i).set(j, a.get(i).get(j) + b.get(i).get(j));
        }
    }

    void doMultiplication(int pos) {
        int totalRepo = N * N;

        for(int crt = pos; crt < totalRepo; crt += NRThreads) {
            int i = crt / N ;
            int j = crt % N ;

            for(int h = 0; h < N; h++) {
                c.get(i).set(j, c.get(i).get(j) + a.get(i).get(h) * b.get(h).get(j));
            }
        }
    }

    public Problem() {
        a = new Vector< Vector<Integer> >();
        b = new Vector< Vector<Integer> >();
        c = new Vector< Vector<Integer> >();

        for(int i = 0; i < N; i++) {
            a.add(new Vector<Integer>(N));
            b.add(new Vector<Integer>(N));
            c.add(new Vector<Integer>(N));

            for(int j = 0; j < N; j++) {
                a.get(i).add(101);
                b.get(i).add(201);
                c.get(i).add(0);
            }
        }

        ExecutorService executorService = Executors.newFixedThreadPool(NRThreads);
        Vector<FutureTask<String>> futures = new Vector<>();

        for(int i = 0 ; i < NRThreads; i++) {
            int finalI1 = i;
            futures.add( new FutureTask<String>(new Callable<String>() {
                @Override
                public String call() throws Exception {
                    doAddition(finalI1);
                    return "";
                }
            }) );
            futures.get(i).run();
        }

        for(int i = 0; i < NRThreads; i++) {
            try {
                futures.get(i).get();
            } catch (InterruptedException e) {
                e.printStackTrace();
            } catch (ExecutionException e) {
                e.printStackTrace();
            }
        }

        executorService.shutdown();
    }


}
