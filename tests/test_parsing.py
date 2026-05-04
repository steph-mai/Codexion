#include "codexion.h"
#include <stdio.h>

void run_test(int id, char *desc, int argc, char **argv)
{
    t_args args;
    int result;

    // On remet à zéro pour chaque test
    ft_memset(&args, 0, sizeof(t_args));
    result = fill_args_structure(&args, argc, argv);

    printf("Test %02d: %-40s -> ", id, desc);
    if (result == 1)
        printf("\033[0;32mPASS\033[0m (Values: %zu, %lld, %d)\n",
                args.number_of_coders, args.time_to_burnout, args.scheduler);
    else
        printf("\033[0;31mFAIL\033[0m (Error caught)\n");
}

int main(void)
{
    printf("=== RUNNING PARSING TESTS ===\n\n");

    // 1. Tests de Validité (Doivent PASS)
    char *valid1[] = {"./philo", "5", "800", "200", "200", "200", "7", "100", "fifo"};
    run_test(1, "Valid FIFO input", 9, valid1);

    char *valid2[] = {"./philo", "1", "60", "10", "10", "10", "1", "5", "edf"};
    run_test(2, "Valid EDF input (min values)", 9, valid2);

    // 2. Tests de Format (Doivent FAIL)
    char *fmt1[] = {"./philo", "5", "800", "abc", "200", "200", "7", "100", "fifo"};
    run_test(3, "Letter in arguments", 9, fmt1);

    char *fmt2[] = {"./philo", "5", "800", "200 ", "200", "200", "7", "100", "fifo"};
    run_test(4, "Space after number", 9, fmt2); // is_number_str devrait fail ici

    // 3. Tests d'Overflow (Doivent FAIL)
    char *over1[] = {"./philo", "5", "9223372036854775808", "200", "200", "200", "7", "100", "fifo"};
    run_test(5, "Overflow LLONG_MAX + 1", 9, over1);

    char *over2[] = {"./philo", "5", "9999999999999999999999", "200", "200", "200", "7", "100", "fifo"};
    run_test(6, "Massive Overflow", 9, over2);

    // 4. Tests de Logique Métier (Doivent FAIL via validate_args)
    char *log1[] = {"./philo", "0", "800", "200", "200", "200", "7", "100", "fifo"};
    run_test(7, "0 Coders", 9, log1);

    char *log2[] = {"./philo", "5", "-500", "200", "200", "200", "7", "100", "fifo"};
    run_test(8, "Negative time", 9, log2);

    // 5. Tests du Scheduler (Doivent FAIL)
    char *sched1[] = {"./philo", "5", "800", "200", "200", "200", "7", "100", "other"};
    run_test(9, "Unknown scheduler mode", 9, sched1);

    char *sched2[] = {"./philo", "5", "800", "200", "200", "200", "7", "100", "FIFO"};
    run_test(10, "Scheduler Case Sensitive (FIFO)", 9, sched2);

    return (0);
}
