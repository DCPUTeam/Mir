%{

/**

	File:           parser.y

	Project:        DCPU-16 Toolchain
	Component:      Tool-MsgGen

	Authors:        James Rhodes

	Description:    Defines parser for the message definition generator.

**/

#define YYERROR_VERBOSE

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <simclist.h>
#include <bstring.h>
#include <argtable2.h>

extern FILE* yyin;
extern FILE* yyout;
extern int yylex();
void yyerror(const char* str);
list_t messages;

struct data_entry
{
    bstring type;
    bstring name;
};

struct message_entry
{
    bstring name;
    list_t datas;
};

%}

// Defines a union for storing all of our data types.
%union
{
    bstring word;
    list_t list;
    int token;
}

// Define our lexical token names.
%token <token> KEYWORD_MESSAGE BRACKET_OPEN BRACKET_CLOSE SEMICOLON ERROR
%token <word> NAME

// Define our parser types.
%type <list> datas

// Start at the root node.
%start input

%%

input: definitions ;

definitions:
        /* empty */
        {
        } |
        definitions KEYWORD_MESSAGE NAME BRACKET_OPEN datas BRACKET_CLOSE SEMICOLON
        {
            struct message_entry* e = malloc(sizeof(struct message_entry));
            e->name = $3;
            e->datas = $5;
            list_append(&messages, e);
        } ;
        
datas:
        /* empty */
        {
            list_init(&$$);
        } |
        datas NAME NAME SEMICOLON
        {
            struct data_entry* e = malloc(sizeof(struct data_entry));
            e->type = $2;
            e->name = $3;
            list_append(&$1, e);
            $$ = $1;
        } ;

%%

void yyerror(const char* str)
{
    fprintf(stderr, "error: parsing message definition file failed!\n");
    fprintf(stderr, "%s\n", str);
}

int main(int argc, char** argv)
{
    FILE* input;
    FILE* output_header;
    FILE* output_source;
    struct message_entry* msg;
    struct data_entry* data;
    int nerrors, i;
    bstring params;
    
    // Define arguments.
    struct arg_lit* show_help = arg_litn("h", "help", 0, 2, "Show this help.");
    struct arg_file* output_header_arg = arg_file1(NULL, NULL, "<header>", "The output header filename.");
    struct arg_file* output_source_arg = arg_file1(NULL, NULL, "<source>", "The output source filename.");
    struct arg_file* inputs = arg_filen(NULL, NULL, "<input>", 0, 100, "The input files.");
    struct arg_end* end = arg_end(20);
    void* argtable[] = { show_help, output_header_arg, output_source_arg, inputs, end };
    
    // Parse arguments.
    nerrors = arg_parse(argc, argv, argtable);
    if (nerrors != 0 || show_help->count != 0)
    {
        if (nerrors != 0)
            arg_print_errors(stdout, end, "tool-msggen");

        printf("syntax:\n    tool-msggen");
        arg_print_syntax(stderr, argtable, "\n");
        printf("options:\n");
        arg_print_glossary(stderr, argtable, "    %-25s %s\n");
        arg_freetable(argtable, sizeof(argtable) / sizeof(argtable[0]));
        return 1;
    }

    // Initialize list.
    list_init(&messages);
    
    // Parse all of the input files.
    for (i = 0; i < inputs->count; i++)
    {
        // Parse the input file.
        input = fopen(inputs->filename[i], "r");
        if (input == NULL)
        {
            fprintf(stderr, "error: unable to open input file '%s'.\n", inputs->filename[i]);
            return 1;
        }

        // Assign input / outputs and parse.
        yyout = stderr;
        yyin = input;
        yyparse();
        
        // Close input file.
        fclose(input);
    }
    
    // Open the output files.
    output_header = fopen(output_header_arg->filename[0], "w");
    output_source = fopen(output_source_arg->filename[0], "w");
    if (output_header == NULL || output_source == NULL)
    {
        if (output_header != NULL) fclose(output_header);
        if (output_source != NULL) fclose(output_source);
        fprintf(stderr, "error: unable to open output files.\n");
        return 1;
    }
    
    // Output information at top of files.
    fprintf(output_header,
            "/*\n"
            " * THIS HEADER WAS AUTO-GENERATED!\n"
            " * Modifications to this file will be\n"
            " * lost on next build.\n"
            " */\n");
    fprintf(output_source,
            "/*\n"
            " * THIS SOURCE WAS AUTO-GENERATED!\n"
            " * Modifications to this file will be\n"
            " * lost on next build.\n"
            " */\n");
    
    // Include common headers.
    fprintf(output_header, "\n");
    fprintf(output_header, "#include <string>\n");
    fprintf(output_header, "#include <ObjectMessage.h>\n");
    fprintf(output_header, "\n");
    
    // Define common definitions for source.
    fprintf(output_source, "\n");
    fprintf(output_source, "#define MIR_NETWORK_CONTROLLER_INTERNALS\n");
    fprintf(output_source, "\n");
    fprintf(output_source, "#include <sstream>\n");
    fprintf(output_source, "#include <iostream>\n");
    fprintf(output_source, "#include <exception>\n");
    fprintf(output_source, "#include <boost/tokenizer.hpp>\n");
    fprintf(output_source, "#include <string>\n");
    fprintf(output_source, "#include \"%s\"\n", output_header_arg->filename[0]);
    fprintf(output_source, "\n");
    fprintf(output_source, "bool operator>> (const std::stringstream &ss, std::string& str)\n");
    fprintf(output_source, "{\n");
    fprintf(output_source, "    str = ss.str();\n");
    fprintf(output_source, "    return true;\n");
    fprintf(output_source, "}\n");
    fprintf(output_source, "\n");
    
    // Loop through all of the messages defined and create
    // files based on them.
    list_iterator_start(&messages);
    while (list_iterator_hasnext(&messages))
    {
        msg = list_iterator_next(&messages);
        
        // Generate parameters.
        params = bfromcstr("");
        list_iterator_start(&msg->datas);
        while (list_iterator_hasnext(&msg->datas))
        {
            data = list_iterator_next(&msg->datas);
            bformata(params, ", %s p_%s", data->type->data, data->name->data);
        }
        list_iterator_stop(&msg->datas);
        
        // Print out header information.
        fprintf(output_header, "#ifndef __DCPU_GAME_NETWORKING_%s_H\n", msg->name->data);
        fprintf(output_header, "#define __DCPU_GAME_NETWORKING_%s_H\n", msg->name->data);
        fprintf(output_header, "\n");
        fprintf(output_header, "namespace Network\n");
        fprintf(output_header, "{\n");
        fprintf(output_header, "    class %s : public ObjectMessage\n", msg->name->data);
        fprintf(output_header, "    {\n");
        fprintf(output_header, "    public:\n");
        fprintf(output_header, "#ifdef MIR_NETWORK_CONTROLLER_INTERNALS\n");
        fprintf(output_header, "        %s(Source source);\n", msg->name->data);
        fprintf(output_header, "#endif\n");
        fprintf(output_header, "        %s(std::string identifier%s);\n", msg->name->data, params->data);
        fprintf(output_header, "        virtual std::string Serialize();\n");
        fprintf(output_header, "        virtual void Deserialize(std::string data);\n");
        fprintf(output_header, "        virtual int GetHashCode();\n");
        fprintf(output_header, "        \n");
        list_iterator_start(&msg->datas);
        while (list_iterator_hasnext(&msg->datas))
        {
            data = list_iterator_next(&msg->datas);
            fprintf(output_header, "        %s %s;\n", data->type->data, data->name->data);
        }
        list_iterator_stop(&msg->datas);
        fprintf(output_header, "    };\n");
        fprintf(output_header, "}\n");
        fprintf(output_header, "\n");
        fprintf(output_header, "#endif\n");
        fprintf(output_header, "\n");
        
        // Print out source information.
        fprintf(output_source, "namespace Network\n");
        fprintf(output_source, "{\n");
        fprintf(output_source, "    %s::%s(Source source)\n", msg->name->data, msg->name->data);
        fprintf(output_source, "        : ObjectMessage(source)\n");
        fprintf(output_source, "    {\n");
        fprintf(output_source, "    }\n");
        fprintf(output_source, "    \n");
        fprintf(output_source, "    %s::%s(std::string identifier%s)\n", msg->name->data,
                                                                         msg->name->data,
                                                                         params->data
               );
        fprintf(output_source, "        : ObjectMessage(false)\n");
        fprintf(output_source, "    {\n");
        fprintf(output_source, "        this->Identifier = identifier;\n");
        list_iterator_start(&msg->datas);
        while (list_iterator_hasnext(&msg->datas))
        {
            data = list_iterator_next(&msg->datas);
            fprintf(output_source, "        this->%s = p_%s;\n", data->name->data, data->name->data);
        }
        list_iterator_stop(&msg->datas);
        fprintf(output_source, "    }\n");
        fprintf(output_source, "    \n");
        fprintf(output_source, "    std::string %s::Serialize()\n", msg->name->data);
        fprintf(output_source, "    {\n");
        fprintf(output_source, "        std::stringstream ss;\n");
        fprintf(output_source, "        ss << this->Identifier;\n");
        list_iterator_start(&msg->datas);
        while (list_iterator_hasnext(&msg->datas))
        {
            data = list_iterator_next(&msg->datas);
            fprintf(output_source, "        ss << \":\";\n");
            fprintf(output_source, "        ss << this->%s;\n", data->name->data);
        }
        list_iterator_stop(&msg->datas);
        fprintf(output_source, "        return ss.str();\n");
        fprintf(output_source, "    }\n");
        fprintf(output_source, "    \n");
        fprintf(output_source, "    void %s::Deserialize(std::string data)\n", msg->name->data);
        fprintf(output_source, "    {\n");
        fprintf(output_source, "        typedef boost::tokenizer<boost::char_separator<char> > tokenizer;\n");
        fprintf(output_source, "        boost::char_separator<char> sep(\":\");\n");
        fprintf(output_source, "        tokenizer tokens(data, sep);\n");
        fprintf(output_source, "        tokenizer::iterator i = tokens.begin();\n");
        fprintf(output_source, "        this->Identifier = *(i++);\n");
        list_iterator_start(&msg->datas);
        while (list_iterator_hasnext(&msg->datas))
        {
            data = list_iterator_next(&msg->datas);
            fprintf(output_source, "        if (!(std::stringstream(*(i++)) >> this->%s))\n", data->name->data);
            fprintf(output_source, "            this->%s = %s();\n", data->name->data, data->type->data);
        }
        list_iterator_stop(&msg->datas);
        fprintf(output_source, "    }\n");
        fprintf(output_source, "    \n");
        fprintf(output_source, "    int %s::GetHashCode()\n", msg->name->data);
        fprintf(output_source, "    {\n");
        fprintf(output_source, "        int i = 0;\n");
        fprintf(output_source, "        for (size_t j = 0; j < std::string(\"%s\").length(); j++)\n", msg->name->data);
        fprintf(output_source, "            i += i * \"%s\"[i];\n", msg->name->data);
        fprintf(output_source, "        for (size_t j = 0; j < this->Identifier.length(); j++)\n");
        fprintf(output_source, "            i += i * this->Identifier[i];\n");
        fprintf(output_source, "        return i;\n");
        fprintf(output_source, "    }\n");
        fprintf(output_source, "}\n");
        fprintf(output_source, "\n");
        
        // Print debugging information.
        printf("message: %s\n", msg->name->data);
        list_iterator_start(&msg->datas);
        while (list_iterator_hasnext(&msg->datas))
        {
            data = list_iterator_next(&msg->datas);
            printf("    %s %s\n", data->type->data, data->name->data);
        }
        list_iterator_stop(&msg->datas);
    }
    list_iterator_stop(&messages);
    
    // Close outputs.
    fclose(output_header);
    fclose(output_source);
    
    // Done.
    return 0;
}

