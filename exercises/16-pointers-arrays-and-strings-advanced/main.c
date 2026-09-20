#include <stddef.h>
#include <stdio.h>

struct Command {
    const char *name;
    int (*handler)(void);
};

static int string_equal(const char *left, const char *right)
{
    while (*left == *right) {
        if (*left == '\0') {
            return 1;
        }

        left++;
        right++;
    }

    return 0;
}

static void print_string_pointer_array(const char *const items[], size_t count)
{
    for (size_t i = 0; i < count; i++) {
        printf("items[%zu] = %s, first char = %c\n",
               i,
               items[i],
               items[i][0]);
    }
}

static void walk_with_pointer_to_pointer(const char *const items[], size_t count)
{
    const char *const *cursor = items;
    const char *const *end = items + count;

    printf("pointer-to-pointer walk:");

    while (cursor < end) {
        printf(" %s", *cursor);
        cursor++;
    }

    printf("\n");
}

static void print_arguments(int argc, char *argv[])
{
    printf("argc = %d\n", argc);

    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
}

static void walk_arguments_with_pointer_to_pointer(int argc, char *argv[])
{
    char **cursor = argv;
    char **end = argv + argc;

    printf("argv pointer walk:");

    while (cursor < end) {
        printf(" %s", *cursor);
        cursor++;
    }

    printf("\n");
}

static const char *find_name(const char *const names[], size_t count,
                             const char *target)
{
    for (size_t i = 0; i < count; i++) {
        if (string_equal(names[i], target)) {
            return names[i];
        }
    }

    return NULL;
}

static int run_help(void)
{
    printf("help: available commands are help, status, and version\n");
    return 0;
}

static int run_status(void)
{
    printf("status: all core subsystems are nominal\n");
    return 0;
}

static int run_version(void)
{
    printf("version: day16-dispatch-demo 0.1\n");
    return 0;
}

static const struct Command *find_command(const struct Command commands[],
                                          size_t count,
                                          const char *name)
{
    for (size_t i = 0; i < count; i++) {
        if (string_equal(commands[i].name, name)) {
            return &commands[i];
        }
    }

    return NULL;
}

static int execute_command(const struct Command commands[], size_t count,
                           const char *name)
{
    const struct Command *command = find_command(commands, count, name);

    if (command == NULL) {
        printf("unknown command: %s\n", name);
        return 1;
    }

    printf("executing command: %s\n", command->name);
    return command->handler();
}

static void print_command_table(const struct Command commands[], size_t count)
{
    printf("command table:");

    for (size_t i = 0; i < count; i++) {
        printf(" %s", commands[i].name);
    }

    printf("\n");
}

int main(int argc, char *argv[])
{
    printf("=== Day 16: Pointers, Arrays, and Strings Advanced ===\n\n");

    printf("=== Section 1: Array of String Pointers ===\n\n");

    const char *const names[] = {
        "kernel",
        "memory",
        "scheduler"
    };

    size_t names_count = sizeof names / sizeof names[0];

    print_string_pointer_array(names, names_count);

    printf("names[1] = %s\n", names[1]);
    printf("names[1][0] = %c\n\n", names[1][0]);

    printf("=== Section 2: Pointer to Pointer ===\n\n");

    const char *const *cursor = names;

    printf("*cursor = %s\n", *cursor);
    printf("**cursor = %c\n", **cursor);

    cursor++;

    printf("after cursor++, *cursor = %s\n", *cursor);
    printf("after cursor++, **cursor = %c\n", **cursor);

    walk_with_pointer_to_pointer(names, names_count);

    printf("\n");

    printf("=== Section 3: Pointer Array vs Fixed 2D Array ===\n\n");

    const char *const pointer_commands[] = {
        "help",
        "status",
        "version"
    };

    char fixed_commands[][16] = {
        "help",
        "status",
        "version"
    };

    printf("pointer_commands array object = %zu bytes\n",
           sizeof pointer_commands);
    printf("fixed_commands array object   = %zu bytes\n",
           sizeof fixed_commands);

    printf("pointer_commands[0] = %s\n", pointer_commands[0]);
    printf("fixed_commands[0] before = %s\n", fixed_commands[0]);

    fixed_commands[0][0] = 'H';

    printf("fixed_commands[0] after  = %s\n", fixed_commands[0]);

    char (*row)[16] = fixed_commands;

    printf("sizeof *row = %zu bytes\n", sizeof *row);
    printf("*row = %s\n", *row);

    row++;

    printf("after row++, *row = %s\n\n", *row);

    printf("=== Section 4: Command-line Arguments ===\n\n");

    print_arguments(argc, argv);
    walk_arguments_with_pointer_to_pointer(argc, argv);

    printf("\n");

    printf("=== Section 5: Search in a Pointer Table ===\n\n");

    size_t pointer_commands_count =
        sizeof pointer_commands / sizeof pointer_commands[0];

    const char *found_name = find_name(pointer_commands,
                                       pointer_commands_count,
                                       "status");

    if (found_name != NULL) {
        printf("found command name: %s\n", found_name);
    }

    found_name = find_name(pointer_commands,
                           pointer_commands_count,
                           "reboot");

    if (found_name == NULL) {
        printf("command name not found: reboot\n");
    }

    printf("\n");

    printf("=== Section 6: Function Pointers ===\n\n");

    int (*handler)(void) = run_status;

    printf("calling function through handler pointer\n");
    int handler_result = handler();

    printf("handler returned %d\n\n", handler_result);

    printf("=== Section 7: Dispatch Table ===\n\n");

    const struct Command commands[] = {
        {"help", run_help},
        {"status", run_status},
        {"version", run_version}
    };

    size_t command_count = sizeof commands / sizeof commands[0];

    print_command_table(commands, command_count);

    printf("\n");

    printf("=== Section 8: Mini Shell-style Dispatch ===\n\n");

    int overall_status = 0;

    if (argc <= 1) {
        printf("no command-line commands provided\n");
        printf("running default command sequence\n\n");

        int result;

        result = execute_command(commands, command_count, "help");
        printf("return code = %d\n\n", result);
        if (result != 0 && overall_status == 0) {
            overall_status = result;
        }

        result = execute_command(commands, command_count, "status");
        printf("return code = %d\n\n", result);
        if (result != 0 && overall_status == 0) {
            overall_status = result;
        }

        result = execute_command(commands, command_count, "version");
        printf("return code = %d\n", result);
        if (result != 0 && overall_status == 0) {
            overall_status = result;
        }
    } else {
        for (int i = 1; i < argc; i++) {
            int result = execute_command(commands, command_count, argv[i]);

            printf("return code = %d\n", result);

            if (result != 0 && overall_status == 0) {
                overall_status = result;
            }

            if (i + 1 < argc) {
                printf("\n");
            }
        }
    }

    return overall_status;
}
