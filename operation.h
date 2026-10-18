#ifndef OPERATION_H
#define OPERATION_H

typedef enum
{
    e_failure,
    e_success,
    e_view,
    e_edit,
    e_help

} OperationType;

OperationType read_and_validate_args(int argc, char *argv[]);


void display_help();

void display_error_message();


#endif // OPERATION_H