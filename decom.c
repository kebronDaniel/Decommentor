#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>

// StateEnum : Contains States mentioned in the DFA diagram
typedef enum StateEnum
{
    INSIDE_A_CODE,
    TO_START_A_COMMENT,
    INSIDE_A_COMMENT,
    TO_END_A_COMMENT,
    IN_A_STRING,
    ESCAPE_STRING,
    IN_A_CHAR,
    ESCAPE_CHAR
} State;

// All the functions below Check conditions based on what is in the value to decide which way it should change the state.

// State 1 handler
void inside_a_code_state(State *state, char value)
{

    // Based on the value, conditions change state 1 to state 7 or 5 or 2

    if (value == '\"')
    {
        *state = IN_A_STRING;
    }
    else if (value == '\'')
    {
        *state = IN_A_CHAR;
    }
    else if (value == '/')
    {
        *state = TO_START_A_COMMENT;
    }

    // if the above three conditions arenot met then it's still inside a code or state one, Thus nothing would be done.
    if (*state != TO_START_A_COMMENT)
    {
        putchar(value);
    }
};

// State 2 handler
void to_start_a_comment_state(State *state, char value, int *commentStartLine, int *curruntLine)
{

    // Based on the value, conditions change state 2 to 3 or 1 or would stay at current state.

    if (value == '*')
    {
        *state = INSIDE_A_COMMENT;
        *commentStartLine = *curruntLine; // To keep track of unterminated comment, which is used in the main function.
    }
    else if ((value == '/') && *state == TO_START_A_COMMENT)
    {
        putchar(value);
        *state = TO_START_A_COMMENT;
    }
    else if (value == '/')
    {
        *state = TO_START_A_COMMENT;
    }
    else
    {
        *state = INSIDE_A_CODE;
    }

    if (*state == INSIDE_A_CODE)
    {
        putchar('/');
        putchar(value);
    }
};

// state 3 handler
void inside_a_comment_state(State *state, char value)
{
    // Based on the value, this function only changes state 3 to 4 to start ending the comment if the first condition in met.
    // Except that the state stays in this state.
    if (value == '*')
    {
        *state = TO_END_A_COMMENT;
    }

    if (value == '\n')
    {
        putchar(value);
    }
};

// State 4 handler
void to_end_a_comment_state(State *state, char value)
{
    // Based on the content of value the state changes to either state 1 or 3 or stays in current state.

    if (value == '/')
    {
        *state = INSIDE_A_CODE;
    }
    else if (value == '*')
    {
        *state = TO_END_A_COMMENT;
    }
    else
    {
        *state = INSIDE_A_COMMENT;
    }

    if (*state == INSIDE_A_CODE)
    {
        putchar(' ');
    }
};

// State 5 handler
void in_a_string_state(State *state, char value)
{

    // Based on the contents of the value it either changes to state 1 or 6

    if (value == '\\')
    {
        *state = ESCAPE_STRING;
    }
    else if (value == '\"')
    {
        *state = INSIDE_A_CODE;
    }
    putchar(value);
};

// State 6 handler
void escape_string_state(State *state, char value)
{
    // with any value it writes it out and changes to state 5

    *state = IN_A_STRING;
    putchar(value);
};

// State 7 handler
void in_a_char_state(State *state, char value)
{

    // Changes the state to either 1 or 8 based on the content of the value

    if (value == '\\')
    {
        *state = ESCAPE_CHAR;
    }
    else if (value == '\'')
    {
        *state = INSIDE_A_CODE;
    }
    putchar(value);
};

// State 8 handler
void escape_char_state(State *state, char in)
{

    // with any value it writes it out and changes to state 7

    *state = IN_A_CHAR;
    putchar(in);
};

// To catch incomplete comments
void check_comment_completion(State *state, int lastCommentline)
{

    if (*state == INSIDE_A_COMMENT || *state == TO_END_A_COMMENT)
    {
        fprintf(stderr, "Error: you didn't terminate the comment you started on line %d . \n", lastCommentline);
    }
}

void count_processed_lines(int *line, char ln)
{

    if (ln == '\n')
    {
        *line += 1;
    }
}

// This is the decommentor part which swithces cases to use the functions above to remove the comments only.

void main()
{

    // State 1 -> starting point
    State state = INSIDE_A_CODE;

    int line = 1;
    int commentStartLine = 1;

    for (;;)
    // This is like a while(true), thus we only jump from one state to the other without limits.
    {
        char in = getchar();
        count_processed_lines(&line, in);

        if (in == EOF)
        {
            check_comment_completion(&state, commentStartLine);
            break;
        }

        switch (state)
        {
        case INSIDE_A_CODE:
            inside_a_code_state(&state, in);
            break;
        case TO_START_A_COMMENT:
            to_start_a_comment_state(&state, in, &commentStartLine, &line);
            break;
        case INSIDE_A_COMMENT:
            inside_a_comment_state(&state, in);
            break;
        case TO_END_A_COMMENT:
            to_end_a_comment_state(&state, in);
            break;
        case IN_A_STRING:
            in_a_string_state(&state, in);
            break;
        case ESCAPE_STRING:
            escape_string_state(&state, in);
            break;
        case IN_A_CHAR:
            in_a_char_state(&state, in);
            break;
        case ESCAPE_CHAR:
            escape_char_state(&state, in);
            break;
        }
    }
}
