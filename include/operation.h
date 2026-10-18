#ifndef OPERATION_H
#define OPERATION_H

// operation types
typedef enum
{
    e_failure,
    e_success,
    e_view,
    e_edit,
    e_help
} OperationType;

OperationType read_and_validate_args(int argc, char *argv[]);

#endif // OPERATION_H
