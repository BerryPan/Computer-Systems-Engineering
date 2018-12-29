# Banker’s Algorithm

## Question

For this project, you will write a program that implements the banker’s algorithmdiscussed in Section 8.6.3. Customers request and release resources from the bank. The banker will grant a request only if it leaves the system in a safe state. A request that leaves the system in an unsafe state will be denied. Although the code examples that describe this project are illustrated in C, you may also develop a solution using Java.

## Answer

设Request i是进程Pi的申请向量，如果Request i[j]=K,则表示进程Pi需要K个Rj类型的资源。当Pi发出资源请求后，系统按下述步骤进行检查：

1) 如果Request i[j]<=Need[i,j]，便转向步骤2）；否则认为出错，因为它所需要的资源数已经超过它所宣布的最大值。

2) 如果Request i[j]<=Available[i,j]，便转向步骤3）；否则，表示尚无足够资源，Pi需等待。

3) 系统试探着把资源分配给进程Pi，并修改下面数据结构中的数值：

​       Available[j]:=Available[j]-Request i[j];

​       Allocation[i,j]:=Allocation[i,j]+Request i[j];

​       Need[i,j]:=Need[i,j]-Request i[j];

4) 系统执行安全性算法，检查此次资源分配后系统是否处于安全状态。若安全，才正式将资源分配给进程Pi，以完成本次分配；否则，将本次的试探分配作废，恢复原来的资源分配状态，让进程Pi等待。

代码如下：

```c++
int request_resources(int customer_num, int request[]) {
	int flag = 1;
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i)
		if (request[i] > need[customer_num][i] || request[i] > available[i])
			flag = 0;
	if(flag)
		if (!safe(customer_num, request)) {
			flag = 0;
		}
	return flag;
}
```

系统所执行的安全性算法可描述如下：

1) 设置两个向量

①　工作向量Work，它表示系统可提供给进程继续运行所需的各类资源数目，它含有m个元素，在执行安全算法开始时，Work：=Available。

②　Finish，它表示系统是否有足够的资源分配给进程，使之运行完成。开始时先做Finish[i]:=false;当有足够资源分配给进程时，再令Finish[i]:=ture.

2) 从进程集合中找到一个满足下述条件的进程：

①　Finish[i]=false;

②　Need[i,j]<=Work[j];若找不到，执行步骤3），否则，执行步骤4）。

3） 当进程Pi获得资源后，可顺利执行，直至完成，并释放出分配给它的资源，故应执行：

​     Work[j]:=Work[j]+Allocation[i,j];

​     Finish[i]:=true;

​     Go to step 2;

4) 如果所有进程的Finish[i]=true都满足，则表示系统处于安全状态；否则，系统处于不安全状态。

代码如下：

```c++
bool safe(int customer_num, int request[]) {
	int work[NUMBER_OF_RESOURCES];
	int finish[NUMBER_OF_RESOURCES];
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
		work[i] = available[i];
		finish[i] = false;
	}
	for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
		if (!finish[i] && need[customer_num][i] <= work[i]) {
			work[i] += allocation[customer_num][i];
			finish[i] = true;
		}
		else {
			for (int i = 0; i < NUMBER_OF_RESOURCES; ++i) {
				if (!finish[i])
					return false;
				else
					return true;
			}
		}
	}
}
```

## 运行结果

按照课本中给出的要求，运行结果如下：

输入*输出当前状态

<img src="C:\Users\lenovo\Desktop\3.png" style="zoom:80%" />

输入RQ 0 1 2 3 1输出是否request成功，然后*返回当前状态

<img src="C:\Users\lenovo\Desktop\4.png" style="zoom:80%" />

输入RL 0 1 1 1 1输出是否release成功，然后*返回当前状态

<img src="C:\Users\lenovo\Desktop\5.png" style="zoom:80%" />