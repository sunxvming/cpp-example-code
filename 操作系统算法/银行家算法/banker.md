### 初始化

由用户输入数据，分别对可利用资源向量[矩阵](https://baike.baidu.com/item/%E7%9F%A9%E9%98%B5)AVAILABLE、最大需求矩阵MAX、分配矩阵ALLOCATION、需求矩阵NEED赋值。

### 银行家算法银行家算法

在避免[死锁](https://baike.baidu.com/item/%E6%AD%BB%E9%94%81)的方法中，所施加的限制条件较弱，有可能获得令人满意的系统性能。在该方法中把系统的状态分为安全状态和不安全状态，只要能使系统始终都处于安全状态，便可以避免发生[死锁](https://baike.baidu.com/item/%E6%AD%BB%E9%94%81)。

银行家算法的基本思想是分配资源之前，判断系统是否是安全的；若是，才分配。它是最具有代表性的避免[死锁](https://baike.baidu.com/item/%E6%AD%BB%E9%94%81)的算法。

```
设进程cusneed提出请求REQUEST [i]，则银行家算法按如下规则进行判断。

(1)如果REQUEST [cusneed] [i]<= NEED[cusneed][i]，则转(2)；否则，出错。

(2)如果REQUEST [cusneed] [i]<= AVAILABLE[i]，则转(3)；否则，等待。

(3)系统试探分配资源，修改相关数据：

AVAILABLE[i]-=REQUEST[cusneed][i];

ALLOCATION[cusneed][i]+=REQUEST[cusneed][i];

NEED[cusneed][i]-=REQUEST[cusneed][i];

(4)系统执行安全性检查，如安全，则分配成立；否则试探险性分配作废，系统恢复原状，进程等待。
```
### 银行家算法安全性检查算法
```
(1)设置两个工作向量Work=AVAILABLE;FINISH

(2)从进程集合中找到一个满足下述条件的进程，

FINISH==false;

NEED<=Work;

如找到，执行（3)；否则，执行（4)

(3)设进程获得资源，可顺利执行，直至完成，从而释放资源。

Work=Work+ALLOCATION;

Finish=true;

GOTO 2

(4)如所有的进程Finish= true，则表示安全；否则系统不安全。
```


## 参考链接
- [百度百科-银行家算法](https://baike.baidu.com/item/%E9%93%B6%E8%A1%8C%E5%AE%B6%E7%AE%97%E6%B3%95/1679781?fr=aladdin)
