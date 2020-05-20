## 2PC-Uranus



![](cpp.svg) 

![](logo.png)





## Protocol

### 单参与者同步协议

同步单个参与者, 使用leaderData

```
CoServer to PaServer: SET ${KVDB_sync_one} "maxIndex_syncSize"
PaServer to CoServer: SET ${KVDB_sync_one} "OK"
loop: size = syncSize
     CoServer to PaServer: SET ${key} "${value}"
     PaServer to CoServer: SET SYNC_STATUS "1"
     
例子:
C to P: SET ${KVDB_sync_one} "5_3", 5代表当前日志的索引, 3代表数据库有3个数据需要更新
P to C: SET ${KVDB_sync_one} "OK", 代表正确接收 KVDB_sync_one 的请求
然后循环3次, 因为 syncSize = 3:
	其中一次的例子
    C to P: SET a "value", 将 a 设置成 "value" 值
    P to C: SET SYNC_STATUS "1", 表示同步成功
```



