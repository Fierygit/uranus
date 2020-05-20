## 2PC-Uranus



![](assets/cpp.svg) ![](assets/support.svg)

A two-stage commit project implemented in C + +. A simple key value distributed database is implemented



![](assets/logo.png)





## Protocol



### 同步参与者

> void syncKVDB();

```
同步数据库
先调用 getLatestIndex 查看是否有数据库落下
如果没有:
     退出
否则:
     调用 getLeaderData 获得可以作为 主参与者 的数据
     对每个 需要sync的参与者:
         调用 syncOnePart
```





### 获取最近索引

> void getLatestIndex(Participant *p);

```
获取一个 p 最新的 操作(日志)索引
C to P: GET "${LatestIndex}"
P to C: SET ${LatestIndex} ${value}  // ${value} 代表p的 latestIndex
```



### 获取leader的数据

> std::vector<std::string> getLeaderData(Participant* p);

注意: 这里的leader指的是参与者中数据索引最高的一个, 将它的数据拷贝到其他需要同步的参与者中

```
协议: 获取leader的全部数据, 用来同步那些落后的参与者
C to P: GET "${KVDB}"
P to C: SET ${KVDB_cnt} "${KVDB.size()}"
loop: size = KVDB.size():
     C to P: GET "${KVDB_next}"
     P to C: Encoder(SET item.first "item.second")  // item is a (key, value) in KVDB

save Data to std::vector<std::string> leaderData;

```



### 单参与者同步协议

> void syncOnePart(Participant *p, const std::vector<std::string>& leaderData, int maxIndex);

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



