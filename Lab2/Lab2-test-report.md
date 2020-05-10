<!--
 * @Author: Firefly
 * @Date: 2020-04-29 09:52:40
 * @Descripttion: 
 * @LastEditTime: 2020-04-29 09:54:59
 -->

# 测试使用 apche bench

# 测试环境为

wsl 4核

This is ApacheBench, Version 2.3 <$Revision: 1874286 $>
Copyright 1996 Adam Twiss, Zeus Technology Ltd, http://www.zeustech.net/
Licensed to The Apache Software Foundation, http://www.apache.org/

Benchmarking 127.0.0.1 (be patient).....done


Server Software:
Server Hostname:        127.0.0.1
Server Port:            8888

Document Path:          /index.html
Document Length:        342 bytes

Concurrency Level:      10
Time taken for tests:   31.930 seconds
Complete requests:      10
Failed requests:        3
   (Connect: 3, Receive: 0, Length: 0, Exceptions: 0)
Total transferred:      4070 bytes
HTML transferred:       3420 bytes
Requests per second:    0.31 [#/sec] (mean)
Time per request:       31929.562 [ms] (mean)
Time per request:       3192.956 [ms] (mean, across all concurrent requests)
Transfer rate:          0.12 [Kbytes/sec] received

Connection Times (ms)
              min  mean[+/-sd] median   max
Connect:        0  150 475.0      0    1502
Processing: 24379 24383  10.0  24381   24412
Waiting:       31  333 191.6    348     628
Total:      24379 24534 484.9  24381   25914

Percentage of the requests served within a certain time (ms)
  50%  24381
  66%  24381
  75%  24382
  80%  24382
  90%  25914
  95%  25914
  98%  25914
  99%  25914
 100%  25914 (longest request)