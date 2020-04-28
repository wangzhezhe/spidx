#include "spx_common.h"
#include "stdio.h"
#include "string.h"

void test_nons_key_encode()
{
    fprintf(stdout, "--test_nons_key_encode--\n");

    //generate the entry for int
    int64_t id = 123;
    spx_nonskey_entry *entry_int = (spx_nonskey_entry *)malloc(sizeof(spx_nonskey_entry));
    entry_int->type = SPX_ELEM_INT64;
    entry_int->value = (void *)&id;

    char encoded_buff[BUFLEN];
    enocde_nonspatial_key(entry_int, encoded_buff);
    fprintf(stdout, "test encode int (%s)\n", encoded_buff);

    //generate the entry for str
    char *str = "teststr123";
    spx_nonskey_entry *entry_str = (spx_nonskey_entry *)malloc(sizeof(spx_nonskey_entry));
    entry_str->type = SPX_ELEM_STR;
    entry_str->value = (void *)str;

    enocde_nonspatial_key(entry_str, encoded_buff);
    fprintf(stdout, "test encode str (%s)\n", encoded_buff);

    free(entry_int);
    free(entry_str);

    return;
}

void test_spatial_key_encode()
{
    fprintf(stdout, "--test_spatial_key_encode--\n");

    //generate the bbx_t
    bbx_t *bbx = (bbx_t *)malloc(sizeof(bbx_t));
    bbx->m_dims = 3;

    int i = 0;
    for (i = 0; i < 3; i++)
    {
        bbx->m_lb[i] = 0;
        bbx->m_ub[i] = 5;
    }

    char encoded_buff[BUFLEN];
    encode_spatial_key(bbx, encoded_buff);
    printf("len of the spatial key (%d)\n", strlen(encoded_buff));

    //decode the data back
    bbx_t *decoded_bbx = (bbx_t *)malloc(sizeof(bbx_t));
    decoded_bbx = (bbx_t *)encoded_buff;

    //check the results
    printbbx(decoded_bbx);

    if (decoded_bbx->m_dims != bbx->m_dims || decoded_bbx->m_lb[0] != bbx->m_lb[0] || decoded_bbx->m_ub[0] != bbx->m_ub[0])
    {
        fprintf(stderr, "failed to decode the domain\n");
        exit(-1);
    }
}

void test_boundle_decode()
{

    fprintf(stdout, "--test_boundle_decode--\n");

    //generate the domain boundle array for testing
    int testLen = 10;
    spx_domain_id_bundle_t *domain_id_array[testLen];
    int i, j;
    for (i = 0; i < testLen; i++)
    {
        spx_domain_id_bundle_t *temp_boundle =
            (spx_domain_id_bundle_t *)malloc(sizeof(spx_domain_id_bundle_t));
        temp_boundle->m_domain.m_dims = 3;
        for (j = 0; j < 3; j++)
        {
            temp_boundle->m_domain.m_lb[j] = 0;
            temp_boundle->m_domain.m_ub[j] = 5;
        }
        temp_boundle->m_associated_id = i;
        domain_id_array[i] = temp_boundle;
    }

    //allocate the space for the encoded buffer
    //the 32 is for the bites the 1 is for the string end
    int encoded_buffer_size = testLen * sizeof(spx_domain_id_bundle_t) + 4 + 4 ;
    char *encoded_buffer = malloc(encoded_buffer_size);
    snprintf(encoded_buffer, 4, "%d", testLen);
    spx_domain_id_bundle_t *data_start = (spx_domain_id_bundle_t *)(encoded_buffer + 4 + 4);

    //encode the domain boundle
    for (i = 0; i < testLen; i++)
    {
        memcpy(data_start, domain_id_array[i], sizeof(spx_domain_id_bundle_t));
        data_start++;
    }

    //decode the domain boundle
    decode_domain_id_bundle(encoded_buffer);
}

int main()
{
    test_nons_key_encode();
    test_spatial_key_encode();
    test_boundle_decode();
    return 0;
}