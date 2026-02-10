/* A Bison parser, made by GNU Bison 3.8.2.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015, 2018-2021 Free Software Foundation,
   Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <https://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

/* DO NOT RELY ON FEATURES THAT ARE NOT DOCUMENTED in the manual,
   especially those whose name start with YY_ or yy_.  They are
   private implementation details that can be changed or removed.  */

#ifndef YY_YY_INPUT_YACC_H_INCLUDED
# define YY_YY_INPUT_YACC_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token kinds.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    YYEMPTY = -2,
    YYEOF = 0,                     /* "end of file"  */
    YYerror = 256,                 /* error  */
    YYUNDEF = 257,                 /* "invalid token"  */
    NL = 258,                      /* NL  */
    T_INT_LIT = 259,               /* T_INT_LIT  */
    T_FP_LIT = 260,                /* T_FP_LIT  */
    T_ALPHANUM = 261,              /* T_ALPHANUM  */
    T_STRING = 262,                /* T_STRING  */
    FR_MODE = 263,                 /* FR_MODE  */
    REG_INT = 264,                 /* REG_INT  */
    REG_FP = 265,                  /* REG_FP  */
    DE_DATA = 266,                 /* DE_DATA  */
    DE_TEXT = 267,                 /* DE_TEXT  */
    DE_SPACE = 268,                /* DE_SPACE  */
    DE_GLOBL = 269,                /* DE_GLOBL  */
    DE_IREG = 270,                 /* DE_IREG  */
    DE_FREG = 271,                 /* DE_FREG  */
    DE_ASCIIZ = 272,               /* DE_ASCIIZ  */
    DE_BTB = 273,                  /* DE_BTB  */
    DE_BYTE = 274,                 /* DE_BYTE  */
    DE_HALF = 275,                 /* DE_HALF  */
    DE_WORD = 276,                 /* DE_WORD  */
    DE_DWORD = 277,                /* DE_DWORD  */
    DE_FLOAT = 278,                /* DE_FLOAT  */
    DE_DOUBLE = 279,               /* DE_DOUBLE  */
    FORM_INT_RRI = 280,            /* FORM_INT_RRI  */
    FORM_INT_RRSH = 281,           /* FORM_INT_RRSH  */
    FORM_INT_RRR = 282,            /* FORM_INT_RRR  */
    FORM_INT_RA = 283,             /* FORM_INT_RA  */
    FORM_INT_RI = 284,             /* FORM_INT_RI  */
    FORM_J_RA = 285,               /* FORM_J_RA  */
    FORM_J_RRA = 286,              /* FORM_J_RRA  */
    FORM_INT_B_RRA = 287,          /* FORM_INT_B_RRA  */
    FORM_J_A = 288,                /* FORM_J_A  */
    FORM_INT_L_RSB = 289,          /* FORM_INT_L_RSB  */
    FORM_INT_S_RSB = 290,          /* FORM_INT_S_RSB  */
    FORM_INT_ECALL = 291,          /* FORM_INT_ECALL  */
    FORM_INT_EBREAK = 292,         /* FORM_INT_EBREAK  */
    FORM_FP_FFF = 293,             /* FORM_FP_FFF  */
    FORM_FP_FFFr = 294,            /* FORM_FP_FFFr  */
    FORM_FP_FFFFr = 295,           /* FORM_FP_FFFFr  */
    FORM_FP_FFr = 296,             /* FORM_FP_FFr  */
    FORM_FP_RFF = 297,             /* FORM_FP_RFF  */
    FORM_FP_RFr = 298,             /* FORM_FP_RFr  */
    FORM_FP_RF = 299,              /* FORM_FP_RF  */
    FORM_FP_FR = 300,              /* FORM_FP_FR  */
    FORM_FP_FRr = 301,             /* FORM_FP_FRr  */
    FORM_FP_FF = 302,              /* FORM_FP_FF  */
    FORM_FP_L_FSB = 303,           /* FORM_FP_L_FSB  */
    FORM_FP_S_FSB = 304,           /* FORM_FP_S_FSB  */
    FORM_INT_RB = 305,             /* FORM_INT_RB  */
    FORM_INT_RRB = 306,            /* FORM_INT_RRB  */
    INST_FENCE = 307,              /* INST_FENCE  */
    INST_PRUEBA = 308,             /* INST_PRUEBA  */
    FORM_CSR_RxR = 309,            /* FORM_CSR_RxR  */
    FORM_CSR_RxI = 310,            /* FORM_CSR_RxI  */
    FORM_INM = 311,                /* FORM_INM  */
    FORM_INM_L = 312,              /* FORM_INM_L  */
    FORM_INM_S = 313,              /* FORM_INM_S  */
    FORM_INM_DI = 314,             /* FORM_INM_DI  */
    FORM_INM_B = 315,              /* FORM_INM_B  */
    FORM_INM_B_FI = 316,           /* FORM_INM_B_FI  */
    FORM_INM_B_FFI = 317,          /* FORM_INM_B_FFI  */
    FORM_INM_T = 318,              /* FORM_INM_T  */
    FORM_PSEUDO_NOP = 319,         /* FORM_PSEUDO_NOP  */
    FORM_PSEUDO_RR = 320,          /* FORM_PSEUDO_RR  */
    FORM_PSEUDO_BRA = 321,         /* FORM_PSEUDO_BRA  */
    FORM_PSEUDO_J = 322,           /* FORM_PSEUDO_J  */
    FORM_PSEUDO_JR = 323,          /* FORM_PSEUDO_JR  */
    FORM_PSEUDO_RET = 324,         /* FORM_PSEUDO_RET  */
    FORM_PSEUDO_TAIL = 325,        /* FORM_PSEUDO_TAIL  */
    FORM_PSEUDO_R = 326,           /* FORM_PSEUDO_R  */
    FORM_PSEUDO_CSRR = 327,        /* FORM_PSEUDO_CSRR  */
    FORM_PSEUDO_CSRW = 328,        /* FORM_PSEUDO_CSRW  */
    FORM_PSEUDO_CSRS = 329,        /* FORM_PSEUDO_CSRS  */
    FORM_PSEUDO_CSRC = 330,        /* FORM_PSEUDO_CSRC  */
    FORM_PSEUDO_CSRWI = 331,       /* FORM_PSEUDO_CSRWI  */
    FORM_PSEUDO_CSRSI = 332,       /* FORM_PSEUDO_CSRSI  */
    FORM_PSEUDO_CSRCI = 333,       /* FORM_PSEUDO_CSRCI  */
    FORM_PSEUDO_FRCSR = 334,       /* FORM_PSEUDO_FRCSR  */
    FORM_PSEUDO_FSCSR = 335,       /* FORM_PSEUDO_FSCSR  */
    FORM_PSEUDO_FRRM = 336,        /* FORM_PSEUDO_FRRM  */
    FORM_PSEUDO_FSRM = 337,        /* FORM_PSEUDO_FSRM  */
    FORM_PSEUDO_FRFLAGS = 338,     /* FORM_PSEUDO_FRFLAGS  */
    FORM_PSEUDO_FSFLAGS = 339,     /* FORM_PSEUDO_FSFLAGS  */
    FORM_PSEUDO_BRRA = 340,        /* FORM_PSEUDO_BRRA  */
    FORM_PSEUDO_RRi = 341,         /* FORM_PSEUDO_RRi  */
    FORM_PSEUDO_FF = 342,          /* FORM_PSEUDO_FF  */
    FORM_PSEUDO_LLA = 343,         /* FORM_PSEUDO_LLA  */
    FORM_PSEUDO_L = 344,           /* FORM_PSEUDO_L  */
    FORM_PSEUDO_S = 345,           /* FORM_PSEUDO_S  */
    FORM_PSEUDO_FL = 346,          /* FORM_PSEUDO_FL  */
    FORM_PSEUDO_FS = 347,          /* FORM_PSEUDO_FS  */
    FORM_PSEUDO_LI = 348,          /* FORM_PSEUDO_LI  */
    FORM_PSEUDO_CALL = 349,        /* FORM_PSEUDO_CALL  */
    FORM_PSEUDO_TRAP = 350,        /* FORM_PSEUDO_TRAP  */
    INST_RET = 351,                /* INST_RET  */
    INST_SYSCALL = 352,            /* INST_SYSCALL  */
    FORM_REG = 353,                /* FORM_REG  */
    FORM_REG_FF = 354,             /* FORM_REG_FF  */
    FORM_INM_FP_L = 355,           /* FORM_INM_FP_L  */
    FORM_INM_FP_S = 356,           /* FORM_INM_FP_S  */
    FORM_FP_REG = 357,             /* FORM_FP_REG  */
    FORM_FP_REG_FF = 358,          /* FORM_FP_REG_FF  */
    FORM_FP_REG_DF = 359,          /* FORM_FP_REG_DF  */
    FORM_REG_R_FP = 360,           /* FORM_REG_R_FP  */
    FORM_REG_FP_R = 361,           /* FORM_REG_FP_R  */
    FORM_J = 362,                  /* FORM_J  */
    FORM_J_F = 363,                /* FORM_J_F  */
    LABEL = 364,                   /* LABEL  */
    PINST_LA = 365,                /* PINST_LA  */
    PINST_LI = 366,                /* PINST_LI  */
    M_LO = 367,                    /* M_LO  */
    M_HI = 368,                    /* M_HI  */
    M_DISP = 369,                  /* M_DISP  */
    DE_PC = 370,                   /* DE_PC  */
    FORM_INTERNAL_NOP = 371        /* FORM_INTERNAL_NOP  */
  };
  typedef enum yytokentype yytoken_kind_t;
#endif

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED
union YYSTYPE
{
#line 117 "input.yacc.y"

    double fvalue;
    int64_t ivalue;
    char cvalue[128];

#line 186 "input.yacc.h"

};
typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;


int yyparse (void);


#endif /* !YY_YY_INPUT_YACC_H_INCLUDED  */
