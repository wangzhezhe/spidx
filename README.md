### example for running test case

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


client

```
zw241@amarel2:~/cworkspace/build/build_spidx$ srun ./test/test_client tcp://192.168.40.73:38763
srun: job 95900026 queued and waiting for resources
srun: job 95900026 has been allocated resources
---test_register---
result of test_register 0
---test_update---
result of test_update 0
result of test_update 0
---test_query---
current id 1
current id 2
result of test_query 0
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