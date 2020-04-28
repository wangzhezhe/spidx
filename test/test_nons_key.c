
#include <spx_common.h>

void test_nskey_int64(){
    //attention the lifecycle of the key
    int64_t test_int64 = 123456789;
    spx_nonskey_entry *entry_str = (spx_nonskey_entry *)malloc(sizeof(spx_nonskey_entry));
    entry_str->type = SPX_ELEM_INT64;
    entry_str->value = (void *)test_int64;
    printf("nonskey %d\n",*(int64_t*)entry_str->value);
}

void test_nskey_str(){
    char *str = "teststr123";
    spx_nonskey_entry *entry_str = (spx_nonskey_entry *)malloc(sizeof(spx_nonskey_entry));
    entry_str->type = SPX_ELEM_STR;
    entry_str->value = (void *)str;
    printf("nonskey %s\n",entry_str->value);

}

int main(){
    test_nskey_str();
    test_nskey_int64();
}