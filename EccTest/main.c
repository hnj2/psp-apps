#include <types.h>
#include <cdefs.h>
#include <svc.h>
#include <string.h>
#include <binloader.h>

#include <psp-stub/psp-stub.h>
#include <sev/sev.h>

struct s_ecc_value {
    uint8_t bytes[0x48];
};

typedef struct s_ecc_value ecc_value;

struct s_ecc_point {
    ecc_value x;
    ecc_value y;
};

typedef struct s_ecc_point ecc_point;

#define ECC_FUNCTION_ADD_SCALARS        1
#define ECC_FUNCTION_MUL_SCALARS        2
#define ECC_FUNCTION_INV_SCALAR         3
#define ECC_FUNCTION_MUL_POINT          5
#define ECC_FUNCTION_MUL_AND_ADD_POINT  7

struct s_ecc_request {

    uint32_t function;

    union {

        struct {
            ecc_value * output;
            ecc_value * prime;
            ecc_value * scalar1;
            ecc_value * scalar2;
        } scalar_op;

        struct {
            ecc_point * output;
            ecc_value * prime;
            ecc_point * coefficients;
            ecc_value * factor1;
            ecc_point * point1;
            ecc_value * factor2;
            ecc_point * point2;
        } point_op;

    } params;

};

typedef struct s_ecc_request ecc_request;

uint32_t svc_ecc_math(ecc_request * request);

uint32_t main(uint32_t idCcd, uint32_t cCcds, PSEVCMDBUF pCmdBuf, uint8_t fFirstRun)
{
    // p = 2^384 - 2^128  - 2^96 + 2^32 - 1
    ecc_value prime = {{
        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0x00, //  64
        0x00, 0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, // 128
        0xef, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 192
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 256
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 320
        0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff, // 384
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    }};

    ecc_value two = {{
        0x02, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
    }};

    ecc_value output = {{
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
        0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41, 0x41,
    }};

    ecc_request request;

    request.function = ECC_FUNCTION_INV_SCALAR;
    request.params.scalar_op.output = &output;
    request.params.scalar_op.prime = &prime;
    request.params.scalar_op.scalar1 = &two;
    request.params.scalar_op.scalar2 = NULL;

    return svc_ecc_math(&request);
}

