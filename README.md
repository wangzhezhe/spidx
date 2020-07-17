### example for running test case

**for the 2d case**

client

```
$srun --mpi=pmix_v2 -n 1 ./test/test_client_2d tcp://192.168.40.45:41989
srun: job 37689809 queued and waiting for resources
srun: job 37689809 has been allocated resources
---test_register---
result of test_register 0
---test_update---
result of test_update 0
result of test_update 0
result of test_update 0
result of test_update 0
---test_query---
result of test_query 0
check the return resutls
id is 111
dims 2
m_lb
2,2,
m_ub
3,3,

id is 222
dims 2
m_lb
4,2,
m_ub
5,3,

id is 333
dims 2
m_lb
4,4,
m_ub
5,5,

id is 444
dims 2
m_lb
2,4,
m_ub
3,5,
```


server

```
$srun --mpi=pmix_v2 -n 1 ./test/test_server
srun: job 37689808 queued and waiting for resources
srun: job 37689808 has been allocated resources
test server
Server running at address ofi+tcp;ofi_rxm://192.168.40.45:41989
register following info into hash_domaindims 2
m_lb
0,0,
m_ub
7,7,
server get nons_key testvar
associated_id 111
bbx
dims 2
m_lb
0,0,
m_ub
3,3,
server get nons_key testvar
associated_id 222
bbx
dims 2
m_lb
4,0,
m_ub
7,3,
server get nons_key testvar
associated_id 333
bbx
dims 2
m_lb
4,4,
m_ub
7,7,
server get nons_key testvar
associated_id 444
bbx
dims 2
m_lb
0,4,
m_ub
3,7,
execute spx_query_ult
spatial info:
dim id: 0, lb: 2, ub 3
dim id: 1, lb: 2, ub 3
mem_id 111
spatial info:
dim id: 0, lb: 4, ub 5
dim id: 1, lb: 2, ub 3
mem_id 222
spatial info:
dim id: 0, lb: 4, ub 5
dim id: 1, lb: 4, ub 5
mem_id 333
spatial info:
dim id: 0, lb: 2, ub 3
dim id: 1, lb: 4, ub 5
mem_id 444
```

**for the 3d case**

client

```
zw241@amarel2:~/cworkspace/build/build_spidx$ srun ./test/test_client tcp://192.168.40.73:38763
srun: job 95900026 queued and waiting for resources
srun: job 95900026 has been allocated resources
---test_register---
result of test_register 0
//([0,0,0],[512,512,512],123) is registered
---test_update---
result of test_update 0
result of test_update 0
//([0,0,0],[32,32,32],123) is updated
//([64,64,64],[128,128,128],456) is updated
---test_query---
result of test_query 0
//query ([0,0,0],[128,128,128])
//return a list
check the return resutls
id is 123
dims 3
m_lb
0,0,0,
m_ub
32,32,32,

id is 456
dims 3
m_lb
64,64,64,
m_ub
128,128,128,
```

server

```
zw241@amarel2:~/cworkspace/build/build_spidx$srun ./test/test_server
srun: job 95900022 queued and waiting for resources
srun: job 95900022 has been allocated resources
test server
Server running at address ofi+tcp;ofi_rxm://192.168.40.73:38763
register following info into hash_domaindims 3
m_lb
0,0,0,
m_ub
512,512,512,
server get nons_key teststr123
associated_id 123
bbx
dims 3
m_lb
0,0,0,
m_ub
32,32,32,
server get nons_key teststr123
associated_id 456
bbx
dims 3
m_lb
64,64,64,
m_ub
128,128,128,
execute spx_query_ult
spatial info:
dim id: 0, lb: 0, ub 32
dim id: 1, lb: 0, ub 32
dim id: 2, lb: 0, ub 32
mem_id 123
spatial info:
dim id: 0, lb: 64, ub 128
dim id: 1, lb: 64, ub 128
dim id: 2, lb: 64, ub 128
mem_id 456
```


### TODO list

add multiple global domain

add multiple hashfunction (use the factory method similar to ch-placement)


### references

slides about the spidx

https://docs.google.com/presentation/d/15ClA-3RMb9_m6KnLag4C1D1YBT87TrPBLsebO_YwuZY/edit?usp=sharing
