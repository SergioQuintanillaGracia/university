#!/usr/bin/env python
#! -*- encoding: utf8 -*-
# 3.- Mono Evolved

import argparse
from SAR_p3_monkey_lib import Monkey

if __name__ == "__main__":

    parser = argparse.ArgumentParser(description='Generate sentences based on a n-gram language model.')
    
    parser.add_argument('-f', '--filename', dest='lm_filename', action='store', required=True,
                        help='name of the file with the language model.')
    
    parser.add_argument('-n', action='store', required=False, type=int, default=None,
                        help='n to use in the ngram model.')

    parser.add_argument('-s', '--sentences', action='store', required=False, type=int, default=10,
                        help='number of sentences to produce.')
    
    parser.add_argument('-p', '--prefix', dest='prefix', action='store', required=False, default=None,
                        help='prefix to use in the sentence generation.')
    

    args = parser.parse_args()
    m = Monkey()
    m.load_lm(args.lm_filename)

    if args.n is not None:    
        if args.n > m.get_n():
            print(f'The language model reaches {m.get_n()}-grams')
        elif args.n < 2:
            print(f'n must be > 1')
        else:
            m.generate_sentences(n=args.n, nsentences=args.sentences, prefix=args.prefix)
    else:
        m.generate_sentences(n=None, nsentences=args.sentences, prefix=args.prefix)
